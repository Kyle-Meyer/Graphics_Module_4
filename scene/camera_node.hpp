//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.667 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	Brian Russin
//
//	Author:	 David W. Nesbitt
//	File:    camera_node.hpp
//	Purpose: Provide support for setting the view and updating the view.
//
//============================================================================

#ifndef __SCENE_CAMERA_NODE_HPP__
#define __SCENE_CAMERA_NODE_HPP__

#include "scene/scene_node.hpp"

namespace cg
{

enum class ProjectionType
{
    PERSPECTIVE,
    ORTHOGRAPHIC
};

/**
 * Camera node class.
 */
class CameraNode : public SceneNode
{
  public:
    /**
     * Constructor.
     */
    CameraNode();
};

} // namespace cg

#endif
