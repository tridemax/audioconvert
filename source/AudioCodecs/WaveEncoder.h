#pragma once

#include "AudioFile.h"


namespace audioconvert
{
	class WaveEncoder : public boost::noncopyable
	{
	public:
		bool EncodeSamples(AudioFile& inputAudio, aux::IStream& outputStream);
	};
}
