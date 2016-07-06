#include "platform.h"
#include "AudioFile.h"


namespace audioconvert
{
	//-------------------------------------------------------------------------------------------------
	void AudioFile::SetFormat(const AudioMetadata& metadata)
	{
		if (metadata.m_channelCount == 0u)
		{
			m_channels.reset();
		}
		else if (m_metadata.m_channelCount == metadata.m_channelCount)
		{
			for (uint16_t channelIndex = 0u; channelIndex != metadata.m_channelCount; ++channelIndex)
			{
				m_channels[channelIndex].Seek(aux::IStream::SeekOrigin::Begin, 0u);
			}
		}
		else
		{
			m_channels.reset(new aux::VectorStream<>[metadata.m_channelCount]);
		}

		m_metadata.m_sampleRate = metadata.m_sampleRate;
		m_metadata.m_bytesPerSample = metadata.m_bytesPerSample;
		m_metadata.m_integerFormat = metadata.m_integerFormat;
		m_metadata.m_channelCount = metadata.m_channelCount;
		m_metadata.m_sampleCount = 0u;
	}

	//-------------------------------------------------------------------------------------------------
	void AudioFile::Reserve(uint32_t bytesPerChannel)
	{
		for (uint16_t channelIndex = 0u; channelIndex != m_metadata.m_channelCount; ++channelIndex)
		{
			m_channels[channelIndex].Reserve(bytesPerChannel);
		}
	}

	//-------------------------------------------------------------------------------------------------
	void AudioFile::TransmitInterleavedSamples(const void* buffer, uint32_t bufferSize)
	{
		const uint32_t blockAlign = m_metadata.m_bytesPerSample * m_metadata.m_channelCount;
		const uint32_t blockCount = bufferSize / blockAlign;

		assert(bufferSize % blockAlign == 0u);

		for (uint32_t blockIndex = 0u; blockIndex != blockCount; ++blockIndex)
		{
			for (uint16_t channelIndex = 0u; channelIndex != m_metadata.m_channelCount; ++channelIndex)
			{
				m_channels[channelIndex].FastWrite(reinterpret_cast<const byte*>(buffer) + blockIndex * blockAlign + m_metadata.m_bytesPerSample * channelIndex, m_metadata.m_bytesPerSample);
			}
		}

		m_metadata.m_sampleCount += blockCount;
	}

	//-------------------------------------------------------------------------------------------------
	void AudioFile::TransmitSplittedSamples(const void* const* buffers, uint32_t bufferSize, uint16_t sampleStride)
	{
		const uint32_t sampleCount = static_cast<uint32_t>(bufferSize / sampleStride);

		assert(bufferSize % sampleStride == 0u);

		if (sampleStride == m_metadata.m_bytesPerSample)
		{
			for (uint16_t channelIndex = 0u; channelIndex != m_metadata.m_channelCount; ++channelIndex)
			{
				m_channels[channelIndex].FastWrite(buffers[channelIndex], bufferSize);
			}
		}
		else
		{
			for (uint16_t channelIndex = 0u; channelIndex != m_metadata.m_channelCount; ++channelIndex)
			{
				for (uint32_t sampleIndex = 0u; sampleIndex != sampleCount; ++sampleIndex)
				{
					const byte* sampleShift = reinterpret_cast<const byte*>(buffers[channelIndex]) + sampleIndex * sampleStride;

					m_channels[channelIndex].FastWrite(sampleShift, m_metadata.m_bytesPerSample);
				}
			}
		}

		m_metadata.m_sampleCount += sampleCount;
	}

	//-------------------------------------------------------------------------------------------------
	void AudioFile::TakeAudio(AudioFile& sourceAudio)
	{
		m_metadata = sourceAudio.m_metadata;
		m_channels.reset(sourceAudio.m_channels.release());
	}

	//-------------------------------------------------------------------------------------------------
	void AudioFile::TakeChannel(uint16_t channelIndex, std::vector<byte>& sourceBuffer)
	{
		assert(m_channels && channelIndex < m_metadata.m_channelCount);

		if (m_channels && channelIndex < m_metadata.m_channelCount)
		{
			m_channels[channelIndex].Swap(sourceBuffer);
		}
	}

	//-------------------------------------------------------------------------------------------------
	void AudioFile::ConvertAudioData(AudioDataType newDataType)
	{
		std::vector<byte> outputVector;

		for (uint16_t channelIndex = 0u; channelIndex != m_metadata.m_channelCount; ++channelIndex)
		{
			ConvertChannelData(channelIndex, newDataType, outputVector);
			m_channels[channelIndex].Swap(outputVector);
		}

		m_metadata.SetDataType(newDataType);
	}

	//-------------------------------------------------------------------------------------------------
	void AudioFile::ConvertChannelData(uint16_t channelIndex, AudioDataType outputDataType, std::vector<byte>& outputChannelData) const
	{
		switch (m_metadata.GetDataType())
		{
		case AudioDataType::Integer16:
			InternalConvertChannelData<int16_t>(channelIndex, outputDataType, outputChannelData);
			break;

		case AudioDataType::Integer24:
			InternalConvertChannelData<int24_t>(channelIndex, outputDataType, outputChannelData);
			break;

		case AudioDataType::Integer32:
			InternalConvertChannelData<int32_t>(channelIndex, outputDataType, outputChannelData);
			break;

		case AudioDataType::Float32:
			InternalConvertChannelData<float>(channelIndex, outputDataType, outputChannelData);
			break;

		case AudioDataType::Float64:
			InternalConvertChannelData<double>(channelIndex, outputDataType, outputChannelData);
			break;
		}
	}

	//-------------------------------------------------------------------------------------------------
	void AudioFile::MakeConsistent()
	{
		if (m_channels)
		{
			// Search for minimal samples quantity
			m_metadata.m_sampleCount = static_cast<uint32_t>(m_channels[0u].Length() / m_metadata.m_bytesPerSample);

			for (uint16_t channelIndex = 1u; channelIndex != m_metadata.m_channelCount; ++channelIndex)
			{
				const uint32_t currentSamplesQuantity = static_cast<uint32_t>(m_channels[channelIndex].Length() / m_metadata.m_bytesPerSample);

				if (m_metadata.m_sampleCount > currentSamplesQuantity)
				{
					m_metadata.m_sampleCount = currentSamplesQuantity;
				}
			}

			// Resize channels buffers
			for (uint16_t channelIndex = 0u; channelIndex != m_metadata.m_channelCount; ++channelIndex)
			{
				m_channels[channelIndex].SetLength(m_metadata.m_sampleCount * m_metadata.m_bytesPerSample);
			}
		}
		else
		{
			m_metadata.m_sampleCount = 0u;
		}
	}

	//-------------------------------------------------------------------------------------------------
	bool AudioFile::ReadAlignedSamples(std::function<bool(const byte* const* readingBuffer, uint16_t channelCount, uint32_t readedBytesPerChannel)>&& readingCallback, uint32_t bufferSize) const
	{
		// Calculate buffer alignment and size
		const uint32_t blockAlign = m_metadata.m_bytesPerSample;
		const uint32_t alignedBufferSize = bufferSize - bufferSize % blockAlign;

		uint32_t totalBytes = m_metadata.m_bytesPerSample * m_metadata.m_sampleCount;
		uint32_t remainedBytes = totalBytes;

		// Allocate storage for reading buffers
		const byte** readingSamplesData = reinterpret_cast<const byte**>(alloca(sizeof(byte*) * m_metadata.m_channelCount));

		// Read channels data
		while (remainedBytes != 0u)
		{
			const uint32_t readBytes = std::min(alignedBufferSize, remainedBytes);
			assert(readBytes != 0u && readBytes % blockAlign == 0u);

			for (uint16_t channelIndex = 0u; channelIndex != m_metadata.m_channelCount; ++channelIndex)
			{
				readingSamplesData[channelIndex] = m_channels[channelIndex].Data() + totalBytes - remainedBytes;
			}

			if (!readingCallback(readingSamplesData, m_metadata.m_channelCount, readBytes))
			{
				return false;
			}

			remainedBytes -= readBytes;
		}

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool AudioFile::TestConsistency() const
	{
		if (m_channels)
		{
			for (uint16_t channelIndex = 0u; channelIndex != m_metadata.m_channelCount; ++channelIndex)
			{
				if (m_channels[channelIndex].Length() != m_metadata.m_bytesPerSample * m_metadata.m_sampleCount)
				{
					return false;
				}
			}
		}
		else if (m_metadata.m_sampleCount != 0u)
		{
			return false;
		}
		return true;
	}
}
