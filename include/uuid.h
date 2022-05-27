#pragma once
#include "datatypes.h"
#include <memory>

constexpr auto UUID_LENGTH = 16;

namespace boost {
	namespace uuids {
		struct uuid;
	}
}

/****************************************
 * UUID                                 *
 * Universally Unique Identifier        *
 * A simple wrapper around Boost's UUID *
 ****************************************/
class UUID
{
private:
	std::unique_ptr<boost::uuids::uuid> uuid;
	UUID(const boost::uuids::uuid& uuid);
public:
	UUID();
	UUID(const UUID& uuid);
	UUID(const String& str);
	~UUID();
	const String str();
	const Byte* makeData();
	Int length() { return UUID_LENGTH; }
	UUID& operator=(const UUID& rhs);
	friend std::ostream& operator<<(std::ostream& os, const UUID& uuid);
	friend std::istream& operator>>(std::istream& is, UUID& uuid);

	class generate
	{
	private:
		generate() = delete; // This is a static class
	public:
		// Generates an empty UUID
		static UUID null();

		// Generates a UUID using the newest hash algorithm
		static UUID fromName(const String& name);

		// Generates a UUID using the old MD5 hash algorithm
		static UUID fromNameMD5(const String& name);

		// Generates a UUID using the newer SHA1 hash algorithm
		static UUID fromNameSHA1(const String& name);

		// Generates a UUID from a stringified UUID
		static UUID fromString(const String& uuid);

		// Generates a UUID using a random number
		static UUID random();
	};
};