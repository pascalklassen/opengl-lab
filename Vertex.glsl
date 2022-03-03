#version 330 core
        
in vec3 i_Position;
in vec3 i_Color;
in vec2 i_TexCoord;

out vec3 p_Color;
out vec2 p_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform vec3 u_OverrideColor;
    
void main()
{
    p_Color = u_OverrideColor * i_Color;
    p_TexCoord = i_TexCoord;
    gl_Position = u_Proj * u_View * u_Model * vec4(i_Position, 1.0);
}
