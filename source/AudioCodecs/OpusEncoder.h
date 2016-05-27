#pragma once

#include "AudioFile.h"


namespace AudioCodecs
{
	class OpusEncoder : public boost::noncopyable
	{
	private:
		OpusEncoder*				m_internalEncoder;
		/*
		vorbis_info					m_vorbisInfo;			// struct that stores all the static vorbis bitstream settings
		vorbis_comment				m_vorbisComment;		// struct that stores all the user comments
		vorbis_dsp_state			m_vorbisDspState;		// central working state for the packet->PCM decoder
		vorbis_block				m_vorbisBlock;			// local working space for packet->PCM decode
		ogg_stream_state			m_oggStreamState;		// take physical pages, weld into a logical stream of packets
*/

	public:
		OpusEncoder();
		~OpusEncoder();

		bool EncodeSamples(AudioFile& inputAudio, IStream& outputStream);
		void ResetEncoder();
	};
}
