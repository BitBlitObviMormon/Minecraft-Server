#pragma once

#include "client.h"
#include "../server/servereventhandler.h"
#include <map>

class ClientEventHandler
{
protected:
	volatile Boolean running;
	std::map<SOCKET, Client*> clients;
	ServerEventHandler* serverHandler;
	
	/***************************
	 * CLIENT -> SERVER EVENTS *
	 ***************************/
	void readPacket(Client* client, Byte* buffer, Int length);

	/* HAND SHAKE EVENTS */
	void handShake(Client* client, Byte* buffer, Int length);
	void handShakeLegacy(Client* client, Byte* buffer, Int length);

	/* STATUS EVENTS */
	void request(Client* client, Byte* buffer, Int length);
	void ping(Client* client, Byte* buffer, Int length);

	/* LOGIN EVENTS */
	void loginStart(Client* client, Byte* buffer, Int length);
	void encryptionResponse(Client* client, Byte* buffer, Int length);

	/* GAMEPLAY EVENTS */
	void teleportConfirm(Client* client, Byte* buffer, Int length);
	void tabComplete(Client* client, Byte* buffer, Int length);
	void chatMessage(Client* client, Byte* buffer, Int length);
	void clientStatus(Client* client, Byte* buffer, Int length);
	void clientSettings(Client* client, Byte* buffer, Int length);
	void confirmTransaction(Client* client, Byte* buffer, Int length);
	void enchantItem(Client* client, Byte* buffer, Int length);
	void clickWindow(Client* client, Byte* buffer, Int length);
	void closeWindow(Client* client, Byte* buffer, Int length);
	void pluginMessage(Client* client, Byte* buffer, Int length);
	void useEntity(Client* client, Byte* buffer, Int length);
	void keepAlive(Client* client, Byte* buffer, Int length);
	void playerPosition(Client* client, Byte* buffer, Int length);
	void playerPositionAndLook(Client* client, Byte* buffer, Int length);
	void playerLook(Client* client, Byte* buffer, Int length);
	void playerOnGround(Client* client, Byte* buffer, Int length);
	void vehicleMove(Client* client, Byte* buffer, Int length);
	void steerBoat(Client* client, Byte* buffer, Int length);
	void playerAbilities(Client* client, Byte* buffer, Int length);
	void playerDigging(Client* client, Byte* buffer, Int length);
	void entityAction(Client* client, Byte* buffer, Int length);
	void steerVehicle(Client* client, Byte* buffer, Int length);
	void resourcePackStatus(Client* client, Byte* buffer, Int length);
	void heldItemChange(Client* client, Byte* buffer, Int length);
	void creativeInventoryAction(Client* client, Byte* buffer, Int length);
	void updateSign(Client* client, Byte* buffer, Int length);
	void animation(Client* client, Byte* buffer, Int length);
	void spectate(Client* client, Byte* buffer, Int length);
	void playerBlockPlacement(Client* client, Byte* buffer, Int length);
	void useItem(Client* client, Byte* buffer, Int length);
public:
	ClientEventHandler();
	~ClientEventHandler();
	void addClient(SOCKET newClient);
	Client* getClientFromSocket(SOCKET socket);
	void start();
	void startAsync();
	void stop();
};