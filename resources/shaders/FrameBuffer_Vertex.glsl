#version 330 core

in vec2 i_Position;
in vec2 i_TexCoord;

out vec2 p_TexCoord;

void main()
{
    p_TexCoord = i_TexCoord;
    gl_Position = vec4(i_Position, 0.0, 1.0);
}
