#include "texture.h"
#include "deps/stb_image.h"

bool Texture::try_create(const char *filename) {
    int width;
    int height;
    int channels;
    void *data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data) {
        return false;
    }

    sg_pixel_format pixel_format;
    switch (channels) {
    case 1:
        pixel_format = SG_PIXELFORMAT_R8;
        break;
    case 4:
        pixel_format = SG_PIXELFORMAT_RGBA8;
        break;
    default:
        return false;
    }

    sg_image_desc desc{};
    desc.width = width;
    desc.height = height;
    desc.pixel_format = pixel_format;
    desc.data.subimage[0][0].ptr = data;
    desc.data.subimage[0][0].size = width * height * channels;

    m_image = sg_make_image(desc);
    m_width = width;
    m_height = height;

    stbi_image_free(data);
    return true;
}

void Texture::destroy() { sg_destroy_image(m_image); }

