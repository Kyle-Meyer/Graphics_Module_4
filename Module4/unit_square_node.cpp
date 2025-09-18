#include "Module4/unit_square_node.hpp"
#include "scene/graphics.hpp"

#include <GL/glext.h>
#include <iostream>

namespace cg 
{

UnitSquareNode::UnitSquareNode() : vbo_(0), vao_(0)
{
  create_vertices();
  setup_geometry();
}

UnitSquareNode::~UnitSquareNode()
{
  // Clean up OpenGL resources
  if (vbo_ != 0)
      glDeleteBuffers(1, &vbo_);
  if (vao_ != 0)
      glDeleteVertexArrays(1, &vao_);
}

void UnitSquareNode::create_vertices()
{
  //create the unit square vertices for tirangle strip 
  //in order: bottom left, bottom right, top left, top right 

  std::cout << "sizeof(VertexAndNormal): " << sizeof(VertexAndNormal) << std::endl;
  std::cout << "offsetof vertex: " << offsetof(VertexAndNormal, vertex) << std::endl;
  std::cout << "offsetof normal: " << offsetof(VertexAndNormal, normal) << std::endl;
  std::cout << "sizeof(Point3): " << sizeof(Point3) << std::endl;
  std::cout << "sizeof(Vector3): " << sizeof(Vector3) << std::endl;
  //all normals point in +Z direction 
  Vector3 normal(0.0f, 0.0f, 1.0f);

  //bottom left 
  vertices_[0].vertex = Point3(-0.5f, -0.5f, 0.0f);
  vertices_[0].normal = normal;

  //bottom right 
  vertices_[1].vertex = Point3(0.5f, -0.5f, 0.0f);
  vertices_[1].normal = normal;

  //top left 
  vertices_[2].vertex = Point3(-0.5f, 0.5f, 0.0f);
  vertices_[2].normal = normal;

  //top right 
  vertices_[3].vertex = Point3(0.5f, 0.5f, 0.0f);
  vertices_[3].normal = normal;
}

/*
void UnitSquareNode::setup_geometry()
{
  //bind vao 
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  //bind vbo 
  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);

  glBufferData(GL_ARRAY_BUFFER, 
               NUM_VERTICES * sizeof(VertexAndNormal), 
               vertices_, 
               GL_STATIC_DRAW);

  //unbind vbo and vao 
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
*/ 

void UnitSquareNode::setup_geometry()
{
    std::cout << "Setting up geometry..." << std::endl;
    
    // Generate and bind VAO
    glGenVertexArrays(1, &vao_);
    std::cout << "Generated VAO: " << vao_ << std::endl;
    if (vao_ == 0) {
        std::cout << "ERROR: Failed to generate VAO!" << std::endl;
        return;
    }
    glBindVertexArray(vao_);
    
    // Check for errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL error after VAO setup: " << error << std::endl;
    }
    
    // Generate and bind VBO
    glGenBuffers(1, &vbo_);
    std::cout << "Generated VBO: " << vbo_ << std::endl;
    if (vbo_ == 0) {
        std::cout << "ERROR: Failed to generate VBO!" << std::endl;
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    
    // Upload vertex data to VBO
    std::cout << "Uploading " << NUM_VERTICES * sizeof(VertexAndNormal) << " bytes" << std::endl;
    glBufferData(GL_ARRAY_BUFFER, 
                 NUM_VERTICES * sizeof(VertexAndNormal), 
                 vertices_, 
                 GL_STATIC_DRAW);
    
    error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL error after buffer data: " << error << std::endl;
    }
    
    // Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    std::cout << "Geometry setup complete!" << std::endl;
}

void UnitSquareNode::draw(SceneState &scene_state)
{

    std::cout << "Matrix uniform locations:" << std::endl;
    std::cout << "  pvm_matrix_loc: " << scene_state.pvm_matrix_loc << std::endl;
    std::cout << "  model_matrix_loc: " << scene_state.model_matrix_loc << std::endl;
    std::cout << "  normal_matrix_loc: " << scene_state.normal_matrix_loc << std::endl;
    std::cout << "  material_diffuse_loc: " << scene_state.material_diffuse_loc << std::endl;
    
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL error before draw: " << error << std::endl;
    }
  //rebind VBO and VAO 
  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);

  if(scene_state.position_loc >= 0)
  {
    glVertexAttribPointer(scene_state.position_loc, 
                          3, 
                          GL_FLOAT, 
                          GL_FALSE, 
                          sizeof(VertexAndNormal), 
                          (void*)offsetof(VertexAndNormal, vertex));
    glEnableVertexAttribArray(scene_state.position_loc);
  }

  // Set up normal attribute (normal.x, normal.y, normal.z)
  if (scene_state.normal_loc >= 0)
  {
    glVertexAttribPointer(scene_state.normal_loc,
                          3, GL_FLOAT, GL_FALSE, 
                          sizeof(VertexAndNormal),
                          (void*)offsetof(VertexAndNormal, normal));
    glEnableVertexAttribArray(scene_state.normal_loc);
  }
  //draw the triangle strip
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NUM_VERTICES);

  //cleanup 
  if(scene_state.position_loc >= 0)
    glDisableVertexAttribArray(scene_state.position_loc);
  if(scene_state.normal_loc >= 0)
    glDisableVertexAttribArray(scene_state.normal_loc);

  //unbind buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

    error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL error after draw: " << error << std::endl;
    }

  GeometryNode::draw(scene_state);
}


}
