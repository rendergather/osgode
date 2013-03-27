#include <osgODEUtil/CreateTriMeshFromNode>
#include <osgODE/TriMesh>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>




int
main(int argc, char** argv)
{
    osg::Node*  n = osgDB::readNodeFile("space.osgt") ;


    n->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE) ;


    osgODE::TriMesh*    t = osgODEUtil::createTriMeshFromNode(n) ;

    t->getMatrixTransform()->addChild(n) ;

    t->setKinematic(true) ;

    osgDB::writeObjectFile(*t, "space.osgb") ;

    return 0 ;
}