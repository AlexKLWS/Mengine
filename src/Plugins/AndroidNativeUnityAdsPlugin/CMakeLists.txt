set( MY_LIB_NAME AndroidNativeUnityAdsPlugin )

ADD_FILTER(
src
	AndroidNativeUnityAdsPlugin.h
	AndroidNativeUnityAdsPlugin.cpp
    AndroidNativeUnityAdsModule.h
    AndroidNativeUnityAdsModule.cpp
	UnityAdsEventHandler.h
)

if( MENGINE_PLUGIN_ANDROID_NATIVE_UNITYADS_DLL )
   add_definitions(-DMENGINE_PLUGIN_DLL)
endif()

INCLUDE_DIRECTORIES( 
		${THIRDPARTY_DIR}/pybind/include
)

ADD_MENGINE_LIBRARY()

set_target_properties (${MY_LIB_NAME} PROPERTIES
    FOLDER Plugins
)

SET( APPLICATION_DEPENDENCIES
 ${APPLICATION_DEPENDENCIES}
 ${MY_LIB_NAME}
 Engine
)

SET( APPLICATION_DEPENDENCIES
 ${APPLICATION_DEPENDENCIES}
 PARENT_SCOPE
)

SET( APPLICATION_LIBS
    ${APPLICATION_LIBS}
	Kernel
	Engine
	AndroidUtils	
    ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}pybind${MENGINE_LIB_SUFFIX}
    )

SET( APPLICATION_LIBS
 ${APPLICATION_LIBS}
 PARENT_SCOPE
)