#version 330 core

layout (location = 0) in vec3 vert_coords;
layout (location = 1) in vec2 text_coords;

uniform mat4 model_matrix; // from local to world coords

out vec2 v_text_coords;

void main()
{
    gl_Position = model_matrix * vec4(vert_coords, 1.0f);

    v_text_coords = text_coords;
}

