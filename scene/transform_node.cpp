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
    Matrix4x4 normal_matrix;
    
    // Method 1: Proper 3x3 extraction and normal matrix calculation
    // Extract upper 3x3 portion and create a proper 4x4 matrix for inversion
    Matrix4x4 upper3x3_as_4x4;
    upper3x3_as_4x4.set_identity();  // Start with identity
    
    // Copy ONLY the upper 3x3 rotation/scaling part
    upper3x3_as_4x4.m00() = scene_state.model_matrix.m00();
    upper3x3_as_4x4.m01() = scene_state.model_matrix.m01();
    upper3x3_as_4x4.m02() = scene_state.model_matrix.m02();
    upper3x3_as_4x4.m10() = scene_state.model_matrix.m10();
    upper3x3_as_4x4.m11() = scene_state.model_matrix.m11();
    upper3x3_as_4x4.m12() = scene_state.model_matrix.m12();
    upper3x3_as_4x4.m20() = scene_state.model_matrix.m20();
    upper3x3_as_4x4.m21() = scene_state.model_matrix.m21();
    upper3x3_as_4x4.m22() = scene_state.model_matrix.m22();
    // Leave m03, m13, m23 as 0.0 (no translation for normal matrix)
    // Leave m30, m31, m32 as 0.0 and m33 as 1.0 (identity bottom row)
    
    // Calculate inverse transpose: (M^-1)^T = (M^T)^-1
    // We can do this either way - let's use the transpose-then-invert approach
    Matrix4x4 transposed = upper3x3_as_4x4.get_transpose();
    normal_matrix = transposed.get_inverse();
    
    // Alternative approach - invert then transpose (mathematically equivalent):
    // Matrix4x4 inverted = upper3x3_as_4x4.get_inverse();
    // normal_matrix = inverted.get_transpose();
    
    // Calculate composite PVM matrix (projection * view * model)
    Matrix4x4 pvm_matrix = scene_state.pv * scene_state.model_matrix;
    
    // Set the GLSL uniforms if their locations are valid
    if (scene_state.model_matrix_loc >= 0) {
        glUniformMatrix4fv(scene_state.model_matrix_loc, 1, GL_FALSE, scene_state.model_matrix.get());
        
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cout << "OpenGL error setting model matrix: " << error << std::endl;
        }
    }
    
    if (scene_state.normal_matrix_loc >= 0) {
        glUniformMatrix4fv(scene_state.normal_matrix_loc, 1, GL_FALSE, normal_matrix.get());
        
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cout << "OpenGL error setting normal matrix: " << error << std::endl;
        }
    }
    
    if (scene_state.pvm_matrix_loc >= 0) {
        glUniformMatrix4fv(scene_state.pvm_matrix_loc, 1, GL_FALSE, pvm_matrix.get());
        
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cout << "OpenGL error setting PVM matrix: " << error << std::endl;
        }
    }

    // Draw all children with the updated transformation state
    SceneNode::draw(scene_state);
    
    // Restore the previous model matrix state by popping from the stack
    scene_state.pop_transforms();
}

void TransformNode::update(SceneState &scene_state) {}

} // namespace cg
