#include "ResourceModel3D.h"

#include "Interface/ConverterInterface.h"
#include "Interface/CodecInterface.h"
#include "Interface/ResourceInterface.h"

#include "Metacode/Metacode.h"

#include "Logger/Logger.h"

namespace Mengine
{
	//////////////////////////////////////////////////////////////////////////
	ResourceModel3D::ResourceModel3D()
		: m_imageResource(nullptr)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	void ResourceModel3D::setDataflow( const ConstString & _dataflow )
	{
		m_dataflow = _dataflow;
	}
	//////////////////////////////////////////////////////////////////////////
	const ConstString & ResourceModel3D::getDataflow() const
	{
		return m_dataflow;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ResourceModel3D::_loader( const Metabuf::Metadata * _meta )
	{
		const Metacode::Meta_Data::Meta_DataBlock::Meta_ResourceModel3D * metadata 
			= static_cast<const Metacode::Meta_Data::Meta_DataBlock::Meta_ResourceModel3D *>(_meta);

        m_path = metadata->get_File_Path();

		metadata->get_File_Dataflow( &m_dataflow );
		metadata->get_File_Converter( &m_converterType );

        m_imageResourceName = metadata->get_Image_Resource();
		
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ResourceModel3D::_convert()
	{
		if( m_path.empty() == true )
		{
			return false;
		}

		if( m_converterType.empty() == false )
		{
			if( CONVERTER_SERVICE()
				->convert( m_converterType, m_category, m_path, m_path ) == false )
			{
				LOGGER_ERROR("ResourceModel3D::_convert: '%s' can't convert '%s':'%s'"
					, this->getName().c_str() 
					, m_path.c_str()
					, m_converterType.c_str()
					);

				return false;
			}
		}

		if( m_dataflow.empty() == true )
		{
			m_dataflow = CODEC_SERVICE()
				->findCodecType( m_path );
		}

		if( m_dataflow.empty() == true )
		{
			LOGGER_ERROR("ResourceMovie::_convert: '%s' you must determine codec for file '%s'"
				, this->getName().c_str()
				, m_path.c_str()
				);

			return false;
		}

		return true;			 
	}
	//////////////////////////////////////////////////////////////////////////
	bool ResourceModel3D::_compile()
	{
		if( m_imageResourceName.empty() == true )
		{
			return false;
		}

		const ConstString & category = this->getCategory();

		InputStreamInterfacePtr stream = FILE_SERVICE()
			->openInputFile( category, m_path, false );

		if( stream == nullptr )
		{
			return false;
		}

		m_model = DATA_SERVICE()
			->dataflowT<Model3DInterfacePtr>( m_dataflow, stream );

		if( m_model == nullptr )
		{
			return false;
		}

		ResourceImagePtr resource = RESOURCE_SERVICE()
			->getResourceT<ResourceImagePtr>( m_imageResourceName );

		if( resource == nullptr )
		{
			LOGGER_ERROR("ResourceModel3D::_compile: '%s' not found image resource '%s'"
				, this->getName().c_str()
				, m_imageResourceName.c_str() 
				);

			return false;
		}

		m_imageResource = resource;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void ResourceModel3D::_release()
	{
		m_model = nullptr;

		if( m_imageResource != nullptr )
		{
			m_imageResource->decrementReference();
			m_imageResource = nullptr;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool ResourceModel3D::_isValid() const
	{
		const ConstString & category = this->getCategory();

		InputStreamInterfacePtr stream = FILE_SERVICE()
			->openInputFile( category, m_path, false );

		if( stream == nullptr )
		{
			return false;				 
		}

		Model3DInterfacePtr model = DATA_SERVICE()
			->dataflowT<Model3DInterfacePtr>( m_dataflow, stream );

		if( model == nullptr )
		{
			return false;
		}

		return true;
	}
}