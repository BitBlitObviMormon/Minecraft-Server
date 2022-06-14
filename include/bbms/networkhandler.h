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

	/* Get a client from its socket alone */
	virtual LockedClient* getClientFromSocket(SOCKET& socket);

	/* Send all of the data */
	virtual Int send(SOCKET s, const char* buf, Int len, Int flags);

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
	virtual void loginPluginResponse(LockedClient& client, const Byte* buffer, Int length);

	/* GAMEPLAY EVENTS */
	virtual void teleportConfirm(LockedClient& client, const Byte* buffer, Int length);
	virtual void queryBlockNBT(LockedClient& client, const Byte* buffer, Int length);
	virtual void setDifficulty(LockedClient& client, const Byte* buffer, Int length);
	virtual void tabComplete(LockedClient& client, const Byte* buffer, Int length);
	virtual void chatMessage(LockedClient& client, const Byte* buffer, Int length);
	virtual void clientStatus(LockedClient& client, const Byte* buffer, Int length);
	virtual void clientSettings(LockedClient& client, const Byte* buffer, Int length);
	virtual void clickWindowButton(LockedClient& client, const Byte* buffer, Int length);
	virtual void clickWindow(LockedClient& client, const Byte* buffer, Int length);
	virtual void closeWindow(LockedClient& client, const Byte* buffer, Int length);
	virtual void pluginMessage(LockedClient& client, const Byte* buffer, Int length);
	virtual void editBook(LockedClient& client, const Byte* buffer, Int length);
	virtual void queryEntityNBT(LockedClient& client, const Byte* buffer, Int length);
	virtual void interactEntity(LockedClient& client, const Byte* buffer, Int length);
	virtual void generateStructure(LockedClient& client, const Byte* buffer, Int length);
	virtual void keepAlive(LockedClient& client, const Byte* buffer, Int length);
	virtual void lockDifficulty(LockedClient& client, const Byte* buffer, Int length);
	virtual void playerPosition(LockedClient& client, const Byte* buffer, Int length);
	virtual void playerPositionAndRotation(LockedClient& client, const Byte* buffer, Int length);
	virtual void playerRotation(LockedClient& client, const Byte* buffer, Int length);
	virtual void playerMovement(LockedClient& client, const Byte* buffer, Int length);
	virtual void vehicleMove(LockedClient& client, const Byte* buffer, Int length);
	virtual void steerBoat(LockedClient& client, const Byte* buffer, Int length);
	virtual void pickItem(LockedClient& client, const Byte* buffer, Int length);
	virtual void craftRecipeRequest(LockedClient& client, const Byte* buffer, Int length);
	virtual void playerAbilities(LockedClient& client, const Byte* buffer, Int length);
	virtual void playerDigging(LockedClient& client, const Byte* buffer, Int length);
	virtual void entityAction(LockedClient& client, const Byte* buffer, Int length);
	virtual void steerVehicle(LockedClient& client, const Byte* buffer, Int length);
	virtual void pong(LockedClient& client, const Byte* buffer, Int length);
	virtual void setRecipeBookState(LockedClient& client, const Byte* buffer, Int length);
	virtual void setDisplayedRecipe(LockedClient& client, const Byte* buffer, Int length);
	virtual void nameItem(LockedClient& client, const Byte* buffer, Int length);
	virtual void resourcePackStatus(LockedClient& client, const Byte* buffer, Int length);
	virtual void advancementTab(LockedClient& client, const Byte* buffer, Int length);
	virtual void selectTrade(LockedClient& client, const Byte* buffer, Int length);
	virtual void setBeaconEffect(LockedClient& client, const Byte* buffer, Int length);
	virtual void heldItemChange(LockedClient& client, const Byte* buffer, Int length);
	virtual void updateCommandBlock(LockedClient& client, const Byte* buffer, Int length);
	virtual void updateCommandBlockMinecart(LockedClient& client, const Byte* buffer, Int length);
	virtual void creativeInventoryAction(LockedClient& client, const Byte* buffer, Int length);
	virtual void updateJigsawBlock(LockedClient& client, const Byte* buffer, Int length);
	virtual void updateStructureBlock(LockedClient& client, const Byte* buffer, Int length);
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
	virtual Int sendResponse(const Client* client, String json);
	virtual Int sendPong(const Client* client, Long payload);

	/* Login Packets */
	virtual Int sendDisconnect(const Client* client, String reason);
	virtual Int sendEncryptionRequest(const Client* client, String serverID, Byte* publicKey, Int publicKeyLen, Byte* verifyToken, Int verifyTokenLen);
	virtual Int sendLoginSuccess(const Client* client, UUID uuid, String username);
	virtual Int sendSetCompression(const Client* client, Int maxPacketSize);

	/* Play Packets */
	virtual Int sendSpawnObject(const Client* client, Object object);
	virtual Int sendSpawnExperienceOrb(const Client* client, ExperienceOrb xpOrb);
	virtual Int sendGlobalEntity(const Client* client, GlobalEntity entity);
	virtual Int sendSpawnMob(const Client* client, Entity mob);
	virtual Int sendSpawnPainting(const Client* client, Painting painting);
	virtual Int sendSpawnPlayer(const Client* client, PlayerEntity player);
	virtual Int sendAnimation(const Client* client, Int entityID, Animation animation);
	virtual Int sendStatistics(const Client* client, Statistic stat);
	virtual Int sendBlockBreakAnimation(const Client* client, Int entityID, Position pos, Byte destroyStage);
	virtual Int sendUpdateBlockEntity(const Client* client, BlockEntity entity);
	virtual Int sendBlockAction(const Client* client); // TODO: Add args
	virtual Int sendBlockChange(const Client* client, Block block);
	virtual Int sendBossBar(const Client* client, BossBar bar, BossBarAction action);
	virtual Int sendServerDifficulty(const Client* client, Difficulty difficulty);
	virtual Int sendTabComplete(const Client* client, String result);
	virtual Int sendChatMessage(const Client* client, String message, ChatMessageType type = ChatMessageType::Chat, Boolean isJson = false);
	virtual Int sendConfirmTransaction(const Client* client, Byte windowID, Short actionNum, Boolean accepted);
	virtual Int sendCloseWindow(const Client* client, Byte windowID);
	virtual Int sendOpenWindow(const Client* client, Window window);
	virtual Int sendWindowItems(const Client* client); // TODO: Add args
	virtual Int sendWindowProperty(const Client* client, Byte windowID, Short Property, Short Value);
	virtual Int sendSetSlot(const Client* client); // TODO: Add args
	virtual Int sendSetCooldown(const Client* client, Int itemID, Int numTicks);
	virtual Int sendPluginMessage(const Client* client, String channel, Byte* data, Int dataLen);
	virtual Int sendNamedSoundEffect(const Client* client, String name, SoundCategory category, Position pos, Float volume, Float pitch);
	virtual Int sendEntityStatus(const Client* client, Int entityID, Byte entityStatus);
	virtual Int sendUnloadChunk(const Client* client, Int x, Int z);
	virtual Int sendUnloadChunk(const Client* client, std::pair<Int, Int> chunk) { return sendUnloadChunk(client, chunk.first, chunk.second); }
	virtual Int sendChangeGameState(const Client* client, GameStateReason reason, Float value); // FLOAT?? TODO: See if value should be int
	virtual Int sendKeepAlive(const Client* client, Long id);
	virtual Int sendChunkData(const Client* client); // TODO: Add args
	virtual Int sendChunk(const Client* client, Int x, Int z, ChunkColumn& column, Boolean createChunk = false, Boolean inOverworld = true);
	virtual Int sendChunk(const Client* client, Int x, Int z, Boolean createChunk = false, Boolean inOverworld = true);
	void sendChunk(const Client* client, std::pair<Int, Int> chunk, Boolean createChunk = false, Boolean inOverworld = true)
		{ sendChunk(client, chunk.first, chunk.second, createChunk, inOverworld); }
	void sendChunk(const Client* client, std::pair<Int, Int> chunk, ChunkColumn& column, Boolean createChunk = false, Boolean inOverworld = true)
		{ sendChunk(client, chunk.first, chunk.second, column, createChunk, inOverworld); }
	virtual Int sendEffect(const Client* client, EffectID effectID, Position pos, Int data = 0, Boolean disableRelativeVolume = false);
	virtual Int sendParticle(const Client* client, Particle particle, Int num, Byte* data = nullptr, Int dataLen = 0);
	virtual Int sendJoinGame(const Client* client, Int entityID, Gamemode gamemode, Dimension dimension, Difficulty difficulty, Byte maxPlayers, LevelType levelType, Boolean reducedDebugInfo = false);
	virtual Int sendMap(const Client* client, Map map);
	virtual Int sendEntityRelativeMove(const Client* client, Int entityID, PositionF deltaPos, Boolean isOnGround);
	virtual Int sendEntityLookAndRelativeMove(const Client* client, Int entityID, PositionF deltaPos, Byte yaw, Byte pitch, Boolean isOnGround);
	virtual Int sendEntityLook(const Client* client, Int entityID, Byte yaw, Byte pitch, Boolean isOnGround);
	virtual Int sendEntity(const Client* client, Int entityID);
	virtual Int sendVehicleMove(const Client* client, PositionF position, Float yaw, Float pitch);
	virtual Int sendOpenSignEditor(const Client* client, Position pos);
	virtual Int sendCraftRecipeResponse(const Client* client, Byte windowID, Int recipeID);
	virtual Int sendPlayerAbilities(const Client* client, PlayerAbilities abilities, Float flyingSpeed, Float fovModifier);
	virtual Int sendEnterCombatEvent(const Client* client);
	virtual Int sendEndCombatEvent(const Client* client, Int duration, Int entityID);
	virtual Int sendEntityKilledEvent(const Client* client, Int killerID, Int killedID, String message);
	virtual Int sendPlayerListItem(const Client* client); // TODO: Add args
	virtual Int sendPlayerPositionAndLook(const Client* client, PositionF pos, Float yaw, Float pitch, PlayerPositionAndLookFlags flags, Int teleportID, Boolean dismountVehicle);
	virtual Int sendUseBed(const Client* client, Int entityID, Position pos);
	virtual Int sendRemoveEntityEffect(const Client* client, Int entityID, StatusEffect effect);
	virtual Int sendResourcePack(const Client* client, String url, String hash);
	virtual Int sendRespawn(const Client* client, Dimension dimension, Difficulty difficulty, Gamemode gamemode, LevelType level);
	virtual Int sendEntityHeadLook(const Client* client, Int entityID, Float yaw);
	virtual Int sendSelectAdvancementTab(const Client* client, String id = "");
	virtual Int sendWorldBorder(const Client* client); // TODO: Add args
	virtual Int sendCamera(const Client* client, Int entityID);
	virtual Int sendHeldItemChange(const Client* client, Byte hotbarSlot);
	virtual Int sendDisplayScoreboard(const Client* client, ScoreboardPosition position, String name);
	virtual Int sendEntityMetadata(const Client* client); // TODO: Add args
	virtual Int sendAttachEntity(const Client* client, Int leashedEntityID, Int holdingEntityID);
	virtual Int sendDetachEntity(const Client* client, Int entityID) { return sendAttachEntity(client, entityID, -1); }
	virtual Int sendEntityVelocity(const Client* client, Int entityID, PositionF velocity);
	virtual Int sendEntityEquipment(const Client* client, Int entityID, EntityEquipmentSlot slot, Slot item); // TODO: Change item var type
	virtual Int sendSetExperience(const Client* client, Float xpBar/* Between 0 & 1 */, Int level); // Converts to (Float xpBar, Int level, Int totalXP)
	virtual Int sendSetExperience(const Client* client, Int totalXP); // Converts to (Float xpBar, Int level, Int totalXP)
	virtual Int sendUpdateHealth(const Client* client, Float hp, Int food, Float foodSaturation);
	virtual Int sendScoreboardObjective(const Client* client, String name, ScoreboardMode mode, String displayName = "", ObjectiveType type = "");
	virtual Int sendRemoveTeam(const Client* client, String name);
	virtual Int sendUpdateScore(const Client* client, String entityName, String objectiveName, Int value);
	virtual Int sendRemoveScore(const Client* client, String entityName, String objectiveName);
	virtual Int sendSpawnPosition(const Client* client, Position pos);
	virtual Int sendTimeUpdate(const Client* client, Long worldAge, Long timeOfDay);
	virtual Int sendSetTitle(const Client* client, String title);
	virtual Int sendSetSubtitle(const Client* client, String subtitle);
	virtual Int sendSetActionBar(const Client* client, String text);
	virtual Int sendTitleFadeout(const Client* client, Int timeBeforeFade, Int fadeInLength, Int fadeOutLength);
	virtual Int sendHideTitle(const Client* client);
	virtual Int sendResetTitle(const Client* client);
	virtual Int sendSoundEffect(const Client* client, Sound sound, SoundCategory category, Position position, Float volume, Float pitch);
	virtual Int sendPlayerListHeaderAndFooter(const Client* client, String header, String footer);
	virtual Int sendCollectItem(const Client* client, Int collectedID, Int collectorID, Int count);
	virtual Int sendEntityTeleport(const Client* client, Int entityID, PositionF pos, Float yaw, Float pitch, Boolean onGround);
	virtual Int sendAdvancements(const Client* client); // TODO: Add args
	virtual Int sendEntityProperties(const Client* client); // TODO: Add args
	virtual Int sendEntityEffect(const Client* client, Int entityID, StatusEffect effect, Int level, Int duration, StatusEffectFlags flags);
	virtual Int sendStatistics(const Client* client, List<Int> statistics); // TODO: Create Statistic class
	virtual Int sendTabComplete(const Client* client, List<String> results);
	virtual Int sendBlockChanges(const Client* client, List<Block> blocks);
	virtual Int sendExplosion(const Client* client, PositionF pos, Float radius, PositionF newClientVelocity, List<Position> blocksToRemove);
	virtual Int sendUnlockRecipes(const Client* client, List<Int> recipes1, List<Int> recipes2, Boolean craftingBookOpen, Boolean filteringCraftable); // TODO: Implement Recipe class!
	virtual Int sendDestroyEntities(const Client* client, List<Int> entities);
	virtual Int sendSetPassengers(const Client* client, Int vehicleID, List<Int> passengers);
	virtual Int sendCreateTeam(const Client* client, List<String> team);
	virtual Int sendUpdateTeamInfo(const Client* client, Team team); // Ignores member list
	virtual Int sendAddTeamMembers(const Client* client, Team team, List<String> members);
	virtual Int sendRemoveTeamMembers(const Client* client, Team team, List<String> members);
	virtual Int sendLegacyKick(const Client* client);

	NetworkHandler();
	virtual ~NetworkHandler();
	virtual void addClient(SOCKET& newClient);
	virtual void disconnectClient(std::shared_ptr<const Client> client);
	virtual void start();
	virtual void startAsync();
	virtual void stop();

	/* Our friend, the Server */
	friend class Server;
};
