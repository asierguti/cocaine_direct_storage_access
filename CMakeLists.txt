#IF(DIRECT_ACCESS)
    INCLUDE_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}/include
	/usr/include/
)#cocaine_direct_storage_access/

    ADD_LIBRARY(direct_access MODULE
        src/direct_access
        src/module)

    TARGET_LINK_LIBRARIES(direct_access
        cocaine-core
        ${Boost_LIBRARIES})

    SET_TARGET_PROPERTIES(direct_access PROPERTIES
        PREFIX ""
        SUFFIX ".cocaine-plugin"
        COMPILE_FLAGS "-std=gnu++0x")

    INSTALL(TARGETS direct_access
        LIBRARY DESTINATION lib/cocaine
        COMPONENT runtime)

    INSTALL(
        DIRECTORY
            include/cocaine/framework
            include/cocaine/idl
        DESTINATION include/cocaine
        COMPONENT development)
#ENDIF(DIRECT_ACCESS)