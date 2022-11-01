#ifndef CUBE_H
#define CUBE_H


#include <vector>
#include <QMatrix4x4>

const float LENGTH = 1.0f;


class Cube
{
public:
    Cube();
    Cube(const float length);

private:
    void _genVerticesAndIndices();

public:
    std::vector<float> vers;  // 顶点
    QMatrix4x4 mat_model;

private:
    float _length;
};

#endif // CUBE_H
