#version 330 core

in vec3 p_Color;
in vec2 p_TexCoord;

out vec4 color;

uniform sampler2D u_Kitten;
uniform sampler2D u_Puppy;
        
void main()
{
    vec4 colKitten = texture(u_Kitten, p_TexCoord);
    vec4 colPuppy = texture(u_Puppy, p_TexCoord);
    color = vec4(p_Color, 1.0) * mix(colKitten, colPuppy, 0.5);
}
