//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.667 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	Brian Russin
//
//	Author:  David W. Nesbitt
//	File:    presentation_node.hpp
//	Purpose: Scene graph presentation node.
//
//============================================================================

#ifndef __SCENE_PRESENTATION_NODE_HPP__
#define __SCENE_PRESENTATION_NODE_HPP__

#include "scene/scene_node.hpp"

namespace cg
{

/**
 * Presentation node. Holds material properties.
 */
class PresentationNode : public SceneNode
{
  public:
    /**
     * Constructor
     */
    PresentationNode();

    /**
     * Draw. Sets the material properties.
     * @param  scene_state  Scene state (holds material uniform locations)
     */
    void draw(SceneState &scene_state) override;
};

} // namespace cg

#endif
