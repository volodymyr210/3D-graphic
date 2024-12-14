#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <glad/glad.h>

class Sphere {
public:
    Sphere(float radius, unsigned int sectorCount, unsigned int stackCount);

    unsigned int getVAO() const { return VAO; }
    unsigned int getVertexCount() const { return vertexCount; }

private:
    unsigned int VAO, VBO, EBO;
    unsigned int vertexCount;

    void generateSphere(float radius, unsigned int sectorCount, unsigned int stackCount);
};

#endif 

