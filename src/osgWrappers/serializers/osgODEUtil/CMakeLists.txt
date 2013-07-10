SET(MODULE_NAME osgdb_serializers_osgodeutil)




SET(TARGET_SRC
    AnimationHelper_serializer.cpp
    HashTable_serializer.cpp
    HashTablePair_serializer.cpp
    Picker_serializer.cpp
)




IF( WIN32 )
    ADD_DEFINITIONS( "/wd4250" )
ENDIF( WIN32 )


SET(MODULE_LINK_LIBRARIES_WIN "osgDB osg OpenThreads opengl32 osgODEUtil")

OSGODE_SETUP_PLUGIN()
