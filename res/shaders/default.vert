#version 410 core
layout (location = 0) in vec2 aPos;

out vec3 color;

uniform vec3 colorUni;

void main()
{
  gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
  color = colorUni;
}
