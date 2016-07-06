#pragma once

#include "AudioFile.h"


namespace audioconvert
{
	//-------------------------------------------------------------------------------------------------
	/// MpegEncoder
	//-------------------------------------------------------------------------------------------------
	class MpegEncoder : public boost::noncopyable
	{
	private:
		lame_global_flags*			m_internalEncoder;

	public:
		MpegEncoder();
		~MpegEncoder();

		bool EncodeSamples(AudioFile& inputAudio, aux::IStream& outputStream);
		void ResetEncoder();

	private:
		//---------------------------------------------------------------------------------------------
		forceinline uint32_t ResolveOutputSampleRate(uint32_t sampleRate) const
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
