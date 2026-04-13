#version 400

in vec3 sv_position;
in vec3 sv_color;

out vec4 color;

void main()
{
    color = vec4(sv_color, 1.0);
    gl_Position = vec4(sv_position, 1.0);
}