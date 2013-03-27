#include <osgODE/World>
#include <osgODE/ODEObject>

#include <iostream>



namespace {
    class TestTraverse: public osg::Referenced {
    public:
        void    doSomething(osgODE::ODEObject* obj) {
            std::cout << obj << std::endl ;
        }
    } ;
}



int
main(int argc, char** argv)
{
    osg::ref_ptr<osgODE::World> world = new osgODE::World() ;


    for(unsigned int i=0; i<10; i++) {
        world->addObject( new osgODE::ODEObject() ) ;
    }

    osg::ref_ptr<TestTraverse>  tt = new TestTraverse() ;


    world->traverseObjects(*tt, &TestTraverse::doSomething) ;




    // ciao
    return 0 ;
}
