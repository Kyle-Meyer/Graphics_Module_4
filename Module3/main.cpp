//============================================================================
//	Johns Hopkins University Whiting School of Engineering
//	605.667  Computer Graphics
//	Instructor:	Brian Russin
//
//	Author:  David W. Nesbitt
//	File:    Module3/main.cpp
//	Purpose: OpenGL shader program to draw points or lines based on mouse
//           clicks.
//
//============================================================================

#include "filesystem_support/file_locator.hpp"
#include "geometry/geometry.hpp"
#include "scene/graphics.hpp"
#include "scene/scene.hpp"

#include "Module3/color_blending_node.hpp"
#include "Module3/drag_line_node.hpp"
#include "Module3/line_shader_node.hpp"
#include "Module3/ngon_node.hpp"
#include "Module3/ngon_shader_node.hpp"
#include "Module3/point_node.hpp"
#include "Module3/point_shader_node.hpp"
#include "Module3/shader_src.hpp"

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
    if(lfile == NULL) { lfile = fopen("Module3.log", "w"); }

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

// Scene state
cg::SceneState g_scene_state;

// NGons
std::shared_ptr<cg::NGonNode> g_octagon;
std::shared_ptr<cg::NGonNode> g_circle;
std::shared_ptr<cg::NGonNode> g_hexagon;

// Draggable line
std::shared_ptr<cg::DragLineNode> g_drag_line;
float                             g_drag_line_width = 1.0f;

// Are we drawing a line with the mouse? This global flag allows us to
// follow mouse motion events and draw if the left button is down.
bool g_draw_line = false;

// Current line being drawn (while dragging the mouse)
cg::LineSegment2 g_current_line;

// Intersection points
std::shared_ptr<cg::PointNode> g_intersection_points;

// Inverse transformation from screen window coordinates (mouse event positions)
// into world coordinates.
cg::Matrix4x4 g_inverse;

// Sleep function to help run a reasonable timer
void sleep(int32_t milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

/**
 * Reshape callback. Load a 2-D orthographic projection matrix. Use a world
 * window with width or height of 10 units along the smallest of the screen
 * window dimensions. Set a viewport to draw into the entire window.
 * Note that this callback will be called when the window is opened.
 * @param  width  Window width
 * @param  height Window height
 */
void reshape(int32_t width, int32_t height)
{
    // Set world window boundaries so the world window has dimensions of 10
    // in the smallest screen window dimension. Match the other dimension to
    // the screen window aspect.
    float w = static_cast<float>(width);
    float h = static_cast<float>(height);
    float aspect = w / h;
    float wl, wr, wt, wb;
    if(width < height)
    {
        wl = -5.0f;
        wr = 5.0f;
        wb = -5.0f / aspect;
        wt = 5.0f / aspect;
    }
    else
    {
        wl = -5.0f * aspect;
        wr = 5.0f * aspect;
        wb = -5.0f;
        wt = 5.0f;
    }

    // Create an orthographic transformation to transform the world window
    // into clip coordinates with vl = -1, vr = 1, vt = 1, and vb = -1.
    // Set as a 4x4 matrix with the scaling factors A,B and translation C,D
    g_scene_state.ortho_matrix.m00() = 2.0f / (wr - wl); // A
    g_scene_state.ortho_matrix.m03() = -(wr + wl) / (wr - wl);
    g_scene_state.ortho_matrix.m11() = 2.0f / (wt - wb);
    g_scene_state.ortho_matrix.m13() = -(wt + wb) / (wt - wb);
    printf("matrix m13 = %f\n", g_scene_state.ortho_matrix.m13());
    g_scene_state.ortho_matrix.log("ortho");
    // Set the Inverse transformation from screen coordinates into world coordinates
    g_inverse.m00() = (wr - wl) / w;              // A
    g_inverse.m03() = wl;                         // C
    g_inverse.m11() = (wt - wb) / -h;             // B
    g_inverse.m13() = wb - (g_inverse.m11() * h); // D

    // Update the viewport
    glViewport(0, 0, width, height);
}

/**
 * Display callback function
 */
void display(void)
{
    // Clear the framebuffer
    glClear(GL_COLOR_BUFFER_BIT);

    g_scene_root->draw(g_scene_state);
    cg::check_error("After Draw");

    // Swap buffers
    SDL_GL_SwapWindow(g_sdl_window);
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
 * Keyboard event handler.
 */
bool handle_key_event(const SDL_Event &event)
{
    // bool key_down = event.type == SDL_KEYDOWN; // unused in this module
    bool cont_program = true;
    bool upper_case = event.key.mod & SDL_KMOD_SHIFT || event.key.mod & SDL_KMOD_CAPS;

    switch(event.key.key)
    {
        case SDLK_ESCAPE: cont_program = false; break;
        case SDLK_M:
            if(upper_case) glEnable(GL_MULTISAMPLE);
            else glDisable(GL_MULTISAMPLE);
            display();
            break;
        default: break;
    }

    return cont_program;
}

/**
 * Mouse button handler (called when a mouse button state changes). Starts a
 * new draggable line when the left button is down. When left button up
 * the line is cleared.
 */
void handle_mouse_event(const SDL_Event &event)
{
    if(event.button.button != 1) return;

    // On a button down event add a point as the start of a line
    if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        float x_pos = static_cast<float>(event.button.x);
        float y_pos = static_cast<float>(event.button.y);

        // Set the current line endpoints (both) to the mouse position and
        // set flag indicating we are dragging a line
        cg::HPoint3 world = g_inverse * cg::Point3(x_pos, y_pos, 0.0f);
        g_draw_line = true;
        g_current_line.a.set(world.x, world.y);
        g_current_line.b.set(world.x, world.y);
        g_drag_line->replace_point_0(g_current_line.a);
    }
    else if(event.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        // No longer dragging a line - clear it
        g_draw_line = false;
        g_drag_line->clear();
        g_intersection_points->clear();

        // Force a redisplay to clear the line
        display();
    }
}

// Test if 2 points are equal within tolerance
bool equal(const cg::Point2 &p0, const cg::Point2 &p1)
{
    return (std::abs(p1.x - p0.x) < cg::EPSILON && std::abs(p1.y - p0.y) < cg::EPSILON);
}

/**
 * Mouse motion callback (called when mouse button is depressed)
 */
void handle_mouse_motion_event(const SDL_Event &event)
{
    // Set the current line endpoints to the mouse position
    if(g_draw_line)
    {
        // Update the current line VBO
        cg::HPoint3 world = g_inverse * cg::Point3(event.motion.x, event.motion.y, 0.0f);
        g_current_line.b.set(world.x, world.y);
        g_drag_line->replace_point_1(g_current_line.b);

        // Compute intersection points with n-gons
        std::vector<cg::Point2> int_pts;

        auto clip_result = g_current_line.clip_to_polygon(g_hexagon->get_vertex_list());
        if(clip_result.clipped)
        {
            if(!equal(clip_result.clip_segment.a, g_current_line.a))
            {
                int_pts.push_back(clip_result.clip_segment.a);
            }
            if(!equal(clip_result.clip_segment.b, g_current_line.b))
            {
                int_pts.push_back(clip_result.clip_segment.b);
            }
        }

        clip_result = g_current_line.clip_to_polygon(g_circle->get_vertex_list());
        if(clip_result.clipped)
        {
            if(!equal(clip_result.clip_segment.a, g_current_line.a))
            {
                int_pts.push_back(clip_result.clip_segment.a);
            }
            if(!equal(clip_result.clip_segment.b, g_current_line.b))
            {
                int_pts.push_back(clip_result.clip_segment.b);
            }
        }

        clip_result = g_current_line.clip_to_polygon(g_octagon->get_vertex_list());
        if(clip_result.clipped)
        {
            if(!equal(clip_result.clip_segment.a, g_current_line.a))
            {
                int_pts.push_back(clip_result.clip_segment.a);
            }
            if(!equal(clip_result.clip_segment.b, g_current_line.b))
            {
                int_pts.push_back(clip_result.clip_segment.b);
            }
        }
        g_intersection_points->update(int_pts);
    }
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

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            case SDL_EVENT_MOUSE_BUTTON_UP: handle_mouse_event(e); break;

            case SDL_EVENT_MOUSE_MOTION: handle_mouse_motion_event(e); break;

            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP: cont_program = handle_key_event(e); break;
            default: break;
        }
    }
    return cont_program;
}

/**
 * Create the scene.
 */
void create_scene()
{
    // Shader for n-gons
    auto ngon_shader = std::make_shared<cg::NGonShaderNode>();
    if(!ngon_shader->create("Module3/ngon.vert", "Module3/ngon.frag") ||
       !ngon_shader->get_locations())
    // if(!ngon_shader->create_from_source(ngon_vert, ngon_frag) || !ngon_shader->get_locations())
    {
        exit(-1);
    }

    // Shader for draggable smooth shaded line
    auto line_shader = std::make_shared<cg::LineShaderNode>();
    if(!line_shader->create("Module3/shaded_line.vert", "Module3/shaded_line.frag") ||
       !line_shader->get_locations())
    // if(!line_shader->create_from_source(shaded_line_vert, shaded_line_frag) ||
    //    !line_shader->get_locations())
    {
        exit(-1);
    }

    // Shader for intersection points
    auto point_shader = std::make_shared<cg::PointShaderNode>();
    if(!point_shader->create("Module3/points.vert", "Module3/points.frag") ||
       !point_shader->get_locations())
    // if(!point_shader->create_from_source(points_vert, points_frag) ||
    // !point_shader->get_locations())
    {
        exit(-1);
    }

    // Create an octagon centered at (2.5, 2.5) with radius = 2
    g_octagon = std::make_shared<cg::NGonNode>(
        cg::Point2(2.5f, 2.5f), 8, 2.0f, ngon_shader->get_position_loc());

    // Create a circle approximation with 72 sides centered at (0,0)
    // with radius 4.5
    g_circle = std::make_shared<cg::NGonNode>(
        cg::Point2(0.0f, 0.0f), 72, 4.5f, ngon_shader->get_position_loc());

    // Create a hexagon centered at (-2,-2) with radius = 3
    g_hexagon = std::make_shared<cg::NGonNode>(
        cg::Point2(-2.0f, -2.0f), 6, 3.0f, ngon_shader->get_position_loc());

    // Circle is red and not blended - it is the "background" color and is
    // drawn first so the other 2 filled objects blend with its color
    auto circle_color =
        std::make_shared<cg::ColorBlendingNode>(cg::Color4(0.75f, 0.0f, 0.0f, 1.0f), false);

    // Octagon is green and 50% opaque
    auto octagon_color =
        std::make_shared<cg::ColorBlendingNode>(cg::Color4(0.0f, 1.0f, 0.0f, 0.5f), true);

    // Hexagon is blue and 25% opaque (75 % transparent)
    auto hexagon_color =
        std::make_shared<cg::ColorBlendingNode>(cg::Color4(0.0f, 0.0f, 1.0f, 0.25f), true);

    // Create a node for managing current line. Use a width = 4 for current lines
    cg::Color4 drag_color0(0.8f, 0.1f, 0.1f, 1.0f); // Color at the start of the line
    cg::Color4 drag_color1(0.1f, 0.8f, 0.1f, 1.0f); // Color at the end of the line
    g_drag_line = std::make_shared<cg::DragLineNode>(drag_color0,
                                                     drag_color1,
                                                     g_drag_line_width,
                                                     line_shader->get_position_loc(),
                                                     line_shader->get_color_loc());

    // Intersection points - capacity = 6 (max. intersections with n-gons)
    g_intersection_points = std::make_shared<cg::PointNode>(6, point_shader->get_position_loc());

    // Create scene graph
    g_scene_root = std::make_shared<cg::SceneNode>();

    // Add n-gons with appropriate color/presentation nodes
    g_scene_root->add_child(ngon_shader);
    ngon_shader->add_child(circle_color);
    circle_color->add_child(g_circle);
    ngon_shader->add_child(octagon_color);
    octagon_color->add_child(g_octagon);
    ngon_shader->add_child(hexagon_color);
    hexagon_color->add_child(g_hexagon);

    // Add draggable line
    g_scene_root->add_child(line_shader);
    line_shader->add_child(g_drag_line);

    // Add intersection points
    g_scene_root->add_child(point_shader);
    point_shader->add_child(g_intersection_points);
}

/**
 * Main - entry point for GetStarted GLUT application.
 */
int main(int argc, char **argv)
{
    cg::set_root_paths(argv[0]);
    std::cout << "Keyboard Controls:\n";
    std::cout << "M : Enable MSAA    m : Disable MSAA\n";
    std::cout << "ESC - Exit program\n";

    // Initialize SDL
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Error initializing SDL: " << SDL_GetError() << '\n';
        exit(1);
    }

    // Initialize display mode and window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_PropertiesID props = SDL_CreateProperties();
    if(props == 0)
    {
        std::cout << "Error creating SDL Window Properties: " << SDL_GetError() << '\n';
        exit(1);
    }

    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "Module3 by Brian Russin");
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN, true);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, 800);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, 800);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, 200);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, 200);

    g_sdl_window = SDL_CreateWindowWithProperties(props);
    if(g_sdl_window == nullptr)
    {
        std::cout << "Error initializing SDL Window" << SDL_GetError() << '\n';
        exit(1);
    }

    // Initialize OpenGL
    g_gl_context = SDL_GL_CreateContext(g_sdl_window);

    if(g_gl_context == nullptr)
    {
        std::cout << "Error Getting OpenGL Context: " << SDL_GetError() << '\n';
        exit(1);
    }

    std::cout << "OpenGL  " << glGetString(GL_VERSION) << ", GLSL "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

// Initialize GLEW (if using Windows)
#if BUILD_WINDOWS
    int32_t glew_init_result = glewInit();
    if(GLEW_OK != glew_init_result)
    {
        std::cout << "GLEW Error: " << glewGetErrorString(glew_init_result) << '\n';
        exit(EXIT_FAILURE);
    }
#endif

    float aliased_line_width_range[2];
    float point_size_range[2];

    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, aliased_line_width_range);
    glGetFloatv(GL_POINT_SIZE_RANGE, point_size_range);

    std::cout << "GL_ALIASED_LINE_WIDTH_RANGE: " << aliased_line_width_range[0] << ", "
              << aliased_line_width_range[1] << '\n';
    std::cout << "GL_POINT_SIZE_RANGE: " << point_size_range[0] << ", " << point_size_range[1]
              << '\n';

    // Set the clear color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    if(aliased_line_width_range[1] < 4.0f) g_drag_line_width = aliased_line_width_range[1];
    else g_drag_line_width = 4.0f;

    glPointSize(8.0f);
    glLineWidth(g_drag_line_width);

    // Setting so gl_PointCoord has origin at lower left.
    glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);

#if defined(GL_POINT_SPRITE)
    // NOTE: Some windowing systems contain a bug that requires the following line of code to
    // execute BEFORE vertex shaders will populate 'gl_PointCoord'
    glEnable(GL_POINT_SPRITE);
#endif

    // Create the scene
    create_scene();
    cg::check_error("create_scene");

    reshape(800, 800);

    display();

    // Main loop
    while(handle_events())
    {
        display();
        sleep(DRAW_INTERVAL_MILLIS);
    }

    // Destroy OpenGL Context, SDL Window and SDL
    SDL_GL_DestroyContext(g_gl_context);
    SDL_DestroyWindow(g_sdl_window);
    SDL_Quit();

    return 0;
}
