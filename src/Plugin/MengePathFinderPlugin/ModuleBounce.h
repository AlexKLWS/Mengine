#pragma once

#include "Core/ModuleBase.h"

#include "BounceWorld.h"
#include "BounceActor.h"

namespace Mengine
{
	class ModuleBounce
		: public ModuleBase
	{
	public:
		ModuleBounce();
		~ModuleBounce();

	public:
		bool _initialize() override;
		void _finalize() override;

	public:
		BounceWorldPtr createBounceWorld();
		void removeBounceWorld( const BounceWorldPtr & _collision );

	public:
		void _tick( float _time, float _timing ) override;
		void _render( const RenderObjectState * _state, uint32_t _debugMask ) override;

	protected:
		FactoryPtr m_factoryBounceWorlds;

		typedef stdex::vector<BounceWorldPtr> TVectorBounceWorlds;
		TVectorBounceWorlds m_bounceWorlds;
	};
}