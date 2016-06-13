#pragma once

#include "AudioMetadata.h"


namespace AudioCodecs
{
	using namespace Aux;
	typedef Aux::IStream IStream;

	class AudioSink
	{
	public:
		virtual void SetFormat(const AudioMetadata& metadata = AudioMetadata()) = 0;
		virtual void Reserve(uint32_t bytesPerChannel) = 0;
		virtual void TransmitInterleavedSamples(const void* buffer, uint32_t bufferSize) = 0;
		virtual void TransmitSplittedSamples(const void* const* buffers, uint32_t bufferSize, uint16_t sampleStride) = 0;
	};
}
