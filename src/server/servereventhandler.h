#pragma once

#include "../data/datatypes.h"
#include "../client/client.h"

class ClientEventHandler;
class ServerEventHandler
{
protected:
	volatile Boolean running;
	ClientEventHandler* clientHandler;
public:
	/*****************
	* SERVER EVENTS *
	*****************/
	void serverTick();

	/***************************
	* SERVER -> CLIENT EVENTS *
	***************************/
	/* STATUS EVENTS */
	void response(Client* client);
	void pong(Client* client);

	/* LOGIN EVENTS */
	void disconnect(Client* client);
	void encryptionRequest(Client* client);
	void loginSuccess(Client* client);
	void setCompression(Client* client);

	/* GAMEPLAY EVENTS */


	/* Constructors */
	ServerEventHandler(ClientEventHandler* eventHandler = NULL);
	~ServerEventHandler();

	/**********************************************************
	 * These start and stop the server's tick loop.           *
	 * They are needed to run logic outside of client events. *
	 **********************************************************/
	void startTick(Int delay);
	void stopTick();

	/***********************************************************
	 * ServerEventHandler :: triggerEvent                      *
	 * Runs the given event (may be run on a different thread) *
	 ***********************************************************/
	template <class T, class... Args>
	void triggerEvent(T&& fn, Args&&... args)
	{
		(*T)(*args);
	}
};