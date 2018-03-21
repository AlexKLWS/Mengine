#include "ThreadTaskPrefetchSoundDecoder.h"

#include "Interface/FileSystemInterface.h"
#include "Interface/CodecInterface.h"
#include "Interface/MemoryInterface.h"
#include "Interface/StringizeInterface.h"

#include "Factory/FactorableUnique.h"

#include "Logger/Logger.h"

namespace Mengine
{
	//////////////////////////////////////////////////////////////////////////
	ThreadTaskPrefetchSoundDecoder::ThreadTaskPrefetchSoundDecoder()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	void ThreadTaskPrefetchSoundDecoder::setSoundCodec( const ConstString & _codec )
	{
		m_codec = _codec;
	}
	//////////////////////////////////////////////////////////////////////////
	const SoundDecoderInterfacePtr & ThreadTaskPrefetchSoundDecoder::getDecoder() const
	{
		return m_soundDecoder;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ThreadTaskPrefetchSoundDecoder::_onRun()
	{
        if( ThreadTaskPrefetch::_onRun() == false )
        {
            return false;
        }

		m_group = FILE_SERVICE()
			->getFileGroup( m_pakName );

		if( m_group == nullptr )
		{
			LOGGER_ERROR("FileEngine::openInputFile can't get group '%s'"
				, m_pakName.c_str()
				);

			return false;
		}

		m_stream = m_group->createInputFile( m_filePath, false );

		if( m_stream == nullptr )
		{
			LOGGER_ERROR("FileEngine::openInputFile can't create input file '%s'"
				, m_pakName.c_str()
				);

			return false;
		}
		
		m_soundDecoder = CODEC_SERVICE()
			->createDecoderT<SoundDecoderInterfacePtr>( m_codec );

		if( m_soundDecoder == nullptr )
		{
			LOGGER_ERROR("ThreadTaskPrefetchSoundDecoder::_onRun: invalide create codec %s"
				, m_codec.c_str() 
				);

			return false;
		}
	
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ThreadTaskPrefetchSoundDecoder::_onMain()
	{		
		if( m_group->openInputFile( m_filePath, m_stream, 0, 0, false ) == false )
		{
			LOGGER_ERROR("ThreadTaskPrefetchSoundDecoder::_onRun: invalide open file '%s:%s'"
				, m_pakName.c_str()
				, m_filePath.c_str()
				);

			return false;
		}

		size_t stream_size = m_stream->size();

		MemoryInputInterfacePtr memoryInput = MEMORY_SERVICE()
			->createMemoryInput();

		if( memoryInput == nullptr )
		{
			LOGGER_ERROR("ThreadTaskPrefetchSoundDecoder::_onMain: '%s:%s' invalid create memory input"
				, m_pakName.c_str()
				, m_filePath.c_str()
				, stream_size
				);

			return false;
		}

		void * memory = memoryInput->newMemory( stream_size );

		if( memory == nullptr )
		{
			LOGGER_ERROR("ThreadTaskPrefetchSoundDecoder::_onMain: '%s:%s' invalid alloc memory '%d'"
				, m_pakName.c_str()
				, m_filePath.c_str() 
				, stream_size
				);

			return false;
		}

		if( m_stream->read( memory, stream_size ) != stream_size )
		{
			LOGGER_ERROR("ThreadTaskPrefetchSoundDecoder::_onMain: '%s:%s' invalid read stream '%d'"
				, m_pakName.c_str()
				, m_filePath.c_str() 
				, stream_size
				);

			return false;
		}

		if( m_soundDecoder->prepareData( memoryInput ) == false )
		{
			LOGGER_ERROR("ThreadTaskPrefetchSoundDecoder::_onMain: decoder for file '%s:%s' was not initialize"
				, m_pakName.c_str()
				, m_filePath.c_str() 
				);

			return false;
		}

		return true;
	}
}
