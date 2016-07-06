#pragma once

#include "AudioMetadata.h"


namespace audioconvert
{
	//-------------------------------------------------------------------------------------------------
	/// AudioSink
	//-------------------------------------------------------------------------------------------------
	class AudioSink
	{
	public:
		virtual void SetFormat(const AudioMetadata& metadata = AudioMetadata()) = 0;
		virtual void Reserve(uint32_t bytesPerChannel) = 0;
		virtual void TransmitInterleavedSamples(const void* buffer, uint32_t bufferSize) = 0;
		virtual void TransmitSplittedSamples(const void* const* buffers, uint32_t bufferSize, uint16_t sampleStride) = 0;
	};

	//-------------------------------------------------------------------------------------------------
	/// AudioDecoder
	//-------------------------------------------------------------------------------------------------
/*
	class AudioDecoder
	{
	public:
		virtual bool DecodeChunk(const uint8_t* chunk, size_t chunkLength, AudioSink* outputSink) = 0;
		virtual bool ResetDecoder() = 0;
	};
*/
	//-------------------------------------------------------------------------------------------------
	/// AudioEncoder
	//-------------------------------------------------------------------------------------------------
	/*
	class AudioEncoder
	{
	public:
		virtual bool EncodeSamples(AudioFile& inputAudio, aux::IStream& outputStream) = 0;
	};
	*/
}
