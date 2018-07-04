#pragma once

#include "Interface/ServiceInterface.h"
#include "Interface/StreamInterface.h"

#include "Core/FilePath.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
	class FileInputStreamInterface
		: public InputStreamInterface
	{
	public:
		virtual bool open( const FilePath & _relationPath, const FilePath & _folderPath, const FilePath & _filePath, size_t _offset, size_t _size ) = 0;
	};
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<FileInputStreamInterface> FileInputStreamInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
}

