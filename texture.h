#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>

class Texture {
public:
    unsigned int ID;

    Texture(const std::string& path);

    void bind(unsigned int unit = 0) const;
};

#endif
