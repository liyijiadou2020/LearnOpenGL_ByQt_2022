#ifndef BALL_H
#define BALL_H

#include <vector>
#include <QMatrix4x4>


class Ball
{
public:
    Ball()=default;
    Ball(const unsigned int x_segments, const unsigned int y_segments);
    unsigned int getNumTriangles();

    std::vector<float> vers;  // 顶点
    std::vector<int> inds; // EBO 索引s
    QMatrix4x4 mat_model;
private:
    void _genVerticesAndIndices();

    unsigned int _x_segments;
    unsigned int _y_segments;
};

#endif // BALL_H
