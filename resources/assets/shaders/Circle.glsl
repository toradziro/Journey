#type vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in float a_radius;
layout(location = 3) in float a_thikness;
layout(location = 4) in vec2 a_localPos;
layout(location = 5) in int a_entityId;

uniform mat4 u_vpMatrix;

out vec4 v_color;
out vec2 v_localPos;

flat out float v_radius;
flat out float v_thikness;
flat out int v_entityId;

void main()
{
	v_color = a_color;
	v_entityId = a_entityId;
	v_radius = a_radius;
	v_thikness = a_thikness;

	v_localPos = a_localPos;

	gl_Position = u_vpMatrix * vec4(a_position, 1.0f);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 colorDepth;
layout(location = 2) out int entityId;

in vec4 v_color;
in vec2 v_localPos;

flat in float v_radius;
flat in float v_thikness;
flat in int v_entityId;

void main()
{
	// Normalize local coordinates
	vec2 uv = v_localPos / v_radius;

	// How long away we from the center
	float dist = length(uv);
	
	float outerEdge = 1.0;
	float innerEdge = 1.0 - v_thikness;

	float mask = smoothstep(outerEdge, outerEdge - 0.01, dist) -
				 smoothstep(innerEdge, innerEdge - 0.01, dist);

	entityId = v_entityId;
	colorDepth = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0);
	if (mask < 1.0)
	{
		discard;
	}
	color = vec4(v_color.rgb, v_color.a * mask);
}
