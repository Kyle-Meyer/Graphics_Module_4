#include "Module3/color_blending_node.hpp"

namespace cg
{

ColorBlendingNode::ColorBlendingNode(const Color4 &c, bool b)
{
    color_ = c;
    blending_ = b;
}

void ColorBlendingNode::draw(SceneState &scene_state)
{
    // Enable blending
    if(blending_)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
    }

    // Set the current color and draw all children.
    glUniform4fv(scene_state.color_loc, 1, &color_.r);
    SceneNode::draw(scene_state);

    // Disable blending so it is not enabled except for children of this node
    if(blending_) { glDisable(GL_BLEND); }
}

} // namespace cg
