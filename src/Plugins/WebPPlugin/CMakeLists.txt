set( MY_LIB_NAME WebPPlugin )

ADD_FILTER(
src
	WebPPlugin.h
	WebPPlugin.cpp
    WebPPlugin.def
    
    ImageDecoderWebP.h
    ImageDecoderWebP.cpp
)
    
INCLUDE_DIRECTORIES( ${THIRDPARTY_DIR}/libwebp/src )

INCLUDE_DIRECTORIES( ${THIRDPARTY_DIR}/stdex/include )

ADD_MENGINE_LIBRARY()

set_target_properties (${MY_LIB_NAME} PROPERTIES
    FOLDER Plugins
)

SET( APPLICATION_DEPENDENCIES
 ${APPLICATION_DEPENDENCIES}
 ${MY_LIB_NAME}
)

SET( APPLICATION_DEPENDENCIES
 ${APPLICATION_DEPENDENCIES}
 PARENT_SCOPE
)

SET( APPLICATION_LIBS 
    ${APPLICATION_LIBS} 
    ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}webp${MENGINE_LIB_SUFFIX}
    ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}webpdecoder${MENGINE_LIB_SUFFIX}
    )

SET( APPLICATION_LIB_PATHS
 ${APPLICATION_LIB_PATHS}
 PARENT_SCOPE
)

SET( APPLICATION_LIBS
 ${APPLICATION_LIBS}
 PARENT_SCOPE
)



