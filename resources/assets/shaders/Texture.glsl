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

flat out float v_TextureIndex;
flat out float v_TilingFactor;

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
layout(location = 1) out vec4 colorTst;
layout(location = 2) out vec4 colorTst1;

in vec2 v_TexturePos;
in vec4 v_Color;

flat in float v_TextureIndex;
flat in float v_TilingFactor;

uniform sampler2D u_textures[32];

void main()
{
	vec4 texColor = v_Color;

	switch(int(v_TextureIndex))
	{
		case 0: texColor *= texture(u_textures[0], v_TexturePos * v_TilingFactor); break;
		case 1: texColor *= texture(u_textures[1], v_TexturePos * v_TilingFactor); break;
		case 2: texColor *= texture(u_textures[2], v_TexturePos * v_TilingFactor); break;
		case 3: texColor *= texture(u_textures[3], v_TexturePos * v_TilingFactor); break;
		case 4: texColor *= texture(u_textures[4], v_TexturePos * v_TilingFactor); break;
		case 5: texColor *= texture(u_textures[5], v_TexturePos * v_TilingFactor); break;
		case 6: texColor *= texture(u_textures[6], v_TexturePos * v_TilingFactor); break;
		case 7: texColor *= texture(u_textures[7], v_TexturePos * v_TilingFactor); break;
		case 8: texColor *= texture(u_textures[8], v_TexturePos * v_TilingFactor); break;
		case 9: texColor *= texture(u_textures[9], v_TexturePos * v_TilingFactor); break;
		case 10: texColor *= texture(u_textures[10], v_TexturePos * v_TilingFactor); break;
		case 11: texColor *= texture(u_textures[11], v_TexturePos * v_TilingFactor); break;
		case 12: texColor *= texture(u_textures[12], v_TexturePos * v_TilingFactor); break;
		case 13: texColor *= texture(u_textures[13], v_TexturePos * v_TilingFactor); break;
		case 14: texColor *= texture(u_textures[14], v_TexturePos * v_TilingFactor); break;
		case 15: texColor *= texture(u_textures[15], v_TexturePos * v_TilingFactor); break;
		case 16: texColor *= texture(u_textures[16], v_TexturePos * v_TilingFactor); break;
		case 17: texColor *= texture(u_textures[17], v_TexturePos * v_TilingFactor); break;
		case 18: texColor *= texture(u_textures[18], v_TexturePos * v_TilingFactor); break;
		case 19: texColor *= texture(u_textures[19], v_TexturePos * v_TilingFactor); break;
		case 20: texColor *= texture(u_textures[20], v_TexturePos * v_TilingFactor); break;
		case 21: texColor *= texture(u_textures[21], v_TexturePos * v_TilingFactor); break;
		case 22: texColor *= texture(u_textures[22], v_TexturePos * v_TilingFactor); break;
		case 23: texColor *= texture(u_textures[23], v_TexturePos * v_TilingFactor); break;
		case 24: texColor *= texture(u_textures[24], v_TexturePos * v_TilingFactor); break;
		case 25: texColor *= texture(u_textures[25], v_TexturePos * v_TilingFactor); break;
		case 26: texColor *= texture(u_textures[26], v_TexturePos * v_TilingFactor); break;
		case 27: texColor *= texture(u_textures[27], v_TexturePos * v_TilingFactor); break;
		case 28: texColor *= texture(u_textures[28], v_TexturePos * v_TilingFactor); break;
		case 29: texColor *= texture(u_textures[29], v_TexturePos * v_TilingFactor); break;
		case 30: texColor *= texture(u_textures[30], v_TexturePos * v_TilingFactor); break;
		case 31: texColor *= texture(u_textures[31], v_TexturePos * v_TilingFactor); break;
		default: texColor = v_Color; break;
	}
	color = texColor;
	colorTst1 = vec4(v_TexturePos, 0.0, 1.0);
	colorTst = vec4(vec3(v_TextureIndex / 32.0), 1.0);
}
