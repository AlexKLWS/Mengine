#	pragma once

#	include "Interface/ServiceInterface.h"
#	include "Interface/ServantInterface.h"

#   include "Config/String.h"

namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	class PluginInterface
		: public ServantInterface
	{
	public:
		virtual const Char * getPluginName() const = 0;

	public:
		virtual void setDynamicLoad( bool _dynamicLoad ) = 0;
		virtual bool isDynamicLoad() const = 0;

	public:
		virtual bool initialize() = 0;
		virtual void finalize() = 0;

	public:
		virtual bool isInitialize() const = 0;
	};
	//////////////////////////////////////////////////////////////////////////
	typedef stdex::intrusive_ptr<PluginInterface> PluginInterfacePtr;
	//////////////////////////////////////////////////////////////////////////
	typedef bool( *TPluginCreate )(PluginInterface ** _plugin, bool _dynamic);
	//////////////////////////////////////////////////////////////////////////
	typedef void * (*TDynamicLibraryFunction)(void *);
	//////////////////////////////////////////////////////////////////////////
	class DynamicLibraryInterface
		: public ServantInterface
	{
	public:
		virtual bool load() = 0;

	public:
		virtual TDynamicLibraryFunction getSymbol( const Char * _name ) const = 0;
	};
	//////////////////////////////////////////////////////////////////////////
	typedef stdex::intrusive_ptr<DynamicLibraryInterface> DynamicLibraryInterfacePtr;
	//////////////////////////////////////////////////////////////////////////
	class PluginSystemInterface
		: public ServiceInterface
	{
		SERVICE_DECLARE( "PluginSystem" )

	public:
		virtual DynamicLibraryInterfacePtr loadDynamicLibrary( const WString & _dllName ) = 0;
	};
	//////////////////////////////////////////////////////////////////////////
#   define PLUGIN_SYSTEM( serviceProvider )\
    ((Menge::PluginSystemInterface*)SERVICE_GET(serviceProvider, Menge::PluginSystemInterface))
	//////////////////////////////////////////////////////////////////////////
    class PluginServiceInterface
        : public ServiceInterface
    {
        SERVICE_DECLARE("PluginService")

    public:
        virtual bool loadPlugin( const WString & _dllName ) = 0;
		virtual bool createPlugin( const DynamicLibraryInterfacePtr & _dlib, TPluginCreate _create, bool _dynamic ) = 0;
		
	public:		
		virtual bool addPlugin(const DynamicLibraryInterfacePtr & _dlib, const PluginInterfacePtr & _plugin) = 0;
		virtual bool removePlugin( const PluginInterfacePtr & _plugin ) = 0;
		virtual bool hasPlugin( const Char * _name ) const = 0;
		virtual const PluginInterfacePtr & getPlugin( const Char * _name ) const = 0;
	};
	//////////////////////////////////////////////////////////////////////////
#   define PLUGIN_SERVICE( serviceProvider )\
    ((Menge::PluginServiceInterface*)SERVICE_GET(serviceProvider, Menge::PluginServiceInterface))
	//////////////////////////////////////////////////////////////////////////
#   define PLUGIN_DECLARE( Name )\
    public:\
		inline const Char * getPluginName() const override { return Name; };\
    protected:
	//////////////////////////////////////////////////////////////////////////
#	define PLUGIN_FUNCTION(Name)\
	initPlugin##Name
	//////////////////////////////////////////////////////////////////////////
#	define PLUGIN_CREATE(serviceProvider, Name)\
	PLUGIN_SERVICE( serviceProvider )->createPlugin( nullptr, &PLUGIN_FUNCTION( Name ), false )
	//////////////////////////////////////////////////////////////////////////
#	define PLUGIN_FACTORY_STATIC(Name, Type)\
	extern "C"{bool PLUGIN_FUNCTION(Name)( Menge::PluginInterface ** _plugin, bool _dynamic ){\
	if( _dynamic == true ){stdex_allocator_initialize();}\
	Menge::PluginInterface * plugin = new Type();\
	if( plugin == nullptr ){ return false; }\
	plugin->setDynamicLoad( _dynamic );\
	*_plugin = plugin;\
	return true;}}
	//////////////////////////////////////////////////////////////////////////
#	define PLUGIN_FACTORY_DYNAMIC(Name, Type)\
	extern "C"\
	{\
		__declspec(dllexport) bool dllCreatePlugin( Menge::PluginInterface ** _plugin )\
		{\
			return PLUGIN_FUNCTION(Name)( _plugin, true );\
		}\
	}
	//////////////////////////////////////////////////////////////////////////
#   ifdef MENGE_PLUGIN_DLL
#	define PLUGIN_FACTORY(Name, Type)\
	PLUGIN_FACTORY_STATIC(Name, Type)\
	PLUGIN_FACTORY_DYNAMIC( Name, Type )
#	else
#	define PLUGIN_FACTORY(Name, Type)\
	PLUGIN_FACTORY_STATIC(Name, Type)
#	endif
	//////////////////////////////////////////////////////////////////////////
#	define PLUGIN_EXPORT(Name)\
	extern "C"\
	{\
		extern bool PLUGIN_FUNCTION(Name)( Menge::PluginInterface ** _plugin, bool _dynamic );\
	}
}