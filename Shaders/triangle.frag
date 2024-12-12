#version 450

layout (location = 0) out vec4 outColor;

layout (location = 0) in vec3 color;
layout (location = 1) in vec2 UV;

layout (binding = 1) uniform sampler2D tex;

void main()
{
    outColor = vec4(texture(tex, 2.0 * UV).rgb * color, 1.0);
}