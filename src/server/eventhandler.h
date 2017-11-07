#pragma once

#include "../data/datatypes.h"
#include "../client/client.h"
#include "../client/clientevents.h"
#include "networkhandler.h"
#include <map>
#include <thread>
#include "vld.h"

class Server;
class NetworkHandler;
class EventHandler
{
private:
	std::thread tickClock;
	Double tickDelay;
	volatile Boolean running;
	void runTickClock();
	void seedNetwork(NetworkHandler* networkHandler);
protected:
	std::map<SOCKET, Client*> clients;
	NetworkHandler* networkHandler;

	/*****************
	 * SERVER EVENTS *
	 *****************/
	void onTick(Double dt, Int ticksSkipped);

	/***************************
	 * CLIENT -> SERVER EVENTS *
	 ***************************/
	/* ERROR-TRACKING EVENTS */
	void invalidPacket(InvalidPacketEventArgs e);
	void invalidState(InvalidStateEventArgs e);
	void invalidLength(InvalidLengthEventArgs e);
	void clientDisconnect(ClientDisconnectEventArgs e);

	/* HANDSHAKING EVENTS */
	void handshake(HandShakeEventArgs e);
	void legacyServerListPing(LegacyServerListPingEventArgs e);

	/* STATUS EVENTS */
	void ping(PingEventArgs e);
	void request(RequestEventArgs e);

	/* LOGIN EVENTS */
	void encryptionResponse(EncryptionResponseEventArgs e);
	void loginStart(LoginStartEventArgs e);

	/* PLAY EVENTS */
	void animation(AnimationEventArgs e);
	void chatMessage(ChatMessageEventArgs e);
	void clickWindow(ClickWindowEventArgs e);
	void clientSettings(ClientSettingsEventArgs e);
	void clientStatus(ClientStatusEventArgs e);
	void closeWindow(CloseWindowEventArgs e);
	void confirmTransaction(ConfirmTransactionEventArgs e);
	void creativeInventoryAction(CreativeInventoryActionEventArgs e);
	void enchantItem(EnchantItemEventArgs e);
	void entityAction(EntityActionEventArgs e);
	void heldItemChange(HeldItemChangeEventArgs e);
	void keepAlive(KeepAliveEventArgs e);
	void playerAbilities(PlayerAbilitiesEventArgs e);
	void playerBlockPlacement(PlayerBlockPlacementEventArgs e);
	void playerDigging(PlayerDiggingEventArgs e);
	void playerLook(PlayerLookEventArgs e);
	void playerOnGround(PlayerOnGroundEventArgs e);
	void playerPosition(PlayerPositionEventArgs e);
	void playerPositionAndLook(PlayerPositionAndLookEventArgs e);
	void pluginMessage(PluginMessageEventArgs e);
	void resourcePackStatus(ResourcePackStatusEventArgs e);
	void spectate(SpectateEventArgs e);
	void steerBoat(SteerBoatEventArgs e);
	void steerVehicle(SteerVehicleEventArgs e);
	void tabComplete(TabCompleteEventArgs e);
	void teleportConfirm(TeleportConfirmEventArgs e);
	void updateSign(UpdateSignEventArgs e);
	void useEntity(UseEntityEventArgs e);
	void useItem(UseItemEventArgs e);
	void vehicleMove(VehicleMoveEventArgs e);
public:
	/* Constructors */
	EventHandler();
	~EventHandler();

	/* Our friend, the NetworkHandler */
	friend class NetworkHandler;
	friend class Server;

	/**********************************************************
	 * These start and stop the server's tick loop.           *
	 * They are needed to run logic outside of client events. *
	 * The delay is in seconds                                *
	 **********************************************************/
	void startTickClock(Double delay = 0.05);
	void stopTickClock();

	/***********************************************************
	 * EventHandler :: triggerEvent                            *
	 * Runs the given event (may be run on a different thread) *
	 ***********************************************************/
	template <typename T, typename P>
	void triggerEvent(T&& fn, EventHandler* eventHandler, P e)
	{
		((eventHandler)->*(fn))(e);
	}

	/***********************************************************
	 * EventHandler :: triggerEvent                            *
	 * Runs the given event (may be run on a different thread) *
	 ***********************************************************/
	template <typename T>
	void triggerEvent(T&& fn)
	{
		fn();
	}
};