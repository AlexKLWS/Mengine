#pragma once

#include "Kernel/IntrusivePtr.h"
#include "Kernel/IntrusivePtrBase.h"

#include "stdex/intrusive_slug_linked_ptr.h"

namespace Mengine
{ 
    template<class T, class D = void>
    class IntrusiveSlugHead
        : public stdex::intrusive_slug_linked_ptr<T, D, IntrusivePtr, IntrusivePtrBase>
    {
    public:
        IntrusiveSlugHead()
            : stdex::intrusive_slug_linked_ptr<T, D, IntrusivePtr, IntrusivePtrBase>( stdex::EILT_END )
        {
        }

    protected:
        void incref() override
        {
        }

        void decref() override
        {
        }

        uint32_t getrefcount() const override
        {
            return 1;
        };
    };
}
