#version 400 

in vec3 inputPosition;

void main(void)
{
  gl_Position = vec4(inputPosition.x, inputPosition.y, inputPosition.z, 1.0f);
}
