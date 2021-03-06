#pragma once

#include "SpineInterface.h"

#include "Kernel/Resource.h"
#include "Kernel/Content.h"

#include "spine/spine.h"

namespace Mengine
{
    class ResourceSpineAtlas
        : public Resource
    {
        DECLARE_VISITABLE( Resource );

    public:
        virtual spAtlas * getSpineAtlas() const = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef stdex::intrusive_ptr<ResourceSpineAtlas> ResourceSpineAtlasPtr;
    //////////////////////////////////////////////////////////////////////////
}
