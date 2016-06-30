#pragma once


extern "C"
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

	//-------------------------------------------------------------------------------------------------
	/// AudioSegment
	//-------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
	struct AudioSegment
	{
		uint16_t					m_channelIndex;
		uint32_t					m_timeStart;					///< in milliseconds
		uint32_t					m_timeLength;					///< in milliseconds
	};
#pragma pack(pop)

	//-------------------------------------------------------------------------------------------------
	/// AudioDataType
	//-------------------------------------------------------------------------------------------------
	enum class AudioDataType : uint32_t
	{
		Unknown = 0u,
		Integer16,
		Integer24,
		Integer32,
		Float32,
		Float64
	};

	//-------------------------------------------------------------------------------------------------
	/// AudioData
	//-------------------------------------------------------------------------------------------------
#pragma pack(push, 1)
	struct AudioData
	{
		const void*					m_data;
		size_t						m_dataLength;
	};
#pragma pack(pop)

	//-------------------------------------------------------------------------------------------------
	/// ResultsEnumerator
	//-------------------------------------------------------------------------------------------------
	typedef bool(* ResultsEnumerator)(const void* resultData, size_t resultLength);

	//-------------------------------------------------------------------------------------------------
	void* AudioConverter_Allocate();

	//-------------------------------------------------------------------------------------------------
	void AudioConverter_Free(void* audioConverter);

	//-------------------------------------------------------------------------------------------------
	void AudioConverter_SetInput(void* audioConverter, void* inputData, size_t inputLength, AudioFormat inputFormat);

	//-------------------------------------------------------------------------------------------------
	bool AudioConverter_ResampleAudio(void* audioConverter, uint32_t outputSampleRate);

	//-------------------------------------------------------------------------------------------------
	bool AudioConverter_ConvertAudio(void* audioConverter, AudioFormat outputFormat);

	//-------------------------------------------------------------------------------------------------
	bool AudioConverter_ExtractAudioData(void* audioConverter, AudioDataType outputDataType);

	//-------------------------------------------------------------------------------------------------
	bool AudioConverter_SplitAudio(void* audioConverter, AudioFormat outputFormat, AudioSegment* audioSegments, uint32_t audioSegmentsCount);

	//-------------------------------------------------------------------------------------------------
	uint32_t AudioConverter_GetResultsCount(void* audioConverter);

	//-------------------------------------------------------------------------------------------------
	AudioData AudioConverter_GetResult(void* audioConverter, uint32_t resultIndex);

	//-------------------------------------------------------------------------------------------------
	bool AudioConverter_EnumerateResults(void* audioConverter, ResultsEnumerator resultsEnumerator);

	//-------------------------------------------------------------------------------------------------
	const char* AudioConverter_LastError(void* audioConverter);
}
