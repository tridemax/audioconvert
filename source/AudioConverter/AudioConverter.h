#pragma once

#include "../AudioCodecs/AudioFile.h"


namespace audioconvert
{
	//-------------------------------------------------------------------------------------------------
	/// AudioFormat
	//-------------------------------------------------------------------------------------------------
	enum class AudioFormat : uint32_t
	{
		AudioUnknown = 0u,						///< Unknown audio format
		AudioWave,								///< Uncompressed Wave with RIFF header
		AudioFlac,								///< FLAC with FLAC header
		AudioOpus,								///< Opus audio in Ogg container
		AudioVorbis,							///< Vorbis audio in Ogg container
		AudioMp3,								///< MPEG layer-3 with id3v2 header
		AudioAac								///< Advanced Audio Coding in MPEG-4 Part 14 container
	};

	inline AudioFormat AudioFormatFromMediaType(const char* mediaType)
	{
		if (!mediaType || !mediaType[0])
		{
			return AudioFormat::AudioUnknown;
		}
		if (strcmp(mediaType, "audio/wav") == 0 ||
			strcmp(mediaType, "audio/wave") == 0 ||
			strcmp(mediaType, "audio/x-wav"))
		{
			return AudioFormat::AudioWave;
		}
		if (strcmp(mediaType, "audio/flac") == 0 ||
			strcmp(mediaType, "audio/x-flac"))
		{
			return AudioFormat::AudioFlac;
		}
		if (strcmp(mediaType, "audio/opus") == 0)
		{
			return AudioFormat::AudioOpus;
		}
		if (strcmp(mediaType, "audio/vorbis") == 0)
		{
			return AudioFormat::AudioVorbis;
		}
		if (strcmp(mediaType, "audio/mpeg") == 0)
		{
			return AudioFormat::AudioMp3;
		}
		if (strcmp(mediaType, "audio/aac") == 0 ||
			strcmp(mediaType, "audio/mp4"))
		{
			return AudioFormat::AudioAac;
		}
		return AudioFormat::AudioUnknown;
	}

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

		bool DecodeAudio(aux::IMemoryStream& inputStream, AudioFormat audioFormat, AudioFile& decodedAudio);
		bool EncodeAudio(AudioFile& inputAudio, aux::IStream& outputStream, AudioFormat audioFormat);
		bool TranscodeAudio(aux::IMemoryStream& inputStream, AudioFormat inputFormat, aux::IStream& outputStream, AudioFormat audioFormat);
		const std::string& LastError();

	private:
	};
}
