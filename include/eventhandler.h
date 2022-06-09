#pragma once

#include "client.h"
#include "clientevents.h"
#include "lockedclient.h"
#include <list>
#include <atomic>
#include <memory>
#include <functional>
#include <shared_mutex>

namespace boost {
	namespace asio {
		class thread_pool;
	}
}

class Server;
class NetworkHandler;
class EventHandler
{
protected:
	NetworkHandler* networkHandler;
	std::unique_ptr<boost::asio::thread_pool> tpool;
	std::list<LockedClient> clients;
	std::shared_mutex clientsMutex;
	std::atomic<Long> nextEntityID;

	/*****************
	 * SERVER EVENTS *
	 *****************/
	virtual void onTick(Double dt, Int ticksSkipped);

	/***************************
	 * CLIENT -> SERVER EVENTS *
	 ***************************/
	/* ERROR-TRACKING EVENTS */
	virtual void invalidPacket(InvalidPacketEventArgs e);
	virtual void invalidState(InvalidPacketEventArgs e);
	virtual void invalidLength(InvalidLengthEventArgs e);
	virtual void clientDisconnected(const Client* client);

	/* HANDSHAKING EVENTS */
	virtual void handshake(HandShakeEventArgs e);
	virtual void legacyServerListPing(LegacyServerListPingEventArgs e);

	/* STATUS EVENTS */
	virtual void ping(PingEventArgs e);
	virtual void request(RequestEventArgs e);

	/* LOGIN EVENTS */
	virtual void encryptionResponse(EncryptionResponseEventArgs e);
	virtual void loginStart(LoginStartEventArgs e);

	/* PLAY EVENTS */
	virtual void handAnimation(HandAnimationEventArgs e);
	virtual void chatMessage(ChatMessageEventArgs e);
	virtual void clickWindow(ClickWindowEventArgs e);
	virtual void clientSettings(ClientSettingsEventArgs e);
	virtual void clientStatus(ClientStatusEventArgs e);
	virtual void closeWindow(CloseWindowEventArgs e);
	virtual void confirmTransaction(ConfirmTransactionEventArgs e);
	virtual void creativeInventoryAction(CreativeInventoryActionEventArgs e);
	virtual void enchantItem(EnchantItemEventArgs e);
	virtual void entityAction(EntityActionEventArgs e);
	virtual void heldItemChange(HeldItemChangeEventArgs e);
	virtual void keepAlive(KeepAliveEventArgs e);
	virtual void playerAbilities(PlayerAbilitiesEventArgs e);
	virtual void playerBlockPlacement(PlayerBlockPlacementEventArgs e);
	virtual void playerDigging(PlayerDiggingEventArgs e);
	virtual void playerMoved(PlayerMovedEventArgs e);
	virtual void pluginMessage(PluginMessageEventArgs e);
	virtual void resourcePackStatus(ResourcePackStatusEventArgs e);
	virtual void spectate(SpectateEventArgs e);
	virtual void steerBoat(SteerBoatEventArgs e);
	virtual void steerVehicle(SteerVehicleEventArgs e);
	virtual void tabComplete(TabCompleteEventArgs e);
	virtual void teleportConfirm(TeleportConfirmEventArgs e);
	virtual void updateSign(UpdateSignEventArgs e);
	virtual void useEntity(UseEntityEventArgs e);
	virtual void useItem(UseItemEventArgs e);
	virtual void vehicleMoved(VehicleMovedEventArgs e);

	/* DATA-PASSING EVENTS */
	virtual ChunkSection& getChunkSection(GetChunkEventArgs& e);
	virtual ChunkColumn& getChunk(GetChunkEventArgs& e);
	virtual BiomeID* getBiomes(GetBiomeEventArgs& e);
	virtual Client* createClient(SOCKET socket);

public:
	/* Constructors */
	EventHandler();
	virtual ~EventHandler();

	/* Our friend, the NetworkHandler */
	friend class NetworkHandler;
	friend class Server;

	// Executes a function on a server thread
	void exec(std::function<void(void)> func);
};