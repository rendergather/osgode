SET(TARGET_SRC osgodecollisionsound.cpp  )


IF( WIN32 )
    SET(PSOUND_INCLUDE "$ENV{PROGRAMFILES}\\pSound\\include" CACHE PATH "pSound Headers")
    SET(OPENAL_INCLUDE "$ENV{PROGRAMFILES}\\OpenAL 1.1 SDK\\include" CACHE PATH "OpenAL headers")
    SET(PSOUND_LIBRARIES "$ENV{PROGRAMFILES}\\pSound\\lib" CACHE PATH "pSound Libraries")
    SET(OPENAL_LIBRARIES "$ENV{PROGRAMFILES}\\OpenAL 1.1 SDK\\libs\\Win32" CACHE PATH "OpenAL libraries")

ELSE( WIN32 )
    EXECUTE_PROCESS(COMMAND pkg-config pSound --cflags-only-I OUTPUT_VARIABLE tmp_var)
    STRING(REGEX REPLACE "\n" "" tmp_var "${tmp_var}")
    STRING(REGEX REPLACE "-I" "" tmp_var "${tmp_var}")

    SET(PSOUND_INCLUDE ${tmp_var} CACHE PATH "pSound Headers")

    EXECUTE_PROCESS(COMMAND pkg-config pSound --libs-only-L OUTPUT_VARIABLE tmp_var)
    STRING(REGEX REPLACE "\n" "" tmp_var "${tmp_var}")
    STRING(REGEX REPLACE "-L" "" tmp_var "${tmp_var}")

    SET(PSOUND_LIBRARIES ${tmp_var} CACHE PATH "pSound libraries")



    EXECUTE_PROCESS(COMMAND pkg-config openal --cflags-only-I OUTPUT_VARIABLE tmp_var)
    STRING(REGEX REPLACE "\n" "" tmp_var "${tmp_var}")
    STRING(REGEX REPLACE "-I" "" tmp_var "${tmp_var}")

    SET(OPENAL_INCLUDE ${tmp_var} CACHE PATH "OpenAL Headers")



    EXECUTE_PROCESS(COMMAND pkg-config openal --libs-only-L OUTPUT_VARIABLE tmp_var)
    STRING(REGEX REPLACE "\n" "" tmp_var "${tmp_var}")
    STRING(REGEX REPLACE "-L" "" tmp_var "${tmp_var}")

    SET(OPENAL_LIBRARIES ${tmp_var} CACHE PATH "OpenAL libraries")
ENDIF( WIN32 )

INCLUDE_DIRECTORIES(${PSOUND_INCLUDE})
INCLUDE_DIRECTORIES(${OPENAL_INCLUDE})
LINK_DIRECTORIES(${PSOUND_LIBRARIES})
LINK_DIRECTORIES(${OPENAL_LIBRARIES})

SET(TARGET_COMMON_LIBRARIES ${TARGET_COMMON_LIBRARIES} pSound)

IF( WIN32 )
    SET(TARGET_COMMON_LIBRARIES ${TARGET_COMMON_LIBRARIES} OpenAL32)
ENDIF( WIN32 )

OSGODE_SETUP_EXAMPLE( osgodecollisionsound )
