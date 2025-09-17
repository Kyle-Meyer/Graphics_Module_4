//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.667 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	Brian Russin
//
//	Author:	David W. Nesbitt
//	File:    point_node.hpp
//	Purpose:	Simple geometry node that draws points.
//
//============================================================================

#ifndef __MODULE3_POINT_NODE_HPP__
#define __MODULE3_POINT_NODE_HPP__

#include "geometry/point2.hpp"
#include "scene/geometry_node.hpp"

#include <vector>

namespace cg
{

/**
 * Geometry node for adding and drawing points.
 */
class PointNode : public GeometryNode
{
  public:
    /**
     * Constructor.
     * @param  capacity      Maximum number of vertices in the VBO.
     * @param  position_loc  Shader vertex position attribute location.
     */
    PointNode(int32_t capacity, int32_t position_loc);

    /**
     * Destructor
     */
    ~PointNode();

    /**
     * Adds vertices to the list. Reloads the point VBO.
     * @param  vtx_list  Point vertex positions.
     */
    void update(const std::vector<Point2> &vtx_list);

    /**
     * Clears the vertices (sets vertex_count to 0).
     */
    void clear();

    /**
     * Draw the set of points.
     * @param  scene_state  Current scene state.
     */
    void draw(SceneState &scene_state) override;

  protected:
    GLsizei vertex_count_; // Current number of vertices
    int32_t pos_loc_;      // Shader vertex position attribute.
    GLuint  vbo_;          // VBO
    GLuint  vao_;          // Vertex Array Object
};

} // namespace cg

#endif
