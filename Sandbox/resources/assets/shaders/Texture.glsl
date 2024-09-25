#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexturePos;
layout(location = 3) in float a_TextureIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_vpMatrix;

out vec2 v_TexturePos;
out vec4 v_Color;
out float v_TextureIndex;
out float v_TilingFactor;

void main()
{
	v_Color = a_Color;
	v_TexturePos = a_TexturePos;
	v_TextureIndex = a_TextureIndex;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_vpMatrix * vec4(a_Position, 1.0f);
}


#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexturePos;
in vec4 v_Color;
in float v_TextureIndex;
in float v_TilingFactor;

uniform sampler2D u_textures[32];

void main()
{
	color = texture(u_textures[int(v_TextureIndex)], v_TexturePos * v_TilingFactor) * v_Color;
}
