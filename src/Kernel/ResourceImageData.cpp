#include "ResourceImageData.h"

#include "Interface/FileSystemInterface.h"
#include "Interface/ImageCodecInterface.h"
#include "Interface/ConfigInterface.h"

#include "Metacode/Metacode.h"

#include "Logger/Logger.h"

#include "Core/ConstString.h"
#include "Core/String.h"

#include "Core/MemoryAllocator.h"

namespace Mengine
{
	//////////////////////////////////////////////////////////////////////////
	ResourceImageData::ResourceImageData()
		: m_maxSize(0.f, 0.f)
		, m_buffer(nullptr)
		, m_validNoExist(false)		
	{
	}
    //////////////////////////////////////////////////////////////////////////
    ResourceImageData::~ResourceImageData()
    {
    }
	//////////////////////////////////////////////////////////////////////////
	const FilePath & ResourceImageData::getFileName() const
	{
		return m_fileName;
	}
	//////////////////////////////////////////////////////////////////////////
	const ConstString & ResourceImageData::getCodecType() const
	{
		return m_codecType;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ResourceImageData::_isValid() const
	{
        const FileGroupInterfacePtr & category = this->getCategory();

        bool exist = category->existFile( m_fileName );
		
		if( exist == false )
		{
			if( m_validNoExist == true )
			{
				return true;
			}

			LOGGER_ERROR("ResourceImageDefault::_isValid %s not exist file %s:%s"
				, this->getName().c_str()
                , this->getCategory()->getName().c_str()
				, this->getFileName().c_str()
				);

			return false;
		}
        
        InputStreamInterfacePtr stream = FILE_SERVICE()
			->openInputFile( category, m_fileName, false );

        if( stream == nullptr )
        {
			LOGGER_ERROR("ResourceImageDefault::_isValid %s invalid open file %s:%s"
				, this->getName().c_str()
				, this->getCategory()->getName().c_str()
				, this->getFileName().c_str()
				);

            return false;
        }

        ImageDecoderInterfacePtr imageDecoder = CODEC_SERVICE()
            ->createDecoderT<ImageDecoderInterfacePtr>( m_codecType );

        if( imageDecoder == nullptr )
        {
			LOGGER_ERROR("ResourceImageDefault::_isValid %s file %s:%s invalid decoder %s"
                , this->getName().c_str()
                , this->getCategory()->getName().c_str()
                , this->getFileName().c_str()
				, this->getCodecType().c_str()
				);

            return false;
        }

		if( imageDecoder->prepareData( stream ) == false )
		{
			LOGGER_ERROR("ResourceImageDefault::_isValid %s file %s:%s decoder initialize failed %s"
                , this->getName().c_str()
                , this->getCategory()->getName().c_str()
                , this->getFileName().c_str()
                , this->getCodecType().c_str()
				);

			return false;
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ResourceImageData::_loader( const Metabuf::Metadata * _meta )
	{
        const Metacode::Meta_Data::Meta_DataBlock::Meta_ResourceImageData * metadata 
            = static_cast<const Metacode::Meta_Data::Meta_DataBlock::Meta_ResourceImageData *>(_meta);
		        
        m_fileName = metadata->get_File_Path();

        if( metadata->get_File_Codec( &m_codecType ) == false )
        {
            m_codecType = CODEC_SERVICE()
                ->findCodecType( m_fileName );
        }
		        		
		m_maxSize = metadata->get_File_MaxSize();

		m_validNoExist = false;
		metadata->get_File_NoExist( &m_validNoExist );
		
        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	bool ResourceImageData::_compile()
	{	
        const FileGroupInterfacePtr & category = this->getCategory();
					
		InputStreamInterfacePtr stream = FILE_SERVICE()
            ->openInputFile( category, m_fileName, false );

		if( stream == nullptr )
		{
			LOGGER_ERROR("ResourceImageData::_compile: Image file '%s:%s' was not found"
				, this->getCategory()->getName().c_str()
				, this->getFileName().c_str() 
				);

			return false;
		}	

		ImageDecoderInterfacePtr imageDecoder = CODEC_SERVICE()
			->createDecoderT<ImageDecoderInterfacePtr>( m_codecType );

		if( imageDecoder == nullptr )
		{
			LOGGER_ERROR("ResourceImageData::_compile: Image decoder '%s' for file '%s:%s' was not found"
				, this->getCodecType().c_str()
                , this->getCategory()->getName().c_str()
                , this->getFileName().c_str()
				);

			return false;
		}

		if( imageDecoder->prepareData( stream ) == false )
		{
			LOGGER_ERROR("ResourceImageData::_compile: Image decoder '%s' for file '%s:%s' was not found"
                , this->getCodecType().c_str()
                , this->getCategory()->getName().c_str()
                , this->getFileName().c_str()
				);

			return false;
		}

		const ImageCodecDataInfo * dataInfo = imageDecoder->getCodecDataInfo();

		uint32_t width = dataInfo->width;
		uint32_t height = dataInfo->height;
		uint32_t channels = dataInfo->channels;
		PixelFormat format = dataInfo->format;

		size_t memorySize = Helper::getTextureMemorySize( width, height, channels, 1, format );

		m_buffer = Helper::allocateArrayT<unsigned char>( memorySize );

		ImageCodecOptions options;
		options.pitch = width * channels;
		options.channels = channels;

		if( imageDecoder->setOptions( &options ) == false )
		{
			LOGGER_ERROR("ResourceImageData::_compile: Image decoder '%s' for file '%s:%s' invalid optionize"
                , this->getCodecType().c_str()
                , this->getCategory()->getName().c_str()
                , this->getFileName().c_str()
				);

			return false;
		}
		
		if( imageDecoder->decode( m_buffer, memorySize ) == 0 )
		{
			LOGGER_ERROR("ResourceImageData::_compile: Image decoder '%s' for file '%s:%s' invalid decode"
                , this->getCodecType().c_str()
                , this->getCategory()->getName().c_str()
                , this->getFileName().c_str()
				);

			return false;
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void ResourceImageData::_release()
	{
		Helper::freeMemoryT( m_buffer );
		m_buffer = nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	void ResourceImageData::setImagePath( const FilePath & _imagePath )
	{
        m_fileName = _imagePath;

        m_codecType = CODEC_SERVICE()
            ->findCodecType( _imagePath );

		m_maxSize = mt::vec2f(0.f, 0.f);
	}
	//////////////////////////////////////////////////////////////////////////
	const FilePath & ResourceImageData::getImagePath() const
	{
		return m_fileName;
	}
	//////////////////////////////////////////////////////////////////////////
	const mt::vec2f & ResourceImageData::getImageMaxSize() const
	{
		return m_maxSize;
	}
	//////////////////////////////////////////////////////////////////////////
    uint8_t * ResourceImageData::getImageBuffer() const
	{
		return m_buffer;
	}
}
