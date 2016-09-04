#ifndef FLOATING_EYE_PRIMITIVES_H
#define FLOATING_EYE_PRIMITIVES_H

#include "IrrIMGUI/IrrIMGUI.h"

irr::scene::SMeshBuffer* createCircle(irr::u32 verts, irr::f32 radius, const irr::video::SColor& color);
irr::video::ITexture* createColorTexture(irr::video::IVideoDriver* driver, const irr::video::SColor& color);

#endif // FLOATING_EYE_PRIMITIVES_H
