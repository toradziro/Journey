#type vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in int a_entityId;

uniform mat4 u_vpMatrix;

out vec4 v_color;
flat out int v_entityId;

void main()
{
	v_color = a_color;
	v_entityId = a_entityId;

	gl_Position = u_vpMatrix * vec4(a_position, 1.0f);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 colorDepth;
layout(location = 2) out int entityId;

in vec4 v_color;
flat in int v_entityId;

void main()
{
	entityId = v_entityId;
	colorDepth = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0);
	color = v_color;
}
