#version 330 core
in vec3 vecColor;
in vec2 TexCord; 

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(ourTexture, TexCord) * vec4(vecColor, 1.0);
}   
