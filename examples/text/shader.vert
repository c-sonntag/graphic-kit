#version 330 core

layout( location = 0 ) in vec3 vertex_from_buffer;

uniform mat4 MVP;

void main()
{
  vec4 v = vec4( vertex_from_buffer, 1 );
  gl_Position = MVP * v;
};
