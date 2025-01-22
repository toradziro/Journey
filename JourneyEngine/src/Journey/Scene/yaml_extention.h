#pragma once

#include <yaml-cpp/yaml.h>

namespace YAML
{

//============================= VEC2 =============================
Emitter& operator<<(Emitter& out, const glm::vec2& v)
{
	out << Flow;
	out << BeginSeq << v.x << v.y << EndSeq;
	return out;
}

template<>
struct convert<glm::vec2>
{
	static Node encode(const glm::vec2& vec)
	{
		Node node;
		node.push_back(vec.x);
		node.push_back(vec.y);
		return node;
	}

	static bool decode(const Node& node, glm::vec2& vec)
	{
		if (!node.IsSequence() || node.size() != 2) {
			return false;
		}
		vec.x = node[0].as<float>();
		vec.y = node[1].as<float>();
		return true;
	}
};

//============================= VEC3 =============================
Emitter& operator<<(Emitter& out, const glm::vec3& v)
{
	out << Flow;
	out << BeginSeq << v.x << v.y << v.z << EndSeq;
	return out;
}

template<>
struct convert<glm::vec3>
{
	static Node encode(const glm::vec3& vec)
	{
		Node node;
		node.push_back(vec.x);
		node.push_back(vec.y);
		node.push_back(vec.z);
		return node;
	}

	static bool decode(const Node& node, glm::vec3& vec)
	{
		if (!node.IsSequence() || node.size() != 3) {
			return false;
		}
		vec.x = node[0].as<float>();
		vec.y = node[1].as<float>();
		vec.z = node[2].as<float>();
		return true;
	}
};

//============================= VEC4 =============================
Emitter& operator<<(Emitter& out, const glm::vec4& v)
{
	out << Flow;
	out << BeginSeq << v.r << v.g << v.b << v.a << EndSeq;
	return out;
}

template<>
struct convert<glm::vec4>
{
	static Node encode(const glm::vec4& vec)
	{
		Node node;
		node.push_back(vec.r);
		node.push_back(vec.g);
		node.push_back(vec.b);
		node.push_back(vec.a);
		return node;
	}

	static bool decode(const Node& node, glm::vec4& vec)
	{
		if (!node.IsSequence() || node.size() != 4) {
			return false;
		}
		vec.r = node[0].as<float>();
		vec.g = node[1].as<float>();
		vec.b = node[2].as<float>();
		vec.a = node[3].as<float>();
		return true;
	}
};

}