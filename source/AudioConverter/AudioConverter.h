#pragma once

#include "../AudioCodecs/AudioFile.h"
#include "AudioConverterAPI.h"


namespace AudioConvert
{
	using namespace Aux;

	//-------------------------------------------------------------------------------------------------
	/// AudioConverter
	//-------------------------------------------------------------------------------------------------
	class AudioConverter : public boost::noncopyable
	{
	private:
		std::string					m_lastError;

	public:
		AudioConverter();
		~AudioConverter();

		bool DecodeAudio(Aux::IMemoryStream& inputStream, AudioFormat audioFormat, AudioCodecs::AudioFile& decodedAudio);
		bool EncodeAudio(AudioCodecs::AudioFile& inputAudio, Aux::IStream& outputStream, AudioFormat audioFormat);
		const std::string& LastError();

	private:
	};
}
