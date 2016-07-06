#pragma once

#include "AudioFile.h"


namespace audioconvert
{
	class FlacEncoder : public boost::noncopyable
	{
	private:
		FLAC__StreamEncoder*		m_internalEncoder;

	public:
		FlacEncoder();
		~FlacEncoder();

		bool EncodeSamples(AudioFile& inputAudio, aux::IStream& outputStream);
		void ResetEncoder();

	private:
		static FLAC__StreamEncoderWriteStatus WriteCallback(const FLAC__StreamEncoder* encoder, const FLAC__byte* buffer, size_t bytes, unsigned int samples, unsigned int current_frame, void* client_data);
		static FLAC__StreamEncoderSeekStatus SeekCallback(const FLAC__StreamEncoder* encoder, FLAC__uint64 absolute_byte_offset, void* client_data);
		static FLAC__StreamEncoderTellStatus TellCallback(const FLAC__StreamEncoder* encoder, FLAC__uint64* absolute_byte_offset, void* client_data);
	};
}
