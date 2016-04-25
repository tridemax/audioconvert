#pragma once

#include "AudioFile.h"


namespace AudioCodecs
{
	class FlacEncoder : public NonCopyable
	{
	private:
		FLAC__StreamEncoder*		m_internalEncoder;

	public:
		FlacEncoder();
		~FlacEncoder();

		bool EncodeSamples(AudioFile& inputAudio, IStream& outputStream);
		void ResetEncoder();

	private:
		static FLAC__StreamEncoderWriteStatus WriteCallback(const FLAC__StreamEncoder* encoder, const FLAC__byte* buffer, size_t bytes, unsigned int samples, unsigned int current_frame, void* client_data);
		static FLAC__StreamEncoderSeekStatus SeekCallback(const FLAC__StreamEncoder* encoder, FLAC__uint64 absolute_byte_offset, void* client_data);
		static FLAC__StreamEncoderTellStatus TellCallback(const FLAC__StreamEncoder* encoder, FLAC__uint64* absolute_byte_offset, void* client_data);
	};
}
