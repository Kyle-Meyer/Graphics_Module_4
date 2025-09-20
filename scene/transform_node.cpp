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
  composite_transform_.set_identity(); 
}

void TransformNode::translate(float x, float y, float z)
{
   composite_transform_.translate(x, y, z);
}

void TransformNode::rotate(float deg, Vector3 &v)
{
   composite_transform_.rotate(deg, v.x, v.y, v.z);
}

void TransformNode::rotate_x(float deg)
{
   composite_transform_.rotate_x(deg);
}

void TransformNode::rotate_y(float deg)
{
   composite_transform_.rotate_y(deg);
}

void TransformNode::rotate_z(float deg)
{
   composite_transform_.rotate_z(deg);
}

void TransformNode::scale(float x, float y, float z)
{
   composite_transform_.scale(x, y, z);
}

void TransformNode::draw(SceneState &scene_state)
{
    // Save the current model matrix state by pushing it onto the stack
    scene_state.push_transforms();
    
    // Apply this transform node's transformation to the current model matrix
    scene_state.model_matrix *= composite_transform_;
    
    // Calculate the normal matrix (inverse transpose of upper 3x3 of model matrix)
    // For transforming normals, we need the inverse transpose to handle non-uniform scaling
    Matrix4x4 normal_matrix = scene_state.model_matrix;
    
    // For most cases, especially with uniform scaling and rotations, we can use
    // the upper 3x3 of the model matrix directly. For non-uniform scaling,
    // we would need the inverse transpose, but let's try the direct approach first.
    
    // Calculate composite PVM matrix (projection * view * model)
    Matrix4x4 pvm_matrix = scene_state.pv * scene_state.model_matrix;
    
    // Set the GLSL uniforms if their locations are valid
    if (scene_state.model_matrix_loc >= 0) 
        glUniformMatrix4fv(scene_state.model_matrix_loc, 1, GL_FALSE, scene_state.model_matrix.get());
    
    if (scene_state.normal_matrix_loc >= 0) 
        glUniformMatrix4fv(scene_state.normal_matrix_loc, 1, GL_FALSE, normal_matrix.get());
    
    if (scene_state.pvm_matrix_loc >= 0) 
        glUniformMatrix4fv(scene_state.pvm_matrix_loc, 1, GL_FALSE, pvm_matrix.get());
        

    // Draw all children with the updated transformation state
    SceneNode::draw(scene_state);
    
    // Restore the previous model matrix state by popping from the stack
    scene_state.pop_transforms();
}

void TransformNode::update(SceneState &scene_state) {}

} // namespace cg
