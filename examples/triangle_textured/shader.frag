#version 330 core

uniform sampler2D texture;

in vec2 uv;

out vec3 color;

void main()
{
  color = texture( texture, uv ).rgb;
};
