#pragma once

#include "tinyini/tinyini.hpp"

#include "Interface/ServiceInterface.h"
#include "Interface/InputStreamInterface.h"
#include "Interface/FileGroupInterface.h"
#include "Interface/MemoryInterface.h"

#include "Kernel/Params.h"
#include "Kernel/String.h"
#include "Kernel/ConstString.h"
#include "Kernel/VectorConstString.h"
#include "Kernel/FilePath.h"
#include "Kernel/Tags.h"
#include "Kernel/FilePath.h"
#include "Kernel/Resolution.h"
#include "Kernel/Viewport.h"
#include "Kernel/AspectRatioViewports.h"
#include "Kernel/VectorAspectRatioViewports.h"
#include "Kernel/Color.h"

#ifndef MENGINE_INI_BUFFER_SIZE 
#define MENGINE_INI_BUFFER_SIZE 16384U
#endif

namespace Mengine
{
    namespace IniUtil
    {
        //////////////////////////////////////////////////////////////////////////
        struct IniStore
        {
            IniStore();
            ~IniStore();

            FilePath path;
            MemoryInterfacePtr memory;

            tinyini_t ini;
        };
        //////////////////////////////////////////////////////////////////////////
        bool loadIni( IniStore & _ini, const FileGroupInterfacePtr & _fileGroup, const FilePath & _path );
        bool loadIni( IniStore & _ini, const InputStreamInterfacePtr & _stream );
        //////////////////////////////////////////////////////////////////////////
        bool hasIniSection( const IniStore & _ini, const Char * _section );
        bool hasIniValue( const IniStore & _ini, const Char * _section, const Char * _key );
        //////////////////////////////////////////////////////////////////////////
        bool getIniValue( const IniStore & _ini, const Char * _section, const Char * _key, const Char ** _value );
        bool getIniValue( const IniStore & _ini, const Char * _section, const Char * _key, ConstString & _value );
        bool getIniValue( const IniStore & _ini, const Char * _section, const Char * _key, Tags & _value );
        bool getIniValue( const IniStore & _ini, const Char * _section, const Char * _key, Resolution & _value );
        bool getIniValue( const IniStore & _ini, const Char * _section, const Char * _key, Color & _value );
        bool getIniValue( const IniStore & _ini, const Char * _section, const Char * _key, FilePath & _value );
        bool getIniValue( const IniStore & _ini, const Char * _section, const Char * _key, bool & _value );
        bool getIniValue( const IniStore & _ini, const Char * _section, const Char * _key, int32_t & _value );
        bool getIniValue( const IniStore & _ini, const Char * _section, const Char * _key, uint32_t & _value );
        bool getIniValue( const IniStore & _ini, const Char * _section, const Char * _key, uint64_t & _value );
        bool getIniValue( const IniStore & _ini, const Char * _section, const Char * _key, float & _value );
        bool getIniValue( const IniStore & _ini, const Char * _section, const Char * _key, double & _value );
        bool getIniValue( const IniStore & _ini, const Char * _section, const Char * _key, VectorString & _values );
        bool getIniValue( const IniStore & _ini, const Char * _section, const Char * _key, VectorConstString & _values );
        bool getIniValue( const IniStore & _ini, const Char * _section, const Char * _key, VectorFilePath & _values );
        bool getIniValue( const IniStore & _ini, const Char * _section, const Char * _key, VectorAspectRatioViewports & _values );
        //////////////////////////////////////////////////////////////////////////
        bool getIniAllSettings( const IniStore & _ini, const Char * _section, MapParams & _values );        
    }
}