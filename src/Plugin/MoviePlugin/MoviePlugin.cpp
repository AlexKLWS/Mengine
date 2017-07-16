#	include "MoviePlugin.h"

#	include "Interface/PrototypeManagerInterface.h"
#	include "Interface/StringizeInterface.h"
#	include "Interface/ScriptSystemInterface.h"
#	include "Interface/ConfigInterface.h"

#   include "Kernel/ScriptWrapper.h"

#	include "Kernel/NodePrototypeGenerator.h"
#	include "Kernel/ResourcePrototypeGenerator.h"

#	include "Movie2.h"
#	include "ResourceMovie2.h"

#	include "Core/ModuleFactory.h"

#	include "Logger/Logger.h"

#	include "pybind/pybind.hpp"

#   include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
PLUGIN_FACTORY( Movie, Menge::MoviePlugin )
//////////////////////////////////////////////////////////////////////////
namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	static void * stdex_movie_alloc( void * _data, size_t _size )
	{
		(void)_data;

		//return stdex_malloc( _size );
		//return new uint8_t[_size];
		return malloc( _size );
	}
	//////////////////////////////////////////////////////////////////////////
	static void * stdex_movie_alloc_n( void * _data, size_t _size, size_t _count )
	{
		(void)_data;

		size_t total = _size * _count;

		//return stdex_malloc( total );
		//return new uint8_t[total];
		return malloc( total );
	}
	//////////////////////////////////////////////////////////////////////////
	static void stdex_movie_free( void * _data, const void * _ptr )
	{
		(void)_data;

		//stdex_free( (void *)_ptr );
		//delete[] _ptr;
		free( (void *)_ptr );
	}
	//////////////////////////////////////////////////////////////////////////
	static void stdex_movie_free_n( void * _data, const void * _ptr )
	{
		(void)_data;

		//stdex_free( (void *)_ptr );
		//delete[] _ptr;
		free( (void *)_ptr );
	}
	//////////////////////////////////////////////////////////////////////////
	static void stdex_movie_logerror( void * _data, aeMovieErrorCode _code, const char * _format, ... )
	{
		va_list argList;

		va_start( argList, _format );
		char msg[4096];
		vsprintf( msg, _format, argList );
		va_end( argList );

		MoviePlugin * plugin = static_cast<MoviePlugin *>(_data);
		ServiceProviderInterface * serviceProvider = plugin->getServiceProvider();

		LOGGER_ERROR( serviceProvider )("MoviePlugin error '%s' code '%d'"
			, msg
			, _code			
			);
	}
	//////////////////////////////////////////////////////////////////////////
	MoviePlugin::MoviePlugin()
		: m_instance( nullptr )
	{
	}
	//////////////////////////////////////////////////////////////////////////
	namespace
	{		
		class ResourceMovie2PrototypeGenerator
			: public ResourcePrototypeGenerator<ResourceMovie2, 128>
		{
		public:
			ResourceMovie2PrototypeGenerator( aeMovieInstance * _instance )
				: m_instance( _instance )
			{

			}

		protected:
			bool setup( ResourceMovie2 * _resource ) override
			{
				_resource->setMovieInstance( m_instance );

				return true;
			}

		protected:
			aeMovieInstance * m_instance;
		};
	}
	//////////////////////////////////////////////////////////////////////////
	bool MoviePlugin::_avaliable()
	{
		m_hashkey = CONFIG_VALUE( m_serviceProvider, "MoviePlugin", "HASHKEY", "" );

		if( m_hashkey.empty() == true )
		{
			return false;
		}

		if( m_hashkey.size() != 20 )
		{
			return false;
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool MoviePlugin::_initialize()
	{
		pybind::kernel_interface * kernel = pybind::get_kernel();
		
		m_instance = ae_create_movie_instance( m_hashkey.c_str(), &stdex_movie_alloc, &stdex_movie_alloc_n, &stdex_movie_free, &stdex_movie_free_n, 0, &stdex_movie_logerror, this );

		pybind::interface_<Movie2, pybind::bases<Node, Animatable> >( kernel, "Movie2", false )
			.def( "setResourceMovie2", &Movie2::setResourceMovie2 )
			.def( "getResourceMovie2", &Movie2::getResourceMovie2 )
			.def( "setCompositionName", &Movie2::setCompositionName )
			.def( "getCompositionName", &Movie2::getCompositionName )
			.def( "getDuration", &Movie2::getDuration )
			.def( "setWorkAreaFromEvent", &Movie2::setWorkAreaFromEvent )
			.def( "removeWorkArea", &Movie2::removeWorkArea )
			.def( "playSubComposition", &Movie2::playSubComposition )
			.def( "stopSubComposition", &Movie2::stopSubComposition )
			;

		pybind::interface_<ResourceMovie2, pybind::bases<ResourceReference> >( kernel, "ResourceMovie2", false )
			;
		
		SCRIPT_SERVICE( m_serviceProvider )
			->setWrapper( STRINGIZE_STRING_LOCAL( m_serviceProvider, "Movie2" ), new ScriptWrapper<Movie2>() );

		SCRIPT_SERVICE( m_serviceProvider )
			->setWrapper( STRINGIZE_STRING_LOCAL( m_serviceProvider, "ResourceMovie2" ), new ScriptWrapper<ResourceMovie2>() );

		if( PROTOTYPE_SERVICE( m_serviceProvider )
			->addPrototype( STRINGIZE_STRING_LOCAL( m_serviceProvider, "Node" ), STRINGIZE_STRING_LOCAL( m_serviceProvider, "Movie2" ), new NodePrototypeGenerator<Movie2, 128> ) == false )
		{
			return false;
		}

		if( PROTOTYPE_SERVICE( m_serviceProvider )
			->addPrototype( STRINGIZE_STRING_LOCAL( m_serviceProvider, "Resource" ), STRINGIZE_STRING_LOCAL( m_serviceProvider, "ResourceMovie2" ), new ResourceMovie2PrototypeGenerator(m_instance) ) == false )
		{
			return false;
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void MoviePlugin::_finalize()
	{
		ae_delete_movie_instance( m_instance );
		m_instance = nullptr;

        SCRIPT_SERVICE( m_serviceProvider )
            ->removeWrapper( STRINGIZE_STRING_LOCAL( m_serviceProvider, "Movie2" ) );

        SCRIPT_SERVICE( m_serviceProvider )
            ->removeWrapper( STRINGIZE_STRING_LOCAL( m_serviceProvider, "ResourceMovie2" ) );

        PROTOTYPE_SERVICE( m_serviceProvider )
            ->removePrototype( STRINGIZE_STRING_LOCAL( m_serviceProvider, "Node" ), STRINGIZE_STRING_LOCAL( m_serviceProvider, "Movie2" ) );
        
        PROTOTYPE_SERVICE( m_serviceProvider )
            ->removePrototype( STRINGIZE_STRING_LOCAL( m_serviceProvider, "Resource" ), STRINGIZE_STRING_LOCAL( m_serviceProvider, "ResourceMovie2" ) );
	}
}
