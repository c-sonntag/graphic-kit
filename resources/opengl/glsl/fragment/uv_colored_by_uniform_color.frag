#version 330 core

//
uniform vec4 uniform_color = vec4( 1.f, 0.f, 0.f, 1.f );

//
in vec2 uv2d;
out vec4 color;

//
void main()
{
  //color = length( uv2d + 0.5f ) * uniform_color;
  color = smoothstep( 0.2, 1.f, 1.0 - length( uv2d  - 0.5 ) ) * uniform_color;
}
