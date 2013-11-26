SET(TARGET_DEFAULT_PREFIX "example_")
SET(TARGET_DEFAULT_LABEL_PREFIX "Examples")



SET(TARGET_COMMON_LIBRARIES
    OpenThreads
    osg
    osgDB
    osgUtil
    osgGA
    osgViewer
    osgODE
    osgODEUtil
)


IF( WIN32 )
    ADD_DEFINITIONS( /wd4250 )
ENDIF( WIN32 )

ADD_DEFINITIONS( -DOSGODE_DATA_PATH="${CMAKE_SOURCE_DIR}/data" )




ADD_SUBDIRECTORY( osgodeaerodynamicdevice )
ADD_SUBDIRECTORY( osgodeamotor )
ADD_SUBDIRECTORY( osgodeamotorstop )
# ADD_SUBDIRECTORY( osgodeandroidgles2 )
ADD_SUBDIRECTORY( osgodebox )
ADD_SUBDIRECTORY( osgodecapsule )

IF( BUILD_OSGODE_PSOUND_EXAMPLES )
    ADD_SUBDIRECTORY( osgodecar )
ENDIF( BUILD_OSGODE_PSOUND_EXAMPLES )

ADD_SUBDIRECTORY( osgodecharacter )
ADD_SUBDIRECTORY( osgodecharacterbase )
ADD_SUBDIRECTORY( osgodecollisionprediction )

IF( BUILD_OSGODE_PSOUND_EXAMPLES )
    ADD_SUBDIRECTORY( osgodecollisionsound )
ENDIF( BUILD_OSGODE_PSOUND_EXAMPLES )

ADD_SUBDIRECTORY( osgodedhingejoint )
ADD_SUBDIRECTORY( osgodedifferentialjoint )
ADD_SUBDIRECTORY( osgodedynamicparticle )
ADD_SUBDIRECTORY( osgodeengine )
ADD_SUBDIRECTORY( osgodegearboxjoint )
ADD_SUBDIRECTORY( osgodehingejoint )
ADD_SUBDIRECTORY( osgodelmotor )
ADD_SUBDIRECTORY( osgodelmplusjoint )
ADD_SUBDIRECTORY( osgodependulum )
ADD_SUBDIRECTORY( osgodeplane2djoint )
ADD_SUBDIRECTORY( osgoderigidbody )
ADD_SUBDIRECTORY( osgoderope )
ADD_SUBDIRECTORY( osgodesliderjoint )
ADD_SUBDIRECTORY( osgodespacestress )
ADD_SUBDIRECTORY( osgodetrimesh )
ADD_SUBDIRECTORY( osgodeuniversaljoint )
ADD_SUBDIRECTORY( osgodeutilcontrollerbase )
ADD_SUBDIRECTORY( osgodeutilpicker )
