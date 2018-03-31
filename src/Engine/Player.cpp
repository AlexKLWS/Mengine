#include "Player.h"

#include "Interface/RenderSystemInterface.h"
#include "Interface/ScriptSystemInterface.h"
#include "Interface/ParticleSystemInterface.h"
#include "Interface/ResourceInterface.h"
#include "Interface/GraveyardInterface.h"
#include "Interface/TimelineInterface.h"

#include "Interface/UnicodeInterface.h"

#include "Interface/NotificationServiceInterface.h"
#include "Interface/StringizeInterface.h"
#include "Interface/PrefetcherInterface.h"
#include "Interface/ModuleInterface.h"
#include "Interface/ApplicationInterface.h"

#include "Config/Stringstream.h"

#include "Kernel/Scene.h"

#include "Kernel/RenderViewport.h"
#include "Kernel/RenderClipplane.h"
#include "Kernel/RenderCameraOrthogonal.h"
#include "Kernel/RenderCameraHelper.h"
#include "Kernel/Arrow.h"

#include "Kernel/ScheduleManager.h"

#include "Factory/FactoryDefault.h"

#include "Consts.h"

#include "Logger/Logger.h"

#include "TextField.h"

#include "Kernel/ResourceReference.h"

#include "math/mat3.h"

#include "pybind/pybind.hpp"

#include <iomanip>

//////////////////////////////////////////////////////////////////////////
SERVICE_EXTERN( MousePickerSystem );
SERVICE_EXTERN( GlobalHandleSystem );
//////////////////////////////////////////////////////////////////////////
SERVICE_FACTORY( PlayerService, Mengine::Player );
//////////////////////////////////////////////////////////////////////////
namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////    
    namespace
    {
        class PlayerResourceUselessCompile
        {
        public:
            PlayerResourceUselessCompile()
            {
            }

            ~PlayerResourceUselessCompile()
            {
            }

        public:
            void begin()
            {
                m_observerResourceCompile = NOTIFICATION_SERVICE()
                    ->addObserverMethod( NOTIFICATOR_RESOURCE_COMPILE, this, &PlayerResourceUselessCompile::resourceCompile );

                m_observerResourceRelease = NOTIFICATION_SERVICE()
                    ->addObserverMethod( NOTIFICATOR_RESOURCE_RELEASE, this, &PlayerResourceUselessCompile::resourceRelease );
            }

            void end()
            {
                m_observerResourceCompile = nullptr;
                m_observerResourceRelease = nullptr;
            }

        protected:
            void resourceCompile( ResourceReference * _resource )
            {
                m_resources.push_back( _resource );
            }

            void resourceRelease( ResourceReference * _resource )
            {
                TVectorResourceDesc::iterator it_remove =
                    std::find( m_resources.begin(), m_resources.end(), _resource );

                if( it_remove == m_resources.end() )
                {
                    return;
                }

                LOGGER_PERFORMANCE("Useless Compile %s %s:%s"
                    , _resource->getType().c_str()
                    , _resource->getGroup().c_str()
                    , _resource->getName().c_str()
                    );

                m_resources.erase( it_remove );
            }

        protected:
            ObserverInterfacePtr m_observerResourceCompile;
            ObserverInterfacePtr m_observerResourceRelease;

            typedef stdex::vector<ResourceReference *> TVectorResourceDesc;
            TVectorResourceDesc m_resources;
        };
    }
	//////////////////////////////////////////////////////////////////////////
	Player::Player()
		: m_scene( nullptr )
		, m_arrow( nullptr )
		, m_globalScene( nullptr )
		, m_scheduleManager( nullptr )
		, m_scheduleManagerGlobal( nullptr )
		, m_arrowCamera2D( nullptr )
		, m_renderCamera( nullptr )
		, m_renderViewport( nullptr )
		, m_renderClipplane( nullptr )
		, m_renderTarget( nullptr )
		, m_switchSceneTo( nullptr )
		, m_mousePickerSystem( nullptr )
		//, m_switchScene2( true )
		//, m_switchScene( false )
		//, m_removeScene( false )
		//, m_destroyOldScene( false )
		//, m_restartScene( false )
		, m_arrowHided( false )
		, m_fps( 0 )
		, m_affectorable( nullptr )
		, m_affectorableGlobal( nullptr )
		, m_showDebugText( 0 )
		, m_debugText( nullptr )
		, m_camera2D( nullptr )
		, m_viewport2D( nullptr )
		, m_debugCamera2D( nullptr )
		, m_focus( true )
	{
	}
	//////////////////////////////////////////////////////////////////////////
	Player::~Player()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool Player::setCurrentScene( Scene * _scene, bool _destroyOld, const SceneChangeCallbackInterfacePtr & _cb )
	{
		if( _scene == nullptr )
		{
			return false;
		}

        MODULE_SERVICE()
            ->messageAll( STRINGIZE_STRING_LOCAL( "onSceneChange" ), TMapWParams() );

        Scene * oldScene = m_scene;
        m_scene = nullptr;

        if( m_arrow != nullptr )
        {
            m_arrow->removeFromParent();
            m_arrow->disable();
        }

        m_scheduleManager->removeAll();
        m_affectorable->stopAllAffectors();

        if( oldScene != nullptr && _destroyOld == true )
        {
            oldScene->destroy();

            //NODE_SERVICE() 
            //    ->clearHomeless();

            if( SERVICE_EXIST( Mengine::GraveyardInterface ) == true )
            {
                GRAVEYARD_SERVICE()
                    ->clearTextures();
            }
        }
        
        if( _cb != nullptr )
        {
            _cb->onSceneChange( nullptr, false, false );
        }

        m_scene = _scene;

        if( m_mousePickerSystem != nullptr )
        {
            m_mousePickerSystem->setScene( m_scene );
        }

        if( _cb != nullptr )
        {
            _cb->onSceneChange( m_scene, false, false );
        }

#   ifndef MENGINE_MASTER_RELEASE
        PlayerResourceUselessCompile unlessCompile;
        unlessCompile.begin();
#   endif

        m_scene->enable();

#   ifndef MENGINE_MASTER_RELEASE
        unlessCompile.end();
#   endif

        if( m_arrow != nullptr )
        {
            m_arrow->enable();
        }

        if( _cb != nullptr )
        {
            _cb->onSceneChange( m_scene, true, false );
        }

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Player::restartCurrentScene( const SceneChangeCallbackInterfacePtr & _cb )
	{
        MODULE_SERVICE()
            ->messageAll( STRINGIZE_STRING_LOCAL( "onSceneChange" ), TMapWParams() );

        if( m_arrow != nullptr )
        {
            m_arrow->removeFromParent();
            m_arrow->disable();
        }

        m_scheduleManager->removeAll();
        m_affectorable->stopAllAffectors();

        m_scene->disable();

        //NODE_SERVICE() 
        //    ->clearHomeless();

        if( SERVICE_EXIST( Mengine::GraveyardInterface ) == true )
        {
            GRAVEYARD_SERVICE()
                ->clearTextures();
        }

        if( _cb != nullptr )
        {
            _cb->onSceneChange( nullptr, false, false );
        }

        if( _cb != nullptr )
        {
            _cb->onSceneChange( m_scene, false, false );
        }

#   ifndef MENGINE_MASTER_RELEASE
        PlayerResourceUselessCompile unlessCompile;
        unlessCompile.begin();
#   endif

        m_scene->enable();

#   ifndef MENGINE_MASTER_RELEASE
        unlessCompile.end();
#   endif

        if( m_arrow != nullptr )
        {
            m_arrow->enable();
        }

        if( _cb != nullptr )
        {
            _cb->onSceneChange( m_scene, true, false );
        }

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Player::removeCurrentScene( const SceneChangeCallbackInterfacePtr & _cb )
	{
        if( m_scene != nullptr )
        {
            if( m_arrow != nullptr )
            {
                m_arrow->removeFromParent();
                m_arrow->disable();
            }

            m_scheduleManager->removeAll();
            m_affectorable->stopAllAffectors();

            m_globalHandleSystem->clear();

            m_scene->release();

            //NODE_SERVICE() 
            //    ->clearHomeless();

            if( SERVICE_EXIST( Mengine::GraveyardInterface ) == true )
            {
                GRAVEYARD_SERVICE()
                    ->clearTextures();
            }

            m_scene = nullptr;
        }

        if( m_mousePickerSystem != nullptr )
        {
            m_mousePickerSystem->setScene( nullptr );
        }

        if( _cb != nullptr )
        {
            _cb->onSceneChange( nullptr, false, true );
        }

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::destroyCurrentScene()
	{
		if( m_scene != nullptr )
		{
            Scene * destroyScene = m_scene;
            m_scene = nullptr;

            destroyScene->destroy();
		}
	}
	//////////////////////////////////////////////////////////////////////////
	Scene * Player::getCurrentScene()
	{
		return m_scene;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Player::createGlobalScene()
	{
		Scene * scene = PROTOTYPE_SERVICE()
			->generatePrototype( STRINGIZE_STRING_LOCAL( "Node" ), STRINGIZE_STRING_LOCAL( "Scene" ) );

		if( scene == nullptr )
		{
			return false;
		}

		m_globalScene = scene;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::removeGlobalScene()
	{
		if( m_globalScene != nullptr )
		{
			m_globalScene->destroy();
			m_globalScene = nullptr;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	Scene * Player::getGlobalScene()
	{
		return m_globalScene;
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::setArrow(Arrow * _arrow)
	{
		if( m_arrow != nullptr )
		{
			m_arrow->disable();
		}

		m_arrow = _arrow;

		if( m_arrow != nullptr)
		{
			m_arrow->setRenderCamera( m_arrowCamera2D );
			m_arrow->setRenderViewport( m_renderViewport );
			m_arrow->setRenderClipplane( m_renderClipplane );

			if( m_scene != nullptr )
			{
				m_arrow->enable();
			}
		}

        if( m_mousePickerSystem != nullptr )
        {
		    m_mousePickerSystem->setArrow( m_arrow );
        }
	}
	//////////////////////////////////////////////////////////////////////////
	Arrow * Player::getArrow() const
	{
		return m_arrow;
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::calcGlobalMouseWorldPosition( const mt::vec2f & _screenPoint, mt::vec2f & _worldPoint )
	{
		Helper::screenToWorldPosition( m_renderCamera, _screenPoint, _worldPoint );
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::calcGlobalMouseWorldDelta( const mt::vec2f & _screenPoint, const mt::vec2f & _screenDeltha, mt::vec2f & _worldDeltha )
	{
		Helper::screenToWorldDelta( m_renderCamera, _screenPoint, _screenDeltha, _worldDeltha );
	}
	//////////////////////////////////////////////////////////////////////////
	ScheduleManagerInterfacePtr Player::createSchedulerManager()
	{
		ScheduleManagerInterfacePtr sm = m_factoryScheduleManager->createObject();

		m_schedulers.push_back( sm );

		return sm;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Player::destroySchedulerManager( const ScheduleManagerInterfacePtr & _scheduler )
	{	
		TVectorUserScheduler::iterator it_found = std::find( m_schedulers.begin(), m_schedulers.end(), _scheduler );

		if( it_found == m_schedulers.end() )
		{
			LOGGER_ERROR("Player::destroySchedulerManager scheduler not found!"
				);

			return false;
		}
		
		m_schedulers.erase( it_found );

		return true;
	}
    //////////////////////////////////////////////////////////////////////////
    MousePickerSystemInterface * Player::getMousePickerSystem() const
    {
        return m_mousePickerSystem;
    }
    //////////////////////////////////////////////////////////////////////////
    GlobalHandleSystemInterface * Player::getGlobalHandleSystem() const
    {
        return m_globalHandleSystem;
    }
	//////////////////////////////////////////////////////////////////////////
	const ScheduleManagerInterfacePtr & Player::getScheduleManager() const
	{
		return m_scheduleManager;
	}
	//////////////////////////////////////////////////////////////////////////
	const ScheduleManagerInterfacePtr & Player::getScheduleManagerGlobal() const
	{
		return m_scheduleManagerGlobal;
	}	
	//////////////////////////////////////////////////////////////////////////
	Affectorable * Player::getAffectorable() const
	{
		return m_affectorable;
	}
	//////////////////////////////////////////////////////////////////////////
	Affectorable * Player::getAffectorableGlobal() const
	{
		return m_affectorableGlobal;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Player::_initialize()
	{
		m_mousePickerSystem = SERVICE_GENERATE( MousePickerSystem, MousePickerSystemInterface );
        m_globalHandleSystem = SERVICE_GENERATE( GlobalHandleSystem, GlobalHandleSystemInterface );

        m_factoryScheduleManager = new FactoryDefault<ScheduleManager>();

		ScheduleManagerInterfacePtr scheduleManager = m_factoryScheduleManager->createObject();

		m_scheduleManager = scheduleManager;

		ScheduleManagerInterfacePtr scheduleManagerGlobal = m_factoryScheduleManager->createObject();

		m_scheduleManagerGlobal = scheduleManagerGlobal;

		m_affectorable = new Affectorable;
		m_affectorableGlobal = new Affectorable;
        
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::_finalize()
	{
        if( m_scene != nullptr )
        {
            m_scene->destroy();
            m_scene = nullptr;
        }

		if( m_globalScene != nullptr )
		{
			m_globalScene->destroy();
			m_globalScene = nullptr;
		}

        if( m_camera2D != nullptr )
        {
            m_camera2D->destroy();
			m_camera2D = nullptr;
		}

		if( m_debugCamera2D != nullptr )
		{
			m_debugCamera2D->destroy();
			m_debugCamera2D = nullptr;
		}

		if( m_arrowCamera2D != nullptr )
		{
			m_arrowCamera2D->destroy();
			m_arrowCamera2D = nullptr;
		}

		if( m_viewport2D != nullptr )
		{
			m_viewport2D->destroy();
			m_viewport2D = nullptr;
		}

		if( m_mousePickerSystem != nullptr )
		{
			delete m_mousePickerSystem;
			m_mousePickerSystem = nullptr;
		}

		if( m_globalHandleSystem != nullptr )
		{
			delete m_globalHandleSystem;
			m_globalHandleSystem = nullptr;
		}

		m_scheduleManager = nullptr;
		m_scheduleManagerGlobal = nullptr;

		m_schedulers.clear();

		if( m_affectorable != nullptr )
		{
			delete m_affectorable;
			m_affectorable = nullptr;
		}

		if( m_affectorableGlobal != nullptr )
		{
			delete m_affectorableGlobal;
			m_affectorableGlobal = nullptr;
		}

        m_factoryScheduleManager = nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::initializeRenderResources()
	{
		m_debugText = NODE_SERVICE()->
			createNodeT<TextField *>( CONST_STRING( TextField ) );

		m_debugText->setFontName( STRINGIZE_STRING_LOCAL( "__CONSOLE_FONT__") );
		m_debugText->setTextID( STRINGIZE_STRING_LOCAL( "__ID_TEXT_CONSOLE") );
		m_debugText->setLocalColor( ColourValue( 1.0, 0.0, 0.0, 1.0 ) );
		m_debugText->enable();

		const Resolution & contentResolution = APPLICATION_SERVICE()
			->getContentResolution();

		mt::vec2f cr;
		contentResolution.calcSize( cr );
		Viewport vp( 0.f, 0.f, cr.x, cr.y );

		m_camera2D = NODE_SERVICE() 
			->createNodeT<RenderCameraOrthogonal *>( CONST_STRING( RenderCameraOrthogonal ) );

		m_camera2D->setOrthogonalViewport( vp );
				
		m_camera2D->enable();

		this->setRenderCamera( m_camera2D );

		m_viewport2D = NODE_SERVICE() 
			->createNodeT<RenderViewport *>( CONST_STRING( RenderViewport ) );

		m_viewport2D->setViewport( vp );
		m_viewport2D->enable();

		this->setRenderViewport( m_viewport2D );

		m_arrowCamera2D = NODE_SERVICE() 
			->createNodeT<RenderCameraOrthogonal *>( CONST_STRING( RenderCameraOrthogonal ) );

		m_arrowCamera2D->setOrthogonalViewport( vp );
		m_arrowCamera2D->enable();

		if( m_arrow != nullptr )
		{
			m_arrow->setRenderCamera( m_arrowCamera2D );
			m_arrow->setRenderViewport( m_renderViewport );
			m_arrow->setRenderClipplane( m_renderClipplane );
		}
		
		m_debugCamera2D = NODE_SERVICE() 
			->createNodeT<RenderCameraOrthogonal *>( CONST_STRING( RenderCameraOrthogonal ) );

		m_debugCamera2D->setOrthogonalViewport( vp );

		m_debugCamera2D->enable();
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::finalizeRenderResources()
	{
		if( m_debugText != nullptr	 )
		{
			m_debugText->destroy();
			m_debugText = nullptr;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool Player::handleKeyEvent( const InputKeyEvent & _event )
	{
		bool handler = false;

		if( m_globalHandleSystem != nullptr )
		{
			m_globalHandleSystem->handleKeyEvent( _event );
		}

		if( m_mousePickerSystem != nullptr )
		{
			if( handler == false )
			{
				handler = m_mousePickerSystem->handleKeyEvent( _event );
			}
		}

		return handler;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Player::handleTextEvent( const InputTextEvent & _event )
	{
		bool handler = false;

		if( m_globalHandleSystem != nullptr )
		{
			m_globalHandleSystem->handleTextEvent( _event );
		}

		if( m_mousePickerSystem != nullptr )
		{
			if( handler == false )
			{
				handler = m_mousePickerSystem->handleTextEvent( _event );
			}
		}

		return handler;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Player::handleMouseButtonEvent( const InputMouseButtonEvent & _event )
	{
		if( m_globalHandleSystem != nullptr )
		{
			m_globalHandleSystem->handleMouseButtonEvent( _event );
		}

        bool handler = false;

		if( m_mousePickerSystem != nullptr )
		{
			if( handler == false )
			{
				handler = m_mousePickerSystem->handleMouseButtonEvent( _event );
			}
		}

		return handler;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Player::handleMouseButtonEventBegin( const InputMouseButtonEvent & _event )
	{
		if( m_globalHandleSystem != nullptr )
		{
			m_globalHandleSystem->handleMouseButtonEventBegin( _event );
		}

        bool handler = false;

        if( m_mousePickerSystem != nullptr )
        {
            if( handler == false )
            {
				handler = m_mousePickerSystem->handleMouseButtonEventBegin( _event );
            }
		}

        return handler;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Player::handleMouseButtonEventEnd( const InputMouseButtonEvent & _event )
	{
		if( m_globalHandleSystem != nullptr )
		{
			m_globalHandleSystem->handleMouseButtonEventEnd( _event );
		}

        bool handler = false;

        if( m_mousePickerSystem != nullptr )
        {
            if( handler == false )
            {
				handler = m_mousePickerSystem->handleMouseButtonEventEnd( _event );
            }
		}

        return handler;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Player::handleMouseMove( const InputMouseMoveEvent & _event )
	{
		if( m_globalHandleSystem != nullptr )
		{
			m_globalHandleSystem->handleMouseMove( _event );
		}

        bool handler = false;

		if( m_mousePickerSystem != nullptr )
		{
			if( handler == false )
			{
				handler = m_mousePickerSystem->handleMouseMove( _event );
			}
		}

		return handler;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Player::handleMouseWheel( const InputMouseWheelEvent & _event )
	{
		if( m_globalHandleSystem != nullptr )
		{
			m_globalHandleSystem->handleMouseWheel( _event );
		}

		bool handler = false;

		if( m_mousePickerSystem != nullptr )
		{
			if( handler == false )
			{
				handler = m_mousePickerSystem->handleMouseWheel( _event );
			}
		}

		return handler;
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::tick( float _time, float _timing )
	{
		static float fpsTiming = 0.0f;
		fpsTiming += _timing;
		if( fpsTiming >= 1000.0f )
		{
            const RenderServiceDebugInfo & debugInfo = RENDER_SERVICE()
                ->getDebugInfo();

			m_fps = debugInfo.frameCount;

            RENDER_SERVICE()
                ->resetFrameCount();

			while( fpsTiming >= 1000.0f )
			{
				fpsTiming -= 1000.0f;
			}
		}

		//if( PhysicEngine2D::get()->isWorldCreate() )
		//{
		//	const mt::vec2f & arrowPos = 
		//		m_arrow->getLocalPosition() + m_renderCamera2D->getViewport().begin;

		//	PhysicEngine2D::get()
		//		->onMouseMove( arrowPos );
		//}

		if( m_camera2D != nullptr )
		{
			m_camera2D->update( _time, _timing );
		}

		if( m_arrowCamera2D != nullptr )
		{
			m_arrowCamera2D->update( _time, _timing );
		}

		if( m_arrow != nullptr )
		{
			m_arrow->update( _time, _timing );
		}

		if( m_globalScene != nullptr )
		{
			m_globalScene->update( _time, _timing );
		}

		if( m_scene != nullptr )
		{
			m_scene->update( _time, _timing );
		}

		if( m_scheduleManager != nullptr )
		{
			m_scheduleManager->update( _time, _timing );
		}

		if( m_scheduleManagerGlobal != nullptr )
		{
			m_scheduleManagerGlobal->update( _time, _timing );
		}

		for( TVectorUserScheduler::iterator
			it = m_schedulers.begin(),
			it_end = m_schedulers.end();
		it != it_end;
		++it )
		{
			const ScheduleManagerInterfacePtr & sm = *it;

			sm->update( _time, _timing );
		}

		if( m_affectorable != nullptr )
		{
			m_affectorable->updateAffectors( _time, _timing );
		}

		if( m_affectorableGlobal != nullptr )
		{
			m_affectorableGlobal->updateAffectors( _time, _timing );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool Player::update()
	{
		if( m_mousePickerSystem != nullptr )
		{
			m_mousePickerSystem->update();
		}

		if( m_globalHandleSystem != nullptr )
		{
			m_globalHandleSystem->update();
		}

        NODE_SERVICE() 
            ->clearHomeless();

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::setRenderCamera( RenderCameraInterface * _camera)
	{
		m_renderCamera = _camera;

		if( m_mousePickerSystem != nullptr )
		{
			m_mousePickerSystem->setRenderCamera( m_renderCamera );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	const RenderCameraInterface * Player::getRenderCamera() const
	{
		return m_renderCamera;
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::setRenderViewport( RenderViewportInterface * _viewport )
	{
		m_renderViewport = _viewport;

		if( m_mousePickerSystem != nullptr )
		{
			m_mousePickerSystem->setRenderViewport( m_renderViewport );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	const RenderViewportInterface * Player::getRenderViewport() const 
	{
		return m_renderViewport;
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::setRenderClipplane( RenderClipplaneInterface * _clipplane )
	{
		m_renderClipplane = _clipplane;

		if( m_mousePickerSystem != nullptr )
		{
			m_mousePickerSystem->setRenderClipplane( _clipplane );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	const RenderClipplaneInterface * Player::getRenderClipplane() const
	{
		return m_renderClipplane;
	}
    //////////////////////////////////////////////////////////////////////////
    namespace
    {
        //////////////////////////////////////////////////////////////////////////
        class VisitorPlayerFactoryManager
            : public VisitorPrototypeGenerator
        {
        public:
            VisitorPlayerFactoryManager( const ConstString & _category, Stringstream & _ss )
                : m_category( _category )
                , m_ss( _ss )
            {
            }

        private:
            VisitorPlayerFactoryManager & operator = ( const VisitorPlayerFactoryManager & _vpfm )
            {
                (void)_vpfm;

                return *this;
            }

        protected:
            void visit( const ConstString & _category, const ConstString & _type, const PrototypeGeneratorInterfacePtr & _generator ) override
            {
				if (m_category != _category)
				{
					return;
				}

                uint32_t count = _generator->count();

                if( count == 0 )
                {
                    return;
                }

                m_ss << "Factory Object " << _type.c_str() << ": " << count << "\n";
            }

        protected:
            ConstString m_category;
            Stringstream & m_ss;
        };
    }
	//////////////////////////////////////////////////////////////////////////
	void Player::render()
	{
		//if( this->isChangedScene() == true )
		//{
		//	return;
		//}

        uint32_t debugMask = APPLICATION_SERVICE()
            ->getDebugMask();

		RenderState state;
		state.viewport = m_renderViewport;
		state.camera = m_renderCamera;
		state.clipplane = m_renderClipplane;
		state.target = m_renderTarget;

		RenderServiceInterface * renderService = RENDER_SERVICE();

		if( m_scene != nullptr )
		{
			m_scene->render( renderService, &state, debugMask );
		}

		MODULE_SERVICE()
			->render( &state, debugMask );

		RENDER_SERVICE()
			->endLimitRenderObjects();

		if( m_arrow != nullptr )
		{
			m_arrow->render( renderService, &state, debugMask );
		}

//#	ifndef MENGINE_MASTER_RELEASE

		if( m_showDebugText != 0 )
		{			
			const RenderServiceDebugInfo & rdi = 
				RENDER_SERVICE()->getDebugInfo();

			//size_t particlesCount = 
			//	Holder<ParticleEngine>::get()->getFrameParticlesCount();

			uint32_t particlesCount = 0;

			Stringstream ss;

			ss << "FPS: " << m_fps << std::endl;

			if( m_showDebugText > 1 )
			{
				const Resolution & contentResolution = APPLICATION_SERVICE()
					->getContentResolution();

				double sreenfillrate = rdi.fillrate / double( contentResolution.getWidth() * contentResolution.getHeight() );

				ss << "Fillrate " << std::setiosflags(std::ios::fixed) << std::setprecision(2) << sreenfillrate << " (Object " << rdi.object << " Triangle " << rdi.triangle << ")" << std::endl;
				ss << "DIP: " << rdi.dips << std::endl;

				ERenderBatchMode mode = RENDER_SERVICE()
					->getBatchMode();

				ss << "Smart Batch: " << mode << " " << rdi.batch <<  std::endl;

				uint32_t textureMemoryUse = RENDER_SYSTEM()
					->getTextureMemoryUse();

				ss << "Texture Memory Usage: " << (float)textureMemoryUse / (1024.f*1024.f) << std::endl;

				uint32_t textureCount = RENDER_SYSTEM()
					->getTextureCount();

				ss << "Texture Count: " << textureCount << std::endl;

				ss << "Particles: " << particlesCount << std::endl;
			}

			if( m_showDebugText == 2 )
			{
				class CompileResourceVisitor
					: public Visitor
					, public ConcreteVisitor<ResourceReference>
				{
				public:
					CompileResourceVisitor()
						: m_count(0)
					{
					}

				public:
					uint32_t getCount() const
					{
						return m_count;
					}

				protected:
					void accept( ResourceReference * _resource )
					{
						if( _resource->isCompile() == false )
						{
							return;
						}

						++m_count;
					}

				protected:
					uint32_t m_count;
				};

				CompileResourceVisitor crv;

				RESOURCE_SERVICE()
					->visitResources( &crv );

				ss << "Resources: " << crv.getCount() << std::endl;

				PrefetcherDebugInfo pdInfo = PREFETCHER_SERVICE()
					->getDebugInfo();

				ss << "Prefetcher " << pdInfo.receiverCount << std::endl;

				MousePickerSystemInterface * mousePickerSystem = PLAYER_SERVICE()
                    ->getMousePickerSystem();

				ss << "PickerTrapCount:" << mousePickerSystem->getPickerTrapCount() << std::endl;
			}
			else if( m_showDebugText == 3 )
            {
			    VisitorPlayerFactoryManager pfmv_node(CONST_STRING( Node), ss);

			    PROTOTYPE_SERVICE()
				    ->visitGenerators( &pfmv_node );

				VisitorPlayerFactoryManager pfmv_surface(CONST_STRING( Surface), ss);

				PROTOTYPE_SERVICE()
					->visitGenerators(&pfmv_surface);
            }
			else if( m_showDebugText == 4 )
            {
                class MyVisitorClassTypeScope
                    : public pybind::visitor_class_type_scope
                {
                public:
                    MyVisitorClassTypeScope( Stringstream & _ss )
                        : m_ss(_ss)
                        , m_count(0)
                    {
                    }

                protected:
                    void operator = ( const MyVisitorClassTypeScope & )
                    {
                    }

                protected:
                    void visit_scope( pybind::class_type_scope_interface * _scope ) override
                    {   
                        uint32_t count = _scope->getObjectCount();

						if( count == 0 )
						{
							return;
						}

						if( m_count % 3 == 0 )
						{
							m_ss << "Py: ";
						}
                        
						m_ss << _scope->get_name() << " " << count;
                        
						if( m_count % 3 != 2 )
						{
							m_ss << "        ";
						}
						else
						{
							m_ss << std::endl;
						}

                        ++m_count;
                    }

                protected:                    
                    Stringstream & m_ss;
                    uint32_t m_count;
                };

				pybind::kernel_interface * kernel = pybind::get_kernel();

                MyVisitorClassTypeScope mvcts(ss);
				kernel->visit_types_scope( &mvcts );
            }
            
			String text = ss.str();

			TVectorString args;
			args.push_back( text );
			m_debugText->setTextFormatArgs( args );

            float gameViewportAspect;
            Viewport gameViewport;

            APPLICATION_SERVICE()
                ->getGameViewport( gameViewportAspect, gameViewport );

            m_debugText->setLocalPosition( mt::vec3f(gameViewport.begin, 0.f) );

			const Resolution & resolution = APPLICATION_SERVICE()
				->getCurrentResolution();

			const Resolution & content = APPLICATION_SERVICE()
				->getContentResolution();

			mt::vec2f scale;
			content.calcScale( resolution, scale );

			m_debugText->setScale( mt::vec3f( scale, 1.f ) );

			m_debugText->render( renderService, &state, debugMask );
		}
//#	endif
		//m_renderCamera2D->setLocalPosition( pos );
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::onAppMouseLeave( const InputMousePositionEvent & _event )
	{
		if( m_arrow != nullptr )
		{
			m_arrow->onAppMouseLeave();
		}

		if( m_scene != nullptr && m_scene->isActivate() == true )
		{
			m_scene->onAppMouseLeave();
		}

		if( m_mousePickerSystem != nullptr )
		{
			m_mousePickerSystem->handleMouseLeave( _event );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::onAppMouseEnter( const InputMousePositionEvent & _event )
	{
		if( m_arrow != nullptr )
		{
			m_arrow->onAppMouseEnter();
		}

		if( m_scene != nullptr && m_scene->isActivate() == true )
		{
			m_scene->onAppMouseEnter();
		}

		if( m_mousePickerSystem != nullptr )
		{
			m_mousePickerSystem->handleMouseEnter( _event );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::onAppMousePosition( const InputMousePositionEvent & _event )
	{
		(void)_event;
		//if( m_mousePickerSystem != nullptr )
		//{
		//	m_mousePickerSystem->handleMousePosition( _event );
		//}
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::onFullscreen( const Resolution & _resolution, bool _fullscreen )
	{
		(void)_resolution;
        (void)_fullscreen;
	}
    //////////////////////////////////////////////////////////////////////////
    void Player::onFixedContentResolution( const Resolution & _resolution, bool _fixed )
    {
		(void)_resolution;
        (void)_fixed;
    }
	//////////////////////////////////////////////////////////////////////////
	void Player::onFixedDisplayResolution( const Resolution & _resolution, bool _fixed )
	{
		(void)_resolution;
		(void)_fixed;
	}
	//////////////////////////////////////////////////////////////////////////
	void Player::onFocus( bool _focus )
	{
		m_focus = _focus;

		if( m_scene != nullptr && m_scene->isActivate() == true )
		{
			m_scene->onFocus( _focus );
		}
	}
	//////////////////////////////////////////////////////////////////////////
//#	ifndef MENGINE_MASTER_RELEASE
	//////////////////////////////////////////////////////////////////////////
	void Player::toggleDebugText()
	{
		++m_showDebugText;
        m_showDebugText %= 5;
	}
	//////////////////////////////////////////////////////////////////////////
//#	endif
}