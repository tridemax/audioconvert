#pragma once

#include "AudioSink.h"


namespace audioconvert
{
	class VorbisDecoder : public boost::noncopyable
	{
	private:
		OggVorbis_File				m_internalDecoder;
		bool						m_testOpened;

		static const ov_callbacks	VorbisCallbacks;

	public:
		VorbisDecoder();
		~VorbisDecoder();

		bool TestStream(aux::IMemoryStream& inputStream);
		bool DecodeStream(aux::IMemoryStream& inputStream, AudioSink& outputSink);
		void ResetDecoder();

	private:
		static size_t ReadCallback(void* buffer, size_t blockSize, size_t blockCount, void* userStream);
		static int SeekCallback(void* userStream, ogg_int64_t seekPos, int seekOrigin);
		static long TellCallback(void* userStream);
	};
}
