#version 330 core

// Input vertex data, different for all executions of this shader.
layout( location = 0 ) in vec2 vertex_from_buffer;
layout( location = 1 ) in vec2 uv_from_buffer;

//
uniform mat4 vp_matrix;
uniform mat4 text_model;

// Output data ; will be interpolated for each fragment.
out vec2 text_uv;

void main()
{
  //
  text_uv = uv_from_buffer;

  //
  vec2 vertex_font_size_correction = vertex_from_buffer / vec2( 400, 300 ); // scaling font size
  gl_Position = ( vp_matrix * text_model ) * vec4( vertex_font_size_correction, 0.f, 1.f );
}

