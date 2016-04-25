#pragma once

#include "AudioSink.h"


namespace AudioCodecs
{
	class FlacDecoder : public NonCopyable
	{
	private:
		class DecodingContext
		{
		public:
			IMemoryStream&				m_inputStream;
			AudioSink&					m_outputSink;
			bool						m_decodingError;
			bool						m_metadataRead;
			uint16_t					m_bytesPerSample;
			uint16_t					m_channelCount;
			uint32_t					m_sampleCount;

		public:
			inline DecodingContext(IMemoryStream& inputStream, AudioSink& outputSink) :
				m_inputStream(inputStream),
				m_outputSink(outputSink),
				m_decodingError(false),
				m_metadataRead(false),
				m_bytesPerSample(0u),
				m_channelCount(0u),
				m_sampleCount(0u)
			{
			}
		};

	private:
		FLAC__StreamDecoder*		m_internalDecoder;

	public:
		FlacDecoder();
		~FlacDecoder();

		bool TestStream(IMemoryStream& inputStream);
		bool DecodeStream(IMemoryStream& inputStream, AudioSink& outputSink);
		void ResetDecoder();

	private:
		static FLAC__StreamDecoderReadStatus ReadCallback(const FLAC__StreamDecoder* decoder, FLAC__byte* buffer, size_t* bytes, void* client_data);
		static FLAC__StreamDecoderSeekStatus SeekCallback(const FLAC__StreamDecoder* decoder, FLAC__uint64 absolute_byte_offset, void* client_data);
		static FLAC__StreamDecoderTellStatus TellCallback(const FLAC__StreamDecoder* decoder, FLAC__uint64* absolute_byte_offset, void* client_data);
		static FLAC__StreamDecoderLengthStatus LengthCallback(const FLAC__StreamDecoder* decoder, FLAC__uint64* stream_length, void* client_data);
		static FLAC__bool EofCallback(const FLAC__StreamDecoder* decoder, void* client_data);
		static FLAC__StreamDecoderWriteStatus WriteCallback(const FLAC__StreamDecoder* decoder, const FLAC__Frame* frame, const FLAC__int32* const* buffer, void* client_data);
		static void MetadataCallback(const FLAC__StreamDecoder* decoder, const FLAC__StreamMetadata* metadata, void* client_data);
		static void ErrorCallback(const FLAC__StreamDecoder* decoder, FLAC__StreamDecoderErrorStatus status, void* client_data);
	};
}
