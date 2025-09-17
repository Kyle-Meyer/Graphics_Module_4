#include "Module3/ngon_shader_node.hpp"

#include <iostream>

namespace cg
{

bool NGonShaderNode::get_locations()
{
    ortho_matrix_loc_ = glGetUniformLocation(shader_program_.get_program(), "ortho");
    if(ortho_matrix_loc_ < 0)
    {
        std::cout << "Error getting ortho matrix location\n";
        return false;
    }
    color_loc_ = glGetUniformLocation(shader_program_.get_program(), "color");
    if(color_loc_ < 0)
    {
        std::cout << "Error getting color location\n";
        return false;
    }
    position_loc_ = glGetAttribLocation(shader_program_.get_program(), "vtx_position");
    if(position_loc_ < 0)
    {
        std::cout << "Error getting vertex position location\n";
        return false;
    }
    return true;
}

void NGonShaderNode::draw(SceneState &scene_state)
{
    // Enable this program
    shader_program_.use();

    // Set scene state locations to ones needed for this program
    scene_state.ortho_matrix_loc = ortho_matrix_loc_;
    scene_state.color_loc = color_loc_;
    scene_state.position_loc = position_loc_;

    // Set the matrix (do this here for now)
    glUniformMatrix4fv(ortho_matrix_loc_, 1, GL_FALSE, scene_state.ortho_matrix.get());

    // Draw all children
    SceneNode::draw(scene_state);
}

int32_t NGonShaderNode::get_position_loc() const { return position_loc_; }

} // namespace cg
