#version 330 core

in vec2 p_TexCoord;

out vec4 color;

uniform sampler2D u_Container;
uniform sampler2D u_Face;
        
void main()
{
    color = mix(texture(u_Container, p_TexCoord), texture(u_Face, p_TexCoord), 0.2);
}
