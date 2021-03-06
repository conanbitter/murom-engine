#include "textures.hpp"
#include "app.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace mrm {
Texture::Texture() : width{0}, height{0} {
    gl::GenTextures(1, &handle);
    gl::ActiveTexture(gl::TEXTURE0);

    gl::BindTexture(gl::TEXTURE_2D, handle);

    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
}

Texture::~Texture() {
    if (gl::IsTexture(handle)) {
        gl::DeleteTextures(1, &handle);
    }
}

void Texture::loadFromMemory(int width, int height, const void* data) {
    bind();
    gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGBA, width, height, 0, gl::RGBA, gl::UNSIGNED_BYTE, data);
    this->width = width;
    this->height = height;
}

void Texture::loadFromFile(string filename) {
    int w, h, c;
    stbi_set_flip_vertically_on_load(1);
    void* data = stbi_load(filename.c_str(), &w, &h, &c, 4);
    if (data == nullptr) {
        throw AppException("STB", "Error loading texture", string(stbi_failure_reason()));
    }

    loadFromMemory(w, h, data);

    stbi_image_free(data);
}

void Texture::bind() {
    gl::BindTexture(gl::TEXTURE_2D, handle);
}

Texture::Texture(Texture&& tex) : handle{tex.handle}, width{tex.width}, height{tex.height} {
    tex.width = 0;
    tex.height = 0;
}

Texture& Texture::operator=(Texture&& tex) {
    if (gl::IsTexture(handle)) {
        gl::DeleteTextures(1, &handle);
    }
    handle = tex.handle;
    width = tex.width;
    height = tex.height;
    tex.width = 0;
    tex.height = 0;
    return *this;
}

}  // namespace mrm