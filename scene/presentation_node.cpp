#include "scene/presentation_node.hpp"

namespace cg
{

PresentationNode::PresentationNode() { node_type_ = SceneNodeType::PRESENTATION; }

void PresentationNode::draw(SceneState &scene_state) { SceneNode::draw(scene_state); }

} // namespace cg
