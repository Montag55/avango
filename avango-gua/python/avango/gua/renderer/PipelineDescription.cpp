#include "PipelineDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/PipelineDescription.hpp>

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

av::Link<av::gua::PipelineDescription> create_default_pipeline_description() {
  return av::Link<av::gua::PipelineDescription>(
          new av::gua::PipelineDescription(
            std::make_shared<::gua::PipelineDescription>(::gua::PipelineDescription::make_default())
          )
        );
}

void init_PipelineDescription()
 {

  register_field<av::gua::SFPipelineDescription>("SFPipelineDescription");
  register_multifield<av::gua::MFPipelineDescription>("MFPipelineDescription");
  class_<av::gua::PipelineDescription,
         av::Link<av::gua::PipelineDescription>,
         bases<av::FieldContainer>, boost::noncopyable >("PipelineDescription", "docstring", no_init)
         .def("add_tri_mesh_pass", &av::gua::PipelineDescription::add_tri_mesh_pass,
              return_value_policy<reference_existing_object>())
         ;

  def("create_default_pipeline_description", &create_default_pipeline_description);
 }
