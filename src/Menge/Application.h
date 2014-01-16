#	pragma once

#	include "Interface/ApplicationInterface.h"

#   include "Interface/PrototypeManagerInterface.h"
#   include "Interface/AmplifierServiceInterface.h"
#	include "Interface/TextInterface.h"

#   include "Interface/InputSystemInterface.h"
#   include "Interface/RenderSystemInterface.h"
#	include "Interface/SoundSystemInterface.h"

#   include "Interface/ParticleSystemInterface.h"
#   include "Interface/ScriptSystemInterface.h"
#   include "Interface/EventInterface.h"
#   include "Interface/PhysicSystem2DInterface.h"

#   include "Interface/StringizeInterface.h"

#	include "Interface/NotificationServiceInterace.h"
#	include "Interface/NotificatorInterface.h"

#   include "Interface/MousePickerSystemInterface.h"

#	include "Interface/LoaderInterface.h"
#	include "Interface/NodeInterface.h"
#	include "Interface/ThreadSystemInterface.h"
#	include "Interface/CodecInterface.h"

#   include "Interface/ConverterInterface.h"

#   include "Interface/ResourceInterface.h"
#   include "Interface/AlphaChannelInterface.h"
#   include "Interface/AccountInterface.h"
#   include "Interface/ProfilerInterface.h"

#   include "Interface/GameInterface.h"
#   include "Interface/WatchdogInterface.h"

#   include "Consts.h"

#	include "Core/Resolution.h"
#	include "Core/Viewport.h"
#	include "Core/ConstString.h"

#	include "Logger/Logger.h"

#	include "Math/vec4.h"

#	include <map>

namespace Menge
{
	class ResourceCursor;

	class Application 
		: public ApplicationInterface
		//, public Loadable
	{
	public:
		Application();
		~Application();

    public:
        void setServiceProvider( ServiceProviderInterface * _serviceProvider ) override;
        ServiceProviderInterface * getServiceProvider() const override;

    public:
        bool initialize() override;
        void finalize() override;

    public:
        bool setup( const String& _args, const ApplicationSettings & _setting ) override;
		
	public:
		const FilePath & getBaseDir() const override;

	public:
		PlatformInterface * getPlatform() const override;

	public:
		bool getAllowFullscreenSwitchShortcut() const override;
        	
	public:
		bool createGame( const WString & _module, const ConstString & _language, const TVectorResourcePackDesc & _resourcePack, const TVectorResourcePackDesc & _languagePack ) override;
		bool initializeGame( const TMapParams & _params, const String & _scriptInitParams ) override;

	public:
		void setFullscreenMode( bool _fullscreen ) override;
		bool getFullscreenMode() const override;

    public:
        bool isValidWindowMode() const override;

	public:
		bool getVSync() const override;

	public:
		bool isFocus() const override;

	public:
		const ConstString & getPlatformName() const override;

	protected:
		bool initializeNodeManager_();
		bool initializeLoaderEngine_();
        bool initializeAmplifierService_();
		bool initializeResourceManager_();
		bool initializeSceneManager_();
		bool initializeTextManager_();
        bool initializePrototypeManager_();
        bool initializeWatchdog_();
        bool initializeProfiler_();

	public:
		void setBaseDir( const FilePath & _dir );

		bool createRenderWindow( WindowHandle _renderWindowHandle ) override;

		//void screenshot( const RenderTextureInterfacePtr & _renderTargetImage, const mt::vec4f & _rect );

		void quit();
		
	public:
		void calcWindowResolution( Resolution & _windowResolution ) const override;

		const Resolution & getCurrentResolution() const override;

		const Viewport & getRenderViewport() const override;

		const Resolution & getContentResolution() const override;
		void getGameViewport( float & _aspect, Viewport & _viewport ) const override;

	public:
		bool onRender() override;
		void onFlush() override;
		bool onUpdate() override;
		void onTick( float _timing ) override;
		void onFocus( bool _focus, const mt::vec2f & _point ) override;
		void onClose() override;
        
        void onTurnSound( bool _turn ) override;

		void onAppMouseEnter( const mt::vec2f & _point ) override;
		void onAppMouseLeave() override;		

		bool onKeyEvent( const mt::vec2f & _point, unsigned int _key, unsigned int _char, bool _isDown ) override;
		bool onMouseButtonEvent( unsigned int _touchId, const mt::vec2f & _point, int _button, bool _isDown ) override;		
		bool onMouseMove( unsigned int _touchId, const mt::vec2f & _point, float _x, float _y, int _whell ) override;
		void onMousePosition( unsigned int _touchId, const mt::vec2f & _point ) override;
		
		
		void onPaint() override;

        bool onUserEvent( const ConstString & _event, const TMapParams & _params ) override;

	public:
		void setParticlesEnabled( bool _enabled ) override;
		bool getParticlesEnabled() const override;

	public:
		void setInputMouseButtonEventBlock( bool _block ) override;
		bool getInputMouseButtonEventBlock() const override;

	public:
		void minimizeWindow() override;

		void setMouseBounded( bool _bounded ) override;
		bool getMouseBounded() const override;

		unsigned int getDebugMask() const override;

		const ConstString & getProjectTitle() const override;
		const ConstString & getProjectCodename() const override;
		size_t getProjectVersion() const override;

	public:
		const Resolution & getWindowResolution() const;

    public:
        void setFixedContentResolution( bool _fixedContetResolution ) override; 
		bool getFixedContentResolution() const override;

	public:
		void enableDebug( bool _enable );
        		
		void updateNotification();
		void setVSync( bool _vsync ) override;
		void setCursorMode( bool _mode ) override;
		bool getCursorMode() const override;
		void setCursorIcon( const ConstString & _resourceName ) override;

		//void setAsScreensaver( bool _set );

		void showKeyboard() override;
		void hideKeyboard() override;

    public:
        bool findBestAspectViewport_( float _aspect, float & _bestAspect, Viewport & _viewport ) const; 

	protected:		
		void calcRenderViewport_( const Resolution & _resolution, Viewport & _viewport );
        void invalidateWindow_();

	protected:
		PlatformInterface * m_platform;

		ConsoleInterface * m_console;

		typedef std::map<String, DynamicLibraryInterface *> TDynamicLibraries;
		TDynamicLibraries m_dynamicLibraries;

		GameServiceInterface * m_game;

		Resolution m_currentResolution;

		Viewport m_renderViewport;

		Viewport m_gameViewport;

		bool m_particles;
		bool m_mouseBounded;
		bool m_focus;
		bool m_update;

		bool m_fullscreen;
		Resolution m_renderResolution;

		Resolution m_windowResolution;
		int m_bits;
		bool m_textureFiltering;
		int	m_FSAAType;
		int m_FSAAQuality;

		Resolution m_contentResolution;

        typedef std::map<float, Viewport> TMapAspectRatioViewports;
        TMapAspectRatioViewports m_aspectRatioViewports;

		bool m_fixedContentResolution;


		bool m_createRenderWindow;

		unsigned int m_debugMask;
		
		size_t m_countThreads;

		bool m_resetTiming;
		float m_phycisTiming;
		float m_maxTiming;
		
		ServiceProviderInterface * m_serviceProvider;

		LoaderServiceInterface * m_loaderService;
		ResourceServiceInterface * m_resourceService;
        AmplifierServiceInterface * m_amplifierService;
		TextServiceInterface* m_textService;
		NodeServiceInterface * m_nodeService;		
        PrototypeServiceInterface * m_prototypeService;
        Consts * m_consts;

		WatchdogInterface * m_watchdog;
        ProfilerServiceInterface * m_profiler;

		void parseArguments_( const String& _arguments );

		FilePath m_baseDir;
		
		ResourceCursor * m_cursorResource;

		ConstString m_platformName;
		ConstString m_projectCodename;
		size_t m_projectVersion;
		
		typedef std::vector<PluginInterface *> TVectorPlugins;
		TVectorPlugins m_plugins;

		bool m_allowFullscreenSwitchShortcut;

		bool m_fullScreen;
		bool m_vsync;
		bool m_invalidateVsync;
		bool m_cursorMode;
		bool m_invalidateCursorMode;
		bool m_mouseEnter;

		bool m_inputMouseButtonEventBlock;
		bool m_developmentMode;
        bool m_nofullscreenMode;

        bool m_windowModeCheck;
        bool m_resourceCheck;
	};
}
