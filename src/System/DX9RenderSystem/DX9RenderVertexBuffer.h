#pragma once

#include "Interface/RenderSystemInterface.h"

#include "Core/ServantBase.h"

#include <d3d9.h>

namespace Mengine
{
	class DX9RenderVertexBuffer
		: public ServantBase<RenderVertexBufferInterface>
	{
	public:
		DX9RenderVertexBuffer();
		~DX9RenderVertexBuffer() override;

	public:
		bool initialize( IDirect3DDevice9 * _pD3DDevice, DWORD _vertexDeclaration, uint32_t _vertexSize, EBufferType _bufferType );

    protected:
        uint32_t getVertexCount() const override;
        uint32_t getVertexSize() const override;

    protected:
        bool enable() override;
        void disable() override;

    protected:
        bool resize( uint32_t _count ) override;

    protected:
        MemoryInterfacePtr lock( uint32_t _offset, uint32_t _size, EBufferLockFlag _flags ) override;
		bool unlock() override;

    protected:
        void draw( const void * _buffer, size_t _size ) override;

	protected:
		IDirect3DDevice9 * m_pD3DDevice;
		DWORD m_vertexDeclaration;

        EBufferType m_bufferType;
        uint32_t m_vertexSize;
		uint32_t m_vertexCount;

		DWORD m_usage;
		D3DFORMAT m_format;
		D3DPOOL m_pool;
		IDirect3DVertexBuffer9 * m_pVB;        
	};
	//////////////////////////////////////////////////////////////////////////
	typedef stdex::intrusive_ptr<DX9RenderVertexBuffer> DX9RenderVertexBufferPtr;
}