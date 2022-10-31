#include "ball.h"

Ball::Ball(const unsigned int x_segments, const unsigned int y_segments) :
    _x_segments(x_segments),
    _y_segments(y_segments)
{
    _genVerticesAndIndices();
}


/* 获取绘制三角形的数量 */
unsigned int Ball::getNumTriangles()
{
    return _x_segments * _y_segments * 6;
}

void Ball::_genVerticesAndIndices()
{
    /*2-计算球体顶点*/
    //生成球的顶点
    for (int y = 0; y <= _y_segments; y++)
    {
        for (int x = 0; x <= _x_segments; x++)
        {
            float xSegment = (float)x / (float)_x_segments;
            float ySegment = (float)y / (float)_y_segments;
            float xPos = cos(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);
            float yPos = cos(ySegment * M_PI);
            float zPos = sin(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);
            vers.push_back(xPos);
            vers.push_back(yPos);
            vers.push_back(zPos);
        }
    }

    //生成球的inds
    for (int i = 0; i < _y_segments; i++)
    {
        for (int j = 0; j < _x_segments; j++)
        {
            inds.push_back(i * (_x_segments + 1) + j);
            inds.push_back((i + 1) * (_x_segments + 1) + j);
            inds.push_back((i + 1) * (_x_segments + 1) + j+1);
            inds.push_back(i* (_x_segments + 1) + j);
            inds.push_back((i + 1) * (_x_segments + 1) + j + 1);
            inds.push_back(i * (_x_segments + 1) + j + 1);
        }
    }
}

