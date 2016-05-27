#include "platform.h"
#include "FlacEncoder.h"


namespace AudioCodecs
{
/***************************************************************************************************
*** FlacEncoder
***************************************************************************************************/
	FlacEncoder::FlacEncoder() : m_internalEncoder(FLAC__stream_encoder_new())
	{
	}

	/**********************************************************************************************/
	FlacEncoder::~FlacEncoder()
	{
		if (m_internalEncoder)
		{
			FLAC__stream_encoder_delete(m_internalEncoder);
			m_internalEncoder = nullptr;
		}
	}

	/**********************************************************************************************/
	bool FlacEncoder::EncodeSamples(AudioFile& inputAudio, IStream& outputStream)
	{
		const auto& metadata = inputAudio.GetMetadata();
		const auto dataType = metadata.GetDataType();

		// Set encoding settings
		FLAC__stream_encoder_set_verify(m_internalEncoder, false);
		FLAC__stream_encoder_set_compression_level(m_internalEncoder, 8u);
		FLAC__stream_encoder_set_channels(m_internalEncoder, metadata.m_channelCount);
		FLAC__stream_encoder_set_bits_per_sample(m_internalEncoder, metadata.m_bytesPerSample * 8u);
		FLAC__stream_encoder_set_sample_rate(m_internalEncoder, metadata.m_sampleRate);
		FLAC__stream_encoder_set_total_samples_estimate(m_internalEncoder, metadata.m_sampleCount);
		FLAC__stream_encoder_set_metadata(m_internalEncoder, nullptr, 0);

		// Initialize encoder
		if (FLAC__stream_encoder_init_stream(m_internalEncoder, &WriteCallback, &SeekCallback, &TellCallback, nullptr, &outputStream) != FLAC__STREAM_ENCODER_INIT_STATUS_OK)
		{
			ResetEncoder();
			assert(false);
			return false;
		}

		// Allocate input buffer (in samples units)
		constexpr uint32_t bufferSize = 16384u;

		std::unique_ptr<int32_t[]> inputBuffer(new int32_t[metadata.m_channelCount * bufferSize]);

		int32_t** preparedSamplesData = reinterpret_cast<int32_t**>(_alloca(sizeof(int32_t*) * metadata.m_channelCount));

		// Do encoding and stream out encoded flac blocks
		const bool encodingSuccessfull = inputAudio.ReadAlignedSamples([&](const byte* const* readingBuffer, uint16_t channelCount, uint32_t readedBytesPerChannel) -> bool
		{
			const uint32_t sampleCount = readedBytesPerChannel / metadata.m_bytesPerSample;
			assert(sampleCount <= bufferSize);

			// Iterate over each channel and fill samples buffer
			for (uint16_t channelIndex = 0u; channelIndex != channelCount; ++channelIndex)
			{
				preparedSamplesData[channelIndex] = inputBuffer.get() + sampleCount * channelIndex;

				switch (dataType)
				{
				case AudioDataType::Integer16:		// assign int16_t source to int32_t encoding buffer
					for (uint32_t sampleIndex = 0u; sampleIndex != sampleCount; ++sampleIndex)
					{
						preparedSamplesData[channelIndex][sampleIndex] = *reinterpret_cast<const int16_t*>(readingBuffer[channelIndex] + sampleIndex * sizeof(int16_t));
					}
					break;

				case AudioDataType::Integer24:		// assign int24_t source to int32_t encoding buffer
					for (uint32_t sampleIndex = 0u; sampleIndex != sampleCount; ++sampleIndex)
					{
						preparedSamplesData[channelIndex][sampleIndex] = *reinterpret_cast<const int24_t*>(readingBuffer[channelIndex] + sampleIndex * sizeof(int24_t));
					}
					break;

				case AudioDataType::Integer32:		// copy int32_t source to int32_t encoding buffer
					memcpy(preparedSamplesData[channelIndex], readingBuffer[channelIndex], sampleCount * sizeof(int32_t));
					break;

				case AudioDataType::Float32:		// convert float source to int32_t encoding buffer
					samples_converter<float, int32_t>::convert(readingBuffer[channelIndex], preparedSamplesData[channelIndex], sampleCount);
					break;

				case AudioDataType::Float64:		// convert double source to int32_t encoding buffer
					samples_converter<double, int32_t>::convert(readingBuffer[channelIndex], preparedSamplesData[channelIndex], sampleCount);
					break;
				}
			}

			// Transmit input buffer to encoder
			if (!FLAC__stream_encoder_process(m_internalEncoder, preparedSamplesData, sampleCount))
			{
				assert(false);
				return false;
			}

			return true;
		}, bufferSize * metadata.m_bytesPerSample);

		// Test for successfull encoding
		if (!encodingSuccessfull)
		{
			ResetEncoder();
			assert(false);
			return false;
		}

		// Reset encoder
		ResetEncoder();

		return true;
	}

	/**********************************************************************************************/
	void FlacEncoder::ResetEncoder()
	{
		FLAC__stream_encoder_finish(m_internalEncoder);
	}

	/**********************************************************************************************/
	FLAC__StreamEncoderWriteStatus FlacEncoder::WriteCallback(const FLAC__StreamEncoder* encoder, const FLAC__byte* buffer, size_t bytes, unsigned int samples, unsigned int current_frame, void* client_data)
	{
		IStream* outputStream = reinterpret_cast<IStream*>(client_data);

		if (outputStream->Write(buffer, bytes) != bytes)
		{
			assert(false);
			return FLAC__STREAM_ENCODER_WRITE_STATUS_FATAL_ERROR;
		}

		return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
	}

	/**********************************************************************************************/
	FLAC__StreamEncoderSeekStatus FlacEncoder::SeekCallback(const FLAC__StreamEncoder* encoder, FLAC__uint64 absolute_byte_offset, void* client_data)
	{
		IStream* outputStream = reinterpret_cast<IStream*>(client_data);

		if (outputStream->Seek(IStream::SeekOrigin::Begin, static_cast<intptr_t>(absolute_byte_offset)) != absolute_byte_offset)
		{
			assert(false);
			return FLAC__STREAM_ENCODER_SEEK_STATUS_ERROR;
		}

		return FLAC__STREAM_ENCODER_SEEK_STATUS_OK;
	}

	/**********************************************************************************************/
	FLAC__StreamEncoderTellStatus FlacEncoder::TellCallback(const FLAC__StreamEncoder* encoder, FLAC__uint64* absolute_byte_offset, void* client_data)
	{
		IStream* outputStream = reinterpret_cast<IStream*>(client_data);

		*absolute_byte_offset = outputStream->Tell();

		return FLAC__STREAM_ENCODER_TELL_STATUS_OK;
	}
}
