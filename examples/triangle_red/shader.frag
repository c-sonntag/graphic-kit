#version 330 core

out vec3 color;

uniform vec3 uniform_color;

void main()
{
  //color = vec3(1,0,0);
  //color = vec3(1,1,0);
  color = uniform_color;
};
