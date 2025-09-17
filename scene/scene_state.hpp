//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.667 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	Brian Russin
//
//	Author:	 David W. Nesbitt
//	File:    scene_state.hpp
//	Purpose: Class used to propogate state during traversal of the scene graph.
//
//============================================================================

#ifndef __SCENE_SCENE_STATE_HPP__
#define __SCENE_SCENE_STATE_HPP__

#include "geometry/matrix.hpp"
#include "scene/graphics.hpp"

#include <list>

#include <array>

namespace cg
{

/**
 * Scene state structure. Used to store OpenGL state - shader locations,
 * matrices, etc.
 */
struct SceneState
{
    // Vertex attribute locations
    GLint position_loc;  // Vertex position attribute location
    GLint vtx_color_loc; // Vertex color attribute location
    GLint normal_loc;    // Vertex normal

    // Uniform locations
    GLint ortho_matrix_loc;  // Orthographic projection location (2-D)
    GLint color_loc;         // Constant color
    GLint pvm_matrix_loc;    // Composite project, view, model matrix location
    GLint model_matrix_loc;  // Model matrix location
    GLint normal_matrix_loc; // Normal matrix location

    // Material uniform locations
    GLint material_diffuse_loc; // Material diffuse reflection location

    // Current matrices
    std::array<float, 16> ortho;        // Orthographic projection matrix (2-D)
    Matrix4x4             ortho_matrix; // Orthographic projection matrix (2-D)
    Matrix4x4             pv;           // Current composite projection and view matrix
    Matrix4x4             model_matrix; // Current model matrix

    // Retained state to push/pop modeling matrix
    std::list<Matrix4x4> model_matrix_stack;

    /**
     * Initialize scene state prior to drawing.
     */
    void init();

    /**
     * Copy current matrix onto stack
     */
    void push_transforms();

    /**
     * Remove the current matrix from the stack and revert to prior
     * (or 0 if none are set at this node)
     */
    void pop_transforms();
};

} // namespace cg

#endif
