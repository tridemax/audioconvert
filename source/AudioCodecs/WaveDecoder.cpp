#include "platform.h"
#include "WaveDecoder.h"


namespace audioconvert
{
/***************************************************************************************************
*** WaveDecoder
***************************************************************************************************/
	WaveDecoder::WaveDecoder() : m_parsingStage(ParsingStage::AwaitingInitialHeader)
	{
	}

	/**********************************************************************************************/
	WaveDecoder::~WaveDecoder()
	{
	}

	/**********************************************************************************************/
	bool WaveDecoder::TestStream(aux::IMemoryStream& inputStream)
	{
		return false;
	}

	/**********************************************************************************************/
	bool WaveDecoder::DecodeStream(aux::IMemoryStream& inputStream, AudioSink& outputSink)
	{
		// Do decoding input stream as single chunk
		DecodeChunk(inputStream.EntireData(), static_cast<uint32_t>(inputStream.Length()), outputSink);

		if (!DecodingCompleted())
		{
			ResetDecoder();
			outputSink.SetFormat(AudioMetadata());
			assert(false);
			return false;
		}

		return true;
	}

	/**********************************************************************************************/
	bool WaveDecoder::DecodeChunk(const byte* buffer, size_t bufferLength, AudioSink& outputSink)
	{
		switch (m_parsingStage)
		{
		case ParsingStage::AwaitingInitialHeader:
			if (m_internalBuffer.empty() && bufferLength >= InitialHeaderLength) // most usual case - parsing without buffering
			{
				// Test first dword
				if (*reinterpret_cast<const uint32_t*>(buffer) != FourccRiff)
				{
					ResetDecoder();
					assert(false);
					return false;
				}

				// Test second dword
				if (*reinterpret_cast<const uint32_t*>(buffer + FourccLength) == 0u)
				{
					ResetDecoder();
					assert(false);
					return false;
				}

				// Test third dword
				m_currentChunkId = *reinterpret_cast<const uint32_t*>(buffer + InitialHeaderLength - FourccLength);

				if (m_currentChunkId == FourccWave)
				{
					m_parsingStage = ParsingStage::AwaitingFormatChunk;
				}
				else
				{
					m_parsingStage = ParsingStage::SkippingUnusedChunk;
				}

				return DecodeChunk(buffer + InitialHeaderLength, bufferLength - InitialHeaderLength, outputSink);
			}
			else // unusual case - using the buffer
			{
				assert(false);
			}
			break;

		case ParsingStage::SkippingUnusedSubChunk:
			break;

		case ParsingStage::AwaitingFormatChunk:
			if (m_internalBuffer.empty() && bufferLength >= FormatHeaderLength) // most usual case - parsing without buffering
			{
				// Read chunk id
				m_currentChunkId = *reinterpret_cast<const uint32_t*>(buffer);

				if (m_currentChunkId == FourccFormat)
				{
					// Read chunk length
					m_currentChunkLength = *reinterpret_cast<const uint32_t*>(buffer + FourccLength);

					m_parsingStage = ParsingStage::ReadingFormatChunk;
				}
				else
				{
					m_parsingStage = ParsingStage::SkippingUnusedSubChunk;
				}

				return DecodeChunk(buffer + FormatHeaderLength, bufferLength - FormatHeaderLength, outputSink);
			}
			else // unusual case - using the buffer
			{
				assert(false);
			}
			break;

		case ParsingStage::ReadingFormatChunk:
			if (m_internalBuffer.empty() && bufferLength >= FormatChunkLength) // most usual case - parsing without buffering
			{
				const WavePcmFormat* wavePcmFormat = reinterpret_cast<const WavePcmFormat*>(buffer);

				// Test for extensible wave format
				if (m_currentChunkLength != FormatChunkLength)
				{
					if (m_currentChunkLength >= ExtensibleFormatChunkLength)
					{
						m_parsingStage = ParsingStage::ReadingExtensibleFormatChunk;
						return DecodeChunk(buffer, bufferLength, outputSink);
					}
				}

				// Resolve data format
				if (wavePcmFormat->m_waveFormat == WaveFormatType::WavePcm)
				{
					m_audioMetadata.m_integerFormat = true;
				}
				else if (wavePcmFormat->m_waveFormat == WaveFormatType::WaveIeeeFloat)
				{
					m_audioMetadata.m_integerFormat = false;
				}
				else
				{
					ResetDecoder();
					assert(false);
					return false;
				}

				// Setup metadata
				m_audioMetadata.m_sampleRate = wavePcmFormat->m_sampleRate;
				m_audioMetadata.m_bytesPerSample = wavePcmFormat->m_bitsPerSample / 8u;
				m_audioMetadata.m_channelCount = wavePcmFormat->m_numChannels;
				m_audioMetadata.m_sampleCount = 0u;

				// Test for format consistency
				if (!m_audioMetadata.TestConsistency())
				{
					ResetDecoder();
					assert(false);
					return false;
				}

				m_parsingStage = ParsingStage::AwaitingDataChunk;

				return DecodeChunk(buffer + m_currentChunkLength, bufferLength - m_currentChunkLength, outputSink);
			}
			else // unusual case - using the buffer
			{
				assert(false);
			}
			break;

		case ParsingStage::ReadingExtensibleFormatChunk:
			if (m_internalBuffer.empty() && bufferLength >= ExtensibleFormatChunkLength) // most usual case - parsing without buffering
			{
				const WavePcmExtensibleFormat* wavePcmFormat = reinterpret_cast<const WavePcmExtensibleFormat*>(buffer);

				// Test for chunk length
				if (m_currentChunkLength != FormatChunkLength + sizeof(decltype(wavePcmFormat->m_extensibleSize)) + wavePcmFormat->m_extensibleSize)
				{
					ResetDecoder();
					assert(false);
					return false;
				}

				// Resolve data format
				if (wavePcmFormat->m_baseFormat.m_waveFormat == WaveFormatType::WavePcm)
				{
					m_audioMetadata.m_integerFormat = true;
				}
				else if (wavePcmFormat->m_baseFormat.m_waveFormat == WaveFormatType::WaveIeeeFloat)
				{
					m_audioMetadata.m_integerFormat = false;
				}
				else if (wavePcmFormat->m_baseFormat.m_waveFormat == WaveFormatType::WaveExtensible && m_currentChunkLength >= ExtensibleFormatChunkLength)
				{
					const WaveFormatType waveFormat = static_cast<WaveFormatType>(wavePcmFormat->m_subFormat[0u]);

					if (waveFormat == WaveFormatType::WavePcm)
					{
						m_audioMetadata.m_integerFormat = true;
					}
					else if (waveFormat == WaveFormatType::WaveIeeeFloat)
					{
						m_audioMetadata.m_integerFormat = false;
					}
					else
					{
						ResetDecoder();
						assert(false);
						return false;
					}
				}
				else
				{
					ResetDecoder();
					assert(false);
					return false;
				}

				// Setup metadata
				m_audioMetadata.m_sampleRate = wavePcmFormat->m_baseFormat.m_sampleRate;
				m_audioMetadata.m_bytesPerSample = wavePcmFormat->m_baseFormat.m_bitsPerSample / 8u;
				m_audioMetadata.m_channelCount = wavePcmFormat->m_baseFormat.m_numChannels;
				m_audioMetadata.m_sampleCount = 0u;

				// Test for format consistency
				if (!m_audioMetadata.TestConsistency())
				{
					ResetDecoder();
					assert(false);
					return false;
				}

				m_parsingStage = ParsingStage::AwaitingDataChunk;

				return DecodeChunk(buffer + m_currentChunkLength, bufferLength - m_currentChunkLength, outputSink);
			}
			else // unusual case - using the buffer
			{
				assert(false);
			}
			break;

		case ParsingStage::AwaitingDataChunk:
			if (m_internalBuffer.empty() && bufferLength >= DataHeaderLength) // most usual case - parsing without buffering
			{
				// Read chunk id
				m_currentChunkId = *reinterpret_cast<const uint32_t*>(buffer);

				if (m_currentChunkId == FourccData)
				{
					// Read chunk length
					m_currentChunkLength = *reinterpret_cast<const uint32_t*>(buffer + FourccLength);

					if (m_currentChunkLength != 0u)
					{
						outputSink.SetFormat(m_audioMetadata);
						outputSink.Reserve(m_currentChunkLength / m_audioMetadata.m_channelCount);

						m_parsingStage = ParsingStage::ReadingDataChunk;
					}
					else
					{
						ResetDecoder();
						assert(false);
						return false;
					}
				}
				else
				{
					m_parsingStage = ParsingStage::SkippingUnusedSubChunk;
				}

				return DecodeChunk(buffer + DataHeaderLength, bufferLength - DataHeaderLength, outputSink);
			}
			else // unusual case - using the buffer
			{
				assert(false);
			}
			break;

		case ParsingStage::ReadingDataChunk:
			if (m_internalBuffer.empty()) // most usual case - parsing without buffering (if buffer length aligned for samples block)
			{
				const uint16_t blockAlign = m_audioMetadata.m_channelCount * m_audioMetadata.m_bytesPerSample;
				const uint32_t relevantBufferLength = std::min(static_cast<uint32_t>(bufferLength), m_currentChunkLength);
				const uint32_t alignedBufferLength = relevantBufferLength - relevantBufferLength % blockAlign;

				outputSink.TransmitInterleavedSamples(buffer, alignedBufferLength);

				m_currentChunkLength -= alignedBufferLength;

				if (m_currentChunkLength == 0u)
				{
					m_parsingStage = ParsingStage::ReadingCompleted;
				}
				else if (alignedBufferLength != relevantBufferLength)
				{
					m_internalBuffer.insert(m_internalBuffer.end(), buffer + alignedBufferLength, buffer + relevantBufferLength);
				}
			}
			else // unusual case - using the buffer
			{
				const uint16_t blockAlign = m_audioMetadata.m_channelCount * m_audioMetadata.m_bytesPerSample;
				const uint32_t internalBufferLength = static_cast<uint32_t>(m_internalBuffer.size());
				const uint32_t minimalAlignedLength = internalBufferLength + (blockAlign - m_internalBuffer.size() % blockAlign);
				const uint32_t relevantBufferLength = std::min(static_cast<uint32_t>(bufferLength), m_currentChunkLength);

				if (relevantBufferLength >= minimalAlignedLength)
				{
					// Transmit internal buffer with addition
					const uint32_t incomingAddition = minimalAlignedLength - internalBufferLength;

					m_internalBuffer.insert(m_internalBuffer.end(), buffer, buffer + incomingAddition);

					outputSink.TransmitInterleavedSamples(m_internalBuffer.data(), minimalAlignedLength);

					m_internalBuffer.clear();

					// Test for end of audio
					m_currentChunkLength -= minimalAlignedLength;

					if (m_currentChunkLength == 0u)
					{
						m_parsingStage = ParsingStage::ReadingCompleted;
					}

					// Process the rest of incoming buffer
					return DecodeChunk(buffer + incomingAddition, bufferLength - incomingAddition, outputSink);
				}
				else
				{
					m_internalBuffer.insert(m_internalBuffer.end(), buffer, buffer + relevantBufferLength);
				}
			}
			break;
		}
		return true;
	}

	/**********************************************************************************************/
	void WaveDecoder::ResetDecoder()
	{
		m_parsingStage = ParsingStage::AwaitingInitialHeader;
		m_audioMetadata = AudioMetadata();
		m_currentChunkId = 0u;
		m_currentChunkLength = 0u;
		m_internalBuffer.clear();
	}
}
