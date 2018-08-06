#version 330 core

layout( location = 0 ) in vec2 vertex_from_buffer;

uniform mat4 MVP;

void main()
{
  vec4 v = vec4( vertex_from_buffer, 0.f, 1.f );
  gl_Position = MVP * v;
};
