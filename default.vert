#version 410 core
/*
layout(location = 0) in vec2 inPos;

void main() {
    gl_Position = vec4(inPos, 0.0, 1.0)
}*/

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;
layout(location = 0) out vec3 vColor;

void main() {
    // pass color to fragment shader
    vColor = aColor;
    // define vertex position in space.
    // must be 4-dimensional, so supplement Z and W values.
    gl_Position = vec4(aPosition, 0.0, 1.0);
}

