/*!
 * @file Text.cpp
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
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
// #include <osgODE/Text>
#include "Text"
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
Text::Text(void):
    m_scale( 1.0 )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Text::Text(const Text& other, const osg::CopyOp& copyop):
    osg::Drawable(other, copyop),

    m_text      ( other.m_text ),
    m_scale     ( other.m_scale ),
    m_position  ( other.m_position )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Text::~Text(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Text::drawImplementation(osg::RenderInfo& info) const
{
    (void) info ;


    GLboolean   depth_test ;

    glGetBooleanv( GL_DEPTH_TEST, &depth_test ) ;



    glDisable( GL_DEPTH_TEST ) ;



    glColor4f( 1.0, 1.0, 1.0, 1.0 ) ;

    glNormal3f( 0.0, 0.0, 1.0 ) ;


    const unsigned int  size = m_text.size() ;


    const float xskip = 1.0 / 16.0 ;
    const float yskip = 1.0 / 4.0 ;


    glBegin( GL_QUADS ) ;



    for( unsigned int i=0; i<size; i++ ) {

        const osg::Vec2     TL = _findCoord( m_text[i] ) ;

        const osg::Vec2     BL (    TL.x(),             TL.y() - yskip ) ;

        const osg::Vec2     BR (    TL.x() + xskip,     TL.y() - yskip ) ;

        const osg::Vec2     TR (    TL.x() + xskip,     TL.y() ) ;

        const float         x0 = m_position.x() + i * m_scale ;
        const float         x1 = m_position.x() + (i+1) * m_scale ;
        const float         y0 = m_position.y() ;
        const float         y1 = m_position.y() + m_scale ;


        glTexCoord2f    ( TL.x(),   TL.y()  ) ;
        glVertex2f      ( x0,       y1      ) ;

        glTexCoord2f    ( BL.x(),   BL.y()  ) ;
        glVertex2f      ( x0,       y0      ) ;

        glTexCoord2f    ( BR.x(),   BR.y()  ) ;
        glVertex2f      ( x1,       y0      ) ;

        glTexCoord2f    ( TR.x(),   TR.y()  ) ;
        glVertex2f      ( x1,       y1      ) ;
    }




    glEnd() ;



    if( depth_test ) {
        glEnable( GL_DEPTH_TEST ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osg::Vec2
Text::_findCoord( char c ) const
{
    osg::Vec2   co ;


    if( islower(c) ) {

        int     tmp = c - 'a' ;

        co.x() = tmp % 16 ;
        co.y() = tmp / 16 ;
    }




    else if( isupper(c) ) {

        int tmp = c + 10 - 'A' ;

        co.x() = tmp % 16 ;
        co.y() = 1 + tmp / 16 ;
    }




    else if( isdigit(c) ) {

        int tmp = c + 4 - '0' ;

        co.x() = tmp ;
        co.y() = 3 ;
    }




    else if( isspace(c) ) {

        co.x() = 15 ;
        co.y() = 3 ;
    }




    else  {
        co.x() = 14 ;
        co.y() = 3 ;
    }



    co.x() = co.x() / 16.0 ;
    co.y() = 1.0 - co.y() / 4.0 ;


    return co ;
}
/* ....................................................................... */
/* ======================================================================= */
