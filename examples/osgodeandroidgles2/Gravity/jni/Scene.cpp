/*!
 * @file Scene.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2011 - 2012 by Rocco Martino                            *
 *   martinorocco@gmail.com                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include "Scene.hpp"

#include <osgODE/Notify>
#include <osgODE/Sphere>
#include <osgODE/TriMesh>
#include <osgODE/Manager>
#include <osgODE/Space>
#include <osgODE/DefaultNearCallback>
#include <osgODE/CollisionParameters>
#include <osgODE/ManagerUpdateCallback>

#include <osgODEUtil/DrawStuff>

#include <osg/Program>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osgODE::Manager*
createScene(void)
{
    PS_DBG("%s", "createScene()") ;

    osgODE::Manager*    manager = new osgODE::Manager() ;
    PS_ASSERT1(manager) ;


    manager->setAcceptVisitors(true) ;
    manager->setAutoStartThread(false) ;
	manager->setStepSize(1.0/30.0) ;




    osgODE::World*  world = createWorld() ;
    PS_ASSERT1(world) ;

    manager->setWorld( world ) ;


    osg::NodeCallback*  manager_callback = new osgODE::ManagerUpdateCallback() ;
    PS_ASSERT1(manager_callback) ;

    manager->addUpdateCallback(manager_callback) ;



	setupShaders(manager) ;



    return manager ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osgODE::World*
createWorld(void)
{
    PS_DBG("%s", "createWorld()") ;


    osgODE::Space*  space = new osgODE::Space() ;
    PS_ASSERT1(space) ;




    space->setERP(0.2) ;
    space->setCFM(1.0e-5) ;



    osgODE::NearCallback*   near_callback = createNearCallback() ;
    PS_ASSERT1(near_callback) ;

    space->setNearCallback( near_callback ) ;



    osgODE::ODEObject*  ball = createBall() ;
    PS_ASSERT1(ball) ;

    osgODE::ODEObject*  ground = createGround() ;
    PS_ASSERT1(ground) ;


    space->addObject(ball) ;
    space->addObject(ground) ;



    return space ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osgODE::NearCallback*
createNearCallback(void)
{
    PS_DBG("%s", "createNearCallback()") ;


    osgODE::DefaultNearCallback*    cbk = new osgODE::DefaultNearCallback() ;
    PS_ASSERT1(cbk) ;



    osgODE::CollisionParameters*    cp = new osgODE::CollisionParameters() ;
    PS_ASSERT1(cp) ;

    cp->setMode(dContactApprox1) ;
    cp->setMaxContactNum(2) ;


    cbk->setCollisionParameters(cp) ;



    return cbk ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osgODE::ODEObject*
createBall(void)
{
    PS_DBG("%s", "createBall()") ;


    osgODE::Sphere* ball = new osgODE::Sphere() ;
    PS_ASSERT1(ball) ;



    osg::Node*  graphics = osgODEUtil::createSphere(    1.0,
                                                        16,
                                                        16,
                                                        osg::Vec4(1,1,1,1),
                                                        osg::Vec4(1,0,0,1)  ) ;
    PS_ASSERT1(graphics) ;



    ball->getMatrixTransform()->addChild(graphics) ;


    ball->setPosition( osg::Z_AXIS * 10 ) ;



    return ball ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osgODE::ODEObject*
createGround(void)
{
    PS_DBG("%s", "createGround()") ;


    osg::Vec3Array* vertices = new osg::Vec3Array() ;
    osg::IntArray*  indices = new osg::IntArray() ;




    vertices->push_back(    osg::Vec3(-10, -10, 0.0)    ) ;
    vertices->push_back(    osg::Vec3( 10, -10, 0.0)    ) ;
    vertices->push_back(    osg::Vec3( 10,  10, 0.0)    ) ;
    vertices->push_back(    osg::Vec3(-10,  10, 0.0)    ) ;



    indices->push_back( 0 ) ;
    indices->push_back( 1 ) ;
    indices->push_back( 2 ) ;

    indices->push_back( 0 ) ;
    indices->push_back( 2 ) ;
    indices->push_back( 3 ) ;




    osgODE::TriMesh*    trimesh = new osgODE::TriMesh() ;
    PS_ASSERT1(trimesh) ;




    trimesh->setVertexArray(vertices) ;
    trimesh->setIndexArray(indices) ;

    trimesh->build() ;


    trimesh->setKinematic(true) ;




    osg::Group*     graphics = new osg::Group() ;
    PS_ASSERT1(graphics) ;



    trimesh->getMatrixTransform()->addChild(graphics) ;

    {
        osg::Node*  plane = osgODEUtil::createChessboard(   osg::Vec2(20, 20),
                                                            osg::Vec2(1, 1),
                                                            osg::Vec4(1, 1, 1, 1),
                                                            osg::Vec4(0, 0, 0, 1),
                                                            osg::Matrix::identity() ) ;

        PS_ASSERT1(plane) ;

        graphics->addChild(plane) ;
    }



    return trimesh ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
setupShaders(osg::Node* n)
{
    PS_DBG("%s", "setupShaders()") ;



	const char*	vertex_source =
		"precision highp float ;\n"
#ifdef ANDORID
		"attribute vec4 osg_Color ;\n"
#endif
		"varying vec4   vColor ;\n"
		"void\n"
		"main()"
		"{\n"
#ifdef ANDORID
        "   vColor = osg_Color ;\n"
#else
		"	vColor = gl_Color ;\n"
#endif
		"	gl_Position = ftransform() ;\n"
		"}\n"
		"\n" ;


	const char*	fragment_source =
		"precision highp float ;\n"
		"varying vec4    vColor ;\n"
		"void\n"
		"main()"
		"{\n"
		"	gl_FragColor = vColor ;\n"
		"}\n"
		"\n" ;




	osg::Shader*	vertex = new osg::Shader(osg::Shader::VERTEX, vertex_source) ;
	osg::Shader*	fragment = new osg::Shader(osg::Shader::FRAGMENT, fragment_source) ;

	osg::Program*	program = new osg::Program() ;
	program->addShader(vertex) ;
	program->addShader(fragment) ;

	n->getOrCreateStateSet()->setAttributeAndModes(program, osg::StateAttribute::ON) ;
}
/* ....................................................................... */
/* ======================================================================= */
