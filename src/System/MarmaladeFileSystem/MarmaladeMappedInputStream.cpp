#	include "MarmaladeMappedInputStream.h"

#	include "Interface/LogSystemInterface.h"
#	include "Interface/UnicodeInterface.h"

#   include "Core/MemoryProxyInput.h"

#   include "Logger/Logger.h"

namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	MarmaladeMappedInputStream::MarmaladeMappedInputStream()
        : m_serviceProvider(nullptr)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	MarmaladeMappedInputStream::~MarmaladeMappedInputStream()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	void MarmaladeMappedInputStream::setServiceProvider( ServiceProviderInterface * _serviceProvider )
	{
		m_serviceProvider = _serviceProvider;
	}
    //////////////////////////////////////////////////////////////////////////
    InputStreamInterfacePtr MarmaladeMappedInputStream::createFileStream()
    {
        MemoryInput * memory = m_factoryMemoryInput.createObjectT();

        return memory;
    }
    //////////////////////////////////////////////////////////////////////////
    bool MarmaladeMappedInputStream::openFileStream( const InputStreamInterfacePtr & _stream, size_t _offset, size_t _size, void ** _memory )
    {
        MemoryInput * memory = stdex::intrusive_get<MemoryInput>(_stream);

        void * buffer = memory->newMemory( _size );

        m_inputStream.seek( _offset );
        m_inputStream.read( buffer, _size );

		if( _memory != nullptr )
		{
			*_memory = buffer;
		}

		return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool MarmaladeMappedInputStream::initialize( const FilePath & _folder, const FilePath & _fileName )
    {
        if( m_inputStream.open( _folder, _fileName ) == false )
        {
            return false;
        }

        return true;
    }
}	// namespace Menge
