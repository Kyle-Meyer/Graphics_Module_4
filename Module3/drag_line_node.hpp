//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.667 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	Brian Russin
//
//	Author:	David W. Nesbitt
//	File:   drag_line_node.hpp
//	Purpose:	Simple geometry node that draws a draggable line.
//
//============================================================================

#ifndef __MODULE3_DRAG_LINE_NODE_HPP__
#define __MODULE3_DRAG_LINE_NODE_HPP__

#include "geometry/point2.hpp"
#include "scene/color4.hpp"
#include "scene/geometry_node.hpp"

namespace cg
{

struct PositionAndColor
{
    Point2 position;
    Color4 color;
};

/**
 * Draggable line node.
 */
class DragLineNode : public GeometryNode
{
  public:
    /**
     * Constructor.
     * @param  color1        Color at the start of the line
     * @param  color2        Color at the end of the line
     * @param  w             Line width
     * @param  position_loc  Location of vertex position attribute
     * @param  color_loc     Location of vertex color attribute
     */
    DragLineNode(const Color4 &color1,
                 const Color4 &color2,
                 float         w,
                 int32_t       position_loc,
                 int32_t       color_loc);

    /**
     * Set the line width when drawing the VBO.
     * @param  w  Line width.
     */
    void set_width(float w);

    /**
     * Clears the draggable line (sets draw to false).
     */
    void clear();

    /**
     * Replace the starting vertex of the draggable line.
     * @param pt  Position to set as start of the line segment
     */
    void replace_point_0(const Point2 &pt);

    /**
     * Replace the starting vertex of the draggable line.
     * @param pt  Position to set as start of the line segment
     */
    void replace_point_1(const Point2 &pt);

    /**
     * Draw the lines
     * @param  scene_state  Current scene state.
     */
    void draw(SceneState &scene_state) override;

  protected:
    bool             draw_;
    GLuint           vao_;          // Vertex Array Object
    GLuint           vbo_;          // VBO for positions
    float            width_;        // Line width
    uint32_t         capacity_;     // Maximum number of lines
    PositionAndColor start_vertex_; // Start vertex (position and color)
    PositionAndColor end_vertex_;   // End vertex (position and color)
};

} // namespace cg

#endif
