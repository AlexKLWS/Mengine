#pragma once

#include "Core/ConstString.h"

namespace Mengine
{
    typedef stdex::intrusive_ptr<class ResourceImage> ResourceImagePtr;
    typedef stdex::intrusive_ptr<class RenderMaterialInterface> RenderMaterialInterfacePtr;
    typedef stdex::intrusive_ptr<class RenderTextureInterface> RenderTextureInterfacePtr;

	enum EMaterialBlendMode 
	{	
		EMB_NORMAL,
		EMB_ADD,
		EMB_SCREEN,
		EMB_MULTIPLY
	};

	namespace Helper
	{
        RenderMaterialInterfacePtr makeSolidMaterial( const ConstString & _materialName, EMaterialBlendMode _blendMode, bool _solid );
		RenderMaterialInterfacePtr makeImageMaterial( const ResourceImagePtr & _resourceImage, const ConstString & _materialName, EMaterialBlendMode _blendMode, bool _disableTextureColor, bool _solid );
		RenderMaterialInterfacePtr makeTextureMaterial( const RenderTextureInterfacePtr * _textures, uint32_t _textureCount, const ConstString & _materialName, EMaterialBlendMode _blendMode, bool _premultiply, bool _disableTextureColor, bool _solid );
	}

	class Materialable
	{
	public:
		Materialable();
		~Materialable();

	public:
		void setMaterialName( const ConstString & _materialName );
		const ConstString & getMaterialName() const;

	public:
		void setDisableTextureColor( bool _disable );
		bool getDisableTextureColor() const;

    public:
        void setPremultiplyAlpha( bool _premultiplyAlpha );
        bool getPremultiplyAlpha() const;

	public:
		void setBlendMode( EMaterialBlendMode _blendMode );
		EMaterialBlendMode getBlendMode() const;

	public:
		void releaseMaterial();
		
	protected:
		void updateMaterial_() const;
		
	protected:
		virtual RenderMaterialInterfacePtr _updateMaterial() const = 0;

	protected:
        RenderMaterialInterfacePtr makeSolidMaterial( bool _solid ) const;
        RenderMaterialInterfacePtr makeImageMaterial( const ResourceImagePtr & _resourceImage, bool _solid ) const;
		RenderMaterialInterfacePtr makeTextureMaterial( const RenderTextureInterfacePtr * _textures, uint32_t _textureCount, bool _solid ) const;

	public:
		inline const RenderMaterialInterfacePtr & getMaterial() const;

	protected:
		inline void invalidateMaterial();
		inline bool isInvalidateMaterial() const;

	protected:
		ConstString m_materialName;

		EMaterialBlendMode m_blendMode;

        bool m_premultiplyAlpha;
		bool m_disableTextureColor;

		mutable RenderMaterialInterfacePtr m_material;
		mutable bool m_invalidateMaterial;
	};
	//////////////////////////////////////////////////////////////////////////
	inline void Materialable::invalidateMaterial()
	{
		m_invalidateMaterial = true;
	}
	//////////////////////////////////////////////////////////////////////////
	inline bool Materialable::isInvalidateMaterial() const
	{
		return m_invalidateMaterial;
	}
	//////////////////////////////////////////////////////////////////////////
	inline const RenderMaterialInterfacePtr & Materialable::getMaterial() const
	{
		if( this->isInvalidateMaterial() == true )
		{
			this->updateMaterial_();
		}

		return m_material;
	}
}