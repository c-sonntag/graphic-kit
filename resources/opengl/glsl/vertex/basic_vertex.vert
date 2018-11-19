#version 330 core

layout( location = 0 ) in vec3 vertex_from_buffer;

//
uniform mat4 uniform_mvp;

//
void main()
{
  gl_Position = uniform_mvp * vec4( vertex_from_buffer, 1.f );
};
