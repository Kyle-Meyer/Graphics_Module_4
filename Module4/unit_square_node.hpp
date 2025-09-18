#ifndef __MODULE4_UNIT_SQUARE_NODE_HPP__
#define __MODULE4_UNIT_SQUARE_NODE_HPP__

#include "scene/geometry_node.hpp"
#include "geometry/types.hpp"

namespace cg
{

class UnitSquareNode : public GeometryNode
{

public:
  /**
   * Constructor - sets up the square geometry and OpenGL resources.
   */
  UnitSquareNode();

  /**
   * Destructor - cleans up OpenGL resources.
   */
  ~UnitSquareNode();

  /**
   * Draw method - renders the unit square using triangle strip.
   * @param scene_state Current scene state containing shader attribute locations.
   */
  void draw(SceneState &scene_state) override;

private:
  /**
   * Sets up the vertex data and OpenGL buffers for the unit square.
   */
  void setup_geometry();

  /**
   * Creates the 4 vertices for the triangle strip square.
   */
  void create_vertices();

  // OpenGL resources
  GLuint vbo_;                    // Vertex buffer object
  GLuint vao_;                    // Vertex array object
  
  // Vertex data
  static constexpr int NUM_VERTICES = 4;
  VertexAndNormal vertices_[NUM_VERTICES];
};

}


#endif //end of unit square node hpp def
