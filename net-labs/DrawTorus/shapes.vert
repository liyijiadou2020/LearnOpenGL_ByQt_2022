#version 330
uniform mat4 uPMatrix,camMatrix,uMMatrix;
layout (location = 0) in vec3 aPosition;
smooth out vec3 vPosition; //将顶点坐标传递给片元着色器

void main(void)
{
    gl_Position = uPMatrix * camMatrix *uMMatrix * vec4(aPosition,1);
    vPosition = aPosition;
}
