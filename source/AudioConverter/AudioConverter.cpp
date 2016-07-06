#include "platform.h"
#include "AudioConverter.h"
#include "../AudioCodecs/WaveDecoder.h"
#include "../AudioCodecs/WaveEncoder.h"
#include "../AudioCodecs/FlacDecoder.h"
#include "../AudioCodecs/FlacEncoder.h"
#include "../AudioCodecs/OpusDecoder.h"
#include "../AudioCodecs/OpusEncoder.h"
#include "../AudioCodecs/VorbisDecoder.h"
#include "../AudioCodecs/VorbisEncoder.h"
#include "../AudioCodecs/Mp3Decoder.h"
#include "../AudioCodecs/Mp3Encoder.h"


namespace audioconvert
{
	//-------------------------------------------------------------------------------------------------
	AudioConverter::AudioConverter()
	{
	}

	//-------------------------------------------------------------------------------------------------
	AudioConverter::~AudioConverter()
	{
	}

	//-------------------------------------------------------------------------------------------------
	bool AudioConverter::DecodeAudio(aux::IMemoryStream& inputStream, AudioFormat audioFormat, AudioFile& decodedAudio)
	{
		m_lastError.clear();

		switch (audioFormat)
		{
		case AudioFormat::AudioWave: // Wave with RIFF header
			{
				WaveDecoder waveDecoder;

				if (!waveDecoder.DecodeStream(inputStream, decodedAudio))
				{
					m_lastError = "Failed to decode input data as Wave in Riff";
					assert(false);
					return false;
				}
			}
			break;

		case AudioFormat::AudioFlac: // FLAC with FLAC header
			{
				FlacDecoder flacDecoder;

				if (!flacDecoder.DecodeStream(inputStream, decodedAudio))
				{
					m_lastError = "Failed to decode input data as Flac";
					assert(false);
					return false;
				}
			}
			break;

		case AudioFormat::AudioOpus: // Opus audio in Ogg container
			{
				OpusDecoder opusDecoder;

				if (!opusDecoder.DecodeStream(inputStream, decodedAudio))
				{
					m_lastError = "Failed to decode input data as Opus in Ogg";
					assert(false);
					return false;
				}
			}
			break;

		case AudioFormat::AudioVorbis: // Vorbis audio in Ogg container
			{
				VorbisDecoder vorbisDecoder;

				if (!vorbisDecoder.DecodeStream(inputStream, decodedAudio))
				{
					m_lastError = "Failed to decode input data as Vorbis in Ogg";
					assert(false);
					return false;
				}
			}
			break;

		case AudioFormat::AudioMp3: // MPEG layer-3 with id3v2 header
			if (!MpegDecoder::DecodeStream(inputStream, decodedAudio))
			{
				assert(false);
				return false;
			}
			break;
		}

		// Test decoded audio for consistency
		if (!decodedAudio.TestConsistency())
		{
			assert(false);
			return false;
		}

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool AudioConverter::EncodeAudio(AudioFile& inputAudio, aux::IStream& outputStream, AudioFormat audioFormat)
	{
		m_lastError.clear();

		switch (audioFormat)
		{
		case AudioFormat::AudioWave: // Wave with RIFF header
			{
				WaveEncoder waveEncoder;

				if (!waveEncoder.EncodeSamples(inputAudio, outputStream))
				{
					assert(false);
					return false;
				}
			}
			break;

		case AudioFormat::AudioFlac: // FLAC with FLAC header
			{
				FlacEncoder flacEncoder;

				if (!flacEncoder.EncodeSamples(inputAudio, outputStream))
				{
					assert(false);
					return false;
				}
			}
			break;

		case AudioFormat::AudioOpus: // Opus audio in Ogg container
			{
				OpusEncoder opusEncoder;

				if (!opusEncoder.EncodeSamples(inputAudio, outputStream))
				{
					assert(false);
					return false;
				}
			}
			break;

		case AudioFormat::AudioVorbis: // Vorbis audio in Ogg container
			{
				VorbisEncoder vorbisEncoder;

				if (!vorbisEncoder.EncodeSamples(inputAudio, outputStream))
				{
					assert(false);
					return false;
				}
			}
			break;

		case AudioFormat::AudioMp3: // MPEG layer-3 with id3v2 header
			{
				MpegEncoder mpegEncoder;

				if (!mpegEncoder.EncodeSamples(inputAudio, outputStream))
				{
					assert(false);
					return false;
				}
			}
			break;

		default: // Unknown audio format
//			Logging::Error(loggingEvent, "Unsupported audio format.");
			assert(false);
			return false;
		}

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	const std::string& AudioConverter::LastError()
	{
		return m_lastError;
	}
}
