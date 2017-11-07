#pragma once

#include "../client/client.h"
#include "../data/entity/entities.h"
#include "../data/entity/blockentities.h"
#include "serverevents.h"
#include "eventhandler.h"
#include "vld.h"

#ifdef _WIN32
#else
#define closesocket(x) close(x)
#endif
class EventHandler;
class NetworkHandler
{
protected:
	volatile Boolean running;
	EventHandler* eventHandler;

	/* Read a packet and trigger the corresponding event below */
	void readPacket(Client* client, Byte* buffer, Int length);

	/***************************
	 * CLIENT -> SERVER EVENTS *
	 ***************************/
	/* ERROR TRACKING EVENTS */
	void invalidPacket(Client* client, Byte* buffer, Int length, Int packet);
	void invalidState(Client* client, Byte* buffer, Int length);

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
	/****************************
	 * SERVER -> CLIENT PACKETS *
	 ****************************/
	/* Status Packets */
	void sendResponse(Client* client, String json);
	void sendPong(Client* client, Long payload);

	/* Login Packets */
	void sendDisconnect(Client* client, String reason);
	void sendEncryptionRequest(Client* client, String serverID, Byte* publicKey, Int publicKeyLen, Byte* verifyToken, Int verifyTokenLen);
	void sendLoginSuccess(Client* client, UUID uuid, String username);
	void sendSetCompression(Client* client, Int maxPacketSize);

	/* Play Packets */
	void sendSpawnObject(Client* client, Object object);
	void sendSpawnExperienceOrb(Client* client, ExperienceOrb xpOrb);
	void sendGlobalEntity(Client* client, GlobalEntity entity);
	void sendSpawnMob(Client* client, Entity mob);
	void sendSpawnPainting(Client* client, Painting painting);
	void sendSpawnPlayer(Client* client, Player player);
	void sendAnimation(Client* client, Int entityID, Animation animation);
	void sendStatistics(Client* client, Statistic stat);
	void sendBlockBreakAnimation(Client* client, Int entityID, Position pos, Byte destroyStage);
	void sendUpdateBlockEntity(Client* client, BlockEntity entity);
	void sendBlockAction(Client* client); // TODO: Add args
	void sendBlockChange(Client* client, Block block);
	void sendBossBar(Client* client, BossBar bar, BossBarAction action);
	void sendServerDifficulty(Client* client, Difficulty difficulty);
	void sendTabComplete(Client* client, String result);
	void sendChatMessage(Client* client, String message, ChatMessageType type = ChatMessageType::Chat);
	void sendConfirmTransaction(Client* client, Byte windowID, Short actionNum, Boolean accepted);
	void sendCloseWindow(Client* client, Byte windowID);
	void sendOpenWindow(Client* client, Window window);
	void sendWindowItems(Client* client); // TODO: Add args
	void sendWindowProperty(Client* client, Byte windowID, Short Property, Short Value);
	void sendSetSlot(Client* client); // TODO: Add args
	void sendSetCooldown(Client* client, Int itemID, Int numTicks);
	void sendPluginMessage(Client* client, String channel, Byte* data, Int dataLen);
	void sendNamedSoundEffect(Client* client, String name, SoundCategory category, Position pos, Float volume, Float pitch);
	void sendEntityStatus(Client* client, Int entityID, Byte entityStatus);
	void sendUnloadChunk(Client* client, Int x, Int y);
	void sendChangeGameState(Client* client, GameStateReason reason, Float value); // FLOAT?? TODO: See if value should be int
	void sendKeepAlive(Client* client, Int id);
	void sendChunkData(Client* client); // TODO: Add args
	void sendEffect(Client* client, EffectID effectID, Position pos, Int data = 0, Boolean disableRelativeVolume = false);
	void sendParticle(Client* client, Particle particle, Int num, Byte* data = NULL, Int dataLen = 0);
	void sendJoinGame(Client* client, Int entityID, Gamemode gamemode, Dimension dimension, Difficulty difficulty, Byte maxPlayers, LevelType levelType, Boolean reducedDebugInfo = false);
	void sendMap(Client* client, Map map);
	void sendEntityRelativeMove(Client* client, Int entityID, PositionF deltaPos, Boolean isOnGround);
	void sendEntityLookAndRelativeMove(Client* client, Int entityID, PositionF deltaPos, Byte yaw, Byte pitch, Boolean isOnGround);
	void sendEntityLook(Client* client, Int entityID, Byte yaw, Byte pitch, Boolean isOnGround);
	void sendEntity(Client* client, Int entityID);
	void sendVehicleMove(Client* client, PositionF position, Float yaw, Float pitch);
	void sendOpenSignEditor(Client* client, Position pos);
	void sendPlayerAbilities(Client* client, PlayerAbilities abilities, Float flyingSpeed, Float fovModifier);
	void sendEnterCombatEvent(Client* client);
	void sendEndCombatEvent(Client* client, Int duration, Int entityID);
	void sendEntityKilledEvent(Client* client, Int killerID, Int killedID, String message);
	void sendPlayerListItem(Client* client); // TODO: Add args
	void sendPlayerPositionAndLook(Client* client, PositionF pos, Float yaw, Float pitch, PlayerPositionAndLookFlags flags, Int teleportID);
	void sendUseBed(Client* client, Int entityID, Position pos);
	void sendRemoveEntityEffect(Client* client, Int entityID, StatusEffect effect);
	void sendResourcePack(Client* client, String url, String hash);
	void sendRespawn(Client* client, Dimension dimension, Difficulty difficulty, Gamemode gamemode, LevelType level);
	void sendEntityHeadLook(Client* client, Int entityID, Float yaw);
	void sendWorldBorder(Client* client); // TODO: Add args
	void sendCamera(Client* client, Int entityID);
	void sendHeldItemChange(Client* client, Byte hotbarSlot);
	void sendDisplayScoreboard(Client* client, ScoreboardPosition position, String name);
	void sendEntityMetadata(Client* client); // TODO: Add args
	void sendAttachEntity(Client* client, Int leashedEntityID, Int holdingEntityID);
	void sendDetachEntity(Client* client, Int entityID) { sendAttachEntity(client, entityID, -1); }
	void sendEntityVelocity(Client* client, Int entityID, PositionF velocity);
	void sendEntityEquipment(Client* client, Int entityID, EntityEquipmentSlot slot, Slot item); // TODO: Change item var type
	void sendSetExperience(Client* client, Float xpBar/* Between 0 & 1 */, Int level); // Converts to (Float xpBar, Int level, Int totalXP)
	void sendSetExperience(Client* client, Int totalXP); // Converts to (Float xpBar, Int level, Int totalXP)
	void sendUpdateHealth(Client* client, Float hp, Int food, Float foodSaturation);
	void sendScoreboardObjective(Client* client, String name, ScoreboardMode mode, String displayName = "", ObjectiveType type = "");
	void sendRemoveTeam(Client* client, String name);
	void sendUpdateScore(Client* client, String entityName, String objectiveName, Int value);
	void sendRemoveScore(Client* client, String entityName, String objectiveName);
	void sendSpawnPosition(Client* client, Position pos);
	void sendTimeUpdate(Client* client, Long worldAge, Long timeOfDay);
	void sendSetTitle(Client* client, String title);
	void sendSetSubtitle(Client* client, String subtitle);
	void sendSetActionBar(Client* client, String text);
	void sendTitleFadeout(Client* client, Int timeBeforeFade, Int fadeInLength, Int fadeOutLength);
	void sendHideTitle(Client* client);
	void sendResetTitle(Client* client);
	void sendSoundEffect(Client* client, Sound sound, SoundCategory category, Position position, Float volume, Float pitch);
	void sendPlayerListHeaderAndFooter(Client* client, String header, String footer);
	void sendCollectItem(Client* client, Int collectedID, Int collectorID, Int count);
	void sendEntityTeleport(Client* client, Int entityID, PositionF pos, Float yaw, Float pitch, Boolean onGround);
	void sendEntityProperties(Client* client); // TODO: Add args
	void sendEntityEffect(Client* client, Int entityID, StatusEffect effect, Int level, Int duration, StatusEffectFlags flags);

	template <class T> // Any IEnumerable like vector<Statistic>
	void sendStatistics(Client* client, T statistics)
	{

	}

	template <class T> // Any IEnumerable like vector<String>
	void sendTabComplete(Client* client, T results)
	{

	}

	template <class T> // Any IEnumerable like vector<Block>
	void sendBlockChanges(Client* client, T blocks)
	{

	}

	template <class T> // Any IEnumerable like vector<Position>
	void sendExplosion(Client* client, PositionF pos, Float radius, PositionF newClientVelocity, T blocksToRemove)
	{

	}

	template <class T> // Any IEnumerable like vector<Int>
	void sendDestroyEntities(Client* client, T entities)
	{

	}

	template <class T> // Any IEnumerable like vector<Int>
	void sendSetPassengers(Client* client, Int vehicleID, T passengers)
	{

	}

	template <class T> // Any IEnumerable like vector<String>
	void sendCreateTeam(Client* client, Team<T> team)
	{

	}

	template <class T> // Any IEnumerable like vector<String>
	void sendUpdateTeamInfo(Client* client, Team<T> team) // Ignores member list
	{

	}

	template <class T> // Any IEnumerable like vector<String>
	void sendAddTeamMembers(Client* client, String team, T members)
	{

	}


	template <class T> // Any IEnumerable like vector<String>
	void sendRemoveTeamMembers(Client* client, String team, T members)
	{

	}

	NetworkHandler(EventHandler* eventHandler = NULL);
	~NetworkHandler();
	void addClient(SOCKET newClient);
	void disconnectClient(Client* client);
	Client* getClientFromSocket(SOCKET socket);
	void start();
	void startAsync();
	void stop();
};
