#version 330 core

uniform sampler2D text_sampler;

uniform vec3 text_color;

in vec2 text_uv;

out vec4 color;

void main()
{
  //color = texture( text_sampler, text_uv );

  float brightness = texture( text_sampler, text_uv ).r;

  if ( brightness == 0.f )
    brightness = 1.f;

  color.a = brightness;
  color.rgb = brightness * text_color;



}
