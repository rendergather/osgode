/*!
 * @file Transformable.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 - 2014 by Rocco Martino                            *
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
#include <osgODE/Transformable>
#include <osgODE/Notify>

#include <osgGA/CameraManipulator>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
Transformable::Transformable(void):
    m_matrix_transform                  ( new osg::MatrixTransform() ),
    m_camera_manipulator_center         ( osg::Vec3(0,0,0) ),
    m_camera_manipulator_direction      ( -osg::Z_AXIS ),
    m_camera_manipulator_up             ( osg::Y_AXIS ),
    m_camera_manipulator_up_object      ( false ),
    m_camera_manipulator_center_ws      ( osg::Vec3(0,0,0) ),
    m_camera_manipulator_direction_ws   ( -osg::Z_AXIS ),
    m_camera_manipulator_up_ws          ( osg::Y_AXIS ),
    m_camera_manipulator_elasticity     ( 5 )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Transformable::Transformable(const Transformable& other, const osg::CopyOp& copyop):
    ODEObject(other, copyop),
    m_matrix_transform( static_cast<osg::MatrixTransform*>( other.m_matrix_transform->clone(copyop) ) ),

    m_camera_manipulator                ( other.m_camera_manipulator ),
    m_camera_manipulator_center         ( other.m_camera_manipulator_center ),
    m_camera_manipulator_direction      ( other.m_camera_manipulator_direction ),
    m_camera_manipulator_up             ( other.m_camera_manipulator_up ),
    m_camera_manipulator_up_object      ( other.m_camera_manipulator_up_object ),
    m_camera_manipulator_center_ws      ( other.m_camera_manipulator_center_ws ),
    m_camera_manipulator_direction_ws   ( other.m_camera_manipulator_direction_ws ),
    m_camera_manipulator_up_ws          ( other.m_camera_manipulator_up_ws ),
    m_camera_manipulator_elasticity     ( other.m_camera_manipulator_elasticity )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Transformable::~Transformable(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Transformable::setCameraManipulator(osgGA::CameraManipulator* manipulator)
{
    m_camera_manipulator = manipulator ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Transformable::postUpdate(double step_size)
{
    if( m_camera_manipulator.valid() ) {
        _updateCameraManipulator(step_size) ;
    }


    this->ODEObject::postUpdate(step_size) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Transformable::_updateCameraManipulator(double step_size)
{
    PS_ASSERT1( m_camera_manipulator.valid() ) ;
    PS_ASSERT1( m_matrix_transform.valid() ) ;

    const osg::Matrix&  transform = m_matrix_transform->getMatrix() ;

    osg::Vec3           eye = m_camera_manipulator_center * transform ;
    osg::Vec3           direction = osg::Matrix::transform3x3(m_camera_manipulator_direction, transform) ;
    osg::Vec3           up = osg::Matrix::transform3x3(m_camera_manipulator_up, transform) ;


    const double    elasticity = m_camera_manipulator_elasticity / (step_size * 100) ;


    m_camera_manipulator_center_ws = (m_camera_manipulator_center_ws * elasticity + eye) /
                                            (elasticity + 1) ;

    m_camera_manipulator_direction_ws = (m_camera_manipulator_direction_ws * elasticity + direction) /
                                            (elasticity + 1) ;

    m_camera_manipulator_up_ws = (m_camera_manipulator_up_ws * elasticity + up) /
                                            (elasticity + 1) ;


    m_camera_manipulator_direction_ws.normalize() ;
    m_camera_manipulator_up_ws.normalize() ;

    osg::Matrix     view = osg::Matrix::lookAt(m_camera_manipulator_center_ws,
                                               m_camera_manipulator_center_ws + m_camera_manipulator_direction_ws,
                                               m_camera_manipulator_up_object ? m_camera_manipulator_up_ws : m_camera_manipulator_up
                                              ) ;


    m_camera_manipulator->setByInverseMatrix( view ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Transformable*
Transformable::asTransformable(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Transformable::accept(osg::NodeVisitor& nv)
{
    m_matrix_transform->accept(nv) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
const osg::BoundingSphere&
Transformable::getBound(void) const
{
    return m_matrix_transform->getBound() ;
}
/* ....................................................................... */
/* ======================================================================= */
