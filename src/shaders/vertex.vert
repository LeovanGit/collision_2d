#version 330 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 text_coords;

out vec2 v_text_coords;

void main()
{
    gl_Position = vec4(vert_pos.x * 0.5625, vert_pos.y, vert_pos.z, 1.0);
    v_text_coords = text_coords;
}

