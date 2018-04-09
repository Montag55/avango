#include <avango/gua/scenegraph/Video3DNode.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <functional>
#include <iostream>

AV_FC_DEFINE(av::gua::Video3DNode);

AV_FIELD_DEFINE(av::gua::SFVideo3DNode);
AV_FIELD_DEFINE(av::gua::MFVideo3DNode);

av::gua::Video3DNode::Video3DNode(std::shared_ptr< ::gua::node::Video3DNode> guanode)
  : GeometryNode(guanode)
  , m_guaVideo3DNode(guanode)
{
  AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                        std::bind(&Video3DNode::getGeometryCB, this,std::placeholders::_1),
                        std::bind(&Video3DNode::setGeometryCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                      std::bind(&Video3DNode::getMaterialCB, this,std::placeholders::_1),
                      std::bind(&Video3DNode::setMaterialCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderToGBuffer,
                      std::bind(&Video3DNode::getRenderToGBufferCB, this,std::placeholders::_1),
                      std::bind(&Video3DNode::setRenderToGBufferCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderToStencilBuffer,
                      std::bind(&Video3DNode::getRenderToStencilBufferCB, this,std::placeholders::_1),
                      std::bind(&Video3DNode::setRenderToStencilBufferCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(GlobalCompressionLevel,
                      std::bind(&Video3DNode::getGlobalCompressionLevelCB, this,std::placeholders::_1),
                      std::bind(&Video3DNode::setGlobalCompressionLevelCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(DepthCompressionLevel,
                      std::bind(&Video3DNode::getDepthCompressionLevelCB, this,std::placeholders::_1),
                      std::bind(&Video3DNode::setDepthCompressionLevelCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(ColorCompressionLevel,
                      std::bind(&Video3DNode::getColorCompressionLevelCB, this,std::placeholders::_1),
                      std::bind(&Video3DNode::setColorCompressionLevelCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(DebugMessage,
                      std::bind(&Video3DNode::getDebugMessageCB, this,std::placeholders::_1),
                      std::bind(&Video3DNode::setDebugMessageCB, this,std::placeholders::_1));

  if (guanode->get_material()) {
    m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
  }

}

void av::gua::Video3DNode::on_distribute(av::gua::NetTransform& netNode)
{
    GeometryNode::on_distribute(netNode);

    if (m_Material.isValid()) {
      m_Material->on_distribute(netNode);
    }
    netNode.distributeFieldContainer(m_Material);
}

void av::gua::Video3DNode::on_undistribute(av::gua::NetTransform& netNode)
{
    GeometryNode::on_undistribute(netNode);

    if (m_Material.isValid()) {
      m_Material->on_undistribute(netNode);
    }
    netNode.undistributeFieldContainer(m_Material);
}


void
av::gua::Video3DNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::GeometryNode::initClass();

    AV_FC_INIT(av::gua::GeometryNode, av::gua::Video3DNode, true);

    SFVideo3DNode::initClass("av::gua::SFVideo3DNode", "av::Field");
    MFVideo3DNode::initClass("av::gua::MFVideo3DNode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

void
av::gua::Video3DNode::getGeometryCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaVideo3DNode->get_video_description();
}

void
av::gua::Video3DNode::setGeometryCB(const SFString::SetValueEvent& event)
{
  m_guaVideo3DNode->set_video_description(event.getValue());
}

void
av::gua::Video3DNode::getMaterialCB(const SFMaterial::GetValueEvent& event)
{
  if (m_Material.isValid()) {
    *(event.getValuePtr()) = m_Material;
  }
}

void
av::gua::Video3DNode::setMaterialCB(const SFMaterial::SetValueEvent& event)
{
  if (event.getValue().isValid()) {
    m_Material = event.getValue();
    m_guaVideo3DNode->set_material(m_Material->getGuaMaterial());
  }
}

void
av::gua::Video3DNode::getRenderToGBufferCB(const SFBool::GetValueEvent& event)
{
//  *(event.getValuePtr()) = m_guaVideo3DNode->get_render_to_gbuffer();
}

void
av::gua::Video3DNode::setRenderToGBufferCB(const SFBool::SetValueEvent& event)
{
//  m_guaVideo3DNode->set_render_to_gbuffer(event.getValue());
}


void
av::gua::Video3DNode::getRenderToStencilBufferCB(const SFBool::GetValueEvent& event)
{
//  *(event.getValuePtr()) = m_guaVideo3DNode->get_render_to_stencil_buffer();
}

void
av::gua::Video3DNode::setRenderToStencilBufferCB(const SFBool::SetValueEvent& event)
{
//  m_guaVideo3DNode->set_render_to_stencil_buffer(event.getValue());
}

void
av::gua::Video3DNode::getGlobalCompressionLevelCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaVideo3DNode->get_global_compression_lvl();
}

void
av::gua::Video3DNode::setGlobalCompressionLevelCB(const SFInt::SetValueEvent& event)
{
  m_guaVideo3DNode->set_global_compression_lvl(event.getValue());
}

void
av::gua::Video3DNode::getDepthCompressionLevelCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaVideo3DNode->get_depth_compression_lvl();
}

void
av::gua::Video3DNode::setDepthCompressionLevelCB(const SFInt::SetValueEvent& event)
{
  m_guaVideo3DNode->set_depth_compression_lvl(event.getValue());
}

void
av::gua::Video3DNode::getColorCompressionLevelCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaVideo3DNode->get_color_compression_lvl();
}

void
av::gua::Video3DNode::setColorCompressionLevelCB(const SFInt::SetValueEvent& event)
{
  m_guaVideo3DNode->set_color_compression_lvl(event.getValue());
}

void
av::gua::Video3DNode::getDebugMessageCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaVideo3DNode->get_debug_message();
}

void
av::gua::Video3DNode::setDebugMessageCB(const SFString::SetValueEvent& event)
{
  std::cout << "av::gua::Video3DNode::setDebugMessageCB: DebugMessage cannot be set." << std::endl;
}

std::shared_ptr< ::gua::node::Video3DNode>
av::gua::Video3DNode::getGuaVideo3DNode() const {
  return m_guaVideo3DNode;
}
