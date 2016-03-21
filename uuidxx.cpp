#include "uuidxx.h"
#include <regex>
#include <random>

using namespace std;
using namespace uuidxx;

bool uuid::operator == (const uuid & guid2) const
{
	return memcmp(&guid2, this, sizeof(uuid)) == 0;
}

bool uuid::operator != (const uuid & guid2) const
{
	return !(*this == guid2);
}

bool uuid::operator < (const uuid &guid2) const
{
	return memcmp(this, &guid2, sizeof(uuid)) < 0;
}

bool uuid::operator > (const uuid &guid2) const
{
	return memcmp(this, &guid2, sizeof(uuid)) > 0;
}

uuid::uuid (const char *guidString)
{
	//MSVC is broken, doesn't support hh and insists on two bytes per field
	//sscanf(guidString, "{%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}", &Data1, &Data2, &Data3, &Data4[0], &Data4[1], &Data4[2], &Data4[3], &Data4[4], &Data4[5], &Data4[6], &Data4[7]);
	static auto guidRegex = regex("\\{?([0-9a-fA-F]{8})-([0-9a-fA-F]{4})-([0-9a-fA-F]{4})-([0-9a-fA-F]{2})([0-9a-fA-F]{2})-([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})\\}?");
	cmatch matchResults;
	if (regex_match(guidString, matchResults, guidRegex))
	{
		Data1 = static_cast<uint32_t>(stoul(matchResults[1], nullptr, 16));
		Data2 = static_cast<uint16_t>(stoul(matchResults[2], nullptr, 16));
		Data3 = static_cast<uint16_t>(stoul(matchResults[3], nullptr, 16));
		for (size_t i = 0; i < 8; ++i)
		{
			Data4[i] = static_cast<uint8_t>(stoul(matchResults[4 + i], nullptr, 16));
		}
	}
}

string uuid::ToString(bool withBraces) const
{
	char buffer[39];
	sprintf(buffer, "%s%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X%s", withBraces ? "{" : "", Data1, Data2, Data3, Data4[0], Data4[1], Data4[2], Data4[3], Data4[4], Data4[5], Data4[6], Data4[7], withBraces ? "}" : "");
	return string(buffer);
}

uuid uuid::Generate()
{
	std::random_device rd;
	std::mt19937_64 gen(rd());

	std::uniform_int_distribution<uint32_t> dis32;
	std::uniform_int_distribution<uint16_t> dis16;
	//not supported on Windows. See https://connect.microsoft.com/VisualStudio/feedback/details/2420840
	//std::uniform_int_distribution<uint8_t> dis8;

	uuid newGuid;
	newGuid.Data1 = dis32(gen);
	newGuid.Data2 = dis16(gen);
	newGuid.Data3 = dis16(gen);
	for (auto &data4 : newGuid.Data4)
	{
		data4 = static_cast<uint8_t>(dis16(gen));
	}

	return newGuid;
}
