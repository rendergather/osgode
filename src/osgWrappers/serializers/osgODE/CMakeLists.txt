SET(MODULE_NAME osgdb_serializers_osgode)




SET(TARGET_SRC
    AMotorJoint_serializer.cpp
    AerodynamicDevice_serializer.cpp
    BallJoint_serializer.cpp
    Box_serializer.cpp
    Capsule_serializer.cpp
    Character_serializer.cpp
    CharacterBase_serializer.cpp
    Collidable_serializer.cpp
    CollisionCallback_serializer.cpp
    CollisionParameters_serializer.cpp
    CubicVec3Interpolator_serializer.cpp
    Cylinder_serializer.cpp
    DBallJoint_serializer.cpp
    DHingeJoint_serializer.cpp
    DefaultNearCallback_serializer.cpp
    FixedJoint_serializer.cpp
    GearboxJoint_serializer.cpp
    Hinge2Joint_serializer.cpp
    HingeJoint_serializer.cpp
    InterpolableMatrixTransform_serializer.cpp
    InterpolatorBase_serializer.cpp
    Joint_serializer.cpp
    JointBreakCallback_serializer.cpp
    LinearQuatInterpolator_serializer.cpp
    LinearVec3Interpolator_serializer.cpp
    LMotorJoint_serializer.cpp
    LMPlusJoint_serializer.cpp
    Manager_serializer.cpp
    ManagerUpdateCallback_serializer.cpp
    MotorJoint_serializer.cpp
    MotionPath_serializer.cpp
    NearCallback_serializer.cpp
    ODECallback_serializer.cpp
    ODEObject_serializer.cpp
    ODEObjectContainer_serializer.cpp
    PistonJoint_serializer.cpp
    QuatInterpolator_serializer.cpp
    RigidBody_serializer.cpp
    Sensor_serializer.cpp
    SliderJoint_serializer.cpp
    Space_serializer.cpp
    Sphere_serializer.cpp
    SuspensionJoint_serializer.cpp
    SwaybarJoint_serializer.cpp
    TargetJoint_serializer.cpp
    Transformable_serializer.cpp
    TriMesh_serializer.cpp
    SuspensionJoint_serializer.cpp
    UniversalJoint_serializer.cpp
    Vec3Interpolator_serializer.cpp
    Wheel_serializer.cpp
    World_serializer.cpp
)




SET(MODULE_LINK_LIBRARIES_WIN "osgODE osgDB osg OpenThreads opengl32 ODE_WIN")

OSGODE_SETUP_PLUGIN()
