#include "Util.h"

#include <iterator>
#include <fstream>

std::string Util::ReadFile(const std::string &filename)
{
	using iter = std::istreambuf_iterator<char>;
	std::string source;
	std::ifstream input(filename.c_str());
	if (!input) throw std::runtime_error(std::string("unable to read file ") + filename); // TODO throw a real error here

	copy(iter(input), iter(), std::back_inserter(source));

	return source;
}