#	include "Encoder.h"

namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	Encoder::Encoder()
		: m_stream(0)
		, m_valid(false)
	{		
	}
	//////////////////////////////////////////////////////////////////////////
	void Encoder::initialize( FileOutputInterface * _stream, const String & _type )
	{
		m_stream = _stream;
		m_type = _type;

		this->_initialize();
	}
	//////////////////////////////////////////////////////////////////////////
	void Encoder::_initialize()
	{
		//Empty
	}
	//////////////////////////////////////////////////////////////////////////
	const String & Encoder::getType() const
	{
		return m_type;
	}
	//////////////////////////////////////////////////////////////////////////
	FileOutputInterface* Encoder::getStream()
	{
		return m_stream;
	}
}
