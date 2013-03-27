#include "Car"

#include <osgODE/SuspensionJoint>
#include <osgODE/Notify>




using namespace osgODE ;




void
Car::handleKeyDown(const int& key, const int& mod_key_mask, bool& handled)
{
    const double    F_FORCE = 25 ;
    const double    R_FORCE = 50 ;
    const double    VEL = 12.5 * 2.0 * osg::PI ;
    const double    BRAKE_VEL = 0.0 * 2.0 * osg::PI ;
    const double    R_BRAKE_FORCE = FLT_MAX ;
    const double    F_BRAKE_FORCE = 100.0 ;



    // Right wheel has negative speed because its axis is opposite
    // to the axis of the left wheel


    switch( key )
    {
        case 'w':
            m_hinge_RL->setParam(dParamVel2, -VEL) ;
            m_hinge_RL->setParam(dParamFMax2, R_FORCE) ;

            m_hinge_RR->setParam(dParamVel2,  VEL) ;
            m_hinge_RR->setParam(dParamFMax2, R_FORCE) ;

            m_hinge_FL->setParam(dParamVel2, -VEL) ;
            m_hinge_FL->setParam(dParamFMax2, F_FORCE) ;

            m_hinge_FR->setParam(dParamVel2,  VEL) ;
            m_hinge_FR->setParam(dParamFMax2, F_FORCE) ;

            handled = true ;
        break ;




        case 's':
            m_hinge_RL->setParam(dParamVel2,  BRAKE_VEL) ;
            m_hinge_RL->setParam(dParamFMax2, R_BRAKE_FORCE) ;

            m_hinge_RR->setParam(dParamVel2, -BRAKE_VEL) ;
            m_hinge_RR->setParam(dParamFMax2, R_BRAKE_FORCE) ;


            m_hinge_FL->setParam(dParamVel2,  BRAKE_VEL) ;
            m_hinge_FL->setParam(dParamFMax2, F_BRAKE_FORCE) ;

            m_hinge_FR->setParam(dParamVel2, -BRAKE_VEL) ;
            m_hinge_FR->setParam(dParamFMax2, F_BRAKE_FORCE) ;

            handled = true ;
        break ;




        case 'a':
            m_steering_left = true ;

            handled = true ;
        break ;




        case 'd':
            m_steering_right = true ;

            handled = true ;
        break ;



        default:
            handled = false ;
        break ;
    }
}




void
Car::handleKeyUp(const int& key, const int& mod_key_mask, bool& handled)
{
    const double    F_FORCE = 25 ;
    const double    R_FORCE = 50 ;


    switch( key )
    {
        case 'w':
            m_hinge_RL->setParam(dParamVel2, 0.0) ;
            m_hinge_RL->setParam(dParamFMax2, R_FORCE) ;

            m_hinge_RR->setParam(dParamVel2, 0.0) ;
            m_hinge_RR->setParam(dParamFMax2, R_FORCE) ;


            m_hinge_FL->setParam(dParamVel2, 0.0) ;
            m_hinge_FL->setParam(dParamFMax2, F_FORCE) ;

            m_hinge_FR->setParam(dParamVel2, 0.0) ;
            m_hinge_FR->setParam(dParamFMax2, F_FORCE) ;

            handled = true ;
        break ;




        case 's':
            m_hinge_RL->setParam(dParamVel2, 0.0) ;
            m_hinge_RL->setParam(dParamFMax2, R_FORCE) ;

            m_hinge_RR->setParam(dParamVel2, 0.0) ;
            m_hinge_RR->setParam(dParamFMax2, R_FORCE) ;


            m_hinge_FL->setParam(dParamVel2, 0.0) ;
            m_hinge_FL->setParam(dParamFMax2, F_FORCE) ;

            m_hinge_FR->setParam(dParamVel2, 0.0) ;
            m_hinge_FR->setParam(dParamFMax2, F_FORCE) ;

            handled = true ;
        break ;




        case 'a':
            m_steering_left = false ;

            handled = true ;
        break ;




        case 'd':
            m_steering_right = false ;

            handled = true ;
        break ;




        default:
            handled = false ;
        break ;
    }
}
