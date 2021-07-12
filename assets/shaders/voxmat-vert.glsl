#version 330 core

uniform vec4 u_entity_data[6];
uniform vec4 u_scene_data[6];

layout(location=0) in vec3 a_position;
layout(location=1) in vec3 a_normal;
layout(location=2) in vec3 a_offset;

out vec3 vtx_position;
out vec3 vtx_normal;

mat4 Translation(vec3 v)
{
    mat4 ret = mat4(1.0);
    ret[3][0] = v.x;
    ret[3][1] = v.y;
    ret[3][2] = v.z;
    return ret;
}

void main()
{
    mat4 model = mat4(u_entity_data[0], u_entity_data[1],
                        u_entity_data[2], u_entity_data[3]);
    mat4 mmodel = mat4(1.0);
    mat4 vp = mat4(u_scene_data[0], u_scene_data[1],
                    u_scene_data[2], u_scene_data[3]);

    //vtx_position = vec3(Translation(a_offset) * vec4(a_position, 1.0));
    //vtx_position = vec3(mmodel * vec4(a_position, 1.0));
    vtx_position = a_position + a_offset;
    vtx_normal = a_normal;

    // TODO: use position
    gl_Position = vp * vec4(vtx_position, 1.0);
}
