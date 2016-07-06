#include "platform.h"
#include "OpusDecoder.h"


namespace audioconvert
{
/***************************************************************************************************
*** OpusDecoder
***************************************************************************************************/
	OpusDecoder::OpusDecoder()
	{
		ogg_sync_init(&m_oggSyncState);
		ogg_stream_init(&m_oggStreamState, -1);
	}

	/**********************************************************************************************/
	OpusDecoder::~OpusDecoder()
	{
		ogg_sync_clear(&m_oggSyncState);
		ogg_stream_clear(&m_oggStreamState);
	}

	/**********************************************************************************************/
	bool OpusDecoder::Open(aux::IMemoryStream& inputStream)
	{
		return false;
	}

	/**********************************************************************************************/
	bool OpusDecoder::TestOpen(aux::IMemoryStream& inputStream)
	{
		return false;
	}

	/**********************************************************************************************/
	bool OpusDecoder::DecodeStream(aux::IMemoryStream& inputStream, AudioSink& outputSink)
	{
		return true;
	}

	/**********************************************************************************************/
	bool OpusDecoder::DecodeChunk(const void* inputBuffer, size_t inputBufferLength, AudioSink& outputSink)
	{
		/*
		auto* oggBuffer = ogg_sync_buffer(&m_oggSyncState, inputBufferLength);

		memcpy_s(oggBuffer, inputBufferLength, inputBuffer, inputBufferLength);

		ogg_sync_wrote(&m_oggSyncState, inputBufferLength);
*/
		return true;
	}

	/**********************************************************************************************/
	bool OpusDecoder::DecodingCompleted() const
	{
		return false;
	}
}
