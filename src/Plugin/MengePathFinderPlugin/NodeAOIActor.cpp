#include "NodeAOIActor.h"

namespace Mengine
{
	//////////////////////////////////////////////////////////////////////////
	NodeAOIActor::NodeAOIActor()
		: m_aoi(nullptr)
		, m_radius(0.f)
		, m_iff(0)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	NodeAOIActor::~NodeAOIActor()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	void NodeAOIActor::setRadius( float _radius )
	{
		m_radius = _radius;
	}
	//////////////////////////////////////////////////////////////////////////
	float NodeAOIActor::getRadius() const
	{
		return m_radius;
	}
	//////////////////////////////////////////////////////////////////////////
	void NodeAOIActor::setIFF( uint32_t _iff )
	{
		m_iff = _iff;
	}
	//////////////////////////////////////////////////////////////////////////
	uint32_t NodeAOIActor::getIFF() const
	{
		return m_iff;
	}
	//////////////////////////////////////////////////////////////////////////
	void NodeAOIActor::setAOI( AreaOfInterest * _aoi )
	{
		m_aoi = _aoi;
	}
	//////////////////////////////////////////////////////////////////////////
	AreaOfInterest * NodeAOIActor::getAOI() const
	{
		return m_aoi;
	}
	//////////////////////////////////////////////////////////////////////////
	void NodeAOIActor::setActorUserData( const pybind::object & _data )
	{
		m_data = _data;
	}
	//////////////////////////////////////////////////////////////////////////
	const pybind::object & NodeAOIActor::getActorUserData() const
	{
		return m_data;
	}
	//////////////////////////////////////////////////////////////////////////
	bool NodeAOIActor::_activate()
	{
		if( m_aoi == nullptr )
		{
			return false;
		}

		m_actor = m_aoi->createActor( this );

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void NodeAOIActor::_deactivate()
	{
		m_aoi->removeActor( m_actor );
		m_actor = nullptr;
	}
}