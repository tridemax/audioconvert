#include "platform.h"
#include "MpegEncoder.h"


namespace AudioCodecs
{
	//-------------------------------------------------------------------------------------------------
	MpegEncoder::MpegEncoder() : m_internalEncoder(lame_init())
	{
	}

	//-------------------------------------------------------------------------------------------------
	MpegEncoder::~MpegEncoder()
	{
		if (m_internalEncoder)
		{
			lame_close(m_internalEncoder);
			m_internalEncoder = nullptr;
		}
	}

	//-------------------------------------------------------------------------------------------------
	bool MpegEncoder::EncodeSamples(AudioFile& inputAudio, IStream& outputStream)
	{
		const auto& metadata = inputAudio.GetMetadata();
		const auto dataType = metadata.GetDataType();

		// Initialize encoder
		lame_set_num_samples(m_internalEncoder, metadata.m_sampleCount);
		lame_set_in_samplerate(m_internalEncoder, metadata.m_sampleRate);
		lame_set_out_samplerate(m_internalEncoder, ResolveOutputSampleRate(metadata.m_sampleRate));
		lame_set_num_channels(m_internalEncoder, metadata.m_channelCount);
		lame_set_mode(m_internalEncoder, metadata.m_channelCount == 1u ? MPEG_mode::MONO : MPEG_mode::STEREO);
		lame_set_asm_optimizations(m_internalEncoder, asm_optimizations::SSE, 1);
		lame_set_VBR(m_internalEncoder, vbr_mtrh);
		lame_set_VBR_quality(m_internalEncoder, 2);
		lame_set_quality(m_internalEncoder, 2);

		if (lame_init_params(m_internalEncoder) == -1)
		{
			assert(false);
			return false;
		}

		// Reserve memory for output stream
		outputStream.Reserve(metadata.OverallDataSize() / 8u);

		// Allocate output buffer (in samples units)
		constexpr uint32_t bufferSize = 16384u;
		const uint32_t outputBufferLength = bufferSize + bufferSize / 4u + 7200u;

		std::unique_ptr<byte[]> outputBuffer(new byte[outputBufferLength]);

		// Allocate input buffer only for int24 samples format (lame does not support this)
		std::unique_ptr<int32_t[]> inputBuffer;

		if (dataType == AudioDataType::Integer24)
		{
			inputBuffer.reset(new int32_t[bufferSize * std::min<uint16_t>(metadata.m_channelCount, 2u)]);
		}

		// Do encoding and stream out encoded mpeg blocks
		const bool encodingSuccessfull = inputAudio.ReadAlignedSamples([&](const byte* const* readingBuffer, uint16_t channelCount, uint32_t readedBytesPerChannel) -> bool
		{
			const uint32_t sampleCount = readedBytesPerChannel / metadata.m_bytesPerSample;
			assert(sampleCount <= bufferSize);

			int encodingResult = 0;

			switch (dataType)
			{
			case AudioDataType::Integer16:		// Integer16
				encodingResult = lame_encode_buffer(
					m_internalEncoder,
					reinterpret_cast<const int16_t*>(readingBuffer[0u]),
					metadata.m_channelCount == 1u ? nullptr : reinterpret_cast<const int16_t*>(readingBuffer[1u]),
					sampleCount,
					outputBuffer.get(),
					static_cast<int>(outputBufferLength));
				break;

			case AudioDataType::Integer24:		// Integer24 (lame does not support this)
				if (metadata.m_channelCount == 1u)
				{
					samples_converter<int24_t, int32_t>::convert(readingBuffer[0u], inputBuffer.get(), sampleCount);
				}
				else
				{
					samples_converter<int24_t, int32_t>::convert(readingBuffer[0u], inputBuffer.get(), sampleCount);
					samples_converter<int24_t, int32_t>::convert(readingBuffer[1u], inputBuffer.get() + sampleCount, sampleCount);
				}

				encodingResult = lame_encode_buffer_int(
					m_internalEncoder,
					inputBuffer.get(),
					metadata.m_channelCount == 1u ? nullptr : inputBuffer.get() + sampleCount,
					sampleCount,
					outputBuffer.get(),
					static_cast<int>(outputBufferLength));
				break;

			case AudioDataType::Integer32:		// Integer32
				encodingResult = lame_encode_buffer_int(
					m_internalEncoder,
					reinterpret_cast<const int32_t*>(readingBuffer[0u]),
					metadata.m_channelCount == 1u ? nullptr : reinterpret_cast<const int32_t*>(readingBuffer[1u]),
					sampleCount,
					outputBuffer.get(),
					static_cast<int>(outputBufferLength));
				break;

			case AudioDataType::Float32:		// Float32
				encodingResult = lame_encode_buffer_ieee_float(
					m_internalEncoder,
					reinterpret_cast<const float*>(readingBuffer[0u]),
					metadata.m_channelCount == 1u ? nullptr : reinterpret_cast<const float*>(readingBuffer[1u]),
					sampleCount,
					outputBuffer.get(),
					static_cast<int>(outputBufferLength));
				break;

			case AudioDataType::Float64:		// Float64
				encodingResult = lame_encode_buffer_ieee_double(
					m_internalEncoder,
					reinterpret_cast<const double*>(readingBuffer[0u]),
					metadata.m_channelCount == 1u ? nullptr : reinterpret_cast<const double*>(readingBuffer[1u]),
					sampleCount,
					outputBuffer.get(),
					static_cast<int>(outputBufferLength));
				break;
			}

			if (encodingResult > 0)
			{
				outputStream.Write(outputBuffer.get(), encodingResult);
			}
			else if (encodingResult < 0)
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

		// Finalize encoder
		const int encodingResult = lame_encode_flush(m_internalEncoder, outputBuffer.get(), static_cast<int>(outputBufferLength));

		if (encodingResult > 0)
		{
			outputStream.Write(outputBuffer.get(), encodingResult);
		}
		else if (encodingResult < 0)
		{
			ResetEncoder();
			assert(false);
			return false;
		}

		// Reset encoder
		ResetEncoder();

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	void MpegEncoder::ResetEncoder()
	{
	}
}
