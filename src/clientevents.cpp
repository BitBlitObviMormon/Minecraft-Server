#include "bbms/clientevents.h"
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_types.hpp>
#include <boost/thread/shared_mutex.hpp>
using namespace boost;

/******************************************************
 * ClientEventArgs :: ReadLock :: ReadLock            *
 * Constructor; Explicitly calls constructor for lock *
 ******************************************************/
ClientEventArgs::ReadLock::ReadLock() { new (&lock) std::unique_ptr< upgrade_lock<upgrade_mutex> >(nullptr); }

/*******************************************
 * ClientEventArgs :: ReadLock :: ReadLock *
 * Move Constructor                        *
 *******************************************/
ClientEventArgs::ReadLock::ReadLock(ReadLock&& value) : lock(std::move(value.lock)) {}

/****************************************************
 * ClientEventArgs :: ReadLock :: ~ReadLock         *
 * Destructor; explicitly calls destructor for lock *
 ****************************************************/
ClientEventArgs::ReadLock::~ReadLock() { lock.~unique_ptr(); }

/******************************************************
 * ClientEventArgs :: WriteLock :: WriteLock          *
 * Constructor; Explicitly calls constructor for lock *
 ******************************************************/
ClientEventArgs::WriteLock::WriteLock() { new (&lock) std::unique_ptr< upgrade_to_unique_lock<upgrade_mutex> >(nullptr); }

/*********************************************
 * ClientEventArgs :: WriteLock :: WriteLock *
 * Move Constructor                          *
 *********************************************/
ClientEventArgs::WriteLock::WriteLock(WriteLock&& value) : lock(std::move(value.lock)) {}

/****************************************************
 * ClientEventArgs :: WriteLock :: ~WriteLock       *
 * Destructor; explicitly calls destructor for lock *
 ****************************************************/
ClientEventArgs::WriteLock::~WriteLock() { lock.~unique_ptr(); }

/**************************************
 * ClientEventArgs :: ClientEventArgs *
 * Default Constructor                *
 **************************************/
ClientEventArgs::ClientEventArgs(std::shared_ptr<Client> client, upgrade_mutex* mutexPtr)
	: client(client), mutexPtr(mutexPtr), _readLock(), _writeLock() {}

/**************************************
 * ClientEventArgs :: ClientEventArgs *
 * Move Constructor                   *
 **************************************/
ClientEventArgs::ClientEventArgs(ClientEventArgs&& e)
	: client(std::move(e.client)),mutexPtr(std::move(mutexPtr)), _readLock(std::move(e._readLock)), _writeLock(std::move(e._writeLock)) {}

/******************************************************
 * ClientEventArgs :: getReadAccess()                 *
 * Returns a pointer of the client (with read access) *
 ******************************************************/
const Client* ClientEventArgs::getReadAccess()
{
	// Give the thread read-only access before giving the user a readable client variable.
	if (_readLock.lock == nullptr)
		_readLock.lock = std::make_unique< upgrade_lock<upgrade_mutex> >(upgrade_lock<upgrade_mutex>(*mutexPtr));

	return client.get();
}

/*******************************************************
 * ClientEventArgs :: getWriteAccess()                 *
 * Returns a pointer of the client (with write access) *
 *******************************************************/
Client* ClientEventArgs::getWriteAccess()
{
	// Get read access first
	if (_readLock.lock == nullptr)
		_readLock.lock = std::make_unique< upgrade_lock<upgrade_mutex> >(upgrade_lock<upgrade_mutex>(*mutexPtr));


	// Give the thread write-only access before giving the user a writeable client variable.
	if (_writeLock.lock == nullptr)
		_writeLock.lock = std::make_unique< upgrade_to_unique_lock<upgrade_mutex> >(upgrade_to_unique_lock<upgrade_mutex>(*_readLock.lock));

	return client.get();
}

/***************************************************************
 * ClientEventArgs :: unblockWriteAccess()                     *
 * Returns write access to the server (can make things faster) *
 ***************************************************************/
void ClientEventArgs::unblockWriteAccess() { _writeLock.lock.reset(); }

/**************************************************************
 * ClientEventArgs :: unblockReadAccess()                     *
 * Returns read access to the server (can make things faster) *
 * Also calls unblockWriteAccess()                            *
 **************************************************************/
void ClientEventArgs::unblockReadAccess() { _writeLock.lock.reset();  _readLock.lock.reset(); }

/********************************************
 * DisplayedSkinParts :: DisplayedSkinParts *
 * Default Constructor                      *
 ********************************************/
DisplayedSkinParts::DisplayedSkinParts(Byte byte)
{
	cape		= (byte & 0x01) != 0;
	jacket		= (byte & 0x02) != 0;
	leftSleeve	= (byte & 0x04) != 0;
	rightSleeve	= (byte & 0x08) != 0;
	leftPants	= (byte & 0x10) != 0;
	rightPants	= (byte & 0x20) != 0;
	hat			= (byte & 0x40) != 0;
	unusedBit	= (byte & 0x80) != 0;
}
