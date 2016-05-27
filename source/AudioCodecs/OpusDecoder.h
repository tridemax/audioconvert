#pragma once

#include "AudioSink.h"


namespace AudioCodecs
{
	class OpusDecoder : public boost::noncopyable
	{
	private:
		ogg_sync_state				m_oggSyncState;
		ogg_stream_state			m_oggStreamState;

	public:
		OpusDecoder();
		~OpusDecoder();

		bool Open(IMemoryStream& inputStream);
		bool TestOpen(IMemoryStream& inputStream);
		bool DecodeStream(IMemoryStream& inputStream, AudioSink& outputSink);
		bool DecodeChunk(const void* inputBuffer, size_t inputBufferLength, AudioSink& outputSink);
		bool DecodingCompleted() const;
	};
}
