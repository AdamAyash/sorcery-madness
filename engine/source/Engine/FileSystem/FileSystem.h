#pragma once

#include <string>

namespace engine
{
	class FileSystem
	{
	public:
		FileSystem() = delete;
		~FileSystem() = delete;
	public:
		static const std::string ReadFileContents(const std::string& strFilePath);
	};
};
