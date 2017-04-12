#pragma once

#include <string>

namespace Util
{
	std::string ReadFile(const std::string &filename);

	const static std::string TextureDirectory = "Textures/";
	const static std::string ModelDirectory = "Models/";
	const static std::string ShaderDirectory = "Shaders/";
	const static std::string ScriptDirectory = "Scripts/";
	const static std::string AudioDirectory = "\\Audio/";
	const static std::string FontDirectory = "Fonts/";

	class EndGameException : public std::exception {};
	void EndGame();
}