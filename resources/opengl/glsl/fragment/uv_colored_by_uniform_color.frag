#version 330 core

//
uniform vec4 uniform_color = vec4( 1.f, 0.f, 0.f, 1.f );

//
in vec2 uv;
out vec4 color;

//
void main()
{
  //color = length( uv + 0.5f ) * uniform_color;
  color = smoothstep( 0.2, 1.f, 1.0 - length( uv  - 0.5 ) ) * uniform_color;
}
