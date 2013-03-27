/*!
 * @file Notify_android.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2012 by Rocco Martino                                   *
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
#include <osgODE/Notify>
/* ======================================================================= */




#ifdef ANDROID




/* ======================================================================= */
/* ....................................................................... */
#include <osg/Notify>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
class AndroidNotifyHandler: public osg::NotifyHandler
{
public:
    virtual void    notify(osg::NotifySeverity severity, const char* message)
    {
        int     prio = ANDROID_LOG_UNKNOWN ;

        switch(severity)
        {
            case osg::DEBUG_FP:     prio = ANDROID_LOG_DEBUG ;  break ;
            case osg::DEBUG_INFO:   prio = ANDROID_LOG_DEBUG ;  break ;
            case osg::INFO:         prio = ANDROID_LOG_INFO ;   break ;
            case osg::NOTICE:       prio = ANDROID_LOG_INFO ;   break ;
            case osg::WARN:         prio = ANDROID_LOG_WARN ;   break ;
            case osg::FATAL:        prio = ANDROID_LOG_FATAL ;  break ;
        }


        __android_log_print(prio, "OSG", "%s", message) ;
    }
} ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
psInstallAndroidNotifyHandler(void)
{
    osg::setNotifyHandler( new AndroidNotifyHandler() ) ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif // ANDORID
