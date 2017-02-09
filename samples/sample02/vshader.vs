#version 120

attribute vec3 vertexPosition_modelspace;
attribute vec3 vertexColor;

varying vec3 fragment_color;

uniform mat4 mvp;

void main(void)
{
    gl_Position = mvp * vec4(vertexPosition_modelspace);
    fragment_color = vertexColor;
}
