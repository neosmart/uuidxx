#pragma once

#include <cstdint>
#include <string>

namespace uuidxx
{
	struct uuid
	{
		uint32_t Data1;
		uint16_t  Data2;
		uint16_t Data3;
		uint8_t Data4[8];

		bool operator == (const uuid &guid2) const;
		bool operator != (const uuid &guid2) const;
		bool operator < (const uuid &guid2) const;
		bool operator > (const uuid &guid2) const;

		uuid() = default;

		uuid (const char *uuidString);
		uuid (const std::string &uuidString);
		static uuid FromString(const char *uuidString);
		static uuid FromString(const std::string &uuidString);
		static uuid Generate();

		std::string ToString(bool withBraces = true) const;
	};
}
