#pragma once

#include <cstdint>
#include <string>

namespace uuidxx
{
	union uuid
	{
		uint64_t WideIntegers[2];
		struct _internalData
		{
			uint32_t Data1;
			uint16_t  Data2;
			uint16_t Data3;
			uint8_t Data4[8];
		} Uuid;

		bool operator == (const uuid &guid2) const;
		bool operator != (const uuid &guid2) const;
		bool operator < (const uuid &guid2) const;
		bool operator > (const uuid &guid2) const;

		uuid() = default;

		uuid(const char *uuidString);
		uuid(const std::string &uuidString);
		static uuid FromString(const char *uuidString);
		static uuid FromString(const std::string &uuidString);
		static uuid Generate();

		std::string ToString(bool withBraces = true) const;
	};

	static_assert(sizeof(uuid) == 2 * sizeof(int64_t), "Check uuid type declaration/padding!");
}
