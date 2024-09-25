#include "texture.h"
#include "stb_image.h"

Texture2D::Texture2D()
    : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
    glGenTextures(1, &this->ID);
}

void Texture2D::Generate(std::string path, bool alpha)
{
    if(alpha){
        this->Internal_Format = GL_RGBA;
        this->Image_Format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, this->ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannel;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannel, 0);

    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->ID);
}


