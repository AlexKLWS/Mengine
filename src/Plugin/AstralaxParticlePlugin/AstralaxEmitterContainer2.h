#pragma once

#include "AstralaxInterface.h"

#include "Interface/MemoryInterface.h"

#include "AstralaxEmitter2.h"

#include "Kernel/Factorable.h"
#include "Kernel/Factory.h"

#include "Config/Vector.h"

#ifndef MENGINE_UNSUPPORT_PRAGMA_WARNING
#	pragma warning(push, 0) 
#endif 

#include "magic.h"

#ifndef MENGINE_UNSUPPORT_PRAGMA_WARNING
#	pragma warning(pop) 
#endif

namespace Mengine
{
    class AstralaxParticleSystem2;

    class AstralaxEmitterContainer2
        : public AstralaxEmitterContainerInterface
        , public Factorable
    {
    public:
        AstralaxEmitterContainer2();
        ~AstralaxEmitterContainer2() override;

    public:
        bool initialize( const InputStreamInterfacePtr & _stream, const ArchivatorInterfacePtr & _archivator );
        void finalize();

    public:
        bool isValid() const override;

    public:
        uint32_t getPtcId() const;

    public:
        void setAtlasResourceImage( uint32_t _index, const ResourceImagePtr & _resourceImage ) override;

    public:
        const ResourceImagePtr & getAtlasResourceImage( const Char * _file ) const;

    public:
        HM_EMITTER initialEmitterId() const;
        HM_EMITTER createEmitterId();
        void destroyEmitterId( HM_EMITTER _id );

    protected:
        bool loadContainer_( const void * _buffer, size_t _size, HM_FILE & _mf ) const;

    protected:
        HM_FILE m_mf;

        uint32_t m_dublicateCount;

        uint32_t m_ptcId;

        MemoryInterfacePtr m_memory;

        typedef Vector<ResourceImagePtr> VectorResourceImage;
        VectorResourceImage m_resourceImages;
    };

    typedef IntrusivePtr<AstralaxEmitterContainer2> AstralaxEmitterContainer2Ptr;
}