#version 330 core
in vec3 somePos;
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f*abs(somePos.y), 0.5f*length(somePos), 0.2f*abs(somePos.x), 1.0f);
}
