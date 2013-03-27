/*!
 * @file CollisionParameters.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 by Rocco Martino                                   *
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
#include <osgODE/ODE>
#include <osgODE/CollisionParameters>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
#define     CP_DEFAULT_MODE                 dContactApprox1
#define     CP_DEFAULT_MU                   1.0
#define     CP_DEFAULT_MU2                  1.0
#define     CP_DEFAULT_BOUNCE               0.0
#define     CP_DEFAULT_BOUNCE_VEL           0.0
#define     CP_DEFAULT_SOFT_ERP             1.0
#define     CP_DEFAULT_SOFT_CFM             0.0
#define     CP_DEFAULT_MOTION1              0.0
#define     CP_DEFAULT_MOTION2              0.0
#define     CP_DEFAULT_MOTIONN              0.0
#define     CP_DEFAULT_SLIP1                0.0
#define     CP_DEFAULT_SLIP2                0.0
#define     CP_DEFAULT_MAX_CONTACT_NUM      8
#define     CP_DEFAULT_COLLIDE_CONNECTED    false
#define     CP_DEFAULT_COLLISIONS_ENABLED   true
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
CollisionParameters::CollisionParameters(void):
    m_mode                  (CP_DEFAULT_MODE),
    m_mu                    (CP_DEFAULT_MU),
    m_mu2                   (CP_DEFAULT_MU2),
    m_bounce                (CP_DEFAULT_BOUNCE),
    m_bounce_vel            (CP_DEFAULT_BOUNCE_VEL),
    m_soft_erp              (CP_DEFAULT_SOFT_ERP),
    m_soft_cfm              (CP_DEFAULT_SOFT_CFM),
    m_motion1               (CP_DEFAULT_MOTION1),
    m_motion2               (CP_DEFAULT_MOTION2),
    m_motionN               (CP_DEFAULT_MOTIONN),
    m_slip1                 (CP_DEFAULT_SLIP1),
    m_slip2                 (CP_DEFAULT_SLIP2),
    m_max_contact_num       (CP_DEFAULT_MAX_CONTACT_NUM),
    m_collide_connected     (CP_DEFAULT_COLLIDE_CONNECTED),
    m_collisions_enabled    (CP_DEFAULT_COLLISIONS_ENABLED)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
CollisionParameters::CollisionParameters(const CollisionParameters& other, const osg::CopyOp& copyop):
    osg::Object(other, copyop),
    m_mode(other.m_mode),
    m_mu(other.m_mu),
    m_mu2(other.m_mu2),
    m_bounce(other.m_bounce),
    m_bounce_vel(other.m_bounce_vel),
    m_soft_erp(other.m_soft_erp),
    m_soft_cfm(other.m_soft_cfm),
    m_motion1(other.m_motion1),
    m_motion2(other.m_motion2),
    m_motionN(other.m_motionN),
    m_slip1(other.m_slip1),
    m_slip2(other.m_slip2),
    m_max_contact_num(other.m_max_contact_num),
    m_collide_connected(other.m_collide_connected),
    m_collisions_enabled(other.m_collisions_enabled)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
CollisionParameters::~CollisionParameters(void)
{
}
/* ....................................................................... */
/* ======================================================================= */
