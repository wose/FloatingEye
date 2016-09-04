#include "Primitives.h"

#include "IrrIMGUI/IrrIMGUI.h"
#include "IrrIMGUI/IncludeIrrlicht.h"

irr::scene::SMeshBuffer* createCircle(irr::u32 verts, irr::f32 radius, const irr::video::SColor& color)
{
   if (verts < 3)
      verts = 3;
   else if (65534 < verts)
      verts = 65534;

   irr::scene::SMeshBuffer* mb = new irr::scene::SMeshBuffer;
   if (!mb)
      return 0;

   irr::video::S3DVertex vertex;
   vertex.Normal.set(0, 1, 0);
   vertex.Color = color;

   // origin vertex
   vertex.Pos.set(0, 0, 0);
   vertex.TCoords.set(.5f, .5f);
   //mb->Vertices.push_back(vertex);
   //mb->Indices.push_back(0);

   for (irr::s32 n = verts; n >= 0; --n)
   {
      const irr::f32 x = (irr::core::PI * 2.f) * n / verts;

      const irr::f32 dx = sinf(x);
      const irr::f32 dz = cosf(x);

      // vertices are always in the xz plane
      vertex.Pos.set(dx * radius, 0, dz * radius);

      // scale tcoords from -1,1 to 0,1
      vertex.TCoords.set((dx + 1.f) * .5f, (dz + 1.f) * .5f);

      mb->Vertices.push_back(vertex);
      mb->Indices.push_back(n);
   }

   return mb;
}

irr::video::ITexture* createColorTexture(irr::video::IVideoDriver* driver, const irr::video::SColor& color)
{
    auto texture = driver->addTexture(irr::core::dimension2d<irr::u32>(1,1),
                                      "wireFrameColor",
                                      irr::video::ECF_A8R8G8B8);
    irr::s32* p = (irr::s32*)texture->lock();
    p[0] = color.color;
    texture->unlock();

    return texture;
}
