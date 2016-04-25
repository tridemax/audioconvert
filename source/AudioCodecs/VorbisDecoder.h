#pragma once

#include "AudioSink.h"


namespace AudioCodecs
{
	class VorbisDecoder : public NonCopyable
	{
	private:
		OggVorbis_File				m_internalDecoder;
		bool						m_testOpened;

		static const ov_callbacks	VorbisCallbacks;

	public:
		VorbisDecoder();
		~VorbisDecoder();

		bool TestStream(IMemoryStream& inputStream);
		bool DecodeStream(IMemoryStream& inputStream, AudioSink& outputSink);
		void ResetDecoder();

	private:
		static size_t ReadCallback(void* buffer, size_t blockSize, size_t blockCount, void* userStream);
		static int SeekCallback(void* userStream, ogg_int64_t seekPos, int seekOrigin);
		static long TellCallback(void* userStream);
	};
}
