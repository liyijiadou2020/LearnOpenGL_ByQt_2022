#ifndef CONE_H
#define CONE_H

#include <vector>
#include <QMatrix4x4>


class Cone
{
public:
    Cone()=default;
    Cone(const float r, const float height, const float step_angle);
    unsigned int getNumTriangles();

    std::vector<float> vers;  // 顶点
    std::vector<int> inds; // EBO 索引s
    QMatrix4x4 mat_model;    

private:
    float _r;       // 半径
    float _height;  // 高度
    float _step_angle;  // 底的角度间隔
    void _genVerticesAndIndices();
};

#endif // CONE_H
