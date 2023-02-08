#ifndef IONA_DESKTOP_CORE_DISPLAY_L2D_TEXTURE_H
#define IONA_DESKTOP_CORE_DISPLAY_L2D_TEXTURE_H

#include <string>
#include "gl_handle.h"
#include <Type/csmVector.hpp>

namespace IonaDesktop {
namespace CoreDisplay {
    class L2dTexManager
    {
    public:
        struct TextureInfo
            {
                GLuint id;
                int width;
                int height;
                std::string fileName;
            };
        L2dTexManager();
        ~L2dTexManager();

        inline unsigned int Premultiply
            (unsigned char red, unsigned char green,
             unsigned char blue, unsigned char alpha)
        {
            return static_cast<unsigned>(\
                (red * (alpha + 1) >> 8) | \
                ((green * (alpha + 1) >> 8) << 8) | \
                ((blue * (alpha + 1) >> 8) << 16) | \
                (((alpha)) << 24)   \
                );
        }

        TextureInfo* CreateTextureFromPngFile(std::string fileName);
        void ReleaseTextures();
        void ReleaseTexture(Csm::csmUint32 textureId);
        void ReleaseTexture(std::string fileName);
        TextureInfo* GetTextureInfoById(GLuint textureId) const;
    private:
        Csm::csmVector<TextureInfo*> _textures;
    };
}
}

#endif // IONA_DESKTOP_CORE_DISPLAY_L2D_TEXTURE_H
