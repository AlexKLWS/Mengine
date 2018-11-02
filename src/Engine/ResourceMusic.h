#pragma once

#include "Kernel/Resource.h"
#include "Kernel/Content.h"

#include "Config/Vector.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    class ResourceMusic
        : public Resource
        , public Content
    {
        DECLARE_VISITABLE( Resource );

    public:
        ResourceMusic();
        ~ResourceMusic() override;

    public:
        void setVolume( float _volume );
        float getVolume() const;

        void setExternal( bool _external );
        bool isExternal() const;

    protected:
        bool _convert() override;

    private:
        float m_volume;
        bool m_external;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<ResourceMusic> ResourceMusicPtr;
    //////////////////////////////////////////////////////////////////////////
}
