SET(MODULE_NAME osgODE)




SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${MODULE_NAME})




SET(TARGET_H
    ${HEADER_PATH}/AMotorJoint
    ${HEADER_PATH}/AMotorJoint.inl
    ${HEADER_PATH}/AerodynamicDevice
    ${HEADER_PATH}/AerodynamicDevice.inl
    ${HEADER_PATH}/BallJoint
    ${HEADER_PATH}/Box
    ${HEADER_PATH}/Capsule
    ${HEADER_PATH}/Collidable
    ${HEADER_PATH}/Collidable.inl
    ${HEADER_PATH}/CollisionCallback
    ${HEADER_PATH}/CollisionCallback.inl
    ${HEADER_PATH}/CollisionParameters
    ${HEADER_PATH}/CollisionParameters.inl
    ${HEADER_PATH}/Config
    ${HEADER_PATH}/CubicVec3Interpolator
    ${HEADER_PATH}/Cylinder
    ${HEADER_PATH}/DBallJoint
    ${HEADER_PATH}/DHingeJoint
    ${HEADER_PATH}/DefaultNearCallback
    ${HEADER_PATH}/DefaultNearCallback.inl
    ${HEADER_PATH}/FixedJoint
    ${HEADER_PATH}/GearboxJoint
    ${HEADER_PATH}/GearboxJoint.inl
    ${HEADER_PATH}/Hinge2Joint
    ${HEADER_PATH}/Hinge2Joint.inl
    ${HEADER_PATH}/HingeJoint
    ${HEADER_PATH}/HingeJoint.inl
    ${HEADER_PATH}/Interpolator
    ${HEADER_PATH}/InterpolableMatrixTransform
    ${HEADER_PATH}/Joint
    ${HEADER_PATH}/Joint.inl
    ${HEADER_PATH}/JointBreakCallback
    ${HEADER_PATH}/JointBreakCallback.inl
    ${HEADER_PATH}/JointFeedback
    ${HEADER_PATH}/JointFeedback.inl
    ${HEADER_PATH}/LinearQuatInterpolator
    ${HEADER_PATH}/LinearVec3Interpolator
    ${HEADER_PATH}/LMotorJoint
    ${HEADER_PATH}/LMotorJoint.inl
    ${HEADER_PATH}/LMPlusJoint
    ${HEADER_PATH}/Manager
    ${HEADER_PATH}/Manager.inl
    ${HEADER_PATH}/ManagerUpdateCallback
    ${HEADER_PATH}/ManagerUpdateCallback.inl
    ${HEADER_PATH}/MotionPath
    ${HEADER_PATH}/MotionPath.inl
    ${HEADER_PATH}/MotorJoint
    ${HEADER_PATH}/MotorJoint.inl
    ${HEADER_PATH}/NearCallback
    ${HEADER_PATH}/Notify
    ${HEADER_PATH}/Notify_android
    ${HEADER_PATH}/Notify_gnu
    ${HEADER_PATH}/Notify_windows
    ${HEADER_PATH}/ODE
    ${HEADER_PATH}/ODECallback
    ${HEADER_PATH}/ODECallback.inl
    ${HEADER_PATH}/ODEObject
    ${HEADER_PATH}/ODEObject.inl
    ${HEADER_PATH}/ODEObjectContainer
    ${HEADER_PATH}/ODEObjectContainer.inl
    ${HEADER_PATH}/OverlappingPair
    ${HEADER_PATH}/OverlappingPair.inl
    ${HEADER_PATH}/PistonJoint
    ${HEADER_PATH}/PistonJoint.inl
    ${HEADER_PATH}/RigidBody
    ${HEADER_PATH}/RigidBody.inl
    ${HEADER_PATH}/ScopedTimer
    ${HEADER_PATH}/ScopedTimer.inl
    ${HEADER_PATH}/Sensor
    ${HEADER_PATH}/Sensor.inl
    ${HEADER_PATH}/SliderJoint
    ${HEADER_PATH}/SliderJoint.inl
    ${HEADER_PATH}/Space
    ${HEADER_PATH}/Space.inl
    ${HEADER_PATH}/Sphere
    ${HEADER_PATH}/StaticWorld
    ${HEADER_PATH}/StaticWorld.inl
    ${HEADER_PATH}/SuspensionJoint
    ${HEADER_PATH}/SuspensionJoint.inl
    ${HEADER_PATH}/SwaybarJoint
    ${HEADER_PATH}/SwaybarJoint.inl
    ${HEADER_PATH}/TargetJoint
    ${HEADER_PATH}/TargetJoint.inl
    ${HEADER_PATH}/Transformable
    ${HEADER_PATH}/Transformable.inl
    ${HEADER_PATH}/TriMesh
    ${HEADER_PATH}/TriMesh.inl
    ${HEADER_PATH}/UniversalJoint
    ${HEADER_PATH}/UniversalJoint.inl
    ${HEADER_PATH}/Version
    ${HEADER_PATH}/Wheel
    ${HEADER_PATH}/Wheel.inl
    ${HEADER_PATH}/World
    ${HEADER_PATH}/World.inl
)




SET(TARGET_SRC
    AMotorJoint.cpp
    AerodynamicDevice.cpp
    BallJoint.cpp
    Box.cpp
    Capsule.cpp
    Collidable.cpp
    CollisionCallback.cpp
    CollisionParameters.cpp
    CubicVec3Interpolator.cpp
    Cylinder.cpp
    DBallJoint.cpp
    DHingeJoint.cpp
    DefaultNearCallback.cpp
    FixedJoint.cpp
    GearboxJoint.cpp
    Hinge2Joint.cpp
    HingeJoint.cpp
    InterpolableMatrixTransform.cpp
    Joint.cpp
    Joint_implementations.cpp
    JointBreakCallback.cpp
    JointFeedback.cpp
    LinearQuatInterpolator.cpp
    LinearVec3Interpolator.cpp
    LMotorJoint.cpp
    LMPlusJoint.cpp
    Manager.cpp
    ManagerUpdateCallback.cpp
    MotionPath.cpp
    MotorJoint.cpp
    NearCallback.cpp
    Notify.c
    Notify_android.cpp
    Notify_windows.c
    ODECallback.cpp
    ODEObject.cpp
    ODEObjectContainer.cpp
    OverlappingPair.cpp
    PistonJoint.cpp
    RigidBody.cpp
    ScopedTimer.cpp
    Sensor.cpp
    SliderJoint.cpp
    Space.cpp
    Space_rayCast.cpp
    Sphere.cpp
    StaticWorld.cpp
    SuspensionJoint.cpp
    SwaybarJoint.cpp
    TargetJoint.cpp
    Transformable.cpp
    TriMesh.cpp
    UniversalJoint.cpp
    Version.cpp
    Wheel.cpp
    World.cpp
    World_interaction.cpp
)




SET( MODULE_LINK_LIBRARIES_GNU "osg OpenThreads ode" )
SET( MODULE_LINK_LIBRARIES_WIN "opengl32 osg OpenThreads ODE_WIN" )

OSGODE_SETUP_LIBRARY()