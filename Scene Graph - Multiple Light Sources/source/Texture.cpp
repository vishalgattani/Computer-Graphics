#include "../include/Texture.h"
#include "../include/stb_image.h"

Texture::Texture(){
    height = 0;
    width = 0;
    localBuffer = nullptr;
    BPP = 0;
    RID = 0;
}

void Texture::create(const string &path){
    texturePath = path;
    stbi_set_flip_vertically_on_load(1);
    localBuffer = stbi_load(path.c_str(), &width, &height, &BPP, 4);

    glGenTextures(1, &RID);
    glBindTexture(GL_TEXTURE_2D, RID);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if(localBuffer)
    {
        stbi_image_free(localBuffer);
    }
}

void Texture::bind(unsigned int slot){
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, RID);
}

void Texture::unbind(){
    glBindTexture(GL_TEXTURE_2D, 0);
}