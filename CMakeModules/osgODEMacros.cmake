MACRO(OSGODE_SETUP_LIBRARY)


    IF( WIN32 )
        SOURCE_GROUP(Headers FILES ${TARGET_H})
    ENDIF( WIN32 )




    INSTALL(    FILES ${TARGET_H}
                DESTINATION include/${MODULE_NAME}
    )




    ADD_LIBRARY(    ${MODULE_NAME}
                    SHARED
                    ${TARGET_SRC}
                    ${TARGET_H}
    )




    SET_TARGET_PROPERTIES(  ${MODULE_NAME}
                            PROPERTIES
                            VERSION ${OSGODE_VERSION}
                            SOVERSION ${OSGODE_SOVERSION}
    )




    SET(MODULE_LINK_LIBRARIES ${MODULE_LINK_LIBRARIES_GNU})


    IF( WIN32 AND MODULE_LINK_LIBRARIES )


        IF( ODE_DOUBLE_PRECISION )

            STRING(REGEX REPLACE "ODE_WIN" "ode" MODULE_LINK_LIBRARIES ${MODULE_LINK_LIBRARIES_WIN})

        ELSE( ODE_DOUBLE_PRECISION )

            STRING(REGEX REPLACE "ODE_WIN" "ode" MODULE_LINK_LIBRARIES ${MODULE_LINK_LIBRARIES_WIN})

        ENDIF( ODE_DOUBLE_PRECISION )


    ENDIF( WIN32 AND MODULE_LINK_LIBRARIES )




    STRING(REGEX REPLACE " " ";" MODULE_LINK_LIBRARIES ${MODULE_LINK_LIBRARIES})



    TARGET_LINK_LIBRARIES(  ${MODULE_NAME}
                            ${MODULE_LINK_LIBRARIES}
    )




    MESSAGE(STATUS "${MODULE_NAME} link libraries: ${MODULE_LINK_LIBRARIES}")







    IF(NOT WIN32)



        IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )

            INSTALL(    TARGETS ${MODULE_NAME}
                        LIBRARY DESTINATION lib64)

        ELSE( CMAKE_SIZEOF_VOID_P EQUAL 8 )

            INSTALL(    TARGETS ${MODULE_NAME}
                        LIBRARY DESTINATION lib)

        ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )




    ELSE(NOT WIN32)



        INSTALL(    TARGETS ${MODULE_NAME}
                    RUNTIME DESTINATION bin
                    LIBRARY DESTINATION lib
                    ARCHIVE DESTINATION lib)






    ENDIF(NOT WIN32)





    IF(ANDROID)
        FOREACH(arg ${TARGET_SRC})
            SET(MODULE_SRC_FILES "${MODULE_SRC_FILES} ${arg}\\\n                   ")
        ENDFOREACH(arg ${TARGET_SRC})

        CONFIGURE_FILE("${OSGODE_ANDROID_TEMPLATES}/Android.mk.library.in" "${CMAKE_CURRENT_BINARY_DIR}/Android.mk")
    ENDIF(ANDROID)




    FOREACH(arg ${TARGET_H})
        FILE(APPEND ${OSGODE_TEMP_DOXYGEN_INPUT} "${arg}\n")
    ENDFOREACH(arg ${TARGET_H})

    FOREACH(arg ${TARGET_SRC})
        FILE(APPEND ${OSGODE_TEMP_DOXYGEN_INPUT} "${PROJECT_SOURCE_DIR}/src/${MODULE_NAME}/${arg}\n")
    ENDFOREACH(arg ${TARGET_SRC})


ENDMACRO(OSGODE_SETUP_LIBRARY)



















MACRO(OSGODE_SETUP_PLUGIN)

    IF(TARGET_SRC)

        ADD_LIBRARY(    ${MODULE_NAME}
                        SHARED
                        ${TARGET_SRC}
        )




        SET(MODULE_LINK_LIBRARIES ${MODULE_LINK_LIBRARIES_GNU})


        IF( WIN32 )


            IF( ODE_DOUBLE_PRECISION )

                STRING(REGEX REPLACE "ODE_WIN" "ode" MODULE_LINK_LIBRARIES ${MODULE_LINK_LIBRARIES_WIN})

            ELSE( ODE_DOUBLE_PRECISION )

                STRING(REGEX REPLACE "ODE_WIN" "ode" MODULE_LINK_LIBRARIES ${MODULE_LINK_LIBRARIES_WIN})

            ENDIF( ODE_DOUBLE_PRECISION )


        ENDIF( WIN32 )




        IF( MODULE_LINK_LIBRARIES )
            STRING(REGEX REPLACE " " ";" MODULE_LINK_LIBRARIES ${MODULE_LINK_LIBRARIES})
        ENDIF( MODULE_LINK_LIBRARIES )



        TARGET_LINK_LIBRARIES(  ${MODULE_NAME}
                                ${MODULE_LINK_LIBRARIES}
        )




        MESSAGE(STATUS "${MODULE_NAME} link libraries: ${MODULE_LINK_LIBRARIES}")





        SET_TARGET_PROPERTIES(  ${MODULE_NAME}
                                PROPERTIES PREFIX "")





        IF( WIN32 )

            EXECUTE_PROCESS(COMMAND ${OSG_BINARIES}\\osgversion.exe --version-number OUTPUT_VARIABLE OSG_VERSION)
            STRING(REGEX REPLACE "\n" "" OSG_VERSION "${OSG_VERSION}")

        ELSE( WIN32 )

            EXECUTE_PROCESS(COMMAND pkg-config openscenegraph-osg --modversion OUTPUT_VARIABLE OSG_VERSION)
            STRING(REGEX REPLACE "\n" "" OSG_VERSION "${OSG_VERSION}")

        ENDIF( WIN32 )





        IF(NOT WIN32)


            IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )

                INSTALL(TARGETS ${MODULE_NAME}
                        LIBRARY DESTINATION lib64/osgPlugins-${OSG_VERSION})

            ELSE( CMAKE_SIZEOF_VOID_P EQUAL 8 )

                INSTALL(TARGETS ${MODULE_NAME}
                        LIBRARY DESTINATION lib/osgPlugins-${OSG_VERSION})

            ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )


        ELSE(NOT WIN32)


            INSTALL(TARGETS ${MODULE_NAME}
                    RUNTIME DESTINATION bin/osgPlugins-${OSG_VERSION}
                    LIBRARY DESTINATION lib/osgPlugins-${OSG_VERSION}
                    ARCHIVE DESTINATION lib/osgPlugins-${OSG_VERSION})


        ENDIF(NOT WIN32)

    ENDIF(TARGET_SRC)



    IF(ANDROID)

        FOREACH(arg ${TARGET_SRC})
            SET(MODULE_SRC_FILES "${MODULE_SRC_FILES} ${arg}\\\n                   ")
        ENDFOREACH(arg ${TARGET_SRC})

        CONFIGURE_FILE("${OSGODE_ANDROID_TEMPLATES}/Android.mk.library.in" "${CMAKE_CURRENT_BINARY_DIR}/Android.mk")

    ENDIF()


ENDMACRO(OSGODE_SETUP_PLUGIN)
