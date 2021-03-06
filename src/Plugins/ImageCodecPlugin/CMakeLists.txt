set( MY_LIB_NAME ImageCodecPlugin )

ADD_FILTER(
src
	ImageCodecPlugin.h
	ImageCodecPlugin.cpp
)

ADD_FILTER(
jpeg  
	ImageDecoderJPEG.cpp
	ImageDecoderJPEG.h
	ImageEncoderJPEG.cpp
	ImageEncoderJPEG.h    
)

ADD_FILTER(
png
	ImageDecoderPNG.cpp
	ImageDecoderPNG.h
	ImageEncoderPNG.cpp
	ImageEncoderPNG.h
)
    
ADD_FILTER(
etc1    
    ImageDecoderETC1.cpp
    ImageDecoderETC1.h
)

ADD_FILTER(
pvrtc
    ImageDecoderPVRTC.cpp
    ImageDecoderPVRTC.h
)

ADD_FILTER(
hit    
    PickVerifyHIT.h
    
    PickDecoderHIT.cpp
    PickDecoderHIT.h
    
    PickEncoderHIT.cpp
    PickEncoderHIT.h
)

ADD_FILTER(
htf    
    ImageDecoderHTF.cpp
    ImageDecoderHTF.h       
    ImageEncoderHTF.cpp
    ImageEncoderHTF.h
    ImageVerifyHTF.h
)

ADD_FILTER(
acf
    ImageDecoderACF.cpp
    ImageDecoderACF.h       
    ImageEncoderACF.cpp
    ImageEncoderACF.h
    ImageVerifyACF.h
)

ADD_FILTER(
dds    
    ImageCodecDDS.h
    ImageDecoderDDS.cpp
    ImageDecoderDDS.h    
    ImageEncoderDDS.cpp
    ImageEncoderDDS.h
)

INCLUDE_DIRECTORIES( ${THIRDPARTY_DIR}/stdex/include )
INCLUDE_DIRECTORIES( ${THIRDPARTY_DIR}/zlib )
INCLUDE_DIRECTORIES( ${THIRDPARTY_DIR}/libpng )
INCLUDE_DIRECTORIES( ${THIRDPARTY_DIR}/libjpeg )
INCLUDE_DIRECTORIES( ${THIRDPARTY_DIR}/libmath/include )

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

if(MENGINE_TOOLCHAIN_MSVC)
    SET( APPLICATION_LIBS 
		${APPLICATION_LIBS} 
		${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}libpng16_static${MENGINE_DEPENDENCIES_DEBUG_SUFFIX}${MENGINE_LIB_SUFFIX} 
		)    
else()
    SET( APPLICATION_LIBS 
		${APPLICATION_LIBS} 
		${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}png16${MENGINE_DEPENDENCIES_DEBUG_SUFFIX}${MENGINE_LIB_SUFFIX} 
		)    
endif()
    
SET( APPLICATION_LIBS
    ${APPLICATION_LIBS}
    ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}ogg${MENGINE_LIB_SUFFIX}
    ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}zlib${MENGINE_LIB_SUFFIX}
    ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}jpeg${MENGINE_LIB_SUFFIX}
    ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}stdex${MENGINE_LIB_SUFFIX}    
    Kernel
    )

SET( APPLICATION_LIB_PATHS
 ${APPLICATION_LIB_PATHS}
 PARENT_SCOPE
)

SET( APPLICATION_LIBS
 ${APPLICATION_LIBS}
 PARENT_SCOPE
)



