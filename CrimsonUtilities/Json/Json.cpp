#include "Json.h"
#include "nlohmannJson/json.hpp"
#include <sstream>

namespace Crimson
{
	nlohmann::json GetData(const std::string& aFilePath)
	{
		nlohmann::json root;
		std::fstream fileStream(aFilePath, std::ios::in);
		if (fileStream)
		{
			fileStream >> root;
		}
		fileStream.close();

		return root;
	}

	nlohmann::json GetData(std::fstream& aStream, const unsigned int anAmountOfLines)
	{
		nlohmann::json root;

		if (aStream)
		{
			std::stringstream linesRead;
			for (unsigned int i = 0; i < anAmountOfLines; i++)
			{
				std::string line;
				std::getline(aStream, line);
				linesRead << line;
			}
			linesRead >> root;
		}

		return root;
	}

	void SaveData(const nlohmann::json& aRoot, const std::string& aFilePath)
	{
		std::fstream fileStream(aFilePath, std::ios::out);
		if (fileStream)
		{
			fileStream << aRoot;
			fileStream.flush();
		}
		fileStream.close();
	}

	void CompressFile(const std::string& aFilePath, bool aKeepNewLine)
	{
		std::fstream fileStream(aFilePath, std::ios::in);
		if (!fileStream)
		{
			return;
		}

		std::stringstream result;
		std::string string;

		if (aKeepNewLine)
		{
			while (std::getline(fileStream, string))
			{
				std::stringstream line(string);
				while (line >> string)
				{
					result << string;
				}
				result << std::endl;
			}
		}
		else
		{
			while (fileStream >> string)
			{
				result << string;
			}
		}

		fileStream.close();
		fileStream.open(aFilePath, std::ios::out);
		if (!fileStream)
		{
			return;
		}

		if (aKeepNewLine)
		{
			while (result >> string)
			{
				fileStream << string;
				fileStream << std::endl;
			}
		}
		else
		{
			while (result >> string)
			{
				fileStream << string;
			}
		}

		fileStream.flush();
		fileStream.close();
	}

	void CompressFile(const std::string& anInFilePath, const std::string& anOutFilePath, bool aKeepNewLine)
	{
		std::fstream inStream(anInFilePath, std::ios::in);
		if (!inStream)
		{
			return;
		}

		std::fstream outStream(anOutFilePath, std::ios::out);
		if (!outStream)
		{
			return;
		}

		std::string string;
		if (aKeepNewLine)
		{
			while (std::getline(inStream, string))
			{
				std::stringstream line(string);
				while (line >> string)
				{
					outStream << string;
				}
				outStream << std::endl;
			}
		}
		else
		{
			while (inStream >> string)
			{
				outStream << string;
			}
		}

		outStream.flush();
		outStream.close();
		inStream.close();
	}
}