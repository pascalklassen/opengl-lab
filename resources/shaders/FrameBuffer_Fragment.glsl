#version 330  core

in vec2 p_TexCoord;

out vec4 color;

uniform sampler2D u_FrameBuffer;

void main()
{
    color = texture(u_FrameBuffer, p_TexCoord);
}
