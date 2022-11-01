#include <QDebug>
#include "torusrender.h"

// 不带纹理渲染
void TorusRender::initsize(float rBig, float rSmall, int nCol, int nRow)
{
    userTexture_ = false;
    program_.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex,":/shapes.vert");
    program_.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment,":/shapes.frag");
    program_.link();

    float angdegColSpan = 360.0/nCol;
    float angdegRowSpan = 360.0/nRow;
    float A = (rBig - rSmall)/2;
    float D = rSmall + A;
    // originVertPoints：原始顶点列表
    //
    QVector<GLfloat> originVertPoints,originColorPoints;
    for(float angdegCol=0;::ceil(angdegCol)<360+angdegColSpan;angdegCol+=angdegColSpan)	{//对小圆按照等角度间距循环
        double a=angdegCol * PI / 180;//当前小圆弧度
        for(float angdegRow=0;::ceil(angdegRow)<360+angdegRowSpan;angdegRow+=angdegRowSpan)
        {//对大圆按照等角度间距循环
            double u=angdegRow * PI / 180;//当前大圆弧度
            float y=(float) (A*::cos(a));//按照公式计算当前顶点
            float x=(float) ((D+A*::sin(a))*::sin(u));	//的X、Y、Z坐标
            float z=(float) ((D+A*::sin(a))*::cos(u));
            //将计算出来的X、Y、Z坐标放入原始顶点列表
            originVertPoints << x << y << z;
            originColorPoints << 0.4 << 1.0 << 1.0 << 1.0;
        }
    }

    for(int i=0;i<nCol-1;i++){//按照卷绕成三角形的需要
        for(int j=0;j<nRow;j++){//生成顶点编号列表
            int index=i*(nRow+1)+j;//当前四边形第一顶点编号
            vertPoints_ << originVertPoints.at((index+1)*3 + 0); //第一个三角形三个顶点的编号入列表
            vertPoints_ << originVertPoints.at((index+1)*3 + 1);
            vertPoints_ << originVertPoints.at((index+1)*3 + 2);
            vertPoints_ << originVertPoints.at((index+nRow+1)*3 + 0);
            vertPoints_ << originVertPoints.at((index+nRow+1)*3 + 1);
            vertPoints_ << originVertPoints.at((index+nRow+1)*3 + 2);
            vertPoints_ << originVertPoints.at((index+nRow+2)*3 + 0);
            vertPoints_ << originVertPoints.at((index+nRow+2)*3 + 1);
            vertPoints_ << originVertPoints.at((index+nRow+2)*3 + 2);

            textPoints_ << originColorPoints.at((index +1)*2 + 0); //第一个三角形颜色
            textPoints_ << originColorPoints.at((index +1)*2 + 1);
            textPoints_ << originColorPoints.at((index +1)*2 + 2);
            textPoints_ << originColorPoints.at((index +1)*2 + 3);
            textPoints_ << originColorPoints.at((index+nRow+1)*2 + 0);
            textPoints_ << originColorPoints.at((index+nRow+1)*2 + 1);
            textPoints_ << originColorPoints.at((index+nRow+1)*2 + 2);
            textPoints_ << originColorPoints.at((index+nRow+1)*2 + 3);
            textPoints_ << originColorPoints.at((index+nRow+2)*2 + 0);
            textPoints_ << originColorPoints.at((index+nRow+2)*2 + 1);
            textPoints_ << originColorPoints.at((index+nRow+2)*2 + 2);
            textPoints_ << originColorPoints.at((index+nRow+2)*2 + 3);

            vertPoints_ << originVertPoints.at((index+1)*3 + 0); //第二个三角形三个顶点的编号入列表
            vertPoints_ << originVertPoints.at((index+1)*3 + 1);
            vertPoints_ << originVertPoints.at((index+1)*3 + 2);
            vertPoints_ << originVertPoints.at(index*3 + 0);
            vertPoints_ << originVertPoints.at(index*3 + 1);
            vertPoints_ << originVertPoints.at(index*3 + 2);
            vertPoints_ << originVertPoints.at((index+nRow+1)*3 + 0);
            vertPoints_ << originVertPoints.at((index+nRow+1)*3 + 1);
            vertPoints_ << originVertPoints.at((index+nRow+1)*3 + 2);

            textPoints_ << originColorPoints.at((index+1)*2 + 0); //第二个三角形纹理坐标
            textPoints_ << originColorPoints.at((index+1)*2 + 1);
            textPoints_ << originColorPoints.at((index+1)*2 + 2);
            textPoints_ << originColorPoints.at((index+1)*2 + 3);
            textPoints_ << originColorPoints.at(index*2 + 0);
            textPoints_ << originColorPoints.at(index*2 + 1);
            textPoints_ << originColorPoints.at(index*2 + 2);
            textPoints_ << originColorPoints.at(index*2 + 3);
            textPoints_ << originColorPoints.at((index+nRow+1)*2 + 0);
            textPoints_ << originColorPoints.at((index+nRow+1)*2 + 1);
            textPoints_ << originColorPoints.at((index+nRow+1)*2 + 2);
            textPoints_ << originColorPoints.at((index+nRow+1)*2 + 3);
        }
    }

    QVector<GLfloat> vboVec;
    vboVec << vertPoints_ << textPoints_;
    vbo_.create();
    vbo_.bind();
    vbo_.allocate(vboVec.data(),vboVec.count() * sizeof(GLfloat));
}

// f - 好像是用来开启指定的功能的（）
void TorusRender::render(QOpenGLExtraFunctions *f, QMatrix4x4 &pMatrix, QMatrix4x4 &vMatrix, QMatrix4x4 &mMatrix)
{
    f->glEnable(GL_DEPTH_TEST); //开启深度测试
    // 如果启用，根据它们在窗口坐标中的缠绕来剔除多边形。看不出有什么用
    f->glEnable(GL_CULL_FACE);
    program_.bind();
    vbo_.bind();

    program_.setUniformValue("uPMatrix",pMatrix);
    program_.setUniformValue("uVMatrix",vMatrix);
    program_.setUniformValue("uMMatrix",mMatrix);

    program_.enableAttributeArray(0); //坐标
    program_.enableAttributeArray(1); //颜色
    program_.setAttributeBuffer(0,GL_FLOAT,0,3,3*sizeof(GLfloat));
    program_.setAttributeBuffer(1,GL_FLOAT,vertPoints_.count() * sizeof(GLfloat),4,4*sizeof(GLfloat));

    f->glDrawArrays(GL_LINE_STRIP, 0,vertPoints_.count() / 3);
    program_.disableAttributeArray(0);
    program_.disableAttributeArray(1);

    vbo_.release();
    program_.release();
    f->glDisable(GL_CULL_FACE);
    f->glDisable(GL_DEPTH_TEST);
}
