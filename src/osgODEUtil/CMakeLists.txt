SET(MODULE_NAME osgODEUtil)




SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${MODULE_NAME})




SET(TARGET_H
    ${HEADER_PATH}/AddRemoveObjectOperation
    ${HEADER_PATH}/AddRemoveObjectOperation.inl
    ${HEADER_PATH}/AllRayCastResult
    ${HEADER_PATH}/AllRayCastResult.inl
    ${HEADER_PATH}/ControllerBase
    ${HEADER_PATH}/ControllerBase.inl
    ${HEADER_PATH}/CreateTriMeshFromNode
    ${HEADER_PATH}/CreateTriMeshFromNode.inl
    ${HEADER_PATH}/FindObjects
    ${HEADER_PATH}/FindObjects.inl
    ${HEADER_PATH}/HashTable
    ${HEADER_PATH}/HashTable.inl
    ${HEADER_PATH}/Icosahedron
    ${HEADER_PATH}/Icosahedron.inl
    ${HEADER_PATH}/Math
    ${HEADER_PATH}/Math.inl
    ${HEADER_PATH}/MatrixManipulator
    ${HEADER_PATH}/MatrixManipulator.inl
    ${HEADER_PATH}/NearestNotMeRayCastResult
    ${HEADER_PATH}/NearestNotMeRayCastResult.inl
    ${HEADER_PATH}/NearestRayCastResult
    ${HEADER_PATH}/NearestRayCastResult.inl
    ${HEADER_PATH}/Signal
    ${HEADER_PATH}/TrianglesEmitterIndexVisitor
    ${HEADER_PATH}/TrianglesEmitterIndexVisitor.inl
    ${HEADER_PATH}/TrianglesEmitterPrimitiveIndexFunctor
    ${HEADER_PATH}/TrianglesEmitterPrimitiveIndexFunctor.inl
    ${HEADER_PATH}/Version
)




SET(TARGET_SRC
    AddRemoveObjectOperation.cpp
    AllRayCastResult.cpp
    ControllerBase.cpp
    CreateTriMeshFromNode.cpp
    FindObjects.cpp
    HashTable.cpp
    Icosahedron.cpp
    Math.cpp
    MatrixManipulator.cpp
    NearestNotMeRayCastResult.cpp
    NearestRayCastResult.cpp
    TrianglesEmitterIndexVisitor.cpp
    TrianglesEmitterPrimitiveIndexFunctor.cpp
    Version.cpp
)




SET( MODULE_LINK_LIBRARIES_GNU "osgODE osgGA osgDB osgUtil" )
SET( MODULE_LINK_LIBRARIES_WIN "osgODE osgGA osgDB osgUtil osg OpenThreads opengl32 ODE_WIN" )

OSGODE_SETUP_LIBRARY()
