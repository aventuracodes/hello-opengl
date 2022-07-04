#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCord;

out vec3 vecColor;
out vec2 TexCord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vecColor = aColor;
    TexCord = aTexCord;
}