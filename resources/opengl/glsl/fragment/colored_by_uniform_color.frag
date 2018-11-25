#version 330 core

//
uniform vec4 uniform_color = vec4( 1.f, 0.f, 0.f, 1.f );

//
out vec4 color;

//
void main()
{
  color = uniform_color;
}
