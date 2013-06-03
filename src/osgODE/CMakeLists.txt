SET(MODULE_NAME osgODE)




SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${MODULE_NAME})




SET(TARGET_H
    ${HEADER_PATH}/AMotorJoint
    ${HEADER_PATH}/AMotorJoint.inl
    ${HEADER_PATH}/AerodynamicDevice
    ${HEADER_PATH}/AerodynamicDevice.inl
    ${HEADER_PATH}/BallJoint
    ${HEADER_PATH}/Box
    ${HEADER_PATH}/BypassJoint
    ${HEADER_PATH}/BypassJoint.inl
    ${HEADER_PATH}/Capsule
    ${HEADER_PATH}/Character
    ${HEADER_PATH}/CharacterBase
    ${HEADER_PATH}/CharacterBase.inl
    ${HEADER_PATH}/Collidable
    ${HEADER_PATH}/Collidable.inl
    ${HEADER_PATH}/CollisionCallback
    ${HEADER_PATH}/CollisionCallback.inl
    ${HEADER_PATH}/CollisionParameters
    ${HEADER_PATH}/CollisionParameters.inl
    ${HEADER_PATH}/CollisionPredictor
    ${HEADER_PATH}/CommonRayCastResults
    ${HEADER_PATH}/CommonRayCastResults.inl
    ${HEADER_PATH}/CommonWorldOperations
    ${HEADER_PATH}/Config
    ${HEADER_PATH}/Config.in
    ${HEADER_PATH}/Container
    ${HEADER_PATH}/Container.inl
    ${HEADER_PATH}/CubicVec3Interpolator
    ${HEADER_PATH}/Cylinder
    ${HEADER_PATH}/DBallJoint
    ${HEADER_PATH}/DHingeJoint
    ${HEADER_PATH}/DefaultNearCallback
    ${HEADER_PATH}/DefaultNearCallback.inl
    ${HEADER_PATH}/DifferentialJoint
    ${HEADER_PATH}/DifferentialJoint.inl
    ${HEADER_PATH}/DynamicParticleGeode
    ${HEADER_PATH}/DynamicParticleGeode.inl
    ${HEADER_PATH}/DynamicParticleSystem
    ${HEADER_PATH}/DynamicParticleSystem.inl
    ${HEADER_PATH}/Engine
    ${HEADER_PATH}/Engine.inl
    ${HEADER_PATH}/EngineBase
    ${HEADER_PATH}/EngineBase.inl
    ${HEADER_PATH}/FixedJoint
    ${HEADER_PATH}/GearboxJoint
    ${HEADER_PATH}/GearboxJoint.inl
    ${HEADER_PATH}/Hinge2Joint
    ${HEADER_PATH}/Hinge2Joint.inl
    ${HEADER_PATH}/HingeJoint
    ${HEADER_PATH}/HingeJoint.inl
    ${HEADER_PATH}/InterpolableMatrixTransform
    ${HEADER_PATH}/Interpolator
    ${HEADER_PATH}/Joint
    ${HEADER_PATH}/Joint.inl
    ${HEADER_PATH}/JointBreakCallback
    ${HEADER_PATH}/JointBreakCallback.inl
    ${HEADER_PATH}/JointFeedback
    ${HEADER_PATH}/JointFeedback.inl
    ${HEADER_PATH}/LMPlusJoint
    ${HEADER_PATH}/LMotorJoint
    ${HEADER_PATH}/LMotorJoint.inl
    ${HEADER_PATH}/LinearInterpolator
    ${HEADER_PATH}/LinearQuatInterpolator
    ${HEADER_PATH}/LinearVec3Interpolator
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
    ${HEADER_PATH}/OneWayFixedJoint
    ${HEADER_PATH}/OneWayFixedJoint.inl
    ${HEADER_PATH}/OverlappingPair
    ${HEADER_PATH}/OverlappingPair.inl
    ${HEADER_PATH}/PistonJoint
    ${HEADER_PATH}/PistonJoint.inl
    ${HEADER_PATH}/RagDoll
    ${HEADER_PATH}/RagDoll.inl
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
    BypassJoint.cpp
    Capsule.cpp
    Character.cpp
    CharacterBase.cpp
    Collidable.cpp
    CollisionCallback.cpp
    CollisionParameters.cpp
    CollisionPredictor.cpp
    CommonRayCastResults.cpp
    CommonWorldOperations.cpp
    Container.cpp
    CubicVec3Interpolator.cpp
    Cylinder.cpp
    DBallJoint.cpp
    DHingeJoint.cpp
    DefaultNearCallback.cpp
    DifferentialJoint.cpp
    DynamicParticleGeode.cpp
    DynamicParticleSystem.cpp
    Engine.cpp
    EngineBase.cpp
    FixedJoint.cpp
    GearboxJoint.cpp
    Hinge2Joint.cpp
    HingeJoint.cpp
    InterpolableMatrixTransform.cpp
    Joint.cpp
    JointBreakCallback.cpp
    JointFeedback.cpp
    Joint_implementations.cpp
    LMPlusJoint.cpp
    LMotorJoint.cpp
    LinearInterpolator.cpp
    LinearQuatInterpolator.cpp
    LinearVec3Interpolator.cpp
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
    OneWayFixedJoint.cpp
    OverlappingPair.cpp
    PistonJoint.cpp
    RagDoll.cpp
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
    Transformable.cpp
    TriMesh.cpp
    UniversalJoint.cpp
    Version.cpp
    Wheel.cpp
    World.cpp
    World_interaction.cpp
)




SET( MODULE_LINK_LIBRARIES_GNU "osgParticle ode" )
SET( MODULE_LINK_LIBRARIES_WIN "opengl32 osgParticle osg OpenThreads ODE_WIN" )

OSGODE_SETUP_LIBRARY()
