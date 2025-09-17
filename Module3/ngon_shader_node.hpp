//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.667 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	Brian Russin
//
//	Author:	David W. Nesbitt
//	File:     ngon_shader_node.h
//	Purpose:	Derived class to handle simple shaders for drawing ngons.
//
//============================================================================

#ifndef __MODULE3_NGON_SHADER_NODE_HPP__
#define __MODULE3_NGON_SHADER_NODE_HPP__

#include "scene/shader_node.hpp"

namespace cg
{

/**
 * Offset line shader node.
 */
class NGonShaderNode : public ShaderNode
{
  public:
    /**
     * Gets uniform and attribute locations.
     */
    bool get_locations() override;

    /**
     * Draw method for this shader - enable the program and set up uniforms
     * and vertex attribute locations
     * @param  scene_state   Current scene state.
     */
    void draw(SceneState &scene_state) override;

    /**
     * Get the vertex position attribute location
     * @return  Returns the position attribute location.
     */
    int32_t get_position_loc() const;

  protected:
    // Uniform and attribute locations
    GLint ortho_matrix_loc_;
    GLint color_loc_;
    GLint position_loc_;
};

} // namespace cg

#endif
