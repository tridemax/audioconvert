#include "platform.h"
#include "WaveEncoder.h"
#include "WaveDecoder.h"


namespace AudioCodecs
{
/***************************************************************************************************
*** WaveEncoder
***************************************************************************************************/
	bool WaveEncoder::EncodeSamples(AudioFile& inputAudio, IStream& outputStream)
	{
		auto& metadata = inputAudio.GetMetadata();

		// Write headers of "RIFF", "WAVE" and "fmt " chunks
		const uint32_t waveHeader[] =
		{
			WaveDecoder::FourccRiff,
			36u + metadata.m_sampleCount * metadata.m_channelCount * metadata.m_bytesPerSample,
			WaveDecoder::FourccWave,
			WaveDecoder::FourccFormat,
			WaveDecoder::FormatChunkLength
		};

		outputStream.Write(waveHeader, _countof(waveHeader) * 4u);

		// Write content of "fmt " chunk
		WavePcmFormat wavePcmFormat;

		wavePcmFormat.m_waveFormat = metadata.m_integerFormat ? WaveFormatType::WavePcm : WaveFormatType::WaveIeeeFloat;
		wavePcmFormat.m_numChannels = metadata.m_channelCount;
		wavePcmFormat.m_sampleRate = metadata.m_sampleRate;
		wavePcmFormat.m_byteRate = metadata.m_sampleRate * metadata.m_channelCount * metadata.m_bytesPerSample;
		wavePcmFormat.m_blockAlign = metadata.m_channelCount * metadata.m_bytesPerSample;
		wavePcmFormat.m_bitsPerSample = metadata.m_bytesPerSample * 8u;

		outputStream.Write(&wavePcmFormat, sizeof(wavePcmFormat));

		// Write header of "data" chunk
		const uint32_t dataHeader[] =
		{
			WaveDecoder::FourccData,
			metadata.m_sampleCount * metadata.m_channelCount * metadata.m_bytesPerSample
		};

		outputStream.Write(dataHeader, _countof(dataHeader) * 4u);

		// Write content of "data" chunk
		inputAudio.ReadAlignedSamples([&](const byte* const* readingBuffer, uint16_t channelCount, uint32_t readedBytesPerChannel) -> bool
		{
			const uint32_t sampleCount = readedBytesPerChannel / metadata.m_bytesPerSample;

			for (uint32_t sampleIndex = 0u; sampleIndex != sampleCount; ++sampleIndex)
			{
				for (uint16_t channelIndex = 0u; channelIndex != channelCount; ++channelIndex)
				{
					outputStream.Write(readingBuffer[channelIndex] + sampleIndex * metadata.m_bytesPerSample, metadata.m_bytesPerSample);
				}
			}

			return true;
		});

		// Write header and content of "fact" chunk
		const uint32_t factChunk[] =
		{
			WaveDecoder::FourccFact,
			4u,
			metadata.m_sampleCount
		};

		outputStream.Write(factChunk, _countof(factChunk) * 4u);

		// Assert for consistency of encoded audio
		assert(metadata.OverallDataSize() + sizeof(WavePcmFormat) + (_countof(waveHeader) + _countof(dataHeader) + _countof(factChunk)) * 4u == outputStream.Tell());

		return true;
	}
}
