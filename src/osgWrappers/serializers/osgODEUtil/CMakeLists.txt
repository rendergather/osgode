SET(MODULE_NAME osgdb_serializers_osgodeutil)




SET(TARGET_SRC
    HashTable_serializer.cpp
    HashTablePair_serializer.cpp
)




SET(MODULE_LINK_LIBRARIES_WIN "osgDB osg OpenThreads opengl32 osgODEUtil")

OSGODE_SETUP_PLUGIN()
