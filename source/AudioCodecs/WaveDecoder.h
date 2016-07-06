#pragma once

#include "AudioSink.h"


namespace audioconvert
{
/***************************************************************************************************
*** WaveHeader
****************************************************************************************************
	0		4		ChunkID			Contains the letters "RIFF" in ASCII form (0x52494646 big-endian form).

	4		4		ChunkSize		36 + SubChunk2Size, or more precisely:
									4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
									This is the size of the rest of the chunk 
									following this number.  This is the size of the 
									entire file in bytes minus 8 bytes for the
									two fields not included in this count:
									ChunkID and ChunkSize.

	8		4		Format			Contains the letters "WAVE" (0x57415645 big-endian form).

	12		4		Subchunk1ID		Contains the letters "fmt " (0x666d7420 big-endian form).

	16		4		Subchunk1Size	16 for PCM.  This is the size of the rest of the Subchunk which follows this number.

	20		2		AudioFormat		PCM = 1 (i.e. Linear quantization)
									Values other than 1 indicate some form of compression.

	22		2		NumChannels		Mono = 1, Stereo = 2, etc.

	24		4		SampleRate		8000, 44100, etc.

	28		4		ByteRate		== SampleRate * NumChannels * BitsPerSample / 8

	32		2		BlockAlign		== NumChannels * BitsPerSample / 8
									The number of bytes for one sample including all channels.

	34		2		BitsPerSample	8 bits = 8, 16 bits = 16, etc.

	36		4		Subchunk2ID		Contains the letters "data" (0x64617461 big-endian form).

	40		4		Subchunk2Size	== NumSamples * NumChannels * BitsPerSample / 8
									This is the number of bytes in the data.
									You can also think of this as the size
									of the read of the subchunk following this number.
***************************************************************************************************/

	enum class WaveFormatType : uint16_t
	{
		WavePcm						= 0x0001u,
		WaveAdpcm					= 0x0002u,
		WaveIeeeFloat				= 0x0003u,
		WaveAlaw					= 0x0006u,
		WaveMulaw					= 0x0007u,
		WaveExtensible				= 0xFFFEu
	};

	/**********************************************************************************************/
#pragma pack(push, 1)
	class WavePcmFormat
	{
	public:
		WaveFormatType				m_waveFormat;
		uint16_t					m_numChannels;
		uint32_t					m_sampleRate;
		uint32_t					m_byteRate;
		uint16_t					m_blockAlign;
		uint16_t					m_bitsPerSample;
	};
#pragma pack(pop)

	static_assert(sizeof(WavePcmFormat) == 16u, "\"WavePcmFormat\" size not equal to 16 bytes.");

	/**********************************************************************************************/
#pragma pack(push, 1)
	class WavePcmExtensibleFormat
	{
	public:
		WavePcmFormat				m_baseFormat;
		uint16_t					m_extensibleSize;
		uint16_t					m_samplesPerBlock;
		uint32_t					m_channelMask;
		uint32_t					m_subFormat[4u];
	};
#pragma pack(pop)

	static_assert(sizeof(WavePcmExtensibleFormat) == 40u, "\"WavePcmExtensibleFormat\" size not equal to 40 bytes.");

	/**********************************************************************************************/
	class WaveDecoder : public boost::noncopyable
	{
	private:
		friend class WaveEncoder;

		enum class ParsingStage : uint32_t
		{
			AwaitingInitialHeader = 0u,
			SkippingUnusedChunk,
			SkippingUnusedSubChunk,
			AwaitingFormatChunk,
			ReadingFormatChunk,
			ReadingExtensibleFormatChunk,
			AwaitingDataChunk,
			ReadingDataChunk,
			ReadingCompleted
		};

	private:
		ParsingStage				m_parsingStage;
		AudioMetadata				m_audioMetadata;
		uint32_t					m_currentChunkId;
		uint32_t					m_currentChunkLength;
		std::vector<byte>			m_internalBuffer;

		static constexpr uint32_t	FourccRiff					= makefourcc('R', 'I', 'F', 'F');
		static constexpr uint32_t	FourccWave					= makefourcc('W', 'A', 'V', 'E');
		static constexpr uint32_t	FourccFormat				= makefourcc('f', 'm', 't', ' ');
		static constexpr uint32_t	FourccFact					= makefourcc('f', 'a', 'c', 't');
		static constexpr uint32_t	FourccData					= makefourcc('d', 'a', 't', 'a');

		static constexpr uint32_t	FourccLength				= 4u;
		static constexpr uint32_t	InitialHeaderLength			= FourccLength + 4u + FourccLength;
		static constexpr uint32_t	FormatHeaderLength			= FourccLength + 4u;
		static constexpr uint32_t	FormatChunkLength			= sizeof(WavePcmFormat);
		static constexpr uint32_t	ExtensibleFormatChunkLength	= sizeof(WavePcmExtensibleFormat);
		static constexpr uint32_t	DataHeaderLength			= FourccLength + 4u;

	public:
		WaveDecoder();
		~WaveDecoder();

		bool TestStream(aux::IMemoryStream& inputStream);
		bool DecodeStream(aux::IMemoryStream& inputStream, AudioSink& outputSink);
		bool DecodeChunk(const byte* buffer, size_t bufferLength, AudioSink& outputSink);
		void ResetDecoder();

		inline bool DecodingCompleted() const
		{
			return m_parsingStage == ParsingStage::ReadingCompleted;
		}
	};
}
