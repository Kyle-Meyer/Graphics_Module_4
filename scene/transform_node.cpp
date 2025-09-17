#include "scene/transform_node.hpp"

namespace cg
{

TransformNode::TransformNode()
{
    node_type_ = SceneNodeType::TRANSFORM;
    load_identity();
}

TransformNode::~TransformNode() {}

void TransformNode::load_identity()
{
    // Student to complete - Module 4
}

void TransformNode::translate(float x, float y, float z)
{
    // Student to complete - Module 4
}

void TransformNode::rotate(float deg, Vector3 &v)
{
    // Student to complete - Module 4
}

void TransformNode::rotate_x(float deg)
{
    // Student to complete - Module 4
}

void TransformNode::rotate_y(float deg)
{
    // Student to complete - Module 4
}

void TransformNode::rotate_z(float deg)
{
    // Student to complete - Module 4
}

void TransformNode::scale(float x, float y, float z)
{
    // Student to complete - Module 4
}

void TransformNode::draw(SceneState &scene_state)
{
    // Student to complete - Module 4

    // Draw all children
    SceneNode::draw(scene_state);
}

void TransformNode::update(SceneState &scene_state) {}

} // namespace cg
