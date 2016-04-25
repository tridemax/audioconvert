#pragma once

#include "AudioFile.h"


namespace AudioCodecs
{
	//-------------------------------------------------------------------------------------------------
	/// MpegEncoder
	//-------------------------------------------------------------------------------------------------
	class MpegEncoder : public NonCopyable
	{
	private:
		lame_global_flags*			m_internalEncoder;

	public:
		MpegEncoder();
		~MpegEncoder();

		bool EncodeSamples(AudioFile& inputAudio, IStream& outputStream);
		void ResetEncoder();

	private:
		//---------------------------------------------------------------------------------------------
		__forceinline uint32_t ResolveOutputSampleRate(uint32_t sampleRate) const
		{
			if (sampleRate <= 8000u)
			{
				return 8000u;
			}
			if (sampleRate <= 22050u)
			{
				return 22050u;
			}
			if (sampleRate <= 44100u)
			{
				return 44100u;
			}
			if (sampleRate <= 48000u)
			{
				return 48000u;
			}
			return 0u;
		}
	};
}
