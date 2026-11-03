#include "FileSystem.h"
#include <Common/Logger/Logger.h>
#include <fstream>

namespace engine
{
	const std::string FileSystem::ReadFileContents(const std::string& strFilePath)
	{
		std::string strFileContents;
		std::ifstream oInputStream(strFilePath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (oInputStream)
		{
			oInputStream.seekg(0, std::ios::end);
			const size_t nnFileSize = oInputStream.tellg();
			if (nnFileSize)
			{
				strFileContents.resize(nnFileSize);
				oInputStream.seekg(0, std::ios::beg);
				oInputStream.read(&strFileContents[0], nnFileSize);
			}
			else
			{
				LOG_ERROR("Could not read from file '{0}'", strFilePath);
			}
		}
		else
		{
			LOG_ERROR("Could not open file '{0}'", strFilePath);
		}

		return strFileContents;
	}
}