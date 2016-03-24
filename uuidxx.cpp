#include "uuidxx.h"
#include <random>
#include <stdio.h>
#include <inttypes.h>

using namespace std;
using namespace uuidxx;

#ifdef _WIN32
#define sprintf sprintf_s
#define sscanf sscanf_s
#endif

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
	if (uuidString == nullptr)
	{
		return;
	}

	if (uuidString[0] == '{')
	{
		sscanf(uuidString, "{%08" SCNx32 "-%04" SCNx16 "-%04" SCNx16 "-%02" SCNx8 "%02" SCNx8 "-%02" SCNx8 "%02" SCNx8 "%02" SCNx8 "%02" SCNx8 "%02" SCNx8 "%02" SCNx8 "}", &Data1, &Data2, &Data3, &Data4[0], &Data4[1], &Data4[2], &Data4[3], &Data4[4], &Data4[5], &Data4[6], &Data4[7]);
	}
	else
	{
		sscanf(uuidString, "%08" SCNx32 "-%04" SCNx16 "-%04" SCNx16 "-%02" SCNx8 "%02" SCNx8 "-%02" SCNx8 "%02" SCNx8 "%02" SCNx8 "%02" SCNx8 "%02" SCNx8 "%02" SCNx8 "", &Data1, &Data2, &Data3, &Data4[0], &Data4[1], &Data4[2], &Data4[3], &Data4[4], &Data4[5], &Data4[6], &Data4[7]);
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
