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
#include "scene/graphics.hpp"
#include "scene/scene.hpp"

#include "Module4/lighting_shader_node.hpp"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

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
  if (g_scene_root) {
      g_scene_root->draw(g_scene_state);
  }
  
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
    // Student TODO - complete this so the viewport remains square and centered in
    // the window
    glViewport(0, 0, width, height);
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
 * Construct the scene
 */
void construct_scene()
{
    // Shader node
    auto shader = std::make_shared<cg::LightingShaderNode>();
    if(!shader->create("Module4/simple_light.vert", "Module4/simple_light.frag") ||
       !shader->get_locations())
    {
        exit(-1);
    }

    // Student to complete
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
