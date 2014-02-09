#	include "CollisionMotor.h"
#	include "CollisionObject.h"

#	include "Math/angle.h"

namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	CollisionMotor::CollisionMotor()
		: m_linearSpeed(0.f)
		, m_angularSpeed(0.f)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	CollisionMotor::~CollisionMotor()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	void CollisionMotor::setLinearSpeed( float _speed )
	{
		m_linearSpeed = _speed;
	}
	//////////////////////////////////////////////////////////////////////////
	float CollisionMotor::getLinearSpeed() const
	{
		return m_linearSpeed;
	}
	//////////////////////////////////////////////////////////////////////////
	void CollisionMotor::setAngularSpeed( float _speed )
	{
		m_angularSpeed = _speed;
	}
	//////////////////////////////////////////////////////////////////////////
	float CollisionMotor::getAngularSpeed() const
	{
		return m_angularSpeed;
	}
}