#include "platform.h"
#include "OpusEncoder.h"


namespace audioconvert
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
	bool OpusEncoder::EncodeSamples(AudioFile& inputAudio, aux::IStream& outputStream)
	{
		return true;
	}

	/**********************************************************************************************/
	void OpusEncoder::ResetEncoder()
	{
	}
}
