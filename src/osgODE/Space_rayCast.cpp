/*!
 * @file Space_rayCast.cpp
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
#include <osgODE/Space>
#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
namespace {

class RayNearCallbackData {
public:
    RayNearCallbackData(Space::RayCastResult* rr, unsigned int mc)
    {
        Result = rr ;
        MaxContacts = mc ;
        Contacts = new dContactGeom[mc] ;
        NumContacts = 0 ;
    }

    ~RayNearCallbackData(void) { delete Contacts ; }


    osg::ref_ptr<Space::RayCastResult>  Result ;
    unsigned int                        MaxContacts ;
    dContactGeom*                       Contacts ;
    unsigned int                        NumContacts ;
} ;


static void
_rayNearCallback(void* data, dGeomID o1, dGeomID o2)
{

    // ensure the first body is the ray
    PS_ASSERT1( dGeomGetData(o1) == (void*)(0xdeadbeef) ) ;



    // Don't want a ray can collide with itself
    if( o1 == o2 ) {
        return ;
    }


    Collidable*     other = static_cast<Collidable*>( dGeomGetData(o2) ) ;

    PS_ASSERT1( other ) ;


    RayNearCallbackData*    callback_data = static_cast<RayNearCallbackData*>(data) ;

    PS_ASSERT1( callback_data ) ;



    Space::RayCastResult*   result = callback_data->Result.get() ;

    PS_ASSERT1(result) ;



    dContactGeom*   contacts = callback_data->Contacts ;

    int     num_contacts = dCollide(o1, o2, callback_data->MaxContacts, contacts, sizeof(dContactGeom)) ;


    for(int i=0; i<num_contacts; i++) {

        dContactGeom    c = contacts[i] ;


        result->addContact( other,
                            osg::Vec3(c.pos[0], c.pos[1], c.pos[2]),
                            osg::Vec3(c.normal[0], c.normal[1], c.normal[2]),
                            c.depth ) ;
    }

    callback_data->NumContacts += num_contacts ;

}
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */









/* ======================================================================= */
/* ....................................................................... */
unsigned int
Space::performRayCast(  const osg::Vec3&    from,
                        const osg::Vec3&    to,
                        RayCastResult*      result,
                        unsigned int        max_contacts,
                        unsigned int        param_mask,
                        int                 collide_bits,
                        int                 category_bits )
{

    PS_DBG3("oo::Space::performRayCast(%p, [%f, %f, %f], [%f, %f, %f])",
            this,
            from.x(), from.y(), from.z(),
            to.x(), to.y(), to.z()
           ) ;



    PS_ASSERT1( result != NULL ) ;


    /*
     * [1] create the ray
     */
    osg::Vec3   dir = to - from ;

    dGeomID     ray = dCreateRay(m_ODE_space, dir.normalize()) ;

    dGeomRaySet(    ray,
                    from.x(), from.y(), from.z(),
                    dir.x(), dir.y(), dir.z() ) ;

    /*
     * [2] Set ray properties
     */
    bool    first_contact =     0  !=  ( param_mask & FIRST_CONTACT ) ;
    bool    backface_cull =     0  !=  ( param_mask & BACKFACE_CULL ) ;
    bool    closest_hit =       0  !=  ( param_mask & CLOSEST_HIT ) ;

    dGeomRaySetParams(ray, first_contact, backface_cull) ;
    dGeomRaySetClosestHit(ray, closest_hit) ;


    // I use this number to mark the first body
    dGeomSetData(ray, (void*)(0xdeadbeef)) ;


    // category and collide bits
    dGeomSetCategoryBits( ray, category_bits ) ;
    dGeomSetCollideBits( ray, collide_bits ) ;




    /*
     * [3] ray cast
     */
    RayNearCallbackData     rnc_data(result, max_contacts) ;

    dSpaceCollide2(ray, (dGeomID)m_ODE_space, &rnc_data, _rayNearCallback) ;




    /*
     * [4] Done, Destroy everything
     */
    dGeomDestroy(ray) ;

    PS_DBG3("oo::Space::rayCast(%p, ...): %u contacts", this, rnc_data.NumContacts) ;

    return rnc_data.NumContacts ;
}
/* ....................................................................... */
/* ======================================================================= */









/* ======================================================================= */
/* ....................................................................... */
unsigned int
Space::rayCast( const osg::Vec3&    from,
                const osg::Vec3&    to,
                RayCastResult*      result,
                unsigned int        max_contacts,
                bool                first_contact,
                bool                backface_cull,
                bool                closest_hit)
{

    PS_DBG3("oo::Space::rayCast(%p, [%f, %f, %f], [%f, %f, %f])",
            this,
            from.x(), from.y(), from.z(),
            to.x(), to.y(), to.z()
           ) ;



    PS_ASSERT1( result != NULL ) ;


    /*
     * [1] create the ray
     */
    osg::Vec3   dir = to - from ;

    dGeomID     ray = dCreateRay(m_ODE_space, dir.normalize()) ;

    dGeomRaySet(    ray,
                    from.x(), from.y(), from.z(),
                    dir.x(), dir.y(), dir.z() ) ;

    /*
     * [2] Set ray properties
     */
    dGeomRaySetParams(ray, first_contact, backface_cull) ;
    dGeomRaySetClosestHit(ray, closest_hit) ;


    // I use this number to mark the first body
    dGeomSetData(ray, (void*)(0xdeadbeef)) ;




    /*
     * [3] ray cast
     */
    RayNearCallbackData     rnc_data(result, max_contacts) ;

    dSpaceCollide2(ray, (dGeomID)m_ODE_space, &rnc_data, _rayNearCallback) ;




    /*
     * [4] Done, Destroy everything
     */
    dGeomDestroy(ray) ;

    PS_DBG3("oo::Space::rayCast(%p, ...): %u contacts", this, rnc_data.NumContacts) ;

    return rnc_data.NumContacts ;
}
/* ....................................................................... */
/* ======================================================================= */
