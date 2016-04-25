#include "stdafx.h"
#include "VorbisEncoder.h"
#include "../Auxiliary/Miscellaneous.h"


namespace AudioCodecs
{
/***************************************************************************************************
*** VorbisEncoder
***************************************************************************************************/
	VorbisEncoder::VorbisEncoder()
	{
		memset(&m_vorbisInfo, 0, sizeof(vorbis_info));
		memset(&m_vorbisComment, 0, sizeof(vorbis_comment));
		memset(&m_vorbisDspState, 0, sizeof(vorbis_dsp_state));
		memset(&m_vorbisBlock, 0, sizeof(vorbis_block));
		memset(&m_oggStreamState, 0, sizeof(ogg_stream_state));
	}

	/**********************************************************************************************/
	VorbisEncoder::~VorbisEncoder()
	{
	}

	/**********************************************************************************************/
	bool VorbisEncoder::EncodeSamples(AudioFile& inputAudio, IStream& outputStream)
	{
		const auto& metadata = inputAudio.GetMetadata();
		const auto dataType = metadata.GetDataType();

		// Initialize encoder
		vorbis_info_init(&m_vorbisInfo);

		if (vorbis_encode_init_vbr(&m_vorbisInfo, metadata.m_channelCount, metadata.m_sampleRate, 0.4f) != 0)
		{
			ResetEncoder();
			assert(false);
			return false;
		}

		vorbis_comment_init(&m_vorbisComment);
		vorbis_analysis_init(&m_vorbisDspState, &m_vorbisInfo);
		vorbis_block_init(&m_vorbisDspState, &m_vorbisBlock);

		// Initialize ogg stream
		ogg_stream_init(&m_oggStreamState, 0);

		// Reserve memory for output stream
		outputStream.Reserve(metadata.OverallDataSize() / 8u);

		// Stream out the header
		ogg_packet header, headerComm, headerCode;

		if (vorbis_analysis_headerout(&m_vorbisDspState, &m_vorbisComment, &header, &headerComm, &headerCode) != 0)
		{
			ResetEncoder();
			assert(false);
			return false;
		}

		ogg_stream_packetin(&m_oggStreamState, &header);
		ogg_stream_packetin(&m_oggStreamState, &headerComm);
		ogg_stream_packetin(&m_oggStreamState, &headerCode);

		FlushOggStream(outputStream);

		// Do encoding and stream out encoded vorbis blocks
		const bool encodingSuccessfull = inputAudio.ReadAlignedSamples([this, dataType, &metadata, &outputStream](const byte* const* readingBuffer, uint16_t channelCount, uint32_t readedBytesPerChannel) -> bool
		{
			const uint32_t sampleCount = readedBytesPerChannel / metadata.m_bytesPerSample;

			float** preparedSamplesData = vorbis_analysis_buffer(&m_vorbisDspState, sampleCount);

			// Iterate over each channel and fill samples buffer
			for (uint16_t channelIndex = 0u; channelIndex != channelCount; ++channelIndex)
			{
				switch (dataType)
				{
				case AudioDataType::Integer16:
					samples_converter<int16_t, float>::convert(readingBuffer[channelIndex], preparedSamplesData[channelIndex], sampleCount);
					break;

				case AudioDataType::Integer24:
					samples_converter<int24_t, float>::convert(readingBuffer[channelIndex], preparedSamplesData[channelIndex], sampleCount);
					break;

				case AudioDataType::Integer32:
					samples_converter<int32_t, float>::convert(readingBuffer[channelIndex], preparedSamplesData[channelIndex], sampleCount);
					break;

				case AudioDataType::Float32:
					memcpy(preparedSamplesData[channelIndex], readingBuffer[channelIndex], sampleCount * sizeof(float));
					break;

				case AudioDataType::Float64:
					samples_converter<double, float>::convert(readingBuffer[channelIndex], preparedSamplesData[channelIndex], sampleCount);
					break;
				}
			}

			vorbis_analysis_wrote(&m_vorbisDspState, sampleCount);

			EncodeVorbisBlocks(outputStream);

			return true;
		});

		// Test for successfull encoding
		if (!encodingSuccessfull)
		{
			ResetEncoder();
			assert(false);
			return false;
		}

		// Finalize encoder
		vorbis_analysis_wrote(&m_vorbisDspState, 0);

		EncodeVorbisBlocks(outputStream);

		// Reset encoder
		ResetEncoder();

		return true;
	}

	/**********************************************************************************************/
	void VorbisEncoder::EncodeVorbisBlocks(IStream& outputStream)
	{
		ogg_packet oggPacket;

		while (vorbis_analysis_blockout(&m_vorbisDspState, &m_vorbisBlock) == 1)
		{
			// Encode vorbis block
			vorbis_analysis(&m_vorbisBlock, nullptr);
			vorbis_bitrate_addblock(&m_vorbisBlock);

			// Stream out encoded block
			while (vorbis_bitrate_flushpacket(&m_vorbisDspState, &oggPacket) == 1)
			{
				ogg_stream_packetin(&m_oggStreamState, &oggPacket);
			}

			FlushOggStream(outputStream);
		}
	}

	/**********************************************************************************************/
	void VorbisEncoder::FlushOggStream(IStream& outputStream)
	{
		ogg_page oggPage;

		while (ogg_stream_pageout(&m_oggStreamState, &oggPage) != 0)
		{
			outputStream.Write(oggPage.header, oggPage.header_len);
			outputStream.Write(oggPage.body, oggPage.body_len);
		}
	}

	/**********************************************************************************************/
	void VorbisEncoder::ResetEncoder()
	{
		ogg_stream_clear(&m_oggStreamState);
		vorbis_block_clear(&m_vorbisBlock);
		vorbis_dsp_clear(&m_vorbisDspState);
		vorbis_comment_clear(&m_vorbisComment);
		vorbis_info_clear(&m_vorbisInfo);
	}
}
