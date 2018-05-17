#version 330 core

uniform sampler2D text_sampler;

uniform vec3 text_color;

in vec2 text_uv;

out vec4 color;

void main()
{
  float brightness = texture( text_sampler, text_uv ).r;

  //if ( color.r == 0.f )
  //  color.a = 0.f;
  //else if ( color.rgb != vec3( 1.f, 1.f, 1.f ) )
  //  color.a = 1.f - ( color.r * 0.1f );
  // else


  color.a = brightness;
  color.rgb = brightness * text_color;

}
