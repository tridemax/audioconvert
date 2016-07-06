#pragma once

#include "AudioSink.h"


namespace audioconvert
{
	class OpusDecoder : public boost::noncopyable
	{
	private:
		ogg_sync_state				m_oggSyncState;
		ogg_stream_state			m_oggStreamState;

	public:
		OpusDecoder();
		~OpusDecoder();

		bool Open(aux::IMemoryStream& inputStream);
		bool TestOpen(aux::IMemoryStream& inputStream);
		bool DecodeStream(aux::IMemoryStream& inputStream, AudioSink& outputSink);
		bool DecodeChunk(const void* inputBuffer, size_t inputBufferLength, AudioSink& outputSink);
		bool DecodingCompleted() const;
	};
}
