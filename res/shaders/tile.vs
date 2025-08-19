#version 330 core
attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec4 vertexColor;
attribute vec3 vertexNormal;

out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 normal;

uniform mat4 mvp;
uniform mat4 modelmat;
void main() {
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;
    gl_Position = mvp * vec4(vertexPosition, 1.0);
    normal = mat3(modelmat) * vertexNormal;
}