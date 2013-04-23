#include <osgODE/Character>
#include <osgODE/RigidBody>

#include <osgDB/WriteFile>





int
main(int argc, char** argv)
{
    osg::ref_ptr<osgODE::Character> character = new osgODE::Character() ;

    character->init() ;

    character->getBody()->setPosition( osg::Z_AXIS * 5 + osg::Y_AXIS * -15 ) ;


    osgDB::writeObjectFile( *character, "character.osgt" ) ;

    return 0 ;
}
