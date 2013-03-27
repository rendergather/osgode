#include <osgViewer/Viewer>

#include <osgODEUtil/Icosahedron>

#include <osg/Geode>
// #include <osg/PolygonMode>

#include <iostream>




int
main(int argc, char** argv)
{
    osgODEUtil::Icosahedron*    ico = new osgODEUtil::Icosahedron( (argc > 1) ? atoi(argv[1]) : 0, false) ;
    osg::Geode*                 geode = new osg::Geode() ;

    geode->addDrawable(ico) ;
    

//     ico->getOrCreateStateSet()->setAttributeAndModes( new osg::PolygonMode(osg::PolygonMode::FRONT, osg::PolygonMode::LINE), osg::StateAttribute::ON ) ;
//     ico->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::ON) ;


//     std::cout << ico->getVertexArray()->getNumElements() << std::endl ;
//     ico->subdivide() ;
//     std::cout << ico->getVertexArray()->getNumElements() << std::endl ;
//     ico->subdivide() ;
//     std::cout << ico->getVertexArray()->getNumElements() << std::endl ;
//     ico->subdivide() ;
//     std::cout << ico->getVertexArray()->getNumElements() << std::endl ;
//     ico->subdivide() ;
//     std::cout << ico->getVertexArray()->getNumElements() << std::endl ;
//     ico->subdivide() ;
//     std::cout << ico->getVertexArray()->getNumElements() << std::endl ;
//     ico->subdivide() ;
//     std::cout << ico->getVertexArray()->getNumElements() << std::endl ;

    osgViewer::Viewer   viewer ;

    viewer.setSceneData(geode) ;
    return viewer.run() ;
}
