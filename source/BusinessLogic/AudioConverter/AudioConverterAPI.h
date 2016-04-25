#pragma once

#ifdef _WIN32
	#ifndef AudioConverterExports
		#ifdef AUDIOCONVERT_DLL
			#define AudioConverterExports __declspec(dllexport)
		#else
			#define AudioConverterExports __declspec(dllimport)
		#endif
	#endif
#else
	#define AudioConverterExports
	#define __stdcall
#endif

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
		AudioMpeg,								///< MPEG layer-3 with id3v2 header
		AudioOpus,								///< Opus audio in Ogg container
		AudioVorbis								///< Vorbis audio in Ogg container
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
		uint32_t					m_sampleRate;
		uint32_t					m_sampleCount;
		uint16_t					m_channelCount;
	};
#pragma pack(pop)

	//-------------------------------------------------------------------------------------------------
	/// ResultsEnumerator
	//-------------------------------------------------------------------------------------------------
	typedef bool(__stdcall* ResultsEnumerator)(const void* resultData, size_t resultLength);

	//-------------------------------------------------------------------------------------------------
	AudioConverterExports void* __stdcall AudioConverter_Allocate();

	//-------------------------------------------------------------------------------------------------
	AudioConverterExports void __stdcall AudioConverter_Free(void* audioConverter);

	//-------------------------------------------------------------------------------------------------
	AudioConverterExports void __stdcall AudioConverter_SetInput(void* audioConverter, void* inputData, size_t inputLength, AudioFormat inputFormat);

	//-------------------------------------------------------------------------------------------------
	AudioConverterExports bool __stdcall AudioConverter_ResampleAudio(void* audioConverter, uint32_t outputSampleRate);

	//-------------------------------------------------------------------------------------------------
	AudioConverterExports bool __stdcall AudioConverter_ConvertAudio(void* audioConverter, AudioFormat outputFormat);

	//-------------------------------------------------------------------------------------------------
	AudioConverterExports bool __stdcall AudioConverter_ExtractAudioData(void* audioConverter, AudioDataType outputDataType);

	//-------------------------------------------------------------------------------------------------
	AudioConverterExports bool __stdcall AudioConverter_SplitAudio(void* audioConverter, AudioFormat outputFormat, AudioSegment* audioSegments, uint32_t audioSegmentsCount);

	//-------------------------------------------------------------------------------------------------
	AudioConverterExports bool __stdcall AudioConverter_EnumerateResults(void* audioConverter, ResultsEnumerator resultsEnumerator);

	//-------------------------------------------------------------------------------------------------
	AudioConverterExports const char* __stdcall AudioConverter_LastError(void* audioConverter);
}
