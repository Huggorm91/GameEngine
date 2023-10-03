#pragma once
#include "nlohmannJson/json_fwd.hpp"
#include <fstream>

// Example for creating automatic parsing to and from a nlohmann::json object

//using json = nlohmann::json;
//
//namespace ns 
//{
//	  struct person 
//    {
//    std::string name;
//    std::string address;
//    int age;
//    };
// 
//    void to_json(json& j, const person& p) 
//    {
//        j = json{ {"name", p.name}, {"address", p.address}, {"age", p.age} };
//    }
//
//    void from_json(const json& j, person& p) 
//    {
//        j.at("name").get_to(p.name);
//        j.at("address").get_to(p.address);
//        j.at("age").get_to(p.age);
//    }
//} // namespace ns

namespace Crimson
{
	nlohmann::json GetData(const std::string& aFilePath = "data.json");
	nlohmann::json GetData(std::fstream& aStream, const unsigned int anAmountOfLines);

	void SaveData(const nlohmann::json& aRoot, const std::string& aFilePath = "data.json");

	void CompressFile(const std::string& aFilePath = "data.json", bool aKeepNewLine = false);
	void CompressFile(const std::string& anInFilePath, const std::string& anOutFilePath, bool aKeepNewLine = false);
}