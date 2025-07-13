#version 410 core

// Position
layout (location = 0) in vec3 aPos;

// Colors
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform float scale;

void main()
{
  gl_Position = vec4(aPos.x + aPos.y * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0)
  color = aColor;
}
