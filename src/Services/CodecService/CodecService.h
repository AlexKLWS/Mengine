#pragma once

#include "Interface/CodecServiceInterface.h"

#include "Kernel/ServiceBase.h"
#include "Kernel/ConstString.h"

#include "Config/Map.h"

namespace Mengine
{
    class CodecService
        : public ServiceBase<CodecServiceInterface>
    {
    public:
        CodecService();
        ~CodecService() override;

    public:
        bool _initializeService() override;
        void _finalizeService() override;

    public:
        DecoderInterfacePtr createDecoder( const ConstString & _type, const Char * _doc ) override;
        EncoderInterfacePtr createEncoder( const ConstString & _type, const Char * _doc ) override;

    public:
        bool registerCodecExt( const String & _ext, const ConstString & _codecType ) override;
        const ConstString & findCodecType( const FilePath & _path ) const override;

    protected:
        typedef Map<String, ConstString> MapCodecExts;
        MapCodecExts m_codecExts;
    };
}
