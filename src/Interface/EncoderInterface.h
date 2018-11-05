#pragma once

#include "Config/Typedef.h"

#include "Interface/CodecInterface.h"
#include "Interface/OutputStreamInterface.h"

#include "Kernel/Mixin.h"

namespace Mengine
{
    class EncoderInterface
        : public Mixin
    {
    public:
        virtual bool initialize( const OutputStreamInterfacePtr & _stream ) = 0;
        virtual void finalize() = 0;

    public:
        virtual bool setOptions( CodecOptions * _options ) = 0;

    public:
        virtual size_t encode( const void * _buffer, size_t _size, const CodecDataInfo* _dataInfo ) = 0;

    public:
        virtual OutputStreamInterfacePtr getStream() const = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<EncoderInterface> EncoderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
}
