#include "BaseUpdation.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    BaseUpdation::BaseUpdation()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    BaseUpdation::~BaseUpdation()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseUpdation::update( const UpdateContext * _context )
    {
        this->_update( _context );
    }
}