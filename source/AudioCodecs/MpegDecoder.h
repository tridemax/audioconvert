#pragma once

#include "AudioFile.h"


namespace AudioCodecs
{
	class MpegDecoder : public boost::noncopyable
	{
	private:
		mpg123_handle*				m_internalDecoder;

		static const uint32_t		BufferSize = 16384u; // Used as argument for _alloca, do not increase it

	public:
		MpegDecoder();
		~MpegDecoder();

		static bool DecodeStream(IStream& inputStream, AudioFile& outputAudio);

	private:
		static ssize_t ReadCallback(void* userStream, void* readingBuffer, size_t bufferSize);
		static off_t SeekCallback(void* userStream, off_t seekPos, int seekOrigin);
	};
}
