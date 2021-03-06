#include "platform.h"
#include "AudioConverterAPI.h"
#include "AudioConverter.h"


namespace audioconvert
{
	//-------------------------------------------------------------------------------------------------
	class AudioConverterExportsStub
	{
	public:
		typedef std::vector<byte> AudioBlob;
		typedef aux::FixedArray<AudioBlob> MultipleAudioBlob;

		typedef boost::variant<std::nullptr_t, AudioBlob, MultipleAudioBlob> Result;

	public:
		AudioConverter				m_audioConverter;

		byte*						m_inputData = nullptr;
		size_t						m_inputLength = 0u;
		AudioFormat					m_inputFormat = AudioFormat::AudioUnknown;
		AudioFile					m_decodedAudio;
		Result						m_result;

	public:
		//---------------------------------------------------------------------------------------------
		inline bool EnsureInputAudio()
		{
			if (m_decodedAudio.IsEmpty())
			{
				// Validate input data
				if (!m_inputData || m_inputLength == 0u)
				{
					assert(false);
					return false;
				}

				// Try to decode input data
				aux::FixedStream inputStream(m_inputData, m_inputLength);

				if (!m_audioConverter.DecodeAudio(inputStream, m_inputFormat, m_decodedAudio))
				{
					assert(false);
					return false;
				}
			}
			return true;
		}
	};

	//-------------------------------------------------------------------------------------------------
	extern "C" void* AudioConverter_Allocate()
	{
		return new AudioConverterExportsStub();
	}

	//-------------------------------------------------------------------------------------------------
	extern "C" void AudioConverter_Free(void* audioConverter)
	{
		delete reinterpret_cast<AudioConverterExportsStub*>(audioConverter);
	}

	//-------------------------------------------------------------------------------------------------
	extern "C" void AudioConverter_SetInput(void* audioConverter, void* inputData, size_t inputLength, AudioFormat inputFormat)
	{
		auto* audioConverterStub = reinterpret_cast<AudioConverterExportsStub*>(audioConverter);

		audioConverterStub->m_inputData = reinterpret_cast<byte*>(inputData);
		audioConverterStub->m_inputLength = inputLength;
		audioConverterStub->m_inputFormat = inputFormat;
		audioConverterStub->m_decodedAudio.SetFormat();
		audioConverterStub->m_result = nullptr;
	}

	//-------------------------------------------------------------------------------------------------
	extern "C" bool AudioConverter_ResampleAudio(void* audioConverter, uint32_t outputSampleRate)
	{
		auto* audioConverterStub = reinterpret_cast<AudioConverterExportsStub*>(audioConverter);

		// Clear current results
		audioConverterStub->m_result = nullptr;

		// Check for input audio
		if (!audioConverterStub->EnsureInputAudio())
		{
			assert(false);
			return false;
		}

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	extern "C" bool AudioConverter_ConvertAudio(void* audioConverter, AudioFormat outputFormat)
	{
		auto* audioConverterStub = reinterpret_cast<AudioConverterExportsStub*>(audioConverter);

		// Clear current results
		audioConverterStub->m_result = nullptr;

		// Check for input audio
		if (!audioConverterStub->EnsureInputAudio())
		{
			assert(false);
			return false;
		}

		// Encode audio
		audioConverterStub->m_result = AudioConverterExportsStub::AudioBlob();

		aux::VectorStream<false> outputStream(boost::strict_get<AudioConverterExportsStub::AudioBlob>(audioConverterStub->m_result));

		if (!audioConverterStub->m_audioConverter.EncodeAudio(audioConverterStub->m_decodedAudio, outputStream, outputFormat))
		{
			assert(false);
			return false;
		}

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	extern "C" bool AudioConverter_ExtractAudioData(void* audioConverter, AudioDataType outputDataType)
	{
		auto* audioConverterStub = reinterpret_cast<AudioConverterExportsStub*>(audioConverter);

		// Clear current results
		audioConverterStub->m_result = nullptr;

		// Check for input audio
		if (!audioConverterStub->EnsureInputAudio())
		{
			assert(false);
			return false;
		}

		// Convert each channel of the decoded audio into specified samples data format
		audioConverterStub->m_result = AudioConverterExportsStub::MultipleAudioBlob();

		const auto& metadata = audioConverterStub->m_decodedAudio.GetMetadata();

		auto& outputChannels = boost::strict_get<AudioConverterExportsStub::MultipleAudioBlob>(audioConverterStub->m_result);
		outputChannels.Allocate(metadata.m_channelCount);

		for (uint16_t channelIndex = 0u; channelIndex != metadata.m_channelCount; ++channelIndex)
		{
			audioConverterStub->m_decodedAudio.ConvertChannelData(channelIndex, static_cast<AudioDataType>(outputDataType), outputChannels[channelIndex]);
		}

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	extern "C" bool AudioConverter_SplitAudio(void* audioConverter, AudioFormat outputFormat, AudioSegment* audioSegments, uint32_t audioSegmentsCount)
	{
		auto* audioConverterStub = reinterpret_cast<AudioConverterExportsStub*>(audioConverter);

		// Clear current results
		audioConverterStub->m_result = nullptr;

		// Validate arguments
		if (audioSegmentsCount == 0u || audioSegmentsCount > 1024u)
		{
			assert(false);
			return false;
		}

		// Check for input audio
		if (!audioConverterStub->EnsureInputAudio())
		{
			assert(false);
			return false;
		}

		// Validate audio segments and segmentize decoded audio
		audioConverterStub->m_result = AudioConverterExportsStub::MultipleAudioBlob();

		const auto& metadata = audioConverterStub->m_decodedAudio.GetMetadata();

		auto& outputSegments = boost::strict_get<AudioConverterExportsStub::MultipleAudioBlob>(audioConverterStub->m_result);
		outputSegments.Allocate(audioSegmentsCount);

		for (uint32_t segmentIndex = 0u; segmentIndex != audioSegmentsCount; ++segmentIndex)
		{
			// Validate channel index
			if (audioSegments[segmentIndex].m_channelIndex >= metadata.m_channelCount)
			{
				assert(false);
				continue;
			}

			// Calculate samples range
			const uint32_t samplesStart = audioSegments[segmentIndex].m_timeStart * metadata.m_sampleRate / 1000u;
			const uint32_t samplesCount = audioSegments[segmentIndex].m_timeLength == 0u ? metadata.m_sampleCount - samplesStart : audioSegments[segmentIndex].m_timeLength * metadata.m_sampleRate / 1000u;

			// Validate samples range
			if (samplesStart + samplesCount > metadata.m_sampleCount || samplesCount == 0u)
			{
				assert(false);
				continue;
			}

			// Compose audio segment
			const uint32_t segmentShift = metadata.m_bytesPerSample * samplesStart;
			const uint32_t segmentSize = metadata.m_bytesPerSample * samplesCount;
			const void* segmentSamples = audioConverterStub->m_decodedAudio.Samples(audioSegments[segmentIndex].m_channelIndex)->Data() + segmentShift;

			AudioFile outputSegment;
			outputSegment.SetFormat(AudioMetadata(metadata.m_sampleRate, metadata.m_bytesPerSample, metadata.m_integerFormat, 1u, samplesCount));
			outputSegment.TransmitSplittedSamples(&segmentSamples, segmentSize, metadata.m_bytesPerSample);

			// Setup output stream and encode audio segment
			aux::VectorStream<false> outputStream(outputSegments[segmentIndex]);

			if (!audioConverterStub->m_audioConverter.EncodeAudio(outputSegment, outputStream, outputFormat))
			{
				audioConverterStub->m_result = nullptr;
				assert(false);
				return false;
			}
		}

		return false;
	}

	//-------------------------------------------------------------------------------------------------
	extern "C" uint32_t AudioConverter_GetResultsCount(void* audioConverter)
	{
		class Visitor : public boost::static_visitor<uint32_t>
		{
		public:
			constexpr uint32_t operator ()(const std::nullptr_t&) const
			{
				return 0u;
			}

			constexpr uint32_t operator ()(const AudioConverterExportsStub::AudioBlob& audioBlob) const
			{
				return 1u;
			}

			inline uint32_t operator ()(const AudioConverterExportsStub::MultipleAudioBlob& multipleAudioBlob) const
			{
				return static_cast<uint32_t>(multipleAudioBlob.size());
			}
		};

		// Visit results
		auto* audioConverterStub = reinterpret_cast<AudioConverterExportsStub*>(audioConverter);

		return boost::apply_visitor(Visitor(), audioConverterStub->m_result);
	}

	//-------------------------------------------------------------------------------------------------
	extern "C" AudioData AudioConverter_GetResult(void* audioConverter, uint32_t resultIndex)
	{
		class Visitor : public boost::static_visitor<AudioData>
		{
		private:
			const uint32_t				m_resultIndex;

		public:
			inline Visitor(uint32_t resultIndex) : m_resultIndex(resultIndex)
			{
			}

			inline AudioData operator ()(const std::nullptr_t&) const
			{
				return AudioData{ nullptr, 0u };
			}

			inline AudioData operator ()(const AudioConverterExportsStub::AudioBlob& audioBlob) const
			{
				return AudioData{ audioBlob.data(), audioBlob.size() };
			}

			inline AudioData operator ()(const AudioConverterExportsStub::MultipleAudioBlob& multipleAudioBlob) const
			{
				assert(m_resultIndex < multipleAudioBlob.size());
				return AudioData{ multipleAudioBlob[m_resultIndex].data(), multipleAudioBlob[m_resultIndex].size() };
			}
		};

		// Visit results
		auto* audioConverterStub = reinterpret_cast<AudioConverterExportsStub*>(audioConverter);

		return boost::apply_visitor(Visitor(resultIndex), audioConverterStub->m_result);
	}

	//-------------------------------------------------------------------------------------------------
	extern "C" bool AudioConverter_EnumerateResults(void* audioConverter, ResultsEnumerator resultsEnumerator)
	{
		class Visitor : public boost::static_visitor<bool>
		{
		private:
			ResultsEnumerator			m_resultsEnumerator;

		public:
			inline Visitor(ResultsEnumerator resultsEnumerator) : m_resultsEnumerator(resultsEnumerator)
			{
			}

			inline bool operator ()(const std::nullptr_t&) const
			{
				return false;
			}

			inline bool operator ()(const AudioConverterExportsStub::AudioBlob& audioBlob) const
			{
				m_resultsEnumerator(audioBlob.data(), audioBlob.size());
				return true;
			}

			inline bool operator ()(const AudioConverterExportsStub::MultipleAudioBlob& multipleAudioBlob) const
			{
				for (uint32_t audioBlobIndex = 0; audioBlobIndex != multipleAudioBlob.size(); ++audioBlobIndex)
				{
					if (!m_resultsEnumerator(multipleAudioBlob[audioBlobIndex].data(), multipleAudioBlob[audioBlobIndex].size()))
					{
						break;
					}
				}
				return true;
			}
		};

		// Visit results
		auto* audioConverterStub = reinterpret_cast<AudioConverterExportsStub*>(audioConverter);

		return boost::apply_visitor(Visitor(resultsEnumerator), audioConverterStub->m_result);
	}

	//-------------------------------------------------------------------------------------------------
	extern "C" const char* AudioConverter_LastError(void* audioConverter)
	{
		auto* audioConverterStub = reinterpret_cast<AudioConverterExportsStub*>(audioConverter);

		return audioConverterStub->m_audioConverter.LastError().c_str();
	}
}
