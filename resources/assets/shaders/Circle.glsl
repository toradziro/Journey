#type vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in float a_radius;
layout(location = 3) in float a_thikness;
layout(location = 4) in int a_entityId;

uniform mat4 u_vpMatrix;

out vec4 v_color;

flat out float v_radius;
flat out float v_thikness;
flat out int v_entityId;

void main()
{
	v_color = a_color;
	v_entityId = a_entityId;
	v_radius = a_radius;
	v_thikness = a_thikness;
	gl_Position = u_vpMatrix * vec4(a_position, 1.0f);
}


#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 colorDepth;
layout(location = 2) out int entityId;

in vec4 v_color;

flat in float v_radius;
flat in float v_thikness;
flat in int v_entityId;

void main()
{
	// calculate coordinates
	vec2 uv = gl_PointCoord * 2.0 - 1.0;

	// calculate dist from center
	float dist = length(uv);


	float outerRadius = v_radius;
	float innerRadius = v_radius - v_thikness;

	float mask = smoothstep(outerRadius, outerRadius - 0.01, dist) -
				smoothstep(innerRadius, innerRadius - 0.01, dist);

	if (mask <= 0.0)
		discard;

	color = vec4(v_color.rgb, v_color.a * mask);
	colorDepth = vec4(gl_FragCoord.z);
	entityId = v_entityId;
}
