//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.667 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	Brian Russin
//
//	Author:	David W. Nesbitt
//	File:    lighting_shader_node.hpp
//	Purpose: Derived class to handle the lighting shader program.
//
//============================================================================

#ifndef __MODULE4_LIGHTING_SHADER_NODE_HPP__
#define __MODULE4_LIGHTING_SHADER_NODE_HPP__

#include "scene/shader_node.hpp"

namespace cg
{

/**
 * Simple lighting shader node.
 */
class LightingShaderNode : public ShaderNode
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
     * Get the location of the vertex position attribute.
     * @return  Returns the vertex position attribute location.
     */
    int32_t get_position_loc() const;

    /**
     * Get the location of the vertex normal attribute.
     * @return  Returns the vertex normal attribute location.
     */
    int32_t get_normal_loc() const;

  protected:
    // Uniform and attribute locations:
    GLint position_loc_;       // Vertex position attribute location
    GLint vertex_normal_loc_;  // Vertex normal attribute location
    GLint material_color_loc_; // Material diffuse color location
    GLint pvm_matrix_loc_;     // Composite projection, view, model matrix location
    GLint model_matrix_loc_;   // Modeling composite matrix location
    GLint normal_matrix_loc_;  // Normal transformation matrix location
};

} // namespace cg

#endif
