#include "platform.h"
#include "FlacDecoder.h"


namespace audioconvert
{
/***************************************************************************************************
*** FlacDecoder
***************************************************************************************************/
	FlacDecoder::FlacDecoder() : m_internalDecoder(FLAC__stream_decoder_new())
	{
	}

	/**********************************************************************************************/
	FlacDecoder::~FlacDecoder()
	{
		if (m_internalDecoder)
		{
			FLAC__stream_decoder_delete(m_internalDecoder);
			m_internalDecoder = nullptr;
		}
	}

	/**********************************************************************************************/
	bool FlacDecoder::TestStream(aux::IMemoryStream& inputStream)
	{
		return false;
	}

	/**********************************************************************************************
		Output format - int[x]_t.
	*/
	bool FlacDecoder::DecodeStream(aux::IMemoryStream& inputStream, AudioSink& outputSink)
	{
		DecodingContext decodingContext(inputStream, outputSink);

		// Initialize decoder
		if (FLAC__stream_decoder_init_stream(
			m_internalDecoder,
			&ReadCallback,&SeekCallback, &TellCallback, &LengthCallback,
			&EofCallback, &WriteCallback, &MetadataCallback, &ErrorCallback, &decodingContext) != FLAC__STREAM_DECODER_INIT_STATUS_OK)
		{
			ResetDecoder();
			assert(false);
			return false;
		}

		FLAC__stream_decoder_set_md5_checking(m_internalDecoder, false);

		// Try to read the flac metadata
		if (!FLAC__stream_decoder_process_until_end_of_metadata(m_internalDecoder))
		{
			ResetDecoder();
			assert(false);
			return false;
		}

		// Input stream should be valid
		if (decodingContext.m_decodingError)
		{
			ResetDecoder();
			assert(false);
			return false;
		}

		// Output sink should have metadata now
		if (!decodingContext.m_metadataRead)
		{
			ResetDecoder();
			assert(false);
			return false;
		}

		// Reserve memory for output sink
		if (decodingContext.m_sampleCount == 0u)
		{
			outputSink.Reserve(static_cast<uint32_t>(inputStream.Length()) * 2u / decodingContext.m_channelCount);
		}
		else
		{
			outputSink.Reserve(static_cast<uint32_t>(decodingContext.m_sampleCount) * decodingContext.m_bytesPerSample);
		}

		// Do decoding
		if (!FLAC__stream_decoder_process_until_end_of_stream(m_internalDecoder))
		{
			ResetDecoder();
			outputSink.SetFormat(AudioMetadata());
			assert(false);
			return false;
		}

		return true;
	}

	/**********************************************************************************************/
	void FlacDecoder::ResetDecoder()
	{
		FLAC__stream_decoder_finish(m_internalDecoder);
	}

	/**********************************************************************************************/
	FLAC__StreamDecoderReadStatus FlacDecoder::ReadCallback(const FLAC__StreamDecoder* decoder, FLAC__byte* buffer, size_t* bytes, void* client_data)
	{
		DecodingContext* decodingContext = reinterpret_cast<DecodingContext*>(client_data);

		*bytes = decodingContext->m_inputStream.Read(buffer, *bytes);

		if (*bytes == 0u)
		{
			return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
		}

		return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
	}

	/**********************************************************************************************/
	FLAC__StreamDecoderSeekStatus FlacDecoder::SeekCallback(const FLAC__StreamDecoder* decoder, FLAC__uint64 absolute_byte_offset, void* client_data)
	{
		DecodingContext* decodingContext = reinterpret_cast<DecodingContext*>(client_data);

		decodingContext->m_inputStream.Seek(aux::IStream::SeekOrigin::Begin, static_cast<intptr_t>(absolute_byte_offset));

		return FLAC__STREAM_DECODER_SEEK_STATUS_OK;
	}

	/**********************************************************************************************/
	FLAC__StreamDecoderTellStatus FlacDecoder::TellCallback(const FLAC__StreamDecoder* decoder, FLAC__uint64* absolute_byte_offset, void* client_data)
	{
		DecodingContext* decodingContext = reinterpret_cast<DecodingContext*>(client_data);

		*absolute_byte_offset = decodingContext->m_inputStream.Tell();

		return FLAC__STREAM_DECODER_TELL_STATUS_OK;
	}

	/**********************************************************************************************/
	FLAC__StreamDecoderLengthStatus FlacDecoder::LengthCallback(const FLAC__StreamDecoder* decoder, FLAC__uint64* stream_length, void* client_data)
	{
		DecodingContext* decodingContext = reinterpret_cast<DecodingContext*>(client_data);

		*stream_length = decodingContext->m_inputStream.Length();

		return FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
	}

	/**********************************************************************************************/
	FLAC__bool FlacDecoder::EofCallback(const FLAC__StreamDecoder* decoder, void* client_data)
	{
		DecodingContext* decodingContext = reinterpret_cast<DecodingContext*>(client_data);

		return decodingContext->m_inputStream.Tell() == decodingContext->m_inputStream.Length() ? 1 : 0;
	}

	/**********************************************************************************************/
	FLAC__StreamDecoderWriteStatus FlacDecoder::WriteCallback(const FLAC__StreamDecoder* decoder, const FLAC__Frame* frame, const FLAC__int32* const* buffer, void* client_data)
	{
		DecodingContext* decodingContext = reinterpret_cast<DecodingContext*>(client_data);

		if (!decodingContext->m_metadataRead)
		{
			assert(false);
			return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
		}

		decodingContext->m_outputSink.TransmitSplittedSamples(reinterpret_cast<const void* const*>(buffer), frame->header.blocksize * sizeof(FLAC__int32), sizeof(FLAC__int32));

		return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
	}

	/**********************************************************************************************/
	void FlacDecoder::MetadataCallback(const FLAC__StreamDecoder* decoder, const FLAC__StreamMetadata* metadata, void* client_data)
	{
		DecodingContext* decodingContext = reinterpret_cast<DecodingContext*>(client_data);

		if (metadata->type == FLAC__METADATA_TYPE_STREAMINFO && !decodingContext->m_metadataRead)
		{
			decodingContext->m_metadataRead = true;
			decodingContext->m_bytesPerSample = static_cast<uint16_t>(metadata->data.stream_info.bits_per_sample) / 8u;
			decodingContext->m_channelCount = static_cast<uint16_t>(metadata->data.stream_info.channels);
			decodingContext->m_sampleCount = static_cast<uint32_t>(metadata->data.stream_info.total_samples);

			decodingContext->m_outputSink.SetFormat(AudioMetadata(
				metadata->data.stream_info.sample_rate,
				decodingContext->m_bytesPerSample,
				true,
				decodingContext->m_channelCount,
				0u));
		}
	}

	/**********************************************************************************************/
	void FlacDecoder::ErrorCallback(const FLAC__StreamDecoder* decoder, FLAC__StreamDecoderErrorStatus status, void* client_data)
	{
		DecodingContext* decodingContext = reinterpret_cast<DecodingContext*>(client_data);

		decodingContext->m_decodingError = true;
		decodingContext->m_outputSink.SetFormat(AudioMetadata());
	}
}
