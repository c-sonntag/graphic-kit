#version 330 core

uniform sampler2D sampler_texture;

in vec2 uv;

out vec4 color;

void main()
{
  color = texture( sampler_texture, uv );
};
