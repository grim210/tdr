#version 120

attribute vec3 vertexPosition_modelspace;
uniform mat4 MVP;

void main(void)
{
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);
}
