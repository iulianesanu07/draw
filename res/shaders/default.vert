#version 410 core

// Position
layout (location = 0) in vec3 aPos;

// Colors
layout (location = 1) in vec3 aColor;

// Texture
layout (location = 2) in vec2 aTex;

out vec3 color;

out vec2 texCoord;

uniform float scale;

void main()
{
  gl_Position = vec4(aPos * scale, 1.0);
  color = aColor;
  texCoord = aTex;
}
