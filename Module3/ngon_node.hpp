//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.667 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	Brian Russin
//
//	Author:	David W. Nesbitt
//	File:    ngon_node.hpp
//	Purpose: Simple geometry node that draws an n-gon as a triangle fan.
//
//============================================================================

#ifndef __MODULE3_NGON_NODE_HPP__
#define __MODULE3_NGON_NODE_HPP__

#include "geometry/point2.hpp"
#include "scene/geometry_node.hpp"

#include <vector>

namespace cg
{

/**
 * Geometry node for an n-gon drawn as a triangle fan.
 */
class NGonNode : public GeometryNode
{
  public:
    /**
     * Constructor.
     * @param  center        Center of the n-gon
     * @param  num_sides     Number of sides.
     * @param  radius        Radius.
     * @param  position_loc  Shader location for vertex position.
     */
    NGonNode(const Point2 &center, int32_t num_sides, float radius, int32_t position_loc);

    /**
     * Destructor. Delete VBO and VAO.
     */
    ~NGonNode();

    /**
     * Draw the lines
     * @param  scene_state  Current scene state.
     */
    void draw(SceneState &scene_state) override;

    /**
     * Get the vertex list for intersection computations. Removes the
     * first vertex.
     * @return  Returns a const address of the vertex vector.
     */
    const std::vector<Point2> &get_vertex_list() const;

  protected:
    GLsizei             num_verts_;   // Number of vertices in the VBO.
    GLuint              vbo_;         // VBO
    GLuint              vao_;         // Vertex Array Object
    std::vector<Point2> vertex_list_; // Vertex list (excluding first vertex)
};

} // namespace cg

#endif
