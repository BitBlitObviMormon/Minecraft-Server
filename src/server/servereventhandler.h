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
	void loginFailed(Client* client);
	void encryptionRequest(Client* client);
	void loginSuccess(Client* client);
	void setCompression(Client* client);

	/* GAMEPLAY EVENTS */
	void spawnObject(Client* client);
	void spawnExperienceOrb(Client* client);
	void spawnGlobalEntity(Client* client);
	void spawnMob(Client* client);
	void spawnPainting(Client* client);
	void spawnPlayer(Client* client);
	void animation(Client* client);
	void statistics(Client* client);
	void blockBreakAnimation(Client* client);
	void updateBlockEntity(Client* client);
	void blockAction(Client* client);
	void blockChange(Client* client);
	void bossBar(Client* client);
	void serverDifficulty(Client* client);
	void tabComplete(Client* client);
	void chatMessage(Client* client);
	void multiBlockChange(Client* client);
	void confirmTransaction(Client* client);
	void closeWindow(Client* client);
	void openWindow(Client* client);
	void windowItems(Client* client);
	void windowProperty(Client* client);
	void setSlot(Client* client);
	void setCooldown(Client* client);
	void pluginMessage(Client* client);
	void namedSoundEffect(Client* client);
	void disconnect(Client* client);
	void entityStatus(Client* client);
	void explosion(Client* client);
	void unloadChunk(Client* client);
	void changeGameState(Client* client);
	void keepAlive(Client* client);
	void chunkData(Client* client);
	void effect(Client* client);
	void particle(Client* client);
	void joinGame(Client* client);
	void map(Client* client);
	void entityRelativeMove(Client* client);
	void entityLookAndRelativeMove(Client* client);
	void entityLook(Client* client);
	void entity(Client* client);
	void vehicleMove(Client* client);
	void openSignEditor(Client* client);
	void playerAbilities(Client* client);
	void combatEvent(Client* client);
	void playerListItem(Client* client);
	void playerPositionAndLook(Client* client);
	void useBed(Client* client);
	void destroyEntities(Client* client);
	void removeEntityEffect(Client* client);
	void resourcePackSend(Client* client);
	void respawn(Client* client);
	void entityHeadLook(Client* client);
	void worldBorder(Client* client);
	void camera(Client* client);
	void heldItemChange(Client* client);
	void displayScoreBoard(Client* client);
	void entityMetadata(Client* client);
	void attachEntity(Client* client);
	void entityVelocity(Client* client);
	void entityEquipment(Client* client);
	void setExperience(Client* client);
	void updateHealth(Client* client);
	void scoreboardObjective(Client* client);
	void setPassengers(Client* client);
	void teams(Client* client);
	void updateScore(Client* client);
	void spawnPosition(Client* client);
	void timeUpdate(Client* client);
	void title(Client* client);
	void soundEffect(Client* client);
	void playerListHeaderAndFooter(Client* client);
	void collectItem(Client* client);
	void entityTeleport(Client* client);
	void entityProperties(Client* client);
	void entityEffect(Client* client);

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