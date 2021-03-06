#include "ConfigService.h"

#include "Interface/FileServiceInterface.h"
#include "Interface/PlatformInterface.h"

#include "Kernel/Exception.h"
#include "Kernel/AssertionMemoryPanic.h"
#include "Kernel/AssertionFactory.h"
#include "Kernel/Logger.h"
#include "Kernel/Document.h"
#include "Kernel/FactoryPool.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////////
SERVICE_FACTORY( ConfigService, Mengine::ConfigService );
//////////////////////////////////////////////////////////////////////////
namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    ConfigService::ConfigService()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    ConfigService::~ConfigService()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool ConfigService::_initializeService()
    {
        m_factoryConfig = new FactoryPool<IniConfig, 16>();

        m_defaultConfig = m_factoryConfig->createObject( MENGINE_DOCUMENT_FUNCTION );

        SERVICE_WAIT( PlatformInterface, [this]()
        {
            const Tags & platformTags = PLATFORM_SERVICE()
                ->getPlatformTags();

            m_platformTags = platformTags;

            m_defaultConfig->setPlatformTags( m_platformTags );

            return true;
        } );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void ConfigService::_finalizeService()
    {
        m_platformTags.clear();
        
        m_defaultConfig = nullptr;

        MENGINE_ASSERTION_FACTORY_EMPTY( m_factoryConfig );

        m_factoryConfig = nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    ConfigInterfacePtr ConfigService::createConfig( const FileGroupInterfacePtr & _fileGroup, const FilePath & _filePath, const Char * _doc )
    {
        InputStreamInterfacePtr stream = FILE_SERVICE()
            ->openInputFile( _fileGroup, _filePath, false, _doc );

        MENGINE_ASSERTION_MEMORY_PANIC( stream, nullptr, "invalid open config '%s'"
            , _filePath.c_str()
        );

        INIConfigPtr config = m_factoryConfig->createObject( _doc );

        MENGINE_ASSERTION_MEMORY_PANIC( config, nullptr );

        config->setPlatformTags( m_platformTags );

        if( config->load( stream ) == false )
        {
            LOGGER_ERROR( "invalid load config '%s'"
                , _filePath.c_str()
            );

            return nullptr;
        }

        return config;
    }
    //////////////////////////////////////////////////////////////////////////
    bool ConfigService::loadDefaultConfig( const FileGroupInterfacePtr & _fileGroup, const FilePath & _filePath, const Char * _doc )
    {
        InputStreamInterfacePtr stream = FILE_SERVICE()
            ->openInputFile( _fileGroup, _filePath, false, _doc );

        MENGINE_ASSERTION_MEMORY_PANIC( stream, false, "invalid open config '%s'"
            , _filePath.c_str()
        );

        if( m_defaultConfig->load( stream ) == false )
        {
            LOGGER_ERROR( "invalid load config '%s'"
                , _filePath.c_str()
            );

            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    const ConfigInterfacePtr & ConfigService::getDefaultConfig() const
    {
        return m_defaultConfig;
    }
}
