set( MY_LIB_NAME PosixSocketPlugin )

ADD_FILTER(
src
	PosixSocket.cpp
	PosixSocket.h
    PosixSocketProviderInterface.h
	PosixSocketInputStream.cpp
	PosixSocketInputStream.h
	PosixSocketOutputStream.cpp
	PosixSocketOutputStream.h
	PosixSocketSystem.cpp
	PosixSocketSystem.h

    PosixSocketPlugin.cpp
    PosixSocketPlugin.h
	PosixSocketPlugin.def
)

INCLUDE_DIRECTORIES( ${THIRDPARTY_DIR}/stdex/include )

ADD_MENGINE_PLUGIN(MENGINE_PLUGIN_POSIX_SOCKET)

set_target_properties (${MY_LIB_NAME} PROPERTIES
    FOLDER Plugins
)

TARGET_LINK_LIBRARIES(${MY_LIB_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}stdex${MENGINE_LIB_SUFFIX})

TARGET_LINK_LIBRARIES(${MY_LIB_NAME} Kernel)

SET( APPLICATION_DEPENDENCIES
 ${APPLICATION_DEPENDENCIES}
 ${MY_LIB_NAME}
 Kernel
)

SET( APPLICATION_DEPENDENCIES
 ${APPLICATION_DEPENDENCIES}
 PARENT_SCOPE
)
