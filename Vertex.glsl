#version 330 core
        
in vec3 i_Position;
in vec2 i_TexCoord;

out vec2 p_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
    
void main()
{
    gl_Position = u_Proj * u_View * u_Model * vec4(i_Position, 1.0);
    p_TexCoord = i_TexCoord;
}
