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

uuid::uuid (const std::string &uuidString)
	: uuid(uuidString.c_str())
{
}

uuid::uuid (const char *uuidString)
{
	//MSVC is broken, doesn't support hh and insists on two bytes per field
	//sscanf(uuidString, "{%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}", &Data1, &Data2, &Data3, &Data4[0], &Data4[1], &Data4[2], &Data4[3], &Data4[4], &Data4[5], &Data4[6], &Data4[7]);
	static auto guidRegex = regex("\\{?([0-9a-fA-F]{8})-([0-9a-fA-F]{4})-([0-9a-fA-F]{4})-([0-9a-fA-F]{2})([0-9a-fA-F]{2})-([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})\\}?");
	cmatch matchResults;
	if (regex_match(uuidString, matchResults, guidRegex))
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

uuid uuid::FromString(const char *uuidString)
{
	uuid temp(uuidString);
	return temp;
}

uuid uuid::FromString(const std::string &uuidString)
{
	uuid temp(uuidString.c_str());
	return temp;
}

uuid uuid::Generate()
{
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<uint64_t> dis64;

	uuid newGuid;
	uint64_t *local = new (&newGuid) (uint64_t[2]);
	local[0] = dis64(gen);
	local[1] = dis64(gen);

	return newGuid;
}
