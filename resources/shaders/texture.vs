#version 400 

in vec3 inputPosition;
in vec2 inputTexCoord;

out vec2 texCoord;

uniform mat4 transMatrix;

void main(void)
{
  vec4 pos = vec4(inputPosition, 1.0f) * transMatrix;

  gl_Position = vec4(pos.x, pos.y,pos.z, 1.0f);

  texCoord = inputTexCoord;
}
