#include "bbms/lockedclient.h"
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/shared_lock_guard.hpp>

LockedClient::LockedClient(std::shared_ptr<Client> client) : client(client),
   mutex(new boost::upgrade_mutex()) {}
LockedClient::LockedClient(LockedClient&& client) : mutex(std::move(client.mutex)), client(std::move(client.client)) {}
LockedClient::~LockedClient() {}

bool LockedClientComparator::operator()(const LockedClient& lhs, const LockedClient& rhs) const {
	boost::shared_lock_guard<boost::upgrade_mutex> readLock1(*lhs.mutex);
	boost::shared_lock_guard<boost::upgrade_mutex> readLock2(*rhs.mutex);
	return lhs.client->socket > rhs.client->socket;
}