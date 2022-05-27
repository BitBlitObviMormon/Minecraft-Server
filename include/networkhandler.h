#pragma once

#include "lockedclient.h"
#include "datatypes.h"
#include "entities.h"
#include "blockentities.h"
#include "serverevents.h"
#include <utility>
#include <memory>

#ifdef _WIN32
#else
#define closesocket(x) close(x)
#endif
class Server;
class EventHandler;
class NetworkHandler
{
protected:
	volatile Boolean running;
	EventHandler* eventHandler;

	/* Get a client from its socket alone -- NOT THREAD-SAFE */
	virtual LockedClient* getClientFromSocket(SOCKET& socket);

	/* Read a packet and trigger the corresponding event below */
	virtual void readPacket(LockedClient& client, std::unique_ptr<Byte[]> buffer, Int length);

	/***************************
	 * CLIENT -> SERVER EVENTS *
	 ***************************/
	/* ERROR TRACKING EVENTS */
	virtual void invalidPacket(LockedClient& client, const Byte* buffer, Int length, Int packet);
	virtual void invalidState(LockedClient& client, const Byte* buffer, Int length, Int packet);
	virtual void invalidLength(LockedClient& client, const Byte* buffer, Int length, Int actualLength, Int packet);

	/* HAND SHAKE EVENTS */
	virtual void handShake(LockedClient& client, const Byte* buffer, Int length);
	virtual void handShakeLegacy(LockedClient& client, const Byte* buffer, Int length);

	/* STATUS EVENTS */
	virtual void request(LockedClient& client, const Byte* buffer, Int length);
	virtual void ping(LockedClient& client, const Byte* buffer, Int length);

	/* LOGIN EVENTS */
	virtual void loginStart(LockedClient& client, const Byte* buffer, Int length);
	virtual void encryptionResponse(LockedClient& client, const Byte* buffer, Int length);

	/* GAMEPLAY EVENTS */
	virtual void teleportConfirm(LockedClient& client, const Byte* buffer, Int length);
	virtual void tabComplete(LockedClient& client, const Byte* buffer, Int length);
	virtual void chatMessage(LockedClient& client, const Byte* buffer, Int length);
	virtual void clientStatus(LockedClient& client, const Byte* buffer, Int length);
	virtual void clientSettings(LockedClient& client, const Byte* buffer, Int length);
	virtual void confirmTransaction(LockedClient& client, const Byte* buffer, Int length);
	virtual void enchantItem(LockedClient& client, const Byte* buffer, Int length);
	virtual void clickWindow(LockedClient& client, const Byte* buffer, Int length);
	virtual void closeWindow(LockedClient& client, const Byte* buffer, Int length);
	virtual void pluginMessage(LockedClient& client, const Byte* buffer, Int length);
	virtual void useEntity(LockedClient& client, const Byte* buffer, Int length);
	virtual void keepAlive(LockedClient& client, const Byte* buffer, Int length);
	virtual void playerPosition(LockedClient& client, const Byte* buffer, Int length);
	virtual void playerPositionAndLook(LockedClient& client, const Byte* buffer, Int length);
	virtual void playerLook(LockedClient& client, const Byte* buffer, Int length);
	virtual void playerOnGround(LockedClient& client, const Byte* buffer, Int length);
	virtual void vehicleMove(LockedClient& client, const Byte* buffer, Int length);
	virtual void steerBoat(LockedClient& client, const Byte* buffer, Int length);
	virtual void craftRecipeRequest(LockedClient& client, const Byte* buffer, Int length);
	virtual void playerAbilities(LockedClient& client, const Byte* buffer, Int length);
	virtual void playerDigging(LockedClient& client, const Byte* buffer, Int length);
	virtual void entityAction(LockedClient& client, const Byte* buffer, Int length);
	virtual void steerVehicle(LockedClient& client, const Byte* buffer, Int length);
	virtual void craftingBookData(LockedClient& client, const Byte* buffer, Int length);
	virtual void resourcePackStatus(LockedClient& client, const Byte* buffer, Int length);
	virtual void advancementTab(LockedClient& client, const Byte* buffer, Int length);
	virtual void heldItemChange(LockedClient& client, const Byte* buffer, Int length);
	virtual void creativeInventoryAction(LockedClient& client, const Byte* buffer, Int length);
	virtual void updateSign(LockedClient& client, const Byte* buffer, Int length);
	virtual void animation(LockedClient& client, const Byte* buffer, Int length);
	virtual void spectate(LockedClient& client, const Byte* buffer, Int length);
	virtual void playerBlockPlacement(LockedClient& client, const Byte* buffer, Int length);
	virtual void useItem(LockedClient& client, const Byte* buffer, Int length);
public:
	/****************************
	 * SERVER -> CLIENT PACKETS *
	 ****************************/
	/* Status Packets */
	virtual void sendResponse(const Client* client, String json);
	virtual void sendPong(const Client* client, Long payload);

	/* Login Packets */
	virtual void sendDisconnect(const Client* client, String reason);
	virtual void sendEncryptionRequest(const Client* client, String serverID, Byte* publicKey, Int publicKeyLen, Byte* verifyToken, Int verifyTokenLen);
	virtual void sendLoginSuccess(const Client* client, UUID uuid, String username);
	virtual void sendSetCompression(const Client* client, Int maxPacketSize);

	/* Play Packets */
	virtual void sendSpawnObject(const Client* client, Object object);
	virtual void sendSpawnExperienceOrb(const Client* client, ExperienceOrb xpOrb);
	virtual void sendGlobalEntity(const Client* client, GlobalEntity entity);
	virtual void sendSpawnMob(const Client* client, Entity mob);
	virtual void sendSpawnPainting(const Client* client, Painting painting);
	virtual void sendSpawnPlayer(const Client* client, PlayerEntity player);
	virtual void sendAnimation(const Client* client, Int entityID, Animation animation);
	virtual void sendStatistics(const Client* client, Statistic stat);
	virtual void sendBlockBreakAnimation(const Client* client, Int entityID, Position pos, Byte destroyStage);
	virtual void sendUpdateBlockEntity(const Client* client, BlockEntity entity);
	virtual void sendBlockAction(const Client* client); // TODO: Add args
	virtual void sendBlockChange(const Client* client, Block block);
	virtual void sendBossBar(const Client* client, BossBar bar, BossBarAction action);
	virtual void sendServerDifficulty(const Client* client, Difficulty difficulty);
	virtual void sendTabComplete(const Client* client, String result);
	virtual void sendChatMessage(const Client* client, String message, ChatMessageType type = ChatMessageType::Chat, Boolean isJson = false);
	virtual void sendConfirmTransaction(const Client* client, Byte windowID, Short actionNum, Boolean accepted);
	virtual void sendCloseWindow(const Client* client, Byte windowID);
	virtual void sendOpenWindow(const Client* client, Window window);
	virtual void sendWindowItems(const Client* client); // TODO: Add args
	virtual void sendWindowProperty(const Client* client, Byte windowID, Short Property, Short Value);
	virtual void sendSetSlot(const Client* client); // TODO: Add args
	virtual void sendSetCooldown(const Client* client, Int itemID, Int numTicks);
	virtual void sendPluginMessage(const Client* client, String channel, Byte* data, Int dataLen);
	virtual void sendNamedSoundEffect(const Client* client, String name, SoundCategory category, Position pos, Float volume, Float pitch);
	virtual void sendEntityStatus(const Client* client, Int entityID, Byte entityStatus);
	virtual void sendUnloadChunk(const Client* client, Int x, Int z);
	virtual void sendUnloadChunk(const Client* client, std::pair<Int, Int> chunk) { sendUnloadChunk(client, chunk.first, chunk.second); }
	virtual void sendChangeGameState(const Client* client, GameStateReason reason, Float value); // FLOAT?? TODO: See if value should be int
	virtual void sendKeepAlive(const Client* client, Long id);
	virtual void sendChunkData(const Client* client); // TODO: Add args
	virtual void sendChunk(const Client* client, Int x, Int z, ChunkColumn& column, Boolean createChunk = false, Boolean inOverworld = true);
	virtual void sendChunk(const Client* client, Int x, Int z, Boolean createChunk = false, Boolean inOverworld = true);
	void sendChunk(const Client* client, std::pair<Int, Int> chunk, Boolean createChunk = false, Boolean inOverworld = true)
		{ sendChunk(client, chunk.first, chunk.second, createChunk, inOverworld); }
	void sendChunk(const Client* client, std::pair<Int, Int> chunk, ChunkColumn& column, Boolean createChunk = false, Boolean inOverworld = true)
		{ sendChunk(client, chunk.first, chunk.second, column, createChunk, inOverworld); }
	virtual void sendEffect(const Client* client, EffectID effectID, Position pos, Int data = 0, Boolean disableRelativeVolume = false);
	virtual void sendParticle(const Client* client, Particle particle, Int num, Byte* data = nullptr, Int dataLen = 0);
	virtual void sendJoinGame(const Client* client, Int entityID, Gamemode gamemode, Dimension dimension, Difficulty difficulty, Byte maxPlayers, LevelType levelType, Boolean reducedDebugInfo = false);
	virtual void sendMap(const Client* client, Map map);
	virtual void sendEntityRelativeMove(const Client* client, Int entityID, PositionF deltaPos, Boolean isOnGround);
	virtual void sendEntityLookAndRelativeMove(const Client* client, Int entityID, PositionF deltaPos, Byte yaw, Byte pitch, Boolean isOnGround);
	virtual void sendEntityLook(const Client* client, Int entityID, Byte yaw, Byte pitch, Boolean isOnGround);
	virtual void sendEntity(const Client* client, Int entityID);
	virtual void sendVehicleMove(const Client* client, PositionF position, Float yaw, Float pitch);
	virtual void sendOpenSignEditor(const Client* client, Position pos);
	virtual void sendCraftRecipeResponse(const Client* client, Byte windowID, Int recipeID);
	virtual void sendPlayerAbilities(const Client* client, PlayerAbilities abilities, Float flyingSpeed, Float fovModifier);
	virtual void sendEnterCombatEvent(const Client* client);
	virtual void sendEndCombatEvent(const Client* client, Int duration, Int entityID);
	virtual void sendEntityKilledEvent(const Client* client, Int killerID, Int killedID, String message);
	virtual void sendPlayerListItem(const Client* client); // TODO: Add args
	virtual void sendPlayerPositionAndLook(const Client* client, PositionF pos, Float yaw, Float pitch, PlayerPositionAndLookFlags flags, Int teleportID);
	virtual void sendUseBed(const Client* client, Int entityID, Position pos);
	virtual void sendRemoveEntityEffect(const Client* client, Int entityID, StatusEffect effect);
	virtual void sendResourcePack(const Client* client, String url, String hash);
	virtual void sendRespawn(const Client* client, Dimension dimension, Difficulty difficulty, Gamemode gamemode, LevelType level);
	virtual void sendEntityHeadLook(const Client* client, Int entityID, Float yaw);
	virtual void sendSelectAdvancementTab(const Client* client, String id = "");
	virtual void sendWorldBorder(const Client* client); // TODO: Add args
	virtual void sendCamera(const Client* client, Int entityID);
	virtual void sendHeldItemChange(const Client* client, Byte hotbarSlot);
	virtual void sendDisplayScoreboard(const Client* client, ScoreboardPosition position, String name);
	virtual void sendEntityMetadata(const Client* client); // TODO: Add args
	virtual void sendAttachEntity(const Client* client, Int leashedEntityID, Int holdingEntityID);
	virtual void sendDetachEntity(const Client* client, Int entityID) { sendAttachEntity(client, entityID, -1); }
	virtual void sendEntityVelocity(const Client* client, Int entityID, PositionF velocity);
	virtual void sendEntityEquipment(const Client* client, Int entityID, EntityEquipmentSlot slot, Slot item); // TODO: Change item var type
	virtual void sendSetExperience(const Client* client, Float xpBar/* Between 0 & 1 */, Int level); // Converts to (Float xpBar, Int level, Int totalXP)
	virtual void sendSetExperience(const Client* client, Int totalXP); // Converts to (Float xpBar, Int level, Int totalXP)
	virtual void sendUpdateHealth(const Client* client, Float hp, Int food, Float foodSaturation);
	virtual void sendScoreboardObjective(const Client* client, String name, ScoreboardMode mode, String displayName = "", ObjectiveType type = "");
	virtual void sendRemoveTeam(const Client* client, String name);
	virtual void sendUpdateScore(const Client* client, String entityName, String objectiveName, Int value);
	virtual void sendRemoveScore(const Client* client, String entityName, String objectiveName);
	virtual void sendSpawnPosition(const Client* client, Position pos);
	virtual void sendTimeUpdate(const Client* client, Long worldAge, Long timeOfDay);
	virtual void sendSetTitle(const Client* client, String title);
	virtual void sendSetSubtitle(const Client* client, String subtitle);
	virtual void sendSetActionBar(const Client* client, String text);
	virtual void sendTitleFadeout(const Client* client, Int timeBeforeFade, Int fadeInLength, Int fadeOutLength);
	virtual void sendHideTitle(const Client* client);
	virtual void sendResetTitle(const Client* client);
	virtual void sendSoundEffect(const Client* client, Sound sound, SoundCategory category, Position position, Float volume, Float pitch);
	virtual void sendPlayerListHeaderAndFooter(const Client* client, String header, String footer);
	virtual void sendCollectItem(const Client* client, Int collectedID, Int collectorID, Int count);
	virtual void sendEntityTeleport(const Client* client, Int entityID, PositionF pos, Float yaw, Float pitch, Boolean onGround);
	virtual void sendAdvancements(const Client* client); // TODO: Add args
	virtual void sendEntityProperties(const Client* client); // TODO: Add args
	virtual void sendEntityEffect(const Client* client, Int entityID, StatusEffect effect, Int level, Int duration, StatusEffectFlags flags);
	virtual void sendStatistics(const Client* client, List<Int> statistics); // TODO: Create Statistic class
	virtual void sendTabComplete(const Client* client, List<String> results);
	virtual void sendBlockChanges(const Client* client, List<Block> blocks);
	virtual void sendExplosion(const Client* client, PositionF pos, Float radius, PositionF newClientVelocity, List<Position> blocksToRemove);
	virtual void sendUnlockRecipes(const Client* client, List<Int> recipes1, List<Int> recipes2, Boolean craftingBookOpen, Boolean filteringCraftable); // TODO: Implement Recipe class!
	virtual void sendDestroyEntities(const Client* client, List<Int> entities);
	virtual void sendSetPassengers(const Client* client, Int vehicleID, List<Int> passengers);
	virtual void sendCreateTeam(const Client* client, List<String> team);
	virtual void sendUpdateTeamInfo(const Client* client, Team team); // Ignores member list
	virtual void sendAddTeamMembers(const Client* client, Team team, List<String> members);
	virtual void sendRemoveTeamMembers(const Client* client, Team team, List<String> members);

	NetworkHandler();
	virtual ~NetworkHandler();
	virtual void addClient(SOCKET& newClient);
	virtual void disconnectClient(LockedClient& client);
	virtual void start();
	virtual void startAsync();
	virtual void stop();

	/* Our friend, the Server */
	friend class Server;
};
