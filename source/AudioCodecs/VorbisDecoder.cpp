#include "platform.h"
#include "VorbisDecoder.h"


namespace audioconvert
{
/***************************************************************************************************
*** VorbisDecoder
***************************************************************************************************/
	VorbisDecoder::VorbisDecoder() : m_testOpened(false)
	{
		memset(&m_internalDecoder, 0, sizeof(OggVorbis_File));
	}

	/**********************************************************************************************/
	VorbisDecoder::~VorbisDecoder()
	{
		ov_clear(&m_internalDecoder);
	}

	/**********************************************************************************************/
	bool VorbisDecoder::TestStream(aux::IMemoryStream& inputStream)
	{
		// Test for vorbis stream
		if (ov_test_callbacks(&inputStream, &m_internalDecoder, nullptr, 0, VorbisCallbacks) != 0)
		{
			return false;
		}

		m_testOpened = true;

		return true;
	}

	/**********************************************************************************************/
	bool VorbisDecoder::DecodeStream(aux::IMemoryStream& inputStream, AudioSink& outputSink)
	{
		// Initialize decoder
		if (m_testOpened)
		{
			if (ov_test_open(&m_internalDecoder) != 0)
			{
				ResetDecoder();
				assert(false);
				return false;
			}
		}
		else
		{
			if (ov_open_callbacks(&inputStream, &m_internalDecoder, nullptr, 0, VorbisCallbacks) != 0)
			{
				ResetDecoder();
				assert(false);
				return false;
			}
		}

		// Read vorbis info
		vorbis_info* vorbisInfo = ov_info(&m_internalDecoder, -1);

		if (!vorbisInfo)
		{
			ResetDecoder();
			assert(false);
			return false;
		}

		// Save metadata in appropriate form
		const uint32_t sampleRate = static_cast<uint32_t>(vorbisInfo->rate);
		constexpr uint16_t bytesPerSample = sizeof(float);
		const uint16_t channelCount = static_cast<uint16_t>(vorbisInfo->channels);

		// Setup format for output sink
		outputSink.SetFormat(AudioMetadata(sampleRate, bytesPerSample, false, channelCount, 0u));

		// Reserve memory for output sink
		const auto expectedSampleCount = ov_pcm_total(&m_internalDecoder, -1);

		if (expectedSampleCount == OV_EINVAL)
		{
			outputSink.Reserve(static_cast<uint32_t>(inputStream.Length()) * 10u / channelCount);
		}
		else
		{
			outputSink.Reserve(static_cast<uint32_t>(expectedSampleCount) * bytesPerSample);
		}

		// Seek stream to start before decoding
		ov_raw_seek_lap(&m_internalDecoder, 0);

		// Do decoding
		while (true)
		{
			float** decodingBuffer = nullptr;

			const auto readResult = ov_read_float(&m_internalDecoder, &decodingBuffer, 4096, nullptr);

			// Decoding is done
			if (readResult == 0)
			{
				break;
			}

			// This is decoding error, cancel decoding
			if (readResult < 0)
			{
				ResetDecoder();
				outputSink.SetFormat(AudioMetadata());
				assert(false);
				return false;
			}

			// Transmit decoded samples to output sink
			outputSink.TransmitSplittedSamples(reinterpret_cast<void**>(decodingBuffer), readResult * sizeof(float), sizeof(float));
		}

		return true;
	}

	/**********************************************************************************************/
	void VorbisDecoder::ResetDecoder()
	{
		ov_clear(&m_internalDecoder);
		memset(&m_internalDecoder, 0, sizeof(OggVorbis_File));

		m_testOpened = false;
	}

	/**********************************************************************************************/
	size_t VorbisDecoder::ReadCallback(void* buffer, size_t blockSize, size_t blockCount, void* userStream)
	{
		aux::IMemoryStream* sourceStream = reinterpret_cast<aux::IMemoryStream*>(userStream);

		return sourceStream->Read(reinterpret_cast<byte*>(buffer), blockSize * blockCount);
	}

	/**********************************************************************************************/
	int VorbisDecoder::SeekCallback(void* userStream, ogg_int64_t seekPos, int seekOrigin)
	{
		aux::IMemoryStream* sourceStream = reinterpret_cast<aux::IMemoryStream*>(userStream);

		switch (seekOrigin)
		{
		case SEEK_SET:
			sourceStream->Seek(aux::IStream::SeekOrigin::Begin, seekPos);
			break;

		case SEEK_CUR:
			sourceStream->Seek(aux::IStream::SeekOrigin::Current, seekPos);
			break;

		case SEEK_END:
			sourceStream->Seek(aux::IStream::SeekOrigin::End, seekPos);
			break;
		}

		return static_cast<int>(sourceStream->Tell());
	}

	/**********************************************************************************************/
	long VorbisDecoder::TellCallback(void* userStream)
	{
		aux::IMemoryStream* sourceStream = reinterpret_cast<aux::IMemoryStream*>(userStream);

		return static_cast<long>(sourceStream->Tell());
	}

	/**********************************************************************************************/
	const ov_callbacks VorbisDecoder::VorbisCallbacks = { &VorbisDecoder::ReadCallback, &VorbisDecoder::SeekCallback, nullptr, &VorbisDecoder::TellCallback };
}
