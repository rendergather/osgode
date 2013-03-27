#include <osgODE/Manager>
#include <osgODE/World>
#include <osgODE/RigidBody>
#include <osgODE/AMotorJoint>
#include <osgODE/Notify>

#include <osgODEUtil/ControllerBase>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgViewer/Viewer>




#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data/examples"
#endif




class OSG_EXPORT Controller: public osg::Referenced
{
public:
    Controller(osgODE::Joint* joint=NULL):
        m_joint(joint) {}


    void    keyDown(const int& key, const int& mask, bool& handled)
    {
        (void) mask ;

        switch( key )
        {
            case 'x':
            {
                m_joint->setParam(  dParamVel1, m_joint->getParam(dParamVel1) + 1.0 ) ;
                m_joint->setParam(  dParamFMax1, 1.0 ) ;
                handled = true ;
            }
            break ;

            case 'X':
            {
                m_joint->setParam(  dParamVel1, m_joint->getParam(dParamVel1) - 1.0 ) ;
                m_joint->setParam(  dParamFMax1, 1.0 ) ;
                handled = true ;
            }
            break ;



            case 'y':
            {
                m_joint->setParam(  dParamVel2, m_joint->getParam(dParamVel2) + 1.0 ) ;
                m_joint->setParam(  dParamFMax2, 1.0 ) ;
                handled = true ;
            }
            break ;

            case 'Y':
            {
                m_joint->setParam(  dParamVel2, m_joint->getParam(dParamVel2) - 1.0 ) ;
                m_joint->setParam(  dParamFMax2, 1.0 ) ;
                handled = true ;
            }
            break ;



            case 'z':
            {
                m_joint->setParam(  dParamVel3, m_joint->getParam(dParamVel3) + 1.0 ) ;
                m_joint->setParam(  dParamFMax3, 1.0 ) ;
                handled = true ;
            }
            break ;

            case 'Z':
            {
                m_joint->setParam(  dParamVel3, m_joint->getParam(dParamVel3) - 1.0 ) ;
                m_joint->setParam(  dParamFMax3, 1.0 ) ;
                handled = true ;
            }
            break ;



            case 'r':
            {
                m_joint->setParam(  dParamVel1, 0.0 ) ;
                m_joint->setParam(  dParamFMax1, 5.0 ) ;
                m_joint->setParam(  dParamVel2, 0.0 ) ;
                m_joint->setParam(  dParamFMax2, 5.0 ) ;
                m_joint->setParam(  dParamVel3, 0.0 ) ;
                m_joint->setParam(  dParamFMax3, 5.0 ) ;
                handled = true ;
            }
            break ;



            default:
                handled = false ;
            break ;
        }
    }

private:
    osg::ref_ptr<osgODE::Joint>     m_joint ;
} ;




int
main(int argc, char** argv)
{
    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;

    osgODE::Manager*    manager = new osgODE::Manager() ;

    manager->setup(false, true, 1.0/60.0) ;

    manager->setWorld( new osgODE::World() ) ;




    osgODE::Joint*      j = new osgODE::AMotorJoint(osgODE::AMotorJoint::BODY1) ;




    osg::Node*      axis = osgDB::readNodeFile("common/axis3.osgb") ;

    PS_ASSERT1( axis != NULL ) ;

    osgODE::RigidBody*  b1 = new osgODE::RigidBody() ;

    b1->getMatrixTransform()->addChild(axis) ;


    // gravity off
    b1->setGravityMode(false) ;




    j->setBody1(b1) ;

    manager->getWorld()->addObject(b1) ;
    manager->getWorld()->addObject(j) ;



    osgViewer::Viewer   viewer ;
    viewer.setSceneData(manager) ;
    viewer.realize() ;


    osg::ref_ptr<Controller>    controller = new Controller(j) ;

    osgODEUtil::ControllerBase* controller_base = new osgODEUtil::ControllerBase() ;

    controller_base->onKeyPressed()->connect( controller.get(), &Controller::keyDown ) ;

    viewer.getEventHandlers().push_back( controller_base ) ;


    viewer.run() ;

    return 0 ;
}
