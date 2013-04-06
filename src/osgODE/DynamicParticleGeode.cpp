/*!
 * @file DynamicParticleGeode.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2013 by Rocco Martino                                   *
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
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/DynamicParticleGeode>
#include <osgODE/DynamicParticleSystem>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
DynamicParticleGeode::DynamicParticleGeode(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
DynamicParticleGeode::DynamicParticleGeode(const DynamicParticleGeode& other, const osg::CopyOp& copyop):
    osg::Geode(other, copyop)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
DynamicParticleGeode::~DynamicParticleGeode(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
DynamicParticleGeode::traverse(osg::NodeVisitor& nv)
{
    if( nv.getVisitorType() == osg::NodeVisitor::CULL_VISITOR ) {

        const osg::Matrix   wtl = osg::computeWorldToLocal( nv.getNodePath() ) ;


        for(unsigned int i = 0; i < getNumDrawables(); i++) {
            DynamicParticleSystem*  dps = dynamic_cast<DynamicParticleSystem*>( getDrawable(i) ) ;

            if( dps ) {
                dps->setWorldToLocalMatrix( wtl ) ;
            }
        }

    }



    this->osg::Geode::traverse(nv) ;
}
/* ....................................................................... */
/* ======================================================================= */
