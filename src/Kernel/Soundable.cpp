#include "Kernel/Soundable.h"

namespace Mengine
{
	//////////////////////////////////////////////////////////////////////////
	void Soundable::setVolume( float _volume )
	{
		this->_setVolume( _volume );
	}
	//////////////////////////////////////////////////////////////////////////
	float Soundable::getVolume() const
	{
		float volume = this->_getVolume();

		return volume;
	}
	//////////////////////////////////////////////////////////////////////////
	void Soundable::_setVolume( float _volume )
	{
        (void)_volume;
		//Empty
	}
	//////////////////////////////////////////////////////////////////////////
	float Soundable::_getVolume() const
	{
		return 0.f;
	}
	//////////////////////////////////////////////////////////////////////////
}