#version 330 core

layout( location = 0 ) in vec2 vertex_from_buffer;
layout( location = 1 ) in vec2 uv_from_buffer;

uniform mat4 uniform_mvp;
uniform vec2 uniform_model_decal;

out vec2 uv;

void main()
{
  uv = uv_from_buffer;
  vec4 v = vec4( vertex_from_buffer + uniform_model_decal, 0.f, 1.f );
  gl_Position = uniform_mvp * v;
};
