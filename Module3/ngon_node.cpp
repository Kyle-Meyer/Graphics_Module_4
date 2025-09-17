#include "Module3/ngon_node.hpp"

#include "geometry/geometry.hpp"
#include "scene/scene.hpp"

#include <cmath>

namespace cg
{

NGonNode::NGonNode(const Point2 &center, int32_t num_sides, float radius, int32_t position_loc)
{
    // Create the vertex list. The first vertex is the center, then rotate
    // find points on a circle of specified radius to create the specified
    // number of sides;
    vertex_list_.push_back(center);
    float angle = 0.0f;
    float da = (2.0f * PI) / static_cast<float>(num_sides);
    for(int i = 0; i < num_sides; i++, angle += da)
    {
        vertex_list_.push_back(
            {center.x + (radius * std::cos(angle)), center.y + (radius * std::sin(angle))});
    }

    // Close the shape
    vertex_list_.push_back({center.x + radius, center.y});

    // Add the points to a VBO.
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER,
                 vertex_list_.size() * sizeof(Point2),
                 (GLvoid *)&vertex_list_[0],
                 GL_STATIC_DRAW);
    num_verts_ = static_cast<GLsizei>(vertex_list_.size());

    // VAO
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // Remvoe the first vertex and last vertex in the list (for use in
    // intersection computations with the outer boundary of the ngon
    vertex_list_.erase(vertex_list_.begin());
    vertex_list_.pop_back();
}

NGonNode::~NGonNode()
{
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
}

void NGonNode::draw(SceneState &scene_state)
{
    // Bind the VAO and draw the line
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, num_verts_);
    glBindVertexArray(0);
    check_error("End of n-gon:");
}

const std::vector<Point2> &NGonNode::get_vertex_list() const { return vertex_list_; }

} // namespace cg
