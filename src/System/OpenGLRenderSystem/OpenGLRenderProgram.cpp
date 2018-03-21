#include "OpenGLRenderProgram.h"

#include "OpenGLRenderError.h"

#include "Logger/Logger.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    OpenGLRenderProgram::OpenGLRenderProgram()
        : m_program( 0 )
        , m_samplerCount( 0 )
    {
        for( uint32_t i = 0; i != EPML_MAX_COUNT; ++i )
        {
            m_matrixLocation[i] = -1;
        }

        for( uint32_t i = 0; i != MENGINE_MAX_TEXTURE_STAGES; ++i )
        {
            m_samplerLocation[i] = -1;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    OpenGLRenderProgram::~OpenGLRenderProgram()
    {
        this->release();
    }
    //////////////////////////////////////////////////////////////////////////
    GLuint OpenGLRenderProgram::getProgram() const
    {
        return m_program;
    }
    //////////////////////////////////////////////////////////////////////////
    const ConstString & OpenGLRenderProgram::getName() const
    { 
        return m_name;
    }
    //////////////////////////////////////////////////////////////////////////
    RenderVertexAttributeInterfacePtr OpenGLRenderProgram::getVertexAttribute() const
    {
        return m_vertexAttribute;
    }
    //////////////////////////////////////////////////////////////////////////
    RenderFragmentShaderInterfacePtr OpenGLRenderProgram::getFragmentShader() const
    {
        return m_fragmentShader;
    }
    //////////////////////////////////////////////////////////////////////////
    RenderVertexShaderInterfacePtr OpenGLRenderProgram::getVertexShader() const
    {
        return m_vertexShader;
    }
    //////////////////////////////////////////////////////////////////////////
    bool OpenGLRenderProgram::initialize( const ConstString & _name, const OpenGLRenderVertexShaderPtr & _vertexShader, const OpenGLRenderFragmentShaderPtr & _fragmentShader, const OpenGLRenderVertexAttributePtr & _vertexAttribute, uint32_t _samplerCount )
    {
        m_name = _name;
        m_vertexShader = _vertexShader;
        m_fragmentShader = _fragmentShader;
        m_vertexAttribute = _vertexAttribute;
        m_samplerCount = _samplerCount;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool OpenGLRenderProgram::compile()
    {
        if( m_samplerCount > MENGINE_MAX_TEXTURE_STAGES )
        {
            LOGGER_ERROR("'%s' don't support sampler count %d max %d"
                , m_name.c_str()
                , m_samplerCount
                , MENGINE_MAX_TEXTURE_STAGES
                );

            return false;
        }

        GLuint program;
        GLCALLR( program, glCreateProgram, () );

        if( program == 0 )
        {
            LOGGER_ERROR("'%s' invalid create program"
                , m_name.c_str()
                );

            return false;
        }

        if( m_vertexShader != nullptr )
        {
            m_vertexShader->attach( program );
        }

        if( m_fragmentShader != nullptr )
        {
            m_fragmentShader->attach( program );
        }

        m_vertexAttribute->bind( program );
        
        GLCALL( glLinkProgram, (program) );

        GLint linked;
        GLCALL( glGetProgramiv, (program, GL_LINK_STATUS, &linked) );

        if( linked == GL_FALSE )
        {
            GLchar errorLog[1024] = { 0 };
            GLCALL( glGetProgramInfoLog, (program, 1023, NULL, errorLog) );

            LOGGER_ERROR("'%s' - shader linking error '%s'"
                , m_name.c_str()
                , errorLog
                );

            return false;
        }

        const char * matrix_uniforms[] = {"viewMatrix", "projectionMatrix", "worldMatrix", "correctionMatrix", "vpMatrix", "wvpMatrix"};

        for( uint32_t i = 0; i != EPML_MAX_COUNT; ++i )
        {
            const char * uniform = matrix_uniforms[i];

            GLint location;
            GLCALLR( location, glGetUniformLocation, (program, uniform) );

            m_matrixLocation[i] = location;
        }
        
        for( uint32_t index = 0; index != m_samplerCount; ++index )
        {
            char samplerVar[16];
            sprintf( samplerVar, "inSampler%u", index );

            GLint location;
            GLCALLR( location, glGetUniformLocation, (program, samplerVar) );

            m_samplerLocation[index] = location;
        }

        m_program = program;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void OpenGLRenderProgram::release()
    {
        for( uint32_t i = 0; i != EPML_MAX_COUNT; ++i )
        {
            m_matrixLocation[i] = -1;
        }

        for( uint32_t i = 0; i != MENGINE_MAX_TEXTURE_STAGES; ++i )
        {
            m_samplerLocation[i] = -1;
        }

        if( m_program != 0 )
        {
            GLCALL( glDeleteProgram, (m_program) );
            m_program = 0;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void OpenGLRenderProgram::enable() const
    {
        GLCALL( glUseProgram, (m_program) );
    }
    //////////////////////////////////////////////////////////////////////////
    void OpenGLRenderProgram::disable() const
    {
        GLCALL( glUseProgram, (0) );
    }
    //////////////////////////////////////////////////////////////////////////
    void OpenGLRenderProgram::bindMatrix( const mt::mat4f & _worldMatrix, const mt::mat4f & _viewMatrix, const mt::mat4f & _projectionMatrix ) const
    {
        if( m_matrixLocation[EPML_VIEW] != -1 )
        {
            GLCALL( glUniformMatrix4fv, (m_matrixLocation[EPML_VIEW], 1, GL_FALSE, _viewMatrix.buff()) );
        }

        if( m_matrixLocation[EPML_PROJECTION] != -1 )
        {
            GLCALL( glUniformMatrix4fv, (m_matrixLocation[EPML_PROJECTION], 1, GL_FALSE, _projectionMatrix.buff()) );
        }

        if( m_matrixLocation[EPML_WORLD] != -1 )
        {
            GLCALL( glUniformMatrix4fv, (m_matrixLocation[EPML_WORLD], 1, GL_FALSE, _worldMatrix.buff()) );
        }

        if( m_matrixLocation[EPML_VIEW_PROJECTION] != -1 )
        {
            mt::mat4f vpMat = _viewMatrix * _projectionMatrix;

            GLCALL( glUniformMatrix4fv, (m_matrixLocation[EPML_VIEW_PROJECTION], 1, GL_FALSE, vpMat.buff()) );
        }

        if( m_matrixLocation[EPML_WORLD_VIEW_PROJECTION] != -1 )
        {
            mt::mat4f wvpMat = _viewMatrix * _projectionMatrix * _worldMatrix;

            GLCALL( glUniformMatrix4fv, (m_matrixLocation[EPML_WORLD_VIEW_PROJECTION], 1, GL_FALSE, wvpMat.buff()) );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void OpenGLRenderProgram::bindTexture( uint32_t _textureInd ) const
    {   
        if(_textureInd >= m_samplerCount )
        {
            LOGGER_ERROR("'%s' invalid support sampler count %d max %d"
                , m_name.c_str()
                , _textureInd
                , m_samplerCount
                );

            return;
        }

        int location = m_samplerLocation[_textureInd];

        GLCALL( glUniform1i, (location, _textureInd) );
    }
    //////////////////////////////////////////////////////////////////////////
}
