#include "Module3/point_node.hpp"

namespace cg
{

PointNode::PointNode(int32_t capacity, int32_t position_loc)
{
    // Set up a buffer for dynamic draw with the specified capacity.
    std::vector<Point2> pts(capacity);
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(Point2), (GLvoid *)&pts[0], GL_DYNAMIC_DRAW);

    // Allocate a VAO, enable it and set the vertex attribute arrays and pointers
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(position_loc, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(position_loc);

    // Initial vertex count is 0
    vertex_count_ = 0;
}

PointNode::~PointNode()
{
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
}

void PointNode::update(const std::vector<Point2> &vtx_list)
{
    // Update the vertex buffer object.
    if(vtx_list.size() > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferSubData(
            GL_ARRAY_BUFFER, 0, vtx_list.size() * sizeof(Point2), (GLvoid *)&vtx_list[0]);
    }
    vertex_count_ = static_cast<GLsizei>(vtx_list.size());
}

void PointNode::clear() { vertex_count_ = 0; }

void PointNode::draw(SceneState &scene_state)
{
    if(vertex_count_ > 0)
    {
        // Bind the VAO and draw the points
        glBindVertexArray(vao_);
        glDrawArrays(GL_POINTS, 0, vertex_count_);
        glBindVertexArray(0);
    }
}

} // namespace cg
