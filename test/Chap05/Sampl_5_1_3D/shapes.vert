#version 330 core
layout(location = 0) in vec3 aPos;
// 3- 纹理
layout(location = 1) in vec2 aTexCord;
out vec2 TexCord;
// 5 - 坐标变换
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    TexCord=aTexCord;
}
