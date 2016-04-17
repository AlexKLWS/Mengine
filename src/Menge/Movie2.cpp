#	include "Movie2.h"

#   include "Interface/TimelineInterface.h"
#   include "Interface/NodeInterface.h"
#   include "Interface/StringizeInterface.h"

#	include "Video.h"
#	include "SoundEmitter.h"

#	include "Consts.h"

#	include "Kernel/Materialable.h"

#	include "Logger/Logger.h"

namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	Movie2::Movie2()
	{	
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::setResourceMovie2( const ResourceMovie2Ptr & _resourceMovie2 )
	{
		if( m_resourceMovie2 == _resourceMovie2 )
		{
			return;
		}

		m_resourceMovie2 = _resourceMovie2;

		this->recompile();
	}
	//////////////////////////////////////////////////////////////////////////
	const ResourceMovie2Ptr & Movie2::getResourceMovie2() const
	{
		return m_resourceMovie2;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::setCompositionName( const ConstString & _name )
	{
		m_compositionName = _name;
	}
	//////////////////////////////////////////////////////////////////////////
	const ConstString & Movie2::getCompositionName() const
	{
		return m_compositionName;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Movie2::_play( float _time )
	{
		(void)_time;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Movie2::_restart( float _time, uint32_t _enumerator )
	{
		(void)_time;
		(void)_enumerator;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_pause( uint32_t _enumerator )
	{
		(void)_enumerator;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_resume( float _time, uint32_t _enumerator )
	{
		(void)_time;
		(void)_enumerator;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_stop( uint32_t _enumerator )
	{
		(void)_enumerator;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_end( uint32_t _enumerator )
	{
		(void)_enumerator;
	}
	//////////////////////////////////////////////////////////////////////////
	static void * ae_movie_composition_node_camera( const ae_string_t _name, const ae_vector3_t _position, const ae_vector3_t _direction, float _fov, float _width, float _height, void * _data )
	{
		Movie2 * movie2 = (Movie2 *)_data;

		ServiceProviderInterface * serviceProvider = movie2->getServiceProvider();

		ConstString c_name = Helper::stringizeString( serviceProvider, _name );

		Movie2::Camera * old_camera;
		if( movie2->getCamera( c_name, &old_camera ) == true )
		{
			return old_camera;
		}
		
		RenderCameraProjection * renderCameraProjection = NODE_SERVICE( serviceProvider )
			->createNodeT<RenderCameraProjection>( CONST_STRING( serviceProvider, RenderCameraProjection ) );

		renderCameraProjection->setName( c_name );

		mt::vec3f cameraPosition;
		cameraPosition.from_f3( _position );
		mt::vec3f cameraDirection;
		cameraDirection.from_f3( _direction );

		float aspect = _width / _height;
		
		renderCameraProjection->setLocalPosition( cameraPosition );
		renderCameraProjection->setDirection( cameraDirection, mt::vec3f(0.f, 1.f, 0.f) );
		renderCameraProjection->setCameraFOV( _fov );
		renderCameraProjection->setCameraAspect( aspect );
		
		RenderViewport * renderViewport = NODE_SERVICE( serviceProvider )
			->createNodeT<RenderViewport>( CONST_STRING( serviceProvider, RenderViewport ) );

		renderViewport->setName( c_name );

		Viewport vp;
		vp.begin.x = 0.f;
		vp.begin.y = 0.f;

		vp.end.x = _width;
		vp.end.y = _height;

		renderViewport->setViewport( vp );
		
		Movie2::Camera * new_camera = movie2->addCamera( c_name, renderCameraProjection, renderViewport );

		return new_camera;
	}
	//////////////////////////////////////////////////////////////////////////
	static void * ae_movie_composition_node_video( const aeMovieLayerData * _layerData, const aeMovieResourceVideo * _resource, void * _data )
	{
		Movie2 * movie2 = (Movie2 *)_data;

		ServiceProviderInterface * serviceProvider = movie2->getServiceProvider();

		ConstString c_name = Helper::stringizeString( serviceProvider, _layerData->name );

		Video * video = NODE_SERVICE( serviceProvider )
			->createNodeT<Video>( CONST_STRING( serviceProvider, Video ) );

		video->setName( c_name );

		ResourceVideo * resourceVideo = (ResourceVideo *)(_resource->base.data);

		video->setResourceVideo( resourceVideo );

		EMaterialBlendMode blend_mode = EMB_NORMAL;

		switch( _layerData->blend_mode )
		{
		case AE_MOVIE_BLEND_ADD:
			blend_mode = EMB_ADD;
			break;
		};

		video->setBlendMode( blend_mode );

		video->hide( true );
		video->enable();
		
		movie2->addChild( video );
		
		return video;
	}
	//////////////////////////////////////////////////////////////////////////
	static void * ae_movie_composition_node_sound( const aeMovieLayerData * _layerData, const aeMovieResourceSound * _resource, void * _data )
	{
		Movie2 * movie2 = (Movie2 *)_data;

		ServiceProviderInterface * serviceProvider = movie2->getServiceProvider();

		ConstString c_name = Helper::stringizeString( serviceProvider, _layerData->name );

		SoundEmitter * sound = NODE_SERVICE( serviceProvider )
			->createNodeT<SoundEmitter>( CONST_STRING( serviceProvider, SoundEmitter ) );

		sound->setName( c_name );

		ResourceSound * resourceSound = (ResourceSound *)(_resource->base.data);

		sound->setResourceSound( resourceSound );

		sound->enable();

		movie2->addChild( sound );

		return sound;
	}
	//////////////////////////////////////////////////////////////////////////
	Movie2::Camera * Movie2::addCamera( const ConstString & _name, RenderCameraProjection * _projection, RenderViewport * _viewport )
	{
		this->addChild( _projection );
		this->addChild( _viewport );

		Camera c;
		c.projection = _projection;
		c.viewport = _viewport;
		
		TMapCamera::iterator it_found = m_cameras.insert( std::make_pair(_name, c) ).first;

		Camera * new_camera = &it_found->second;

		return new_camera;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Movie2::hasCamera( const ConstString & _name ) const
	{ 
		TMapCamera::const_iterator it_found = m_cameras.find( _name );

		if( it_found == m_cameras.end() )
		{
			return false;
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Movie2::getCamera( const ConstString & _name, Camera ** _camera )
	{
		TMapCamera::iterator it_found = m_cameras.find( _name );

		if( it_found == m_cameras.end() )
		{
			return false;
		}

		Camera & camera = it_found->second;

		*_camera = &camera;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Movie2::_compile()
	{
		if( m_resourceMovie2 == nullptr )
		{
			LOGGER_ERROR(m_serviceProvider)("Movie2::_compile: '%s' can't setup resource"
				, this->getName().c_str()
				);

			return false;
		}

		if( m_resourceMovie2.compile() == false )
		{
			LOGGER_ERROR(m_serviceProvider)("Movie2::_compile '%s' resource %s not compile"
				, m_name.c_str() 
				, m_resourceMovie2->getName().c_str()
				);

			return false;
		}

		m_composition = m_resourceMovie2->createComposition( m_compositionName );

		if( m_composition == nullptr )
		{
			return false;
		}

		aeMovieCompositionNodeProvider provider;
		provider.camera_provider = &ae_movie_composition_node_camera;
		provider.video_provider = &ae_movie_composition_node_video;
		provider.sound_provider = &ae_movie_composition_node_sound;
		

		create_movie_composition_element( m_composition, &provider, this );
				
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_release()
	{	
		m_resourceMovie2->destroyComposition( m_composition );

		m_resourceMovie2.release();
	}
	//////////////////////////////////////////////////////////////////////////
	bool Movie2::_activate()
	{
		if( Node::_activate() == false )
		{
			return false;
		}

		if( m_composition->composition_data->flags & AE_MOVIE_COMPOSITION_ANCHOR_POINT )
		{	
			mt::vec3f origin;

			const float * anchorPoint = m_composition->composition_data->anchorPoint;

			origin.from_f3( anchorPoint );

			this->setOrigin( origin );
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_deactivate()
	{
		

		Node::_deactivate();
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_afterActivate()
	{
		Node::_afterActivate();

		bool autoPlay = this->getAutoPlay();

		if( autoPlay == true )
		{
			float time = TIMELINE_SERVICE( m_serviceProvider )
				->getTime();

			if( this->play( time ) == 0 )
			{
				LOGGER_ERROR( m_serviceProvider )("Movie2::_afterActivate '%s' resource '%s' auto play return 0"
					, this->getName().c_str()
					, this->m_resourceMovie2->getName().c_str()
					);

				return;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	static void begin_movie_node_animate( const void * _element, uint32_t _type, float _offset, void * _data )
	{
		(void)_element;
		(void)_type;
		(void)_data;

		switch( _type )
		{
		case AE_MOVIE_LAYER_TYPE_VIDEO:
			{
				Video * video = (Video *)_element;

				Movie2 * movie2 = (Movie2 *)_data;

				ServiceProviderInterface * serviceProvider = movie2->getServiceProvider();

				float time = TIMELINE_SERVICE( serviceProvider )
					->getTime();

				video->setTiming( _offset );

				if( video->play( time ) == 0 )
				{
					return;
				}
			}break;
		case AE_MOVIE_LAYER_TYPE_SOUND:
			{
				SoundEmitter * sound = (SoundEmitter *)_element;

				Movie2 * movie2 = (Movie2 *)_data;

				ServiceProviderInterface * serviceProvider = movie2->getServiceProvider();

				float time = TIMELINE_SERVICE( serviceProvider )
					->getTime();

				sound->setTiming( _offset );

				if( sound->play( time ) == 0 )
				{
					return;
				}
			}break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	static void end_movie_node_animate( const void * _element, uint32_t _type, void * _data )
	{
		(void)_element;
		(void)_type;
		(void)_data;

		switch( _type )
		{
		case AE_MOVIE_LAYER_TYPE_VIDEO:
			{
				Video * video = (Video *)_element;

				video->stop();
			}break;
		case AE_MOVIE_LAYER_TYPE_SOUND:
			{
				SoundEmitter * sound = (SoundEmitter *)_element;

				sound->stop();
			}break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_update( float _current, float _timing )
	{
		(void)_current;
		(void)_timing;

		if( this->isPlay() == false )
		{
			return;
		}

		update_movie_composition( m_composition, _timing, &begin_movie_node_animate, &end_movie_node_animate, this );
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_render( const RenderObjectState * _state )
	{
		aeMovieRenderContext context;
		if( begin_movie_render_context( m_composition, &context ) == AE_MOVIE_FAILED )
		{
			return;
		}
				
		m_meshes.reserve( context.render_count );
		m_meshes.clear();

		const mt::mat4f & wm = this->getWorldMatrix();

		//printf( "mesh --\n"
		//	);

		for( uint32_t i = 0; i != context.render_count; ++i )
		{
			aeMovieRenderMesh mesh;
			compute_movie_mesh( &context, i, &mesh );

			ResourceReference * resource_reference = (ResourceReference *)mesh.resource_data;

			switch( mesh.resource_type )
			{
			case AE_MOVIE_RESOURCE_SOLID:
				{
					m_meshes.push_back( Mesh() );
					Mesh & m = m_meshes.back();

					m.vertices.resize( mesh.vertexCount );

					ColourValue_ARGB color = Helper::makeARGB( mesh.r, mesh.g, mesh.b, mesh.a );

					for( uint32_t index = 0; index != mesh.vertexCount; ++index )
					{
						RenderVertex2D & v = m.vertices[index];

						mt::vec3f vp;
						vp.from_f3( mesh.position + index * 3 );

						mt::mul_v3_v3_m4( v.position, vp, wm );

						mt::vec2f uv;
						uv.from_f2( mesh.uv + index * 2 );

						v.uv[0] = uv;
						v.uv[1] = uv;

						v.color = color;
					}

					m.indices.assign( mesh.indices, mesh.indices + mesh.indexCount );

					EMaterialBlendMode blend_mode = EMB_NORMAL;

					switch( mesh.blend_mode )
					{
					case AE_MOVIE_BLEND_ADD:
						blend_mode = EMB_ADD;
						break;
					};

					m.material = Helper::makeTextureMaterial( m_serviceProvider, nullptr, 0, ConstString::none(), blend_mode, false, false, false );

					RenderObjectState state;
					if( mesh.camera_data != nullptr )
					{
						Movie2::Camera * camera = (Movie2::Camera *)mesh.camera_data;

						state.camera = camera->projection;
						state.viewport = camera->viewport;
						state.clipplane = _state->clipplane;
						state.target = _state->target;
					}
					else
					{
						state.camera = _state->camera;
						state.viewport = _state->viewport;
						state.clipplane = _state->clipplane;
						state.target = _state->target;
					}

					RENDER_SERVICE( m_serviceProvider )
						->addRenderObject( &state, m.material, &m.vertices[0], m.vertices.size(), &m.indices[0], m.indices.size(), nullptr, false );
				}break;
			case AE_MOVIE_RESOURCE_IMAGE:
				{
					ResourceImage * resource_image = static_cast<ResourceImage *>(resource_reference);

					m_meshes.push_back( Mesh() );
					Mesh & m = m_meshes.back();

					m.vertices.resize( mesh.vertexCount );

					ColourValue_ARGB color = Helper::makeARGB( mesh.r, mesh.g, mesh.b, mesh.a );

					for( uint32_t index = 0; index != mesh.vertexCount; ++index )
					{
						RenderVertex2D & v = m.vertices[index];

						mt::vec3f vp;
						vp.from_f3( mesh.position + index * 3 );

						mt::mul_v3_v3_m4( v.position, vp, wm );

						mt::vec2f uv;
						uv.from_f2( mesh.uv + index * 2 );

						const mt::uv4f & uv_image = resource_image->getUVImage();

						mt::multiply_tetragon_uv4_v2( v.uv[0], uv_image, uv );

						const mt::uv4f & uv_alpha = resource_image->getUVAlpha();
						
						mt::multiply_tetragon_uv4_v2( v.uv[1], uv_alpha, uv );

						v.color = color;
					}					

					m.indices.assign( mesh.indices, mesh.indices + mesh.indexCount );

					EMaterialBlendMode blend_mode = EMB_NORMAL;

					switch( mesh.blend_mode )
					{
					case AE_MOVIE_BLEND_ADD:
						blend_mode = EMB_ADD;
						break;
					};

					m.material = Helper::makeImageMaterial( m_serviceProvider, resource_image, ConstString::none(), blend_mode, false, false );
					
					RenderObjectState state;
					if( mesh.camera_data != nullptr )
					{
						Movie2::Camera * camera = (Movie2::Camera *)mesh.camera_data;

						state.camera = camera->projection;
						state.viewport = camera->viewport;
						state.clipplane = _state->clipplane;
						state.target = _state->target;
					}
					else
					{						
						state.camera = _state->camera;
						state.viewport = _state->viewport;
						state.clipplane = _state->clipplane;
						state.target = _state->target;
					}

					RENDER_SERVICE( m_serviceProvider )
						->addRenderObject( &state, m.material, &m.vertices[0], m.vertices.size(), &m.indices[0], m.indices.size(), nullptr, false );
				}break;
			case AE_MOVIE_RESOURCE_VIDEO:
				{
					//ResourceVideo * resource_video = static_cast<ResourceVideo *>(resource_reference);
					Video * video = static_cast<Video *>(mesh.element_data);

					m_meshes.push_back( Mesh() );
					Mesh & m = m_meshes.back();

					m.vertices.resize( mesh.vertexCount );

					ColourValue_ARGB color = Helper::makeARGB( mesh.r, mesh.g, mesh.b, mesh.a );

					for( uint32_t index = 0; index != mesh.vertexCount; ++index )
					{
						RenderVertex2D & v = m.vertices[index];

						mt::vec3f vp;
						vp.from_f3( mesh.position + index * 3 );

						mt::mul_v3_v3_m4( v.position, vp, wm );

						mt::vec2f uv;
						uv.from_f2( mesh.uv + index * 2 );

						const mt::vec4f & uv_video_mask = video->getUV();

						mt::uv4f uv_video( uv_video_mask );

						mt::multiply_tetragon_uv4_v2( v.uv[0], uv_video, uv );
						mt::multiply_tetragon_uv4_v2( v.uv[1], uv_video, uv );

						v.color = color;
					}

					m.indices.assign( mesh.indices, mesh.indices + mesh.indexCount );

					m.material = video->getMaterial();

					RenderObjectState state;
					if( mesh.camera_data != nullptr )
					{
						Movie2::Camera * camera = (Movie2::Camera *)mesh.camera_data;

						state.camera = camera->projection;
						state.viewport = camera->viewport;
						state.clipplane = _state->clipplane;
						state.target = _state->target;
					}
					else
					{
						state.camera = _state->camera;
						state.viewport = _state->viewport;
						state.clipplane = _state->clipplane;
						state.target = _state->target;
					}

					video->updateVideoBuffer();

					RENDER_SERVICE( m_serviceProvider )
						->addRenderObject( &state, m.material, &m.vertices[0], m.vertices.size(), &m.indices[0], m.indices.size(), nullptr, false );
				}break;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool Movie2::_interrupt( uint32_t _enumerator )
	{
		(void)_enumerator;

		return true;
	}
}