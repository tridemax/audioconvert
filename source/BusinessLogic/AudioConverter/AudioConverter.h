#pragma once

#include "auxiliary/IStream.h"
#include "../../AudioCodecs/AudioFile.h"
#include "AudioConverterAPI.h"


namespace AudioConvert
{
	using namespace Auxiliary;

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

		bool DecodeAudio(Auxiliary::IMemoryStream& inputStream, AudioFormat audioFormat, AudioCodecs::AudioFile& decodedAudio);
		bool EncodeAudio(AudioCodecs::AudioFile& inputAudio, Auxiliary::IStream& outputStream, AudioFormat audioFormat);
		const std::string& LastError();

	private:
	};
}
