#include "uuid.h"
#include <sstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/name_generator_md5.hpp>
#include <boost/uuid/name_generator_sha1.hpp>

// Used to generate the namespace UUID for the name generators; can be any UUID.
// Example:
// #define NAMESPACE_UUID str("deadbeef-ca5e-0000-0123-beefbdead101")
#define NAMESPACE_UUID nil()

struct generators {
	boost::uuids::nil_generator nil;
	boost::uuids::string_generator str;
	boost::uuids::random_generator rand;
	boost::uuids::name_generator_md5 md5;
	boost::uuids::name_generator_sha1 sha1;
	generators() : nil(), str(), rand(), md5(NAMESPACE_UUID), sha1(NAMESPACE_UUID) {}
} generators;

/***********************
 * UUID :: UUID        *
 * default constructor *
 ***********************/
UUID::UUID() : uuid(new boost::uuids::uuid()) {}

/********************
 * UUID :: UUID     *
 * copy constructor *
 ********************/
UUID::UUID(const UUID& uuid) : UUID(*uuid.uuid) {}

/****************************
 * UUID :: UUID             *
 * Private copy constructor *
 ****************************/
UUID::UUID(const boost::uuids::uuid& uuid) : uuid(new boost::uuids::uuid(uuid)) {}

/********************
 * UUID :: UUID     *
 * UUID from string *
 ********************/
UUID::UUID(const String& str) : UUID(UUID::generate::fromString(str)) {}

/*****************
 * UUID :: ~UUID *
 * Destructor    *
 *****************/
UUID::~UUID() {}

/******************
 * UUID :: str()  *
 * Returns String *
 ******************/
const String UUID::str() {
	std::stringstream ss;
	ss << *uuid;
	return ss.str();
}

/**********************
 * UUID :: makeData() *
 * Returns UUID data  *
 **********************/
const Byte* UUID::makeData() { return (Byte*)uuid->data; }

/************************
 * UUID :: = operator   *
 * Assigns UUID to UUID *
 ************************/
UUID& UUID::operator=(const UUID& rhs) { *this->uuid = *rhs.uuid; return *this; }

/****************************
 * UUID << operator         *
 * Allows printing of UUIDs *
 ****************************/
std::ostream& operator<<(std::ostream& os, const UUID& uuid) { return os << uuid.uuid; }

/*************************
 * UUID >> operator      *
 * Allows inputing UUIDs *
 *************************/
std::istream& operator>>(std::istream& is, UUID& uuid) { 
	String temp;
	is >> temp;
	uuid = UUID::generate::fromString(temp);
	return is;
}

/**********************************
 * UUID :: generate :: generators *
 **********************************/
UUID UUID::generate::null() { return UUID(generators.nil()); }
UUID UUID::generate::fromName(const String& name) { return UUID(generators.sha1(name)); }
UUID UUID::generate::fromNameMD5(const String& name) { return UUID(generators.md5(name)); }
UUID UUID::generate::fromNameSHA1(const String& name) { return UUID(generators.sha1(name)); }
UUID UUID::generate::fromString(const String& str) { return UUID(generators.str(str)); }
UUID UUID::generate::random() { return UUID(generators.rand()); }