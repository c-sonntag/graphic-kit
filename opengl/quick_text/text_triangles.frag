#version 330 core

uniform sampler2D text_sampler;

in vec2 text_uv;

out vec4 color;

void main()
{
  color = texture( text_sampler, text_uv );
}
