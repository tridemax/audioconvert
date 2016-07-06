#pragma once

#include "AudioFile.h"


namespace audioconvert
{
	class VorbisEncoder : public boost::noncopyable
	{
	private:
		vorbis_info					m_vorbisInfo;			// struct that stores all the static vorbis bitstream settings
		vorbis_comment				m_vorbisComment;		// struct that stores all the user comments
		vorbis_dsp_state			m_vorbisDspState;		// central working state for the packet->PCM decoder
		vorbis_block				m_vorbisBlock;			// local working space for packet->PCM decode
		ogg_stream_state			m_oggStreamState;		// take physical pages, weld into a logical stream of packets

	public:
		VorbisEncoder();
		~VorbisEncoder();

		bool EncodeSamples(AudioFile& inputAudio, aux::IStream& outputStream);
		void EncodeVorbisBlocks(aux::IStream& outputStream);
		void FlushOggStream(aux::IStream& outputStream);
		void ResetEncoder();
	};
}
