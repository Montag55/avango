// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/osg/ImageStream.h>
#include "OSGImageStream.h"

using namespace boost::python;
using namespace av::python;

namespace boost
 {
  namespace python
   {
    template <class T> struct pointee<av::Link<T> >
     {
      typedef T type;
     };
   }
 }

void init_OSGImageStream(void)
 {
  // wrapping osg::ImageStream functionality
  register_field<av::osg::SFImageStream>("SFImageStream");
  register_multifield<av::osg::MFImageStream>("MFImageStream");
  class_<av::osg::ImageStream, av::Link<av::osg::ImageStream>, bases<av::osg::Image>, boost::noncopyable >("ImageStream", "docstring", no_init);

  enum_<osg::ImageStream::StreamStatus>("streamstatus")
    .value("invalid",   ::osg::ImageStream::INVALID)
    .value("playing",   ::osg::ImageStream::PLAYING)
    .value("paused",    ::osg::ImageStream::PAUSED)
    .value("rewinding", ::osg::ImageStream::REWINDING)
    .export_values()
    ;

 }
