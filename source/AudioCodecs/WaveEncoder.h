#pragma once

#include "AudioFile.h"


namespace AudioCodecs
{
	class WaveEncoder : public boost::noncopyable
	{
	public:
		bool EncodeSamples(AudioFile& inputAudio, IStream& outputStream);
	};
}
