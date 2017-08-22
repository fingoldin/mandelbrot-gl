#version 410 core

layout (location = 0) in vec2 position;

out vec2 pos;

void main()
{
    pos = position;

	gl_Position = vec4(position, 0.0f, 1.0f);
}
