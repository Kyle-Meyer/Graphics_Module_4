//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.667 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	Brian Russin
//
//	Author:	 David W. Nesbitt
//	File:    color_blending_node.hpp
//	Purpose: Simple presentation node that defines a color and blending.
//
//============================================================================

#ifndef __MODULE3_COLOR_BLENDING_NODE_HPP__
#define __MODULE3_COLOR_BLENDING_NODE_HPP__

#include "scene/color4.hpp"
#include "scene/presentation_node.hpp"

namespace cg
{

/**
 * Color presentation node with blending.
 */
class ColorBlendingNode : public PresentationNode
{
  public:
    /**
     * Constructor given a red, green, blue, and alpha value.
     * @param  c  Color including alpha.
     * @param  b  Blending enabled (true or false).
     */
    ColorBlendingNode(const Color4 &c, bool b);

    /**
     * Draw this presentation node and its children
     */
    void draw(SceneState &scene_state) override;

  protected:
    bool   blending_;
    Color4 color_;
};

} // namespace cg

#endif
