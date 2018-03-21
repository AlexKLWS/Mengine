#include "MemoryProxy.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    MemoryProxy::MemoryProxy()
        : m_ptr( nullptr )
        , m_size( 0 )
        , m_file( nullptr )
        , m_line( 0 )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    MemoryProxy::~MemoryProxy()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void MemoryProxy::setMemory( void * _ptr, size_t _size, const Char * _file, uint32_t _line )
    {
        m_ptr = _ptr;
        m_size = _size;

        m_file = _file;
        m_line = _line;
    }
    //////////////////////////////////////////////////////////////////////////
    Pointer MemoryProxy::getMemory() const
    {
        return m_ptr;
    }
    //////////////////////////////////////////////////////////////////////////
    size_t MemoryProxy::getSize() const
    {
        return m_size;
    }
    //////////////////////////////////////////////////////////////////////////
    bool MemoryProxy::empty() const
    {
        return false;
    }
}