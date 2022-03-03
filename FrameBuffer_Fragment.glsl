#version 330  core

in vec2 p_TexCoord;

out vec4 color;

uniform sampler2D u_FrameBuffer;

void main()
{
    vec4 top         = texture(u_FrameBuffer, vec2(p_TexCoord.x, p_TexCoord.y + 1.0 / 200.0));
    vec4 bottom      = texture(u_FrameBuffer, vec2(p_TexCoord.x, p_TexCoord.y - 1.0 / 200.0));
    vec4 left        = texture(u_FrameBuffer, vec2(p_TexCoord.x - 1.0 / 300.0, p_TexCoord.y));
    vec4 right       = texture(u_FrameBuffer, vec2(p_TexCoord.x + 1.0 / 300.0, p_TexCoord.y));
    vec4 topLeft     = texture(u_FrameBuffer, vec2(p_TexCoord.x - 1.0 / 300.0, p_TexCoord.y + 1.0 / 200.0));
    vec4 topRight    = texture(u_FrameBuffer, vec2(p_TexCoord.x + 1.0 / 300.0, p_TexCoord.y + 1.0 / 200.0));
    vec4 bottomLeft  = texture(u_FrameBuffer, vec2(p_TexCoord.x - 1.0 / 300.0, p_TexCoord.y - 1.0 / 200.0));
    vec4 bottomRight = texture(u_FrameBuffer, vec2(p_TexCoord.x + 1.0 / 300.0, p_TexCoord.y - 1.0 / 200.0));
    vec4 sx = -topLeft - 2 * left - bottomLeft + topRight   + 2 * right  + bottomRight;
    vec4 sy = -topLeft - 2 * top  - topRight   + bottomLeft + 2 * bottom + bottomRight;
    vec4 sobel = sqrt(sx * sx + sy * sy);
    color = sobel;
}
