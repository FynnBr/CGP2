#version 410 core
/*
layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
*/

layout(location = 0) in vec3 vColor;
// output written to the screen is always location 0
layout(location = 0) out vec4 fragColor;

void main() {
    // write interplated color to fragment, set alpha = 1.0
    fragColor = vec4(vColor, 1.0);
}
