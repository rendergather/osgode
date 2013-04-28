#include "Car"

#include <osgODE/DifferentialJoint>
#include <osgODE/Notify>




using namespace osgODE ;




void
Car::handleKeyDown(const int& key, const int& mod_key_mask, bool& handled)
{
    const double    F_FORCE = 25 ;
    const double    R_FORCE = 50 ;
    const double    VEL = 12.5 * 2.0 * 2.0 * osg::PI ;
    const double    R_BRAKE_FORCE = 50.0 ;
    const double    F_BRAKE_FORCE = 100.0 ;



    // Right wheel has negative speed because its axis is opposite
    // to the axis of the left wheel


    switch( key )
    {
        case 'w':
            m_rear_differential->setParam(dParamVel, VEL) ;
            m_rear_differential->setParam(dParamFMax, R_FORCE) ;

            m_front_differential->setParam(dParamVel, VEL) ;
            m_front_differential->setParam(dParamFMax, F_FORCE) ;

            handled = true ;
        break ;




        case 's':
            m_rear_differential->setParam(dParamVel, 0.0) ;
            m_rear_differential->setParam(dParamFMax, R_BRAKE_FORCE) ;

            m_front_differential->setParam(dParamVel, 0.0) ;
            m_front_differential->setParam(dParamFMax, F_BRAKE_FORCE) ;

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
            m_rear_differential->setParam(dParamVel, 0.0) ;
            m_rear_differential->setParam(dParamFMax, R_FORCE) ;

            m_front_differential->setParam(dParamVel, 0.0) ;
            m_front_differential->setParam(dParamFMax, F_FORCE) ;

            handled = true ;
        break ;




        case 's':
            m_rear_differential->setParam(dParamVel, 0.0) ;
            m_rear_differential->setParam(dParamFMax, R_FORCE) ;

            m_front_differential->setParam(dParamVel, 0.0) ;
            m_front_differential->setParam(dParamFMax, F_FORCE) ;

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
