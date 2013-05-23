SET(TARGET_SRC Car_init.cpp HUD.cpp Car.cpp Text.cpp osgodecar.cpp Car_handle.cpp  )


IF( WIN32 )
    SET(PSOUND_INCLUDE "$ENV{PROGRAMFILES}\\pSound\\include" CACHE PATH "pSound Headers")
    SET(PSOUND_LIBRARIES "$ENV{PROGRAMFILES}\\pSound\\lib" CACHE PATH "pSound Libraries")

ELSE( WIN32 )
    EXECUTE_PROCESS(COMMAND pkg-config pSound --cflags-only-I OUTPUT_VARIABLE tmp_var)
    STRING(REGEX REPLACE "\n" "" tmp_var "${tmp_var}")
    STRING(REGEX REPLACE "-I" "" tmp_var "${tmp_var}")

    SET(PSOUND_INCLUDE ${tmp_var} CACHE PATH "pSound Headers")

    EXECUTE_PROCESS(COMMAND pkg-config pSound --libs-only-L OUTPUT_VARIABLE tmp_var)
    STRING(REGEX REPLACE "\n" "" tmp_var "${tmp_var}")
    STRING(REGEX REPLACE "-L" "" tmp_var "${tmp_var}")

    SET(PSOUND_LIBRARIES ${tmp_var} CACHE PATH "pSound libraries")
ENDIF( WIN32 )

INCLUDE_DIRECTORIES(${PSOUND_INCLUDE})
LINK_DIRECTORIES(${PSOUND_LIBRARIES})

SET(TARGET_COMMON_LIBRARIES ${TARGET_COMMON_LIBRARIES} pSound)
OSGODE_SETUP_EXAMPLE( osgodecar )
