#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <Windows.h>
#include <sstream>
#include <iomanip>

[[nodiscard]] inline std::string Timestamp()
{
	SYSTEMTIME st;
	GetSystemTime(&st);

	std::stringstream result;
	result << std::setfill('0') << std::setw(2) << st.wYear;
	result << "/";
	result << std::setfill('0') << std::setw(2) << st.wMonth;
	result << "/";
	result << std::setfill('0') << std::setw(2) << st.wDay;
	result << " - ";
	result << std::setfill('0') << std::setw(2) << st.wHour;
	result << ":";
	result << std::setfill('0') << std::setw(2) << st.wMinute;
	result << ":";
	result << std::setfill('0') << std::setw(2) << st.wSecond;

	return result.str();
}