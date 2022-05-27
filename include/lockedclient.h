#pragma once
#include "client.h"
#include <memory>

namespace boost {
	class shared_mutex;
	typedef shared_mutex upgrade_mutex;
}
class LockedClient
{
public:
	std::unique_ptr<boost::upgrade_mutex> mutex;
	std::shared_ptr<Client> client;
	LockedClient(std::shared_ptr<Client> client);
	LockedClient(LockedClient&& client);
	LockedClient(const LockedClient& client) = delete; // No copying of mutexes here
	~LockedClient();
};

// Used to compare Client* by the socket
struct LockedClientComparator
{
	bool operator()(const LockedClient& lhs, const LockedClient& rhs) const;
};
