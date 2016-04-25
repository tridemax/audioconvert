#pragma once

#include "AudioFile.h"


namespace AudioCodecs
{
	class WaveEncoder : public NonCopyable
	{
	public:
		bool EncodeSamples(AudioFile& inputAudio, IStream& outputStream);
	};
}
