#pragma once

#include "AudioSink.h"


namespace AudioCodecs
{
	//-------------------------------------------------------------------------------------------------
	/// AudioFile
	//-------------------------------------------------------------------------------------------------
	class AudioFile : public AudioSink, public boost::noncopyable
	{
	private:
		typedef	std::unique_ptr<VectorStream<>[]> StreamList;

	private:
		AudioMetadata				m_metadata;
		StreamList					m_channels;

	public:
		AudioFile() {}
		virtual ~AudioFile() {}

		virtual void SetFormat(const AudioMetadata& metadata = AudioMetadata()) override;
		virtual void Reserve(uint32_t bytesPerChannel) override;
		virtual void TransmitInterleavedSamples(const void* buffer, uint32_t bufferSize) override;
		virtual void TransmitSplittedSamples(const void* const* buffers, uint32_t bufferSize, uint16_t sampleStride) override;

	public:
		void TakeAudio(AudioFile& sourceAudio);
		void TakeChannel(uint16_t channelIndex, std::vector<byte>& sourceBuffer);
		void ConvertAudioData(AudioDataType newDataType);
		void ConvertChannelData(uint16_t channelIndex, AudioDataType outputDataType, std::vector<byte>& outputChannelData) const;
		void MakeConsistent();
		bool ReadAlignedSamples(std::function<bool(const byte* const* readingBuffer, uint16_t channelCount, uint32_t readedBytesPerChannel)>&& readingCallback, uint32_t bufferSize = 16384u) const;
		bool TestConsistency() const;

	public:
		//---------------------------------------------------------------------------------------------
		__forceinline VectorStream<>* Samples(uint16_t channelIndex)
		{
			assert(m_channels && channelIndex < m_metadata.m_channelCount);

			m_channels[channelIndex].Seek(IStream::SeekOrigin::Begin, 0u);

			return m_channels.get() + channelIndex;
		}

		//---------------------------------------------------------------------------------------------
		__forceinline const AudioMetadata& GetMetadata() const
		{
			return m_metadata;
		}

		//---------------------------------------------------------------------------------------------
		__forceinline AudioMetadata& GetMetadata()
		{
			return m_metadata;
		}

		//---------------------------------------------------------------------------------------------
		__forceinline bool IsEmpty() const
		{
			return !m_metadata.TestConsistency();
		}

	private:
		//---------------------------------------------------------------------------------------------
		template <typename input_t>
		inline void InternalConvertChannelData(uint16_t channelIndex, AudioDataType outputDataType, std::vector<byte>& outputChannelData) const
		{
			switch (outputDataType)
			{
			case AudioDataType::Integer16:
				outputChannelData.resize(sizeof(int16_t) * m_metadata.m_sampleCount);
				samples_converter<input_t, int16_t>::convert(m_channels[channelIndex].Data(), reinterpret_cast<int16_t*>(outputChannelData.data()), m_metadata.m_sampleCount);
				break;

			case AudioDataType::Integer24:
				outputChannelData.resize(sizeof(int24_t) * m_metadata.m_sampleCount);
				samples_converter<input_t, int24_t>::convert(m_channels[channelIndex].Data(), reinterpret_cast<int24_t*>(outputChannelData.data()), m_metadata.m_sampleCount);
				break;

			case AudioDataType::Integer32:
				outputChannelData.resize(sizeof(int32_t) * m_metadata.m_sampleCount);
				samples_converter<input_t, int32_t>::convert(m_channels[channelIndex].Data(), reinterpret_cast<int32_t*>(outputChannelData.data()), m_metadata.m_sampleCount);
				break;

			case AudioDataType::Float32:
				outputChannelData.resize(sizeof(float) * m_metadata.m_sampleCount);
				samples_converter<input_t, float>::convert(m_channels[channelIndex].Data(), reinterpret_cast<float*>(outputChannelData.data()), m_metadata.m_sampleCount);
				break;

			case AudioDataType::Float64:
				outputChannelData.resize(sizeof(double) * m_metadata.m_sampleCount);
				samples_converter<input_t, double>::convert(m_channels[channelIndex].Data(), reinterpret_cast<double*>(outputChannelData.data()), m_metadata.m_sampleCount);
				break;
			}
		}
	};
}
