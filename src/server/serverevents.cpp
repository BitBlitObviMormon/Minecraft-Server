#include "debug.h"
#include "server/serverevents.h"

/* WindowType Enum */
const WindowType WindowType::Container        = "minecraft::container";
const WindowType WindowType::Chest            = "minecraft::chest";
const WindowType WindowType::CraftingTable    = "minecraft::crafting_table";
const WindowType WindowType::Furnace          = "minecraft::furnace";
const WindowType WindowType::Dispenser        = "minecraft::dispenser";
const WindowType WindowType::EnchantmentTable = "minecraft::enchanting_table";
const WindowType WindowType::BrewingStand     = "minecraft::brewing_stand";
const WindowType WindowType::Villager         = "minecraft::villager";
const WindowType WindowType::Beacon           = "minecraft::beacon";
const WindowType WindowType::Anvil            = "minecraft::anvil";
const WindowType WindowType::Hopper           = "minecraft::hopper";
const WindowType WindowType::Dropper          = "minecraft::dropper";
const WindowType WindowType::ShulkerBox       = "minecraft::shulker_box";
const WindowType WindowType::Animal           = "EntityHorse";
WindowType::WindowType(const char* string) : string(string) {}
WindowType::WindowType(const String string) : string(string) {}
WindowType::WindowType(const WindowType& type) : string(type.string) {}
const Boolean WindowType::operator==(WindowType second) const { return string == second.string; }
const Boolean WindowType::operator!=(WindowType second) const { return string != second.string; }
const String WindowType::str() const { return string; }

/* LevelType Enum */
const LevelType LevelType::Default     = "default";
const LevelType LevelType::SuperFlat   = "flat";
const LevelType LevelType::LargeBiomes = "largeBiomes";
const LevelType LevelType::Amplified   = "amplified";
const LevelType LevelType::Default_1_1 = "default_1_1";
LevelType::LevelType(const char* string) : string(string) {}
LevelType::LevelType(const String string) : string(string) {}
LevelType::LevelType(const LevelType& type) : string(type.string) {}
const Boolean LevelType::operator==(LevelType second) const { return string == second.string; }
const Boolean LevelType::operator!=(LevelType second) const { return string != second.string; }
const String LevelType::str() const { return string; }

/* ObjectiveType Enum */
const ObjectiveType ObjectiveType::Integer = "integer";
const ObjectiveType ObjectiveType::Hearts  = "hearts";
ObjectiveType::ObjectiveType(const char* string) : string(string) {}
ObjectiveType::ObjectiveType(const String string) : string(string) {}
ObjectiveType::ObjectiveType(const ObjectiveType& type) : string(type.string) {}
const Boolean ObjectiveType::operator==(ObjectiveType second) const { return string == second.string; }
const Boolean ObjectiveType::operator!=(ObjectiveType second) const { return string != second.string; }
const String ObjectiveType::str() const { return string; }

/* TeamNameTagVisibility Enum */
const TeamNameTagVisibility TeamNameTagVisibility::AlwaysShow        = "always";
const TeamNameTagVisibility TeamNameTagVisibility::HideForOtherTeams = "hideForOtherTeams";
const TeamNameTagVisibility TeamNameTagVisibility::HideForOwnTeam    = "hideForOwnTeam";
const TeamNameTagVisibility TeamNameTagVisibility::AlwaysHide        = "never";
TeamNameTagVisibility::TeamNameTagVisibility(const char* string) : string(string) {}
TeamNameTagVisibility::TeamNameTagVisibility(const String string) : string(string) {}
TeamNameTagVisibility::TeamNameTagVisibility(const TeamNameTagVisibility& type) : string(type.string) {}
const Boolean TeamNameTagVisibility::operator==(TeamNameTagVisibility second) const { return string == second.string; }
const Boolean TeamNameTagVisibility::operator!=(TeamNameTagVisibility second) const { return string != second.string; }
const String TeamNameTagVisibility::str() const { return string; }

/* TeamCollisionRule Enum */
const TeamCollisionRule TeamCollisionRule::AlwaysPush         = "always";
const TeamCollisionRule TeamCollisionRule::PushOtherTeamsOnly = "pushOtherTeams";
const TeamCollisionRule TeamCollisionRule::PushOwnTeamOnly    = "pushOwnTeam";
const TeamCollisionRule TeamCollisionRule::NeverPush          = "never";
TeamCollisionRule::TeamCollisionRule(const char* string) : string(string) {}
TeamCollisionRule::TeamCollisionRule(const String string) : string(string) {}
TeamCollisionRule::TeamCollisionRule(const TeamCollisionRule& type) : string(type.string) {}
const Boolean TeamCollisionRule::operator==(TeamCollisionRule second) const { return string == second.string; }
const Boolean TeamCollisionRule::operator!=(TeamCollisionRule second) const { return string != second.string; }
const String TeamCollisionRule::str() const { return string; }