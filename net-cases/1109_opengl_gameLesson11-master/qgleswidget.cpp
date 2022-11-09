#include "qgleswidget.h"
#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>



QGLESWIDGET::QGLESWIDGET(QWidget *parent) : QWidget(parent)
{

}

QGLESWIDGET::~QGLESWIDGET()
{
    destroyOpenGLES20();
}

bool QGLESWIDGET::init_QGW(std::vector<QString> fileName)
{
    if(!initOpenGLES20())
    {
        return false;
    }
    _shader.initialize();
    QString name = fileName[0];
    _sprit = loadTexture(name);

    glEnable ( GL_BLEND );
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE );

    srand ( 0 );
    memset(_particleData,0,sizeof(_particleData));
    for ( int i = 0; i < NUM_PARTICLES; i++ )//1000
    {
        float *particleData = &_particleData[i * PARTICLE_SIZE];

        // Lifetime of particle
        (*particleData++) = ( (float)(rand() % 10000) / 10000.0f );

        // End position of particle
        (*particleData++) = ( (float)(rand() % 10000) / 5000.0f ) - 1.0f;
        (*particleData++) = ( (float)(rand() % 10000) / 5000.0f ) - 1.0f;
        (*particleData++) = ( (float)(rand() % 10000) / 5000.0f ) - 1.0f;

        // Start position of particle
        (*particleData++) = ( (float)(rand() % 10000) / 40000.0f ) - 0.125f;
        (*particleData++) = ( (float)(rand() % 10000) / 40000.0f ) - 0.125f;
        (*particleData++) = ( (float)(rand() % 10000) / 40000.0f ) - 0.125f;

    }

}

bool QGLESWIDGET::initOpenGLES20()
{
    const EGLint attribs[] =
    {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE,24,
        EGL_NONE
    };
    EGLint 	format(0);
    EGLint	numConfigs(0);
    EGLint  major;
    EGLint  minor;

    //! 1
    _display	    =	eglGetDisplay(EGL_DEFAULT_DISPLAY);

    //! 2init
    eglInitialize(_display, &major, &minor);

    //! 3
    eglChooseConfig(_display, attribs, &_config, 1, &numConfigs);

    eglGetConfigAttrib(_display, _config, EGL_NATIVE_VISUAL_ID, &format);
    //!!! 4 使opengl与qt的窗口进行绑定<this->winId()>
    _surface	    = 	eglCreateWindowSurface(_display, _config, this->winId(), NULL);

    //! 5
    EGLint attr[]   =   { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
    _context 	    = 	eglCreateContext(_display, _config, 0, attr);
    //! 6
    if (eglMakeCurrent(_display, _surface, _surface, _context) == EGL_FALSE)
    {
        return false;
    }

    eglQuerySurface(_display, _surface, EGL_WIDTH,  &_width);
    eglQuerySurface(_display, _surface, EGL_HEIGHT, &_height);

    return  true;
}

void QGLESWIDGET::destroyOpenGLES20()
{
    if (_display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (_context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(_display, _context);
        }
        if (_surface != EGL_NO_SURFACE)
        {
            eglDestroySurface(_display, _surface);
        }
        eglTerminate(_display);
    }
    _display    =   EGL_NO_DISPLAY;
    _context    =   EGL_NO_CONTEXT;
    _surface    =   EGL_NO_SURFACE;//asdsafsaf
}

void QGLESWIDGET::render()
{
    //! 清空缓冲区
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    //! 视口，在Windows窗口指定的位置和大小上绘制OpenGL内容
    glViewport(0,0,_width,_height);

    struct Vertex
    {
        float x,y;
        float r,g,b,a;
    };

    float deltaTime = 0.016f;
    //! 创建一个投影矩阵
    //当图像显示出来与坐标的位置出现导致现象时，可以改变投影矩阵的坐标，使之对齐
    CELL::matrix4   screenProj  =   CELL::ortho<float>(0,float(_width),float(_height),0,-100.0f,100);
    _shader.begin();
    {
        _shader._timer += deltaTime;

         if (  _shader._timer >= 1.0f )
         {
             float centerPos[3];
             float color[4];

              _shader._timer = 0.0f;

             centerPos[0] = ( (float)(rand() % 10000) / 10000.0f ) - 0.5f;
             centerPos[1] = ( (float)(rand() % 10000) / 10000.0f ) - 0.5f;
             centerPos[2] = ( (float)(rand() % 10000) / 10000.0f ) - 0.5f;

             glUniform3fv ( _shader._centerPositionLoc, 1, &centerPos[0] );

             color[0] = ( (float)(rand() % 10000) / 20000.0f ) + 0.5f;
             color[1] = ( (float)(rand() % 10000) / 20000.0f ) + 0.5f;
             color[2] = ( (float)(rand() % 10000) / 20000.0f ) + 0.5f;
             color[3] = 0.5;

             glUniform4fv (  _shader._colorLoc, 1, &color[0] );
         }

         glUniform1f (  _shader._timeLoc, _shader._timer );

         glVertexAttribPointer ( _shader._lifetimeLoc, 1, GL_FLOAT,   GL_FALSE, PARTICLE_SIZE * sizeof(GLfloat), _particleData );

         glVertexAttribPointer ( _shader._endPositionLoc, 3, GL_FLOAT, GL_FALSE, PARTICLE_SIZE * sizeof(GLfloat),&_particleData[1] );

         glVertexAttribPointer ( _shader._startPositionLoc, 3, GL_FLOAT,GL_FALSE, PARTICLE_SIZE * sizeof(GLfloat),&_particleData[4] );


         glUniform1i ( _shader._samplerLoc, 0 );
         glBindTexture ( GL_TEXTURE_2D, _sprit);

         glDrawArrays( GL_POINTS, 0, NUM_PARTICLES );
    }
    _shader.end();
}

unsigned int QGLESWIDGET::loadTexture(QString fileName)
{
    unsigned int textureId = 0;

    //1 获取图片格式
    FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName.toLatin1().data(), 0);

    //2 加载图片
    FIBITMAP    *dib = FreeImage_Load(fifmt, fileName.toLatin1().data(),0);

    int         fmt =   GL_RGB;



    //4 获取数据指针
    BYTE    *pixels =   (BYTE*)FreeImage_GetBits(dib);

    int     width   =   FreeImage_GetWidth(dib);
    int     height  =   FreeImage_GetHeight(dib);


    if (fifmt == FIF_PNG)
    {
        dib     =   FreeImage_ConvertTo32Bits(dib);
        fmt     =   GL_RGBA;
        for (int i = 0 ;i < width * height * 4 ; i+=4 )
        {
            BYTE temp       =   pixels[i];
            pixels[i]       =   pixels[i + 2];
            pixels[i + 2]   =   temp;
        }
    }
    else
    {
        //3 转化为rgb 24色
        dib     =   FreeImage_ConvertTo24Bits(dib);
        for (int i = 0 ;i < width * height * 3 ; i+=3 )
        {
            BYTE temp       =   pixels[i];
            pixels[i]       =   pixels[i + 2];
            pixels[i + 2]   =   temp;
        }
    }


    /**
    *   产生一个纹理Id,可以认为是纹理句柄，后面的操作将书用这个纹理id
    */
    glGenTextures( 1, &textureId );

    /**
    *   使用这个纹理id,或者叫绑定(关联)
    */
    glBindTexture( GL_TEXTURE_2D, textureId );
    /**
    *   指定纹理的放大,缩小滤波，使用线性方式，即当图片放大的时候插值方式
    */
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    /**
    *   将图片的rgb数据上传给opengl.
    */
    glTexImage2D(
        GL_TEXTURE_2D,      //! 指定是二维图片
        0,                  //! 指定为第一级别，纹理可以做mipmap,即lod,离近的就采用级别大的，远则使用较小的纹理
        fmt,                //! 纹理的使用的存储格式
        width,              //! 宽度，老一点的显卡，不支持不规则的纹理，即宽度和高度不是2^n。
        height,             //! 宽度，老一点的显卡，不支持不规则的纹理，即宽度和高度不是2^n。
        0,                  //! 是否的边
        fmt,                //! 数据的格式，bmp中，windows,操作系统中存储的数据是bgr格式
        GL_UNSIGNED_BYTE,   //! 数据是8bit数据
        pixels
        );
    /**
    *   释放内存
    */
    FreeImage_Unload(dib);

    return textureId;
}

void QGLESWIDGET::drawImage()
{
    render();
    eglSwapBuffers(_display,_surface);
}
