#include "platform.h"
#include "MpegDecoder.h"


namespace AudioCodecs
{
/***************************************************************************************************
*** MpegDecoder
***************************************************************************************************/
	MpegDecoder::MpegDecoder()
	{
		class StaticInitializer
		{
		public:
			inline StaticInitializer()
			{
				mpg123_init();
			}

			inline ~StaticInitializer()
			{
				mpg123_exit();
			}
		};

		static const StaticInitializer staticInitializer;

		m_internalDecoder = mpg123_new(nullptr, nullptr);
	}

	/**********************************************************************************************/
	MpegDecoder::~MpegDecoder()
	{
		if (m_internalDecoder)
		{
			mpg123_close(m_internalDecoder);
			mpg123_delete(m_internalDecoder);
			m_internalDecoder = nullptr;
		}
	}

	/**********************************************************************************************
		Output format - int16_t. float is unstable.
	*/
	bool MpegDecoder::DecodeStream(IStream& inputStream, AudioFile& outputAudio)
	{
		// Initialize decoder
		MpegDecoder mpegDecoder;

		mpg123_replace_reader_handle(mpegDecoder.m_internalDecoder, &ReadCallback, &SeekCallback, nullptr);
		mpg123_param(mpegDecoder.m_internalDecoder, MPG123_ADD_FLAGS, MPG123_SKIP_ID3V2, 0.);
		mpg123_param(mpegDecoder.m_internalDecoder, MPG123_RVA, MPG123_RVA_MIX, 0.);

		if (mpg123_open_handle(mpegDecoder.m_internalDecoder, &inputStream) != MPG123_OK)
		{
			assert(false);
			return false;
		}

		// Read first frame for frame info
		mpg123_frameinfo frameInfo;

		if (mpg123_info(mpegDecoder.m_internalDecoder, &frameInfo) != MPG123_OK)
		{
			assert(false);
			return false;
		}

		// Save frame info in appropriate form
		const uint16_t channelCount = frameInfo.mode == MPG123_M_MONO ? 1u : 2u;
		const uint16_t bytesPerSample = static_cast<uint16_t>(mpg123_encsize(MPG123_ENC_SIGNED_16));
		const uint32_t sampleRate = static_cast<uint32_t>(frameInfo.rate);

		// Setup metadata for output audio
		outputAudio.SetFormat(AudioMetadata(sampleRate, bytesPerSample, true, channelCount, 0u));

		// Reserve memory for output audio
		const auto expectedSampleCount = mpg123_length(mpegDecoder.m_internalDecoder);

		if (expectedSampleCount == MPG123_ERR)
		{
			outputAudio.Reserve(static_cast<uint32_t>(inputStream.Length()) * 10u / channelCount);
		}
		else
		{
			outputAudio.Reserve(static_cast<uint32_t>(expectedSampleCount) * bytesPerSample);
		}

		// Setup output format for decoder
		mpg123_format_none(mpegDecoder.m_internalDecoder);
		mpg123_format(mpegDecoder.m_internalDecoder, sampleRate, channelCount, MPG123_ENC_SIGNED_16);

		// Allocate decoding buffer and run the decoding
		size_t decodedBytes = 0u;
		byte* decodingBuffer = reinterpret_cast<byte*>(alloca(BufferSize));

		while (true)
		{
			const int readResult = mpg123_read(mpegDecoder.m_internalDecoder, decodingBuffer, BufferSize, &decodedBytes);

			// This is decoding error, cancel decoding
			if (readResult != MPG123_OK && readResult != MPG123_NEW_FORMAT && readResult != MPG123_DONE)
			{
				outputAudio.SetFormat(AudioMetadata());
				assert(false);
				return false;
			}

			// Trasmit decoded samples to output audio
			if (decodedBytes != 0u)
			{
				assert(decodedBytes % outputAudio.GetMetadata().BlockAlign() == 0u);

				outputAudio.TransmitInterleavedSamples(decodingBuffer, static_cast<uint32_t>(decodedBytes));
			}

			// Decoding is done
			if (readResult == MPG123_DONE)
			{
				break;
			}
		}

		return true;
	}

	/**********************************************************************************************/
	ssize_t MpegDecoder::ReadCallback(void* userStream, void* readingBuffer, size_t bufferSize)
	{
		IStream* sourceStream = reinterpret_cast<IStream*>(userStream);

		return static_cast<ssize_t>(sourceStream->Read(reinterpret_cast<byte*>(readingBuffer), bufferSize));
	}

	/**********************************************************************************************/
	off_t MpegDecoder::SeekCallback(void* userStream, off_t seekPos, int seekOrigin)
	{
		IStream* sourceStream = reinterpret_cast<IStream*>(userStream);

		switch (seekOrigin)
		{
		case SEEK_SET:
			sourceStream->Seek(IStream::SeekOrigin::Begin, seekPos);
			break;

		case SEEK_CUR:
			sourceStream->Seek(IStream::SeekOrigin::Current, seekPos);
			break;

		case SEEK_END:
			sourceStream->Seek(IStream::SeekOrigin::End, seekPos);
			break;
		}

		return static_cast<off_t>(sourceStream->Tell());
	}
}
