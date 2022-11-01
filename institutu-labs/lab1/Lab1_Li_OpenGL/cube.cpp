#include "cube.hpp"

Cube::Cube()
{
    _length = LENGTH;
    _genVerticesAndIndices();
}


Cube::Cube(const float length) :
    _length(length)
{
    _genVerticesAndIndices();
}


void Cube::_genVerticesAndIndices()
{
    float r = _length / 2;
    vers = {
                -r, -r, -r,
                 r, -r, -r,
                 r,  r, -r,
                 r,  r, -r,
                -r,  r, -r,
                -r, -r, -r,

                -r, -r,  r,
                 r, -r,  r,
                 r,  r,  r,
                 r,  r,  r,
                -r,  r,  r,
                -r, -r,  r,

                -r,  r,  r,
                -r,  r, -r,
                -r, -r, -r,
                -r, -r, -r,
                -r, -r,  r,
                -r,  r,  r,

                 r,  r,  r,
                 r,  r, -r,
                 r, -r, -r,
                 r, -r, -r,
                 r, -r,  r,
                 r,  r,  r,

                -r, -r, -r,
                 r, -r, -r,
                 r, -r,  r,
                 r, -r,  r,
                -r, -r,  r,
                -r, -r, -r,

                -r,  r, -r,
                 r,  r, -r,
                 r,  r,  r,
                 r,  r,  r,
                -r,  r,  r,
                -r,  r, -r
                       };
}

