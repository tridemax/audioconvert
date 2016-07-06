#pragma once


namespace audioconvert
{
	//-------------------------------------------------------------------------------------------------
	/// AudioDataType
	//-------------------------------------------------------------------------------------------------
	enum class AudioDataType : uint32_t
	{
		Unknown			= 0u,
		Integer16,
		Integer24,
		Integer32,
		Float32,
		Float64
	};

	//-------------------------------------------------------------------------------------------------
	/// AudioMetadata
	//-------------------------------------------------------------------------------------------------
	class AudioMetadata
	{
	public:
		uint32_t					m_sampleRate;
		uint16_t					m_bytesPerSample;
		bool						m_integerFormat;
		uint16_t					m_channelCount;
		uint32_t					m_sampleCount;

	public:
		//---------------------------------------------------------------------------------------------
		inline AudioMetadata(uint32_t sampleRate, uint16_t bytesPerSample, bool integerFormat, uint16_t channelCount, uint32_t sampleCount) :
			m_sampleRate(sampleRate),
			m_bytesPerSample(bytesPerSample),
			m_integerFormat(integerFormat),
			m_channelCount(channelCount),
			m_sampleCount(sampleCount)
		{
		}

		//---------------------------------------------------------------------------------------------
		inline AudioMetadata() :
			m_sampleRate(0u),
			m_bytesPerSample(0u),
			m_integerFormat(true),
			m_channelCount(0u),
			m_sampleCount(0u)
		{
		}

		//---------------------------------------------------------------------------------------------
		inline void SetDataType(AudioDataType audioDataType)
		{
			switch (audioDataType)
			{
			case AudioDataType::Integer16:
				m_bytesPerSample = 2u;
				m_integerFormat = true;
				break;

			case AudioDataType::Integer24:
				m_bytesPerSample = 3u;
				m_integerFormat = true;
				break;

			case AudioDataType::Integer32:
				m_bytesPerSample = 4u;
				m_integerFormat = true;
				break;

			case AudioDataType::Float32:
				m_bytesPerSample = 4u;
				m_integerFormat = false;
				break;

			case AudioDataType::Float64:
				m_bytesPerSample = 8u;
				m_integerFormat = false;
				break;

			default:
				m_bytesPerSample = 0u;
			}
		}

		//---------------------------------------------------------------------------------------------
		inline AudioDataType GetDataType() const
		{
			if (m_integerFormat)
			{
				switch (m_bytesPerSample)
				{
				case 2u:
					return AudioDataType::Integer16;
				case 3u:
					return AudioDataType::Integer24;
				case 4u:
					return AudioDataType::Integer32;
				}
			}
			else
			{
				switch (m_bytesPerSample)
				{
				case 4u:
					return AudioDataType::Float32;
				case 8u:
					return AudioDataType::Float64;
				}
			}
			return AudioDataType::Unknown;
		}

		//---------------------------------------------------------------------------------------------
		forceinline bool TestConsistency() const
		{
			return m_sampleRate != 0 &&
				GetDataType() != AudioDataType::Unknown &&
				m_channelCount != 0 && m_channelCount <= 32;
		}

		//---------------------------------------------------------------------------------------------
		forceinline uint32_t OverallDataSize() const
		{
			return m_channelCount * m_bytesPerSample * m_sampleCount;
		}

		//---------------------------------------------------------------------------------------------
		forceinline uint32_t ChannelDataSize() const
		{
			return m_bytesPerSample * m_sampleCount;
		}

		//---------------------------------------------------------------------------------------------
		forceinline uint32_t BlockAlign() const
		{
			return m_channelCount * m_bytesPerSample;
		}
	};
}
