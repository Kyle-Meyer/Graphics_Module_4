#include "Module3/drag_line_node.hpp"

#include "geometry/segment2.hpp"
#include "scene/scene.hpp"

#include <vector>

namespace cg
{

DragLineNode::DragLineNode(
    const Color4 &color1, const Color4 &color2, float w, int32_t position_loc, int32_t color_loc)
{
    width_ = w;
    capacity_ = 2;
    start_vertex_.color = color1;
    end_vertex_.color = color2;

    // Create vertex buffer object
    glGenBuffers(1, &vbo_);

    // Load dummy data into the vertex position VBO
    std::vector<LineSegment2> pts(capacity_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER,
                 capacity_ * 2 * sizeof(PositionAndColor),
                 (GLvoid *)&pts[0],
                 GL_DYNAMIC_DRAW);

    // Allocate a VAO, enable it and set the vertex attribute arrays and pointers
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // Enable vertex attribute array and pointer so they are bound to the VAO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(position_loc, 2, GL_FLOAT, GL_FALSE, sizeof(PositionAndColor), (void *)0);
    glEnableVertexAttribArray(position_loc);

    // Enable vertex attribute array and pointer so they are bound to the VAO
    glVertexAttribPointer(
        color_loc, 4, GL_FLOAT, GL_FALSE, sizeof(PositionAndColor), (void *)(sizeof(Point2)));
    glEnableVertexAttribArray(color_loc);

    // Make sure changes to this VAO are local
    glBindVertexArray(0);
}

void DragLineNode::set_width(float w) { width_ = w; }

void DragLineNode::clear() { draw_ = false; }

void DragLineNode::replace_point_0(const Point2 &pt)
{
    // Replace the first vertex and color in the VBO.
    start_vertex_.position = pt;
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(PositionAndColor), (GLvoid *)&start_vertex_);
    draw_ = false;
}

void DragLineNode::replace_point_1(const Point2 &pt)
{
    // Replace the first vertex and color in the VBO.
    end_vertex_.position = pt;
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferSubData(GL_ARRAY_BUFFER,
                    sizeof(PositionAndColor),
                    sizeof(PositionAndColor),
                    (GLvoid *)&end_vertex_);
    draw_ = true;
}

void DragLineNode::draw(SceneState &scene_state)
{
    if(draw_)
    {
        // Line width
        check_error("DragLineNode - before width");
        glLineWidth(width_);
        check_error("DragLineNode - after width");

        // Draw - the count in glDrawArrays is the number of vertices in the list
        glBindVertexArray(vao_);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
    }
}

} // namespace cg
