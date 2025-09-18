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
   // save the current model matrix 
   scene_state.push_transforms();

   //apply this node's transform to the current matrix
   scene_state.model_matrix *= composite_transform_;

   //calc the normal (inverse transpose of upper 3x3)
   Matrix4x4 normal;

   //extract upper 3x3
   Matrix4x4 upper3;
   upper3.set_identity();

   //copy 3x3 
   upper3.m00() = scene_state.model_matrix.m00();
   upper3.m01() = scene_state.model_matrix.m00();
   upper3.m02() = scene_state.model_matrix.m00();
   upper3.m10() = scene_state.model_matrix.m00();
   upper3.m11() = scene_state.model_matrix.m00();
   upper3.m12() = scene_state.model_matrix.m00();
   upper3.m20() = scene_state.model_matrix.m00();
   upper3.m21() = scene_state.model_matrix.m00();
   upper3.m22() = scene_state.model_matrix.m00();

   //get inverse transpose for normal 
   normal = upper3.get_inverse().get_transpose();

   //calculate the composite pvm: projection, view, model
   Matrix4x4 pvm = scene_state.pv * scene_state.model_matrix;

   //set the GLSL uniforms if locations are invalid 
   if(scene_state.model_matrix_loc >= 0)
      glUniformMatrix4fv(scene_state.model_matrix_loc, 1, GL_FALSE, scene_state.model_matrix.get());
   if (scene_state.normal_matrix_loc >= 0)
      glUniformMatrix4fv(scene_state.normal_matrix_loc, 1, GL_FALSE, normal.get());
   if (scene_state.pvm_matrix_loc >= 0) 
      glUniformMatrix4fv(scene_state.pvm_matrix_loc, 1, GL_FALSE, pvm.get());

   // Draw all children
   SceneNode::draw(scene_state);

   //restore the previous model matrix 
   scene_state.pop_transforms();
}

void TransformNode::update(SceneState &scene_state) {}

} // namespace cg
