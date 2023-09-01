#pragma once
#include <unordered_set>

std::unordered_set<std::string> GetAllFilepathsInDirectory(const std::string& aPath);

std::string GetValidPath(const std::string& aPath, const std::string& aDefaultDirectory, bool aLogErrors = true);

std::string CreateValidPath(const std::string& aPath, const std::string& aDefaultDirectory, bool aLogErrors = true);

std::string AddExtensionIfMissing(const std::string& aPath, const std::string& anExtension);