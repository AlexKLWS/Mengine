#pragma once

#include "DevelopmentConverter.h"

namespace Mengine
{
	class VideoConverterFFMPEGToOGVA
		: public DevelopmentConverter
	{
	public:
		VideoConverterFFMPEGToOGVA();
		~VideoConverterFFMPEGToOGVA();

	public:
		bool initialize() override;

    public:
		bool convert() override;
        	
    protected:
		bool convert_( const FilePath & _input, const FilePath & _output );
    };
}	
