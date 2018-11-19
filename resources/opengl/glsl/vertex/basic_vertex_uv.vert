#version 330 core

layout( location = 0 ) in vec3 vertex_from_buffer;
layout( location = 1 ) in vec2 uv_from_buffer;

//
uniform mat4 uniform_mvp;

//
out vec2 uv;

//
void main()
{
  //
  uv = uv_from_buffer;

  //
  gl_Position = uniform_mvp * vec4( vertex_from_buffer, 1.f );
};
