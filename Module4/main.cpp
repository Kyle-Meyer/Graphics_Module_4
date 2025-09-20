//============================================================================
//	Johns Hopkins University Whiting School of Engineering
//	605.667  Computer Graphics
//	Instructor:	Brian Russin
//
//	Author:  David W. Nesbitt
//	File:    Module4/main.cpp
//	Purpose: OpenGL and SDL program to draw a simple 3D animation with
//          collision detection. The scene contains moving spheres inside
//          a fixed, cube enclosure. A simple scene graph is used to define the
//          scene. Transform nodes, geometry nodes, and presentation nodes
//          are used.
//
//============================================================================

#include "filesystem_support/file_locator.hpp"
#include "geometry/geometry.hpp"
#include "scene/color4.hpp"
#include "scene/graphics.hpp"
#include "scene/scene.hpp"

#include "Module4/lighting_shader_node.hpp"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "Module4/unit_square_node.hpp"

namespace cg
{

// Simple logging function, should be defined in the cg namespace
void logmsg(const char *message, ...)
{
    // Open file if not already opened
    static FILE *lfile = NULL;
    if(lfile == NULL) { lfile = fopen("Module4.log", "w"); }

    va_list arg;
    va_start(arg, message);
    vfprintf(lfile, message, arg);
    putc('\n', lfile);
    fflush(lfile);
    va_end(arg);
}

} // namespace cg
// SDL Objects
SDL_Window       *g_sdl_window = nullptr;
SDL_GLContext     g_gl_context;
constexpr int32_t DRAWS_PER_SECOND = 30;
constexpr int32_t DRAW_INTERVAL_MILLIS =
    static_cast<int32_t>(1000.0 / static_cast<double>(DRAWS_PER_SECOND));

// Root of the scene graph
std::shared_ptr<cg::SceneNode> g_scene_root;

cg::SceneState g_scene_state;

// Sleep function to help run a reasonable timer
void sleep(int32_t milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

/**
 Display callback. Clears the prior scene and draws a new one.
 */
void display()
{
  // Clear the color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // Draw the scene if it exists
  if (g_scene_root)
      g_scene_root->draw(g_scene_state);
  
  // Swap buffers to display the rendered frame
  SDL_GL_SwapWindow(g_sdl_window);
}

/**
 * Keyboard event handler.
 */
bool handle_key_event(const SDL_Event &event)
{
    bool cont_program = true;

    switch(event.key.key)
    {
        case SDLK_ESCAPE: cont_program = false; break;
        default: break;
    }

    return cont_program;
}

/**
 * Reshape function. Load a 2-D orthographic projection matrix using the
 * window width and height so we can directly take window coordinates and
 * send to OpenGL
 * @param  width  Window width
 * @param  height Window height
 */
void reshape(int32_t width, int32_t height)
{
    int32_t size = std::min(width, height);
    int32_t x_offset = (width - size) / 2;
    int32_t y_offset = (height - size) / 2;
    
    glViewport(x_offset, y_offset, size, size);
}

/**
 * Window event handler.
 */
bool handle_window_event(const SDL_Event &event)
{
    bool cont_program = true;

    switch(event.type)
    {
        case SDL_EVENT_WINDOW_RESIZED:
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            reshape(event.window.data1, event.window.data2);
            break;
        default: break;
    }

    return cont_program;
}

/**
 * Handle Events function.
 */
bool handle_events()
{
    SDL_Event e;
    bool      cont_program = true;
    while(SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            case SDL_EVENT_QUIT:
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED: cont_program = false; break;

            case SDL_EVENT_WINDOW_RESIZED:
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: cont_program = handle_window_event(e); break;

            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP: cont_program = handle_key_event(e); break;
            default: break;
        }
    }
    return cont_program;
}

/**
 * Construct the complete scene with room and purple box
 */
void construct_scene()
{
    // Shader node (root of scene graph)
    auto shader = std::make_shared<cg::LightingShaderNode>();
    if(!shader->create("Module4/simple_light.vert", "Module4/simple_light.frag") ||
       !shader->get_locations())
    {
        exit(-1);
    }
    
    // Create a single unit square that we'll reuse for everything
    std::shared_ptr<cg::UnitSquareNode> unit_square = std::make_shared<cg::UnitSquareNode>();
    
    // === FLOOR ===
    // Transform: scale to 100x100, keep at Z=0
    auto floor_transform = std::make_shared<cg::TransformNode>();
    floor_transform->scale(100.0f, 100.0f, 1.0f);
    
    auto floor_color = std::make_shared<cg::ColorNode>(cg::Color4(0.6f, 0.5f, 0.2f, 1.0f)); // brownish-green
    
    // Hierarchy: floor_transform -> floor_color -> unit_square
    floor_transform->add_child(floor_color);
    floor_color->add_child(unit_square);
    
    // === LEFT WALL ===
    // Transform: translate to x=-50, rotate 90° around Y, scale to 100x100
    auto left_wall_transform = std::make_shared<cg::TransformNode>();
    left_wall_transform->translate(-50.0f, 0.0f, 50.0f);  // Position at left edge, center height
    left_wall_transform->rotate_y(90.0f);                 // Rotate to face right
    left_wall_transform->scale(100.0f, 100.0f, 1.0f);     // Scale to room size
    
    auto left_wall_color = std::make_shared<cg::ColorNode>(cg::Color4(1.0f, 1.0f, 1.0f, 1.0f)); // white
    
    left_wall_transform->add_child(left_wall_color);
    left_wall_color->add_child(unit_square);
    
    // === RIGHT WALL ===
    // Transform: translate to x=+50, rotate -90° around Y, scale to 100x100
    auto right_wall_transform = std::make_shared<cg::TransformNode>();
    right_wall_transform->translate(50.0f, 0.0f, 50.0f);   // Position at right edge, center height
    right_wall_transform->rotate_y(-90.0f);                // Rotate to face left
    right_wall_transform->scale(100.0f, 100.0f, 1.0f);     // Scale to room size
    
    auto right_wall_color = std::make_shared<cg::ColorNode>(cg::Color4(1.0f, 1.0f, 1.0f, 1.0f)); // white
    
    right_wall_transform->add_child(right_wall_color);
    right_wall_color->add_child(unit_square);
    
    // === BACK WALL ===
    // Transform: translate to y=+50, rotate 90° around X, scale to 100x100
    auto back_wall_transform = std::make_shared<cg::TransformNode>();
    back_wall_transform->translate(0.0f, 50.0f, 50.0f);    // Position at back, center height
    back_wall_transform->rotate_x(90.0f);                  // Rotate to face forward
    back_wall_transform->scale(100.0f, 100.0f, 1.0f);      // Scale to room size
    
    auto back_wall_color = std::make_shared<cg::ColorNode>(cg::Color4(0.9f, 0.7f, 0.5f, 1.0f)); // tan
    
    back_wall_transform->add_child(back_wall_color);
    back_wall_color->add_child(unit_square);
    
    // === CEILING ===
    // Transform: translate to z=+100, scale to 100x100
    auto ceiling_transform = std::make_shared<cg::TransformNode>();
    ceiling_transform->translate(0.0f, 0.0f, 100.0f);      // Position at top
    ceiling_transform->scale(100.0f, 100.0f, 1.0f);        // Scale to room size
    
    auto ceiling_color = std::make_shared<cg::ColorNode>(cg::Color4(0.1f, 0.4f, 1.0f, 1.0f)); // bluish
    
    ceiling_transform->add_child(ceiling_color);
    ceiling_color->add_child(unit_square);
    
    // === PURPLE BOX ===
    // Main box transform: position at (25, 25, 10), rotate 45° around Z
    auto box_main_transform = std::make_shared<cg::TransformNode>();
    box_main_transform->translate(25.0f, 25.0f, 10.0f);    // Position in back-right corner, 10 units up (half height)
    box_main_transform->rotate_z(45.0f);                   // Rotate 45 degrees
    
    // Purple color for all box faces
    auto purple_color = std::make_shared<cg::ColorNode>(cg::Color4(0.5f, 0.0f, 0.5f, 1.0f)); // purple
    
    // Box Face 1: Front (positive Y direction)
    auto box_face1_transform = std::make_shared<cg::TransformNode>();
    box_face1_transform->translate(0.0f, 10.0f, 0.0f);     // Move to front face
    box_face1_transform->rotate_x(90.0f);                  // Rotate to vertical
    box_face1_transform->scale(40.0f, 20.0f, 1.0f);        // Scale to box dimensions
    
    box_face1_transform->add_child(purple_color);
    purple_color->add_child(unit_square);
    
    // Box Face 2: Back (negative Y direction)
    auto box_face2_transform = std::make_shared<cg::TransformNode>();
    box_face2_transform->translate(0.0f, -10.0f, 0.0f);    // Move to back face
    box_face2_transform->rotate_x(90.0f);                  // Rotate to vertical - same as front face
    box_face2_transform->rotate_z(180.0f);                 // Flip to face outward
    box_face2_transform->scale(40.0f, 20.0f, 1.0f);        // Scale to box dimensions
    
    auto purple_color2 = std::make_shared<cg::ColorNode>(cg::Color4(0.5f, 0.0f, 0.5f, 1.0f));
    box_face2_transform->add_child(purple_color2);
    purple_color2->add_child(unit_square);
    
    // Box Face 3: Left (negative X direction)
    auto box_face3_transform = std::make_shared<cg::TransformNode>();
    box_face3_transform->translate(-20.0f, 0.0f, 0.0f);    // Move to left face
    box_face3_transform->rotate_y(-90.0f);                 // Rotate to vertical
    box_face3_transform->scale(20.0f, 20.0f, 1.0f);        // Scale to box dimensions
    
    auto purple_color3 = std::make_shared<cg::ColorNode>(cg::Color4(0.5f, 0.0f, 0.5f, 1.0f));
    box_face3_transform->add_child(purple_color3);
    purple_color3->add_child(unit_square);
    
    // Box Face 4: Right (positive X direction)
    auto box_face4_transform = std::make_shared<cg::TransformNode>();
    box_face4_transform->translate(20.0f, 0.0f, 0.0f);     // Move to right face
    box_face4_transform->rotate_y(90.0f);                  // Rotate to vertical
    box_face4_transform->scale(20.0f, 20.0f, 1.0f);        // Scale to box dimensions
    
    auto purple_color4 = std::make_shared<cg::ColorNode>(cg::Color4(0.5f, 0.0f, 0.5f, 1.0f));
    box_face4_transform->add_child(purple_color4);
    purple_color4->add_child(unit_square);
    
    // Box Face 5: Top (positive Z direction)
    auto box_face5_transform = std::make_shared<cg::TransformNode>();
    box_face5_transform->translate(0.0f, 0.0f, 10.0f);     // Move to top face
    box_face5_transform->scale(40.0f, 20.0f, 1.0f);        // Scale to box dimensions
    
    auto purple_color5 = std::make_shared<cg::ColorNode>(cg::Color4(0.5f, 0.0f, 0.5f, 1.0f));
    box_face5_transform->add_child(purple_color5);
    purple_color5->add_child(unit_square);
    
    // Box Face 6: Bottom (negative Z direction) - rests on floor
    auto box_face6_transform = std::make_shared<cg::TransformNode>();
    box_face6_transform->translate(0.0f, 0.0f, -10.0f);    // Move to bottom face
    box_face6_transform->scale(40.0f, 20.0f, 1.0f);        // Scale to box dimensions
    
    auto purple_color6 = std::make_shared<cg::ColorNode>(cg::Color4(0.5f, 0.0f, 0.5f, 1.0f));
    box_face6_transform->add_child(purple_color6);
    purple_color6->add_child(unit_square);
    
    // Add all box faces to the main box transform
    box_main_transform->add_child(box_face1_transform);
    box_main_transform->add_child(box_face2_transform);
    box_main_transform->add_child(box_face3_transform);
    box_main_transform->add_child(box_face4_transform);
    box_main_transform->add_child(box_face5_transform);
    box_main_transform->add_child(box_face6_transform);
    
    // === BUILD FINAL SCENE GRAPH ===
    // Add all room elements and box to the shader node
    shader->add_child(floor_transform);
    shader->add_child(left_wall_transform);
    shader->add_child(right_wall_transform);
    shader->add_child(back_wall_transform);
    shader->add_child(ceiling_transform);
    shader->add_child(box_main_transform);
    
    g_scene_root = shader;
    
    std::cout << "Complete room scene constructed:\n";
    g_scene_root->print_graph();
}

/**
 * Initialize SDL, create window, and set up OpenGL context
 * @return true if initialization successful, false otherwise
 */
bool initialize_graphics()
{
    // Initialize SDL video subsystem
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        std::cerr << "Failed to initialize SDL video subsystem: " << SDL_GetError() << std::endl;
        return false;
    }

    // Set OpenGL attributes for SDL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    // Create the window
    g_sdl_window = SDL_CreateWindow(
        "Computer Graphics Module 4 - [Your Name Here]",  // Replace with your name
        800, 800,                                          // width, height
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    
    if (!g_sdl_window) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // Set initial window position
    SDL_SetWindowPosition(g_sdl_window, 200, 200);

    // Create OpenGL context
    g_gl_context = SDL_GL_CreateContext(g_sdl_window);
    if (!g_gl_context) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(g_sdl_window);
        SDL_Quit();
        return false;
    }

    // Initialize OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);  // Counter-clockwise winding is front-facing
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Black background

    return true;
}

/**
 * Clean up graphics resources
 */
void cleanup_graphics()
{
    if (g_gl_context) {
        SDL_GL_DestroyContext(g_gl_context);
        g_gl_context = nullptr;
    }
    
    if (g_sdl_window) {
        SDL_DestroyWindow(g_sdl_window);
        g_sdl_window = nullptr;
    }
    
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}


/**
 * Main
 */
int main(int argc, char **argv)
{
    cg::set_root_paths(argv[0]);
    // Initialize SDL
    // Student to complete

    // Initialize display mode and window
    if (!initialize_graphics()) 
        return -1;

    // Initialize OpenGL
    // Student to complete

    std::cout << "OpenGL  " << glGetString(GL_VERSION) << ", GLSL "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

    // Set a fixed perspective projection. fov = 70, aspect = 1.0, near = 1.0 far = 200.
    // We are hard-coding viewing and projection matrices since they do not change in
    // this application.
    cg::Matrix4x4 projection;
    projection.m00() = 1.428f;
    projection.m11() = 1.428f;
    projection.m22() = -1.010f;
    projection.m23() = -2.010f;
    projection.m32() = -1.0f;
    projection.m33() = 0.0f;

    // Set a fixed camera outside the center of the front wall (imagine
    // it being a window) looking parallel to the floor
    cg::Matrix4x4 view;
    view.m00() = 1.0f;
    view.m11() = 0.0f;
    view.m12() = 1.0f;
    view.m13() = -50.0f;
    view.m21() = -1.0f;
    view.m22() = 0.0f;
    view.m23() = -90.0f;
    view.m33() = 1.0f;

    // Set the composite projection and viewing matrix
    // These remain fixed.
    g_scene_state.pv = projection * view;

    construct_scene();

    // Main loop
    while(handle_events())
    {
        display();
        sleep(DRAW_INTERVAL_MILLIS);
    }

    // Destroy OpenGL Context, SDL Window and SDL
    cleanup_graphics();

    return 0;
}
