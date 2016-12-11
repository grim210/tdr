#version 120

attribute vec3 vertexPosition_modelspace;
attribute vec3 vertexColor;

varying vec3 fragmentColor;
uniform mat4 MVP;

void main(void)
{
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
    fragmentColor = vertexColor;
}
