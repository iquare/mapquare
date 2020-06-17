#pragma once
#include "TriggerSystem.h"
#include <vector>
#pragma pack(1)

struct Chunk {
	u32 name;
	u32 size;
};

struct VerChunk {
	Chunk header;
	u16 version;
};

struct TypeChunk {
	Chunk header;
	u32 scenario_type;
};

struct IverChunk {
	Chunk header;
	u16 additional_format;
};

struct Ive2Chunk {
	Chunk header;
	u16 additional_format;
};

struct VcodChunk {
	Chunk header;
	u32 seed[256];
	u8 opcodes[16];
	/*
	00 - XOR's the current hash with the total values of OWNR, SIDE * 256, FORC * 65536
	01 - Adds to the current hash with the total values of OWNR, SIDE * 256, FORC * 65536
	02 - Subtracts from the current hash with the total values of OWNR, SIDE * 256, FORC * 65536
	03 - XOR's the current hash with the VCOD table at index OWNR, SIDE, FORC, and 0
	04 - same as 0x03
	05 - same as 0x03
	06 - Complex function consisting of ORs and shifts
	07 - Inverted version of 0x06, switching direction of all shifts
	08+ - Do nothing
	*/
};

struct IownChunk {
	Chunk header;
	u8 player_type[12];
};

struct EraChunk {
	Chunk header;
	u16 tileset;
};
struct DimChunk {
	Chunk header;
	u16 width;
	u16 height;
};
struct SideChunk {
	Chunk header;
	u8 race[12];
};
struct MtxmChunk {
	Chunk header;
	std::vector<std::vector<u16>> tiles;//width*height
};
struct PuniChunk {
	Chunk header;
	u8 availability[228][12];
	u8 global_avail[228];
	u8 avail_override[228][12];
};
struct UpgrChunk {
	Chunk header;
	//only vanilla
	u8 max[46][12];
	u8 start[46][12];
	u8 global_max[46];
	u8 global_start[46];
	u8 avail_override[46][12];
};
struct PtecChunk {
	Chunk header;
	//only vanilla
	u8 avail[24][12];
	u8 researched[24][12];
	u8 global_avail[24];
	u8 global_researched[24];
	u8 player_override[24][12];
};

struct UnitChkElem {
	u32 class_instance;
	u16 x;
	u16 y;
	u16 id;
	u16 link;//nydus/addon
			 /*
			 Bit 9 - Nydus Link
			 Bit 10 - Addon Link
			 */
	u16 flags;
	/*
	Bit 0 - Cloak is valid
	Bit 1 - Burrow is valid
	Bit 2 - In transit is valid
	Bit 3 - Hallucinated is valid
	Bit 4 - Invincible is valid
	Bit 5-15 - Unused
	*/
	u16 property_flags;
	/*
	Bit 0 - Owner player is valid (the unit is not a critter, start location, etc.; not a neutral unit)
	Bit 1 - HP is valid
	Bit 2 - Shields is valid
	Bit 3 - Energy is valid (unit is a wraith, etc.)
	Bit 4 - Resource amount is valid (unit is a mineral patch, vespene geyser, etc.)
	Bit 5 - Amount in hangar is valid (unit is a reaver, carrier, etc.)
	Bit 6-15 - Unused
	*/
	u8 player;
	u8 hitpoints;
	u8 shields;
	u8 energy;//1-100
	u32 resource;
	u16 hangar;
	u16 state_flags;
	/*
	Bit 0 - Unit is cloaked
	Bit 1 - Unit is burrowed
	Bit 2 - Building is in transit
	Bit 3 - Unit is hallucinated
	Bit 4 - Unit is invincible
	Bit 5-15 - Unused
	*/
	u32 unused;
	u32 related_class_instance;//for links
};

struct UnitChunk {
	Chunk header;
	std::vector<UnitChkElem> units;
};
struct IsomChunk {
	Chunk header;
	std::vector<u16> isom_data;
	//u16[ (width / 2 + 1) * (height + 1) * 4 ]
};

struct TileChunk {
	Chunk header;
	std::vector<std::vector<u16>> tile_data;
	//width*height
};

struct ChkDd2Elem {
	u16 doodad_number;
	u16 x;
	u16 y;
	u8 player;
	u8 flag;
};

struct Dd2Chunk {
	Chunk header;
	std::vector<ChkDd2Elem> doodads;

	/*
	00 - Doodad is enabled (trap can attack, door is closed, etc)
	01 - Doodad is disabled
	*/
};
struct ChkThg2Elem {
	u16 unit_sprite_n;
	u16 x;
	u16 y;
	u8 player;
	u8 unused;
	u16 flags;
};
struct Thg2Chunk {
	Chunk header;
	std::vector<ChkThg2Elem> sprites;
	/*
	Bit 0-11 - Unused
	Bit 12 - Draw as sprite (Determines if it is a sprite or a unit sprite)
	Bit 13 - Unused
	Bit 14 - Unused
	Bit 15 - Disabled (Only valid if Draw as sprite is unchecked, disables the unit)
	*/
};

struct MaskChunk {

	Chunk header;
	std::vector<std::vector<u8>> fog;
	//bit 0-7 player 1-8 fog
};

struct StrChunk {
	Chunk header;
	u16 string_count;
	std::vector<u16> string_ids;//length is string count
	std::vector<std::string> strings_data;
	/*
	u16[Number of strings]: 1 integer for each string specifying the offset (the spot where the string starts in the section from the start of it).
	Strings: After the offsets, this is where every string in the map goes, one after another. Each one is terminated by a null character.
	*/
};

struct UprpChunk {
	Chunk header;
	u16 flag;
	/*
	Bit 0 - Cloak bit is valid
	Bit 1 - Burrowed bit is valid
	Bit 2 - In transit bit is valid
	Bit 3 - Hallucinated bit is valid
	Bit 4 - Invincible bit is valid
	Bit 5-15 - Unknown/unused
	*/
	u8 player;
	u8 hitpoints;//1-100%
	u8 shields;
	u8 energy;
	u32 resource;
	u16 hangar;
	u16 flags;
	/*
	Bit 0 - Unit is cloaked
	Bit 1 - Unit is burrowed
	Bit 2 - Building is in transit
	Bit 3 - Unit is hallucinated
	Bit 4 - Unit is invincible
	Bit 5-15 - Unknown/unused
	*/
	u32 unknown;
};

struct UpusChunk {
	Chunk header;
	u8 data[64];
	/*
	00 - Properties slot is unused
01 - Properties slot is used
	*/
};


struct ChkLocElem {
	
	u32 left;
	u32 top;
	u32 right;
	u32 bottom;
	u16 string;
	u16 elevation_flags;
	/*
	Bit 0 - Low elevation
	Bit 1 - Medium elevation
	Bit 2 - High elevation
	Bit 3 - Low air
	Bit 4 - Medium air
	Bit 5 - High air
	Bit 6-15 - Unused
	*/
};

struct MrgnChunk {
	Chunk header;
	std::vector<ChkLocElem> locs;
};

struct ChkTrigCondition {
	u32 location;
	u32 group;
	u32 number;//unit n/resource n
	u16 unit_id;
	u8 comparison;
	u8 condition_byte;
	u8 resource_type;
	u8 flags;
	/*
	Bit 0 - Unknown/unused
	Bit 1 - Enabled flag. If on, the trigger action/condition is disabled/ignored
	Bit 2 - Always display flag.
	Bit 3 - Unit properties is used. (Note: This is used in *.trg files)
	Bit 4 - Unit type is used. May not be necessary?
	Bit 5 - If on, the unit ID is used. Unnecessary.
	Bit 6-7 - Unknown/unused
	*/
	u16 condition_number;//Used internally by starcraft (number of which condition to process next, and maybe more?)
};


struct ChkTrigAction {
	u32 source;//location
	u32 string_number;
	u32 wav;
	u32 seconds;//or milliseconds
	u32 first_group;
	u32 second_group;
	u16 type;//unit,score,res,alliance
	u8 action_byte;
	u8 number_units;
	u8 flags;
	/*
	Bit 0 - Ignore a wait/transmission once.
	Bit 1 - Enabled flag. If on, the trigger action/condition is disabled.
	Bit 2 - Always display flag.
	Bit 3 - Unit properties is used. Staredit uses this for *.trg files.
	Bit 4 - Unit type is used.
	Bit 5 - If on, the unit ID is used. Unnecessary.
	*/
	u8 condition_number[3];//Used internally by starcraft
};

struct ChkTrigExecution {
	u32 flags;
	/*
	Bit 0 - All conditions are met, executing actions, cleared on the next trigger loop.
	Bit 1 - Ignore the following actions: Defeat, Draw.
	Bit 2 - Preserve trigger. (Can replace Preserve Trigger action)
	Bit 3 - Ignore execution.
	Bit 4 - Ignore all of the following actions for this trigger until the next trigger loop: Wait, PauseGame, Transmission, PlayWAV, DisplayTextMessage, CenterView, MinimapPing, TalkingPortrait, and MuteUnitSpeech.
	Bit 5 - This trigger has paused the game, ignoring subsequent calls to Pause Game (Unpause Game clears this flag only in the same trigger), may automatically call unpause at the end of action execution?
	Bit 6 - Wait skipping disabled for this trigger, cleared on next trigger loop.
	Bit 7-31 - Unknown/unused
	*/
	u8 u[28];
	/*
	1 byte for each player in the #List of Players/Group IDs
	00 - Trigger is not executed for player
	01 - Trigger is executed for player
	*/
};
struct TrigChunk {
	Chunk header;
	ChkTrigCondition conditions[16];//20*16=320
	ChkTrigAction actions[64];//32*64=2048
	ChkTrigExecution execution;//32
};
struct SprpChunk {
	Chunk header;
	u16 scenario_name_string_n;
	u16 scenario_description_string_n;
};

struct ForcChunk {
	Chunk header;
	u8 player_forces[8];
	u16 name_strnig_ids[4];
	u8 flags[4];
	/*
	Bit 0 - Random start location
Bit 1 - Allies
Bit 2 - Allied victory
Bit 3 - Shared vision
Bit 4-7 - Unused
	*/
};
struct WavChunk {
	Chunk header;
	u32 string_indexes[512];
};

struct UnisChunk {
	Chunk header;
	u8 unitids[228];//use or don't use default settings
	u32 hitpoints[228];
	u16 shields[228];
	u8 armor[228];
	u16 buildtime[228];
	u16 mineralcost[228];
	u16 gascost[228];
	u16 string_number[228];
	u16 basedamage[100];
	u16 upgradebonuswpn[100];
};

struct UpgsChunk {
	Chunk header;
	u8 upgradeids[228];//same
	u16 mineralcost[46];
	u16 mineralbonus[46];
	u16 gascost[46];
	u16 gasbonus[46];
	u16 timecost[46];
	u16 timebonus[46];
};

struct TecsChunk {
	Chunk header;
	u8 techids[24];
	u16 mineralcost[24];
	u16 gascost[24];
	u16 timecost[24];
	u16 energycost[24];
};
struct SwnmChunk {//switch names
	Chunk header;
	u32 string_number[256];
};
struct ColrChunk {
	Chunk header;
	u8 colors[8];//player colors
};
struct PupxChunk {
	Chunk header;
	

	u8 max[61][12];
	u8 start[61][12];
	u8 global_max[61];
	u8 global_start[61];
	u8 avail_override[61][12];
};

struct PtexChunk {
	Chunk header;

	u8 avail[44][12];
	u8 researched[44][12];
	u8 global_avail[44];
	u8 global_researched[44];
	u8 player_override[44][12];
};

struct UnixChunk {
	Chunk header;
	u8 unitids[228];//use or don't use default settings
	u32 hitpoints[228];
	u16 shields[228];
	u8 armor[228];
	u16 buildtime[228];
	u16 mineralcost[228];
	u16 gascost[228];
	u16 string_number[228];
	u16 basedamage[130];
	u16 upgradebonuswpn[130];
};
struct UpgxChunk {
	Chunk header;
	u8 byte[61];//uses custom or default
	u8 unused;
	u16 mineralcost[61];
	u16 mineralbonus[61];
	u16 gascost[61];
	u16 gasbonus[61];
	u16 timecost[61];
	u16 timebonus[61];
};

struct TecxChunk {
	Chunk header;

	u8 custom[44];
	u16 min_cost[44];
	u16 gas_cost[44];
	u16 time_req[44];
	u16 energy_cost[44];
};

struct OwnrChunk {
	Chunk header;
	u8 bytes[12];
	/*
	u8[12]: One byte for each player, specifies the owner of the player:
	00 - Inactive
	01 - Computer (game)
	02 - Occupied by Human Player
	03 - Rescue Passive
	04 - Unused
	05 - Computer
	06 - Human (Open Slot)
	07 - Neutral
	08 - Closed slot
	*/
};

class chk {
public:
	std::vector<VerChunk> ver_chunks;
	std::vector<TypeChunk> type_chunks;
	std::vector<IverChunk> iver_chunks;
	std::vector<Ive2Chunk> ive2_chunks;
	std::vector<VcodChunk> vcod_chunks;
	std::vector<IownChunk> iown_chunks;
	std::vector<EraChunk> era_chunks;
	std::vector<DimChunk> dim_chunks;
	std::vector<SideChunk> side_chunks;
	std::vector<MtxmChunk> mtxm_chunks;
	std::vector<PuniChunk> puni_chunks;
	std::vector<UpgrChunk> upgr_chunks;
	std::vector<PtecChunk> ptec_chunks;
	std::vector<PtexChunk> ptex_chunks;
	std::vector<UnitChunk> unit_chunks;
	std::vector<IsomChunk> isom_chunks;
	std::vector<TileChunk> tile_chunks;
	std::vector<Dd2Chunk> dd2_chunks;
	std::vector<Thg2Chunk> thg2_chunks;
	std::vector<MaskChunk> mask_chunks;
	std::vector<StrChunk> str_chunks;
	std::vector<UprpChunk> uprp_chunks;
	std::vector<UpusChunk> upus_chunks;
	std::vector<MrgnChunk> mrgn_chunks;
	std::vector<TrigChunk> trig_chunks;
	std::vector<TrigChunk> mbfr_chunks;//same as trig
	std::vector<SprpChunk> sprp_chunks;
	std::vector<ForcChunk> forc_chunks;
	std::vector<WavChunk> wav_chunks;
	std::vector<UnisChunk> unis_chunks;
	std::vector<UpgsChunk> upgs_chunks;
	std::vector<TecsChunk> tecs_chunks;
	std::vector<SwnmChunk> swnm_chunks;
	std::vector<ColrChunk> colr_chunks;
	std::vector<PupxChunk> pupx_chunks;
	std::vector<UnixChunk> unix_chunks;
	std::vector<UpgxChunk> upgx_chunks;
	std::vector<TecxChunk> tecx_chunks;
	std::vector<OwnrChunk> ownr_chunks;
};
