set(MY_LIB_NAME Bootstrapper)

ADD_FILTER(
Bootstrapper
    Bootstrapper.cpp
    Bootstrapper.h
)

INCLUDE_DIRECTORIES(
	${THIRDPARTY_DIR}/stdex/include
    ${THIRDPARTY_DIR}/libmath/include
    )
    
ADD_MENGINE_LIBRARY()

TARGET_LINK_LIBRARIES(${MY_LIB_NAME} Kernel)

TARGET_LINK_LIBRARIES(${MY_LIB_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}stdex${MENGINE_LIB_SUFFIX})
TARGET_LINK_LIBRARIES(${MY_LIB_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}math${MENGINE_LIB_SUFFIX})

set_target_properties(${MY_LIB_NAME} PROPERTIES
    FOLDER Bootstrapper
)

SET( APPLICATION_LIBS
    ${APPLICATION_LIBS}
)

SET( APPLICATION_LIBS
 ${APPLICATION_LIBS}
 PARENT_SCOPE
)

SET( APPLICATION_DEPENDENCIES
 ${APPLICATION_DEPENDENCIES}
 ${MY_LIB_NAME}
)


SET( APPLICATION_DEPENDENCIES
 ${APPLICATION_DEPENDENCIES}
 PARENT_SCOPE
)