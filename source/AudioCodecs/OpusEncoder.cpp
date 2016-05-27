#include "platform.h"
#include "OpusEncoder.h"


namespace AudioCodecs
{
/***************************************************************************************************
*** OpusEncoder
***************************************************************************************************/
	OpusEncoder::OpusEncoder()
	{
//		m_internalEncoder()		opus_encoder_create()
	}

	/**********************************************************************************************/
	OpusEncoder::~OpusEncoder()
	{
	}

	/**********************************************************************************************/
	bool OpusEncoder::EncodeSamples(AudioFile& inputAudio, IStream& outputStream)
	{
		return true;
	}

	/**********************************************************************************************/
	void OpusEncoder::ResetEncoder()
	{
	}
}
