#include "Box2DPlugin.h"

#include "Interface/PrototypeManagerInterface.h"
#include "Interface/StringizeInterface.h"

#include "Box2DBody.h"
#include "Box2DModule.h"

#include "Core/ModuleFactory.h"

#include "Logger/Logger.h"

//////////////////////////////////////////////////////////////////////////
PLUGIN_FACTORY( Box2D, Mengine::Box2DPlugin )
//////////////////////////////////////////////////////////////////////////
namespace Mengine
{
	//////////////////////////////////////////////////////////////////////////
	Box2DPlugin::Box2DPlugin()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool Box2DPlugin::_initialize()
	{		
		this->addModuleFactory( STRINGIZE_STRING_LOCAL( "ModuleBox2D" )
			, new ModuleFactory<Box2DModule>() );

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void Box2DPlugin::_finalize()
	{
	}
}
