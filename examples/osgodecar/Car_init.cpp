#include "Car"

#include <osgDB/ReadFile>

#include <osgODE/Cylinder>
#include <osgODE/SuspensionJoint>
#include <osgODE/Notify>
#include <osgODE/GearboxJoint>
#include <osgODE/SwaybarJoint>

#include <pSound/Source>




using namespace osgODE ;




namespace {
class OSG_EXPORT WheelCallback: public osgODE::ODECallback
{
public:
META_Object(osgODE, WheelCallback) ;


    WheelCallback(pSound::Buffer* buffer = NULL)
    {
        PS_ASSERT1( buffer != NULL ) ;

        m_source = new pSound::Source() ;
        m_source->setBuffer(buffer) ;

        m_source->setParam(pSound::Source::GAIN, 0.0) ;
        m_source->setParam(pSound::Source::LOOPING, true) ;

        m_source->play() ;
    }




    WheelCallback(const WheelCallback& other, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY):
        ODECallback(other, copyop)
    {
        if( other.m_source.valid() ) {
            m_source = new pSound::Source(*other.m_source, copyop) ;

            m_source->play() ;
        }
    }



    // Compute the axis in world frame and adjust the sound
    virtual void    operator()(ODEObject* obj)
    {
        PS_ASSERT1( m_source.valid() ) ;

        RigidBody*  body = obj->asRigidBody() ;
        PS_ASSERT1( body != NULL ) ;

        osg::Vec3   axis = body->getQuaternion() * osg::Z_AXIS ;

        body->setFiniteRotationAxis(axis) ;



        const double    speed = fabs( axis * body->getAngularVelocity() ) ;

        const double    f = osg::clampTo(speed / 75.0, 0.2, 1.0) ;

        // We have four wheels, so the contribute of each one is 1/4
        m_source->setParam(pSound::Source::GAIN, f * 0.25) ;
        m_source->setParam(pSound::Source::PITCH, f * 2.0) ;

        traverse(obj) ;
    }

private:
    osg::ref_ptr<pSound::Source>    m_source ;
} ;




class OSG_EXPORT BodyCallback: public osgODE::ODECallback
{
public:
    BodyCallback(pSound::Buffer* buffer = NULL)
    {
        PS_ASSERT1( buffer != NULL ) ;

        m_source = new pSound::Source() ;
        m_source->setBuffer(buffer) ;

        m_source->setParam(pSound::Source::GAIN, 0.0) ;
        m_source->setParam(pSound::Source::LOOPING, true) ;

        m_source->play() ;
    }




    // Adjust the wind sound
    virtual void    operator()(ODEObject* obj)
    {
        PS_ASSERT1( m_source.valid() ) ;

        RigidBody*  body = obj->asRigidBody() ;
        PS_ASSERT1( body != NULL ) ;

        const double    speed = body->getLinearVelocity().length() ;

        const double    f = osg::minimum(1.0, speed / 50.0) ;

        m_source->setParam(pSound::Source::GAIN, f * 2.0) ;
        m_source->setParam(pSound::Source::PITCH, f) ;

        traverse(obj) ;
    }

private:
    osg::ref_ptr<pSound::Source>    m_source ;
} ;
}




void
Car::init(void)
{
    PS_ASSERT1( this->getObjectList().size() == 0 ) ;





    {
        // the wheel
        osg::ref_ptr<Cylinder>      wheel = dynamic_cast<Cylinder*>( osgDB::readObjectFile("car_wheel.osgb") ) ;
        PS_ASSERT1( wheel.valid() ) ;


        // engine sound
        pSound::Buffer*     buffer = dynamic_cast<pSound::Buffer*>( osgDB::readObjectFile("engine.wav") ) ;
        PS_ASSERT1( buffer ) ;



        // clear the local parameters, if there are any
        wheel->setCollisionParameters(NULL) ;

        // Just to be sure
        wheel->setFiniteRotationMode(true) ;
        wheel->setAutoDisableFlag(false) ;

        // This callback handles the sound and the rotation axis
        wheel->addUpdateCallback( new WheelCallback(buffer) ) ;



        // Now create and position the wheels


        // I use DEEP_COPY_CALLBACKS because each wheel must have a
        // separate sound source

        m_wheel_RL = osg::clone(wheel.get(), osg::CopyOp::DEEP_COPY_CALLBACKS) ;
        m_wheel_RR = osg::clone(wheel.get(), osg::CopyOp::DEEP_COPY_CALLBACKS) ;
        m_wheel_FR = osg::clone(wheel.get(), osg::CopyOp::DEEP_COPY_CALLBACKS) ;
        m_wheel_FL = osg::clone(wheel.get(), osg::CopyOp::DEEP_COPY_CALLBACKS) ;


        m_wheel_RL->setPosition( osg::Vec3(-1, -1.5, 0.0) ) ;
        m_wheel_RR->setPosition( osg::Vec3( 1, -1.5, 0.0) ) ;
        m_wheel_FR->setPosition( osg::Vec3( 1,  1.5, 0.0) ) ;
        m_wheel_FL->setPosition( osg::Vec3(-1,  1.5, 0.0) ) ;


        this->ODEObjectContainer::addObject( m_wheel_RL ) ;
        this->ODEObjectContainer::addObject( m_wheel_RR ) ;
        this->ODEObjectContainer::addObject( m_wheel_FR ) ;
        this->ODEObjectContainer::addObject( m_wheel_FL ) ;
    }








    {
        m_body = dynamic_cast<RigidBody*>( osgDB::readObjectFile("car_body.osgb") ) ;
        PS_ASSERT1( m_body.valid() ) ;


        pSound::Buffer*     buffer = dynamic_cast<pSound::Buffer*>( osgDB::readObjectFile("wind.wav") ) ;
        PS_ASSERT1( buffer ) ;


        // The mass
        m_body->setMass( m_body->getMass(), osg::Vec3(1.5, 3, 1), dBoxClass ) ;

        // This callback handles the sound source
        m_body->addUpdateCallback( new BodyCallback(buffer) ) ;


        this->ODEObjectContainer::addObject( m_body ) ;
    }








    {
        osg::ref_ptr<osg::Node>     joint_graphics = osgDB::readNodeFile("car_suspension.osgb") ;
        PS_ASSERT1( joint_graphics.valid() ) ;


        m_hinge_RL = _createSuspension( m_wheel_RL, - osg::X_AXIS, joint_graphics ) ;
        m_hinge_FL = _createSuspension( m_wheel_FL, - osg::X_AXIS, joint_graphics ) ;
        m_hinge_RR = _createSuspension( m_wheel_RR,   osg::X_AXIS, joint_graphics ) ;
        m_hinge_FR = _createSuspension( m_wheel_FR,   osg::X_AXIS, joint_graphics ) ;


        this->ODEObjectContainer::addObject( m_hinge_RL ) ;
        this->ODEObjectContainer::addObject( m_hinge_RR ) ;
        this->ODEObjectContainer::addObject( m_hinge_FR ) ;
        this->ODEObjectContainer::addObject( m_hinge_FL ) ;
    }



    // differentials
    {
        GearboxJoint*   front = new GearboxJoint() ;
        GearboxJoint*   rear = new GearboxJoint() ;

        front->setBody1(m_wheel_FL) ;
        front->setBody2(m_wheel_FR) ;

        rear->setBody1(m_wheel_RL) ;
        rear->setBody2(m_wheel_RR) ;

        front->setAxis1( - osg::X_AXIS ) ;
        front->setAxis2(   osg::X_AXIS ) ;

        rear->setAxis1( - osg::X_AXIS ) ;
        rear->setAxis2(   osg::X_AXIS ) ;


        front->setFriction( 0.9 ) ; // quietly locked
        rear->setFriction( 0.9 ) ;  // quietly locked


        this->ODEObjectContainer::addObject( front ) ;
        this->ODEObjectContainer::addObject( rear ) ;
    }



    // sway bars
    {
        SwaybarJoint*   front = new SwaybarJoint() ;
        SwaybarJoint*   rear = new SwaybarJoint() ;

        front->setReferenceBody( m_body ) ;
        rear->setReferenceBody( m_body ) ;

        front->setReferenceAxis( osg::Z_AXIS ) ;
        rear->setReferenceAxis( osg::Z_AXIS ) ;

        front->setBody1(m_wheel_FL) ;
        front->setBody2(m_wheel_FR) ;

        rear->setBody1(m_wheel_RL) ;
        rear->setBody2(m_wheel_RR) ;

        front->setAxis1( osg::Z_AXIS ) ;
        rear->setAxis1( osg::Z_AXIS ) ;


        front->setRigidity(0.25) ;
        rear->setRigidity(0.25) ;


        this->ODEObjectContainer::addObject( front ) ;
        this->ODEObjectContainer::addObject( rear ) ;
    }
}




SuspensionJoint*
Car::_createSuspension(RigidBody* wheel, const osg::Vec3& axis2, osg::Node* graphics)
{
    const double    SPRING = 2000.0 ;
    const double    DAMPER_BOUND = 250.0 ;
    const double    DAMPER_REBOUND = 250.0 ;
    const double    PRELOAD = 500.0 ;



    SuspensionJoint*    joint = new SuspensionJoint() ;

    // The anchor of the wheel is 20 centimeters towards the interior
    // of the car
    const double        offset = 0.2 ;


    // The anchor
    joint->setAnchor1( wheel->getPosition() - axis2 * offset ) ;

    // The vertical axis
    joint->setAxis1( osg::Z_AXIS ) ;

    // Rotation axis
    joint->setAxis2( axis2 ) ;

    // It's appropriate that the wheel is the second body
    joint->setBody1( m_body ) ;
    joint->setBody2( wheel ) ;


    // This sets HiStop and LoStop to 0 on axis 1
    joint->setSteerAngle( 0.0 ) ;


    // spring and damper...

    joint->setSpring( SPRING ) ;
    joint->setDamperBound( DAMPER_BOUND ) ;
    joint->setDamperRebound( DAMPER_REBOUND ) ;
    joint->setPreload( PRELOAD ) ;


    // Graphics
    joint->getMatrixTransform()->addChild( graphics ) ;


    return joint ;
}
