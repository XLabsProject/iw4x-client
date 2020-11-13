#pragma once

namespace Game
{
	template <typename T> static void DB_ConvertOffsetToPointer(T* pointer)
	{
		Utils::Hook::Call<void(T*)>(0x4A82B0)(pointer);
	}
	template <typename T> static T** DB_InsertPointer()
	{
		static auto DB_InsertPointer_Address = 0x43B290;
		T** retval = nullptr;
		
		__asm
		{
			call DB_InsertPointer_Address;
			mov retval, eax;
		}

		return retval;
	}

	std::vector<std::string> Sys_ListFilesWrapper(const std::string& directory, const std::string& extension);
	
	typedef void(__cdecl * AddRefToObject_t)(unsigned int id);
	extern AddRefToObject_t AddRefToObject;

	typedef unsigned int(__cdecl * AllocObject_t)();
	extern AllocObject_t AllocObject;

	typedef void(__cdecl * AngleVectors_t)(float *angles, float *forward, float *right, float *up);
	extern AngleVectors_t AngleVectors;

	typedef unsigned int(__cdecl * BG_GetNumWeapons_t)();
	extern BG_GetNumWeapons_t BG_GetNumWeapons;

	typedef const char*(__cdecl * BG_GetWeaponName_t)(unsigned int index);
	extern BG_GetWeaponName_t BG_GetWeaponName;

	typedef void*(__cdecl * BG_LoadWeaponDef_LoadObj_t)(const char* filename);
	extern BG_LoadWeaponDef_LoadObj_t BG_LoadWeaponDef_LoadObj;

	typedef void(__cdecl * Cbuf_AddServerText_t)();
	extern Cbuf_AddServerText_t Cbuf_AddServerText;

	typedef void(__cdecl * Cbuf_AddText_t)(int localClientNum, const char *text);
	extern Cbuf_AddText_t Cbuf_AddText;

	typedef int(__cdecl * CG_GetClientNum_t)();
	extern CG_GetClientNum_t CG_GetClientNum;

	typedef std::int32_t(__cdecl* CG_PlayBoltedEffect_t) (std::int32_t, FxEffectDef*, std::int32_t, std::uint32_t);
	extern CG_PlayBoltedEffect_t CG_PlayBoltedEffect;

	typedef std::int32_t(__cdecl* CG_GetBoneIndex_t)(std::int32_t, std::uint32_t name, char* index);
	extern CG_GetBoneIndex_t CG_GetBoneIndex;
	
	typedef char*(__cdecl * CL_GetClientName_t)(int localClientNum, int index, char *buf, size_t size);
	extern CL_GetClientName_t CL_GetClientName;

	typedef int(__cdecl * CL_IsCgameInitialized_t)();
	extern CL_IsCgameInitialized_t CL_IsCgameInitialized;

	typedef void(__cdecl * CL_ConnectFromParty_t)(int controllerIndex, _XSESSION_INFO *hostInfo, netadr_t addr, int numPublicSlots, int numPrivateSlots, const char *mapname, const char *gametype);
	extern CL_ConnectFromParty_t CL_ConnectFromParty;

	typedef void(__cdecl * CL_DownloadsComplete_t)(int controller);
	extern CL_DownloadsComplete_t CL_DownloadsComplete;

	typedef void(_cdecl * CL_DrawStretchPicPhysical_t)(float x, float y, float w, float h, float xScale, float yScale, float xay, float yay, const float *color, Game::Material* material);
	extern CL_DrawStretchPicPhysical_t CL_DrawStretchPicPhysical;

	typedef const char*(_cdecl* CL_GetConfigString_t)(int index);
	extern CL_GetConfigString_t CL_GetConfigString;

	typedef int(_cdecl* CL_GetMaxRank_t)();
	extern CL_GetMaxRank_t CL_GetMaxRank;

	typedef int(_cdecl* CL_GetRankForXP_t)(int xp);
	extern CL_GetRankForXP_t CL_GetRankForXP;

	typedef void(__cdecl * CL_GetRankIcon_t)(int level, int prestige, Material** material);
	extern CL_GetRankIcon_t CL_GetRankIcon;

	typedef void(__cdecl * CL_HandleRelayPacket_t)(Game::msg_t* msg, int client);
	extern CL_HandleRelayPacket_t CL_HandleRelayPacket;

	typedef void(__cdecl * CL_ResetViewport_t)();
	extern CL_ResetViewport_t CL_ResetViewport;

	typedef void(__cdecl * CL_SelectStringTableEntryInDvar_f_t)();
	extern CL_SelectStringTableEntryInDvar_f_t CL_SelectStringTableEntryInDvar_f;

	typedef void(__cdecl * Cmd_AddCommand_t)(const char* cmdName, void(*function), cmd_function_t* allocedCmd, bool isKey);
	extern Cmd_AddCommand_t Cmd_AddCommand;

	typedef void(__cdecl * Cmd_AddServerCommand_t)(const char* name, void(*callback), cmd_function_t* data);
	extern Cmd_AddServerCommand_t Cmd_AddServerCommand;

	typedef void(__cdecl * Cmd_ExecuteSingleCommand_t)(int localClientNum, int controllerIndex, const char* cmd);
	extern Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand;

	typedef void(__cdecl * Com_ClientPacketEvent_t)();
	extern Com_ClientPacketEvent_t Com_ClientPacketEvent;

	typedef void(__cdecl * Com_Error_t)(int type, const char* message, ...);
	extern Com_Error_t Com_Error;

	typedef void(__cdecl * Com_Printf_t)(int channel, const char *fmt, ...);
	extern Com_Printf_t Com_Printf;

	typedef void(__cdecl * Com_PrintMessage_t)(int channel, const char *msg, int error);
	extern Com_PrintMessage_t Com_PrintMessage;

	typedef void(__cdecl * Com_EndParseSession_t)();
	extern Com_EndParseSession_t Com_EndParseSession;

	typedef void(__cdecl * Com_BeginParseSession_t)(const char* why);
	extern Com_BeginParseSession_t Com_BeginParseSession;

	typedef void(__cdecl * Com_SetSpaceDelimited_t)(int);
	extern Com_SetSpaceDelimited_t Com_SetSpaceDelimited;

	typedef char* (__cdecl * Com_Parse_t)(const char **data_p);
	extern Com_Parse_t Com_Parse;

	typedef bool (__cdecl * Com_MatchToken_t)(const char **data_p, const char* token, int size);
	extern Com_MatchToken_t Com_MatchToken;

	typedef void(__cdecl * Com_SetSlowMotion_t)(float start, float end, int duration);
	extern Com_SetSlowMotion_t Com_SetSlowMotion;

	typedef char* (__cdecl * Con_DrawMiniConsole_t)(int localClientNum, int xPos, int yPos, float alpha);
	extern Con_DrawMiniConsole_t Con_DrawMiniConsole;

	typedef void (__cdecl * Con_DrawSolidConsole_t)();
	extern Con_DrawSolidConsole_t Con_DrawSolidConsole;

	typedef char *(__cdecl *DB_AllocStreamPos_t)(int alignment);
	extern DB_AllocStreamPos_t DB_AllocStreamPos;

	typedef void(__cdecl * DB_PushStreamPos_t)(unsigned int index);
	extern DB_PushStreamPos_t DB_PushStreamPos;

	typedef void(__cdecl * DB_PopStreamPos_t)();
	extern DB_PopStreamPos_t DB_PopStreamPos;

	typedef void(__cdecl * DB_BeginRecoverLostDevice_t)();
	extern DB_BeginRecoverLostDevice_t DB_BeginRecoverLostDevice;

	typedef void(__cdecl * DB_EndRecoverLostDevice_t)();
	extern DB_EndRecoverLostDevice_t DB_EndRecoverLostDevice;

	typedef void(__cdecl * DB_EnumXAssets_t)(XAssetType type, void(*)(XAssetHeader, void *), void* userdata, bool overrides);
	extern DB_EnumXAssets_t DB_EnumXAssets;

	typedef void(__cdecl * DB_EnumXAssets_Internal_t)(XAssetType type, void(*)(XAssetHeader, void *), void* userdata, bool overrides);
	extern DB_EnumXAssets_Internal_t DB_EnumXAssets_Internal;

	typedef XAssetHeader (__cdecl * DB_FindXAssetHeader_t)(XAssetType type, const char* name);
	extern DB_FindXAssetHeader_t DB_FindXAssetHeader;

	typedef float(__cdecl * DB_GetLoadedFraction_t)();
	extern DB_GetLoadedFraction_t DB_GetLoadedFraction;

	typedef const char* (__cdecl * DB_GetXAssetNameHandler_t)(XAssetHeader* asset);
	extern DB_GetXAssetNameHandler_t* DB_GetXAssetNameHandlers;

	typedef int(__cdecl * DB_GetXAssetSizeHandler_t)();
	extern DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandlers;

	typedef const char *(__cdecl * DB_GetXAssetTypeName_t)(XAssetType type);
	extern DB_GetXAssetTypeName_t DB_GetXAssetTypeName;

	typedef const char *(__cdecl * DB_IsXAssetDefault_t)(XAssetType type, const char* name);
	extern DB_IsXAssetDefault_t DB_IsXAssetDefault;

	typedef void(__cdecl * DB_GetRawBuffer_t)(RawFile* rawfile, char* buffer, int size);
	extern DB_GetRawBuffer_t DB_GetRawBuffer;

	typedef int(__cdecl * DB_GetRawFileLen_t)(RawFile* rawfile);
	extern DB_GetRawFileLen_t DB_GetRawFileLen;

	typedef void(*DB_LoadXAssets_t)(XZoneInfo *zoneInfo, unsigned int zoneCount, int sync);
	extern DB_LoadXAssets_t DB_LoadXAssets;

	typedef void(*DB_LoadXFileData_t)(char *pos, int size);
	extern DB_LoadXFileData_t DB_LoadXFileData;

	typedef void(__cdecl * DB_ReadXFileUncompressed_t)(void* buffer, int size);
	extern DB_ReadXFileUncompressed_t DB_ReadXFileUncompressed;

	typedef void(__cdecl * DB_ReadXFile_t)(void* buffer, int size);
	extern DB_ReadXFile_t DB_ReadXFile;

	typedef void(__cdecl * DB_ReleaseXAssetHandler_t)(XAssetHeader header);
	extern DB_ReleaseXAssetHandler_t* DB_ReleaseXAssetHandlers;

	typedef void(__cdecl * DB_SetXAssetName_t)(XAsset* asset, const char* name);
	extern DB_SetXAssetName_t DB_SetXAssetName;

	typedef void(__cdecl * DB_SetXAssetNameHandler_t)(XAssetHeader* header, const char* name);
	extern DB_SetXAssetNameHandler_t* DB_SetXAssetNameHandlers;

	typedef void(__cdecl * DB_XModelSurfsFixup_t)(XModel* model);
	extern DB_XModelSurfsFixup_t DB_XModelSurfsFixup;

	typedef dvar_t* (__cdecl * Dvar_RegisterBool_t)(const char* name, bool defaultVal, int flags, const char* description);
	extern Dvar_RegisterBool_t Dvar_RegisterBool;

	typedef dvar_t* (__cdecl * Dvar_RegisterFloat_t)(const char* name, float defaultVal, float min, float max, int flags, const char* description);
	extern Dvar_RegisterFloat_t Dvar_RegisterFloat;

	typedef dvar_t* (__cdecl * Dvar_RegisterVec2_t)(const char* name, float defx, float defy, float min, float max, int flags, const char* description);
	extern Dvar_RegisterVec2_t Dvar_RegisterVec2;

	typedef dvar_t* (__cdecl * Dvar_RegisterVec3_t)(const char* name, float defx, float defy, float defz, float min, float max, int flags, const char* description);
	extern Dvar_RegisterVec3_t Dvar_RegisterVec3;

	typedef dvar_t* (__cdecl * Dvar_RegisterVec4_t)(const char* name, float defx, float defy, float defz, float defw, float min, float max, int flags, const char* description);
	extern Dvar_RegisterVec4_t Dvar_RegisterVec4;

	typedef dvar_t* (__cdecl * Dvar_RegisterInt_t)(const char* name, int defaultVal, int min, int max, int flags, const char* description);
	extern Dvar_RegisterInt_t Dvar_RegisterInt;

	typedef dvar_t* (__cdecl * Dvar_RegisterEnum_t)(const char* name, char** enumValues, int defaultVal, int flags, const char* description);
	extern Dvar_RegisterEnum_t Dvar_RegisterEnum;

	typedef dvar_t* (__cdecl * Dvar_RegisterString_t)(const char* name, const char* defaultVal, int, const char*);
	extern Dvar_RegisterString_t Dvar_RegisterString;

	typedef dvar_t* (__cdecl * Dvar_RegisterColor_t)(const char* name, float r, float g, float b, float a, int flags, const char* description);
	extern Dvar_RegisterColor_t Dvar_RegisterColor;

	typedef dvar_t*(__cdecl * Dvar_SetFromStringByName_t)(const char* cvar, const char* value);
	extern Dvar_SetFromStringByName_t Dvar_SetFromStringByName;

	typedef dvar_t*(__cdecl * Dvar_SetFromStringByNameFromSource_t)(const char* cvar, const char* value, DvarSetSource source);
	extern Dvar_SetFromStringByNameFromSource_t Dvar_SetFromStringByNameFromSource;

	typedef void(__cdecl * Dvar_SetStringByName_t)(const char* cvar, const char* value);
	extern Dvar_SetStringByName_t Dvar_SetStringByName;

	typedef void(__cdecl * Dvar_GetUnpackedColorByName_t)(const char* name, float* color);
	extern Dvar_GetUnpackedColorByName_t Dvar_GetUnpackedColorByName;

	typedef dvar_t* (__cdecl * Dvar_FindVar_t)(const char *dvarName);
	extern Dvar_FindVar_t Dvar_FindVar;

	typedef char* (__cdecl* Dvar_InfoString_Big_t)(int typeMask);
	extern Dvar_InfoString_Big_t Dvar_InfoString_Big;

	typedef dvar_t* (__cdecl * Dvar_SetCommand_t)(const char* name, const char* value);
	extern Dvar_SetCommand_t Dvar_SetCommand;

	typedef bool(__cdecl * Encode_Init_t)(const char* );
	extern Encode_Init_t Encode_Init;

	typedef void(__cdecl * Field_Clear_t)(void* field);
	extern Field_Clear_t Field_Clear;

	typedef void(__cdecl * FreeMemory_t)(void* buffer);
	extern FreeMemory_t FreeMemory;

	typedef void(__cdecl * FS_FreeFile_t)(void* buffer);
	extern FS_FreeFile_t FS_FreeFile;

	typedef int(__cdecl * FS_ReadFile_t)(const char* path, char** buffer);
	extern FS_ReadFile_t FS_ReadFile;

	typedef char** (__cdecl * FS_GetFileList_t)(const char *path, const char *extension, FsListBehavior_e behavior, int *numfiles, int allocTrackType);
	extern FS_GetFileList_t FS_GetFileList;

	typedef void(__cdecl * FS_FreeFileList_t)(char** list);
	extern FS_FreeFileList_t FS_FreeFileList;

	typedef int(__cdecl * FS_FOpenFileAppend_t)(const char* file);
	extern FS_FOpenFileAppend_t FS_FOpenFileAppend;
	extern FS_FOpenFileAppend_t FS_FOpenFileWrite;

	typedef int(__cdecl * FS_FOpenFileRead_t)(const char* file, int* fh/*, int uniqueFile*/);
	extern FS_FOpenFileRead_t FS_FOpenFileRead;

	typedef int(__cdecl * FS_FOpenFileReadForThread_t)(const char *filename, int *file, int thread);
	extern FS_FOpenFileReadForThread_t FS_FOpenFileReadForThread;

	typedef int(__cdecl * FS_FCloseFile_t)(int fh);
	extern FS_FCloseFile_t FS_FCloseFile;

	typedef bool(__cdecl * FS_FileExists_t)(const char* file);
	extern FS_FileExists_t FS_FileExists;

	typedef bool(__cdecl * FS_WriteFile_t)(char* filename, char* folder, void* buffer, int size);
	extern FS_WriteFile_t FS_WriteFile;

	typedef int(__cdecl * FS_Write_t)(const void* buffer, size_t size, int file);
	extern FS_Write_t FS_Write;

	typedef int(__cdecl * FS_Read_t)(void* buffer, size_t size, int file);
	extern FS_Read_t FS_Read;

	typedef int(__cdecl * FS_Seek_t)(int fileHandle, int seekPosition, int seekOrigin);
	extern FS_Seek_t FS_Seek;

	typedef int(__cdecl * FS_FTell_t)(int fileHandle);
	extern FS_FTell_t FS_FTell;

	typedef int(__cdecl * FS_Remove_t)(char *);
	extern FS_Remove_t FS_Remove;

	typedef int(__cdecl * FS_Restart_t)(int localClientNum, int checksumFeed);
	extern FS_Restart_t FS_Restart;

	typedef int(__cdecl * FS_BuildPathToFile_t)(const char*, const char*, const char*, char**);
	extern FS_BuildPathToFile_t FS_BuildPathToFile;

	typedef iwd_t*(__cdecl * FS_IsShippedIWD_t)(const char* fullpath, const char* iwd);
	extern FS_IsShippedIWD_t FS_IsShippedIWD;

	typedef int(__cdecl* G_GetWeaponIndexForName_t)(char*);
	extern G_GetWeaponIndexForName_t G_GetWeaponIndexForName;

	typedef void(__cdecl* G_SpawnEntitiesFromString_t)();
	extern G_SpawnEntitiesFromString_t G_SpawnEntitiesFromString;

	typedef void(__cdecl * GScr_LoadGameTypeScript_t)();
	extern GScr_LoadGameTypeScript_t GScr_LoadGameTypeScript;

	typedef int(__cdecl * Reader_t)(char const*, int *);
	typedef bool(__cdecl * Image_LoadFromFileWithReader_t)(GfxImage* image, Reader_t reader);
	extern Image_LoadFromFileWithReader_t Image_LoadFromFileWithReader;

	typedef void(__cdecl * Image_Release_t)(GfxImage* image);
	extern Image_Release_t Image_Release;

	typedef char*(__cdecl * Info_ValueForKey_t)(const char* infoString, const char* key);
	extern Info_ValueForKey_t Info_ValueForKey;

	typedef void(__cdecl * Key_SetCatcher_t)(int localClientNum, int catcher);
	extern Key_SetCatcher_t Key_SetCatcher;

	typedef void(__cdecl * LargeLocalInit_t)();
	extern LargeLocalInit_t LargeLocalInit;

	typedef bool(__cdecl * Load_Stream_t)(bool atStreamStart, const void *ptr, unsigned int size);
	extern Load_Stream_t Load_Stream;

	typedef void(__cdecl * Load_XString_t)(bool atStreamStart);
	extern Load_XString_t Load_XString;

	typedef void(__cdecl * Load_XModelPtr_t)(bool atStreamStart);
	extern Load_XModelPtr_t Load_XModelPtr;

	typedef void(__cdecl * Load_XModelSurfsFixup_t)(XModelSurfs **, XModelLodInfo *);
	extern Load_XModelSurfsFixup_t Load_XModelSurfsFixup;

	typedef void(__cdecl * Load_XStringArray_t)(bool atStreamStart, int count);
	extern Load_XStringArray_t Load_XStringArray;

	typedef void(__cdecl * Load_XStringCustom_t)(const char **str);
	extern Load_XStringCustom_t Load_XStringCustom;

	typedef void(__cdecl *Load_FxEffectDefHandle_t)(bool atStreamStart);
	extern Load_FxEffectDefHandle_t Load_FxEffectDefHandle;

	typedef void(__cdecl *Load_FxElemDef_t)(bool atStreamStart);
	extern Load_FxElemDef_t Load_FxElemDef;

	typedef void(__cdecl *Load_GfxImagePtr_t)(bool atStreamStart);
	extern Load_GfxImagePtr_t Load_GfxImagePtr;

	typedef void(__cdecl *Load_GfxTextureLoad_t)(bool atStreamStart);
	extern Load_GfxTextureLoad_t Load_GfxTextureLoad;

	typedef int(__cdecl *Load_Texture_t)(GfxImageLoadDef **loadDef, GfxImage *image);
	extern Load_Texture_t Load_Texture;

	typedef void(__cdecl * Load_SndAliasCustom_t)(snd_alias_list_t** var);
	extern Load_SndAliasCustom_t Load_SndAliasCustom;

	typedef void(__cdecl *Load_MaterialHandle_t)(bool atStreamStart);
	extern Load_MaterialHandle_t Load_MaterialHandle;

	typedef void(__cdecl *Load_PhysCollmapPtr_t)(bool atStreamStart);
	extern Load_PhysCollmapPtr_t Load_PhysCollmapPtr;

	typedef void(__cdecl *Load_PhysPresetPtr_t)(bool atStreamStart);
	extern Load_PhysPresetPtr_t Load_PhysPresetPtr;

	typedef void(__cdecl *Load_TracerDefPtr_t)(bool atStreamStart);
	extern Load_TracerDefPtr_t Load_TracerDefPtr;

	typedef void(__cdecl *Load_snd_alias_list_nameArray_t)(bool atStreamStart, int count);
	extern Load_snd_alias_list_nameArray_t Load_snd_alias_list_nameArray;

	typedef void(__cdecl * Menus_CloseAll_t)(UiContext *dc);
	extern Menus_CloseAll_t Menus_CloseAll;

    typedef void(__cdecl * Menus_CloseRequest_t)(UiContext *dc, menuDef_t* menu);
    extern Menus_CloseRequest_t Menus_CloseRequest;

	typedef int(__cdecl * Menus_OpenByName_t)(UiContext *dc, const char *p);
	extern Menus_OpenByName_t Menus_OpenByName;

	typedef menuDef_t *(__cdecl * Menus_FindByName_t)(UiContext *dc, const char *name);
	extern Menus_FindByName_t Menus_FindByName;

	typedef bool(__cdecl * Menu_IsVisible_t)(UiContext *dc, menuDef_t *menu);
	extern Menu_IsVisible_t Menu_IsVisible;

	typedef bool(__cdecl * Menus_MenuIsInStack_t)(UiContext *dc, menuDef_t *menu);
	extern Menus_MenuIsInStack_t Menus_MenuIsInStack;

	typedef void(__cdecl * MSG_Init_t)(msg_t *buf, char *data, int length);
	extern MSG_Init_t MSG_Init;

	typedef void(__cdecl * MSG_ReadData_t)(msg_t *msg, void *data, int len);
	extern MSG_ReadData_t MSG_ReadData;

	typedef int(__cdecl * MSG_ReadLong_t)(msg_t* msg);
	extern MSG_ReadLong_t MSG_ReadLong;

	typedef short(__cdecl * MSG_ReadShort_t)(msg_t* msg);
	extern MSG_ReadShort_t MSG_ReadShort;

	typedef __int64(__cdecl * MSG_ReadInt64_t)(msg_t* msg);
	extern MSG_ReadInt64_t MSG_ReadInt64;

	typedef char* (__cdecl * MSG_ReadString_t)(msg_t* msg);
	extern MSG_ReadString_t MSG_ReadString;

	typedef char* (__cdecl * MSG_ReadStringLine_t)(msg_t *msg, char *string, unsigned int maxChars);
	extern MSG_ReadStringLine_t MSG_ReadStringLine;

	typedef int(__cdecl * MSG_ReadByte_t)(msg_t* msg);
	extern MSG_ReadByte_t MSG_ReadByte;

	typedef int(__cdecl * MSG_ReadBitsCompress_t)(const char *from, char *to, int size);
	extern MSG_ReadBitsCompress_t MSG_ReadBitsCompress;

	typedef void(__cdecl * MSG_WriteByte_t)(msg_t* msg, unsigned char c);
	extern MSG_WriteByte_t MSG_WriteByte;

	typedef void(__cdecl * MSG_WriteData_t)(msg_t *buf, const void *data, int length);
	extern MSG_WriteData_t MSG_WriteData;

	typedef void(__cdecl * MSG_WriteLong_t)(msg_t *msg, int c);
	extern MSG_WriteLong_t MSG_WriteLong;

	typedef void(*MSG_WriteShort_t)(msg_t* msg, short s);
	extern MSG_WriteShort_t MSG_WriteShort;

	typedef void(*MSG_WriteString_t)(msg_t* msg, const char *str);
	extern MSG_WriteString_t MSG_WriteString;

	typedef int(__cdecl * MSG_WriteBitsCompress_t)(bool trainHuffman, const char *from, char *to, int size);
	extern MSG_WriteBitsCompress_t MSG_WriteBitsCompress;

	typedef void(__cdecl * NetadrToSockadr_t)(netadr_t *a, sockaddr *s);
	extern NetadrToSockadr_t NetadrToSockadr;

	typedef const char* (__cdecl * NET_AdrToString_t)(netadr_t adr);
	extern NET_AdrToString_t NET_AdrToString;

	typedef bool(__cdecl * NET_CompareAdr_t)(netadr_t a, netadr_t b);
	extern NET_CompareAdr_t NET_CompareAdr;

	typedef const char* (__cdecl * NET_ErrorString_t)();
	extern NET_ErrorString_t NET_ErrorString;

	typedef void(__cdecl * NET_Init_t)();
	extern NET_Init_t NET_Init;

	typedef bool(__cdecl * NET_IsLocalAddress_t)(netadr_t adr);
	extern NET_IsLocalAddress_t NET_IsLocalAddress;

	typedef bool(__cdecl * NET_StringToAdr_t)(const char *s, netadr_t *a);
	extern NET_StringToAdr_t NET_StringToAdr;

	typedef void(__cdecl* NET_OutOfBandPrint_t)(netsrc_t sock, netadr_t adr, const char *data);
	extern NET_OutOfBandPrint_t NET_OutOfBandPrint;

	typedef void(__cdecl* NET_OutOfBandData_t)(netsrc_t sock, netadr_t adr, const char *format, int len);
	extern NET_OutOfBandData_t NET_OutOfBandData;

	typedef void(__cdecl * Live_MPAcceptInvite_t)(_XSESSION_INFO *hostInfo, const int controllerIndex, bool fromGameInvite);
	extern Live_MPAcceptInvite_t Live_MPAcceptInvite;

	typedef int(__cdecl * Live_GetMapIndex_t)(const char* mapname);
	extern Live_GetMapIndex_t Live_GetMapIndex;

	typedef int(__cdecl * Live_GetPrestige_t)(int controllerIndex);
	extern Live_GetPrestige_t Live_GetPrestige;

	typedef int(__cdecl * Live_GetXp_t)(int controllerIndex);
	extern Live_GetXp_t Live_GetXp;

	typedef char* (__cdecl * LoadModdableRawfile_t)(int a1, const char* filename);
	extern LoadModdableRawfile_t LoadModdableRawfile;

	typedef int(__cdecl * PC_ReadToken_t)(source_t*, token_t*);
	extern PC_ReadToken_t PC_ReadToken;

	typedef int(__cdecl * PC_ReadTokenHandle_t)(int handle, pc_token_s *pc_token);
	extern PC_ReadTokenHandle_t PC_ReadTokenHandle;

	typedef void(__cdecl * PC_SourceError_t)(int, const char*, ...);
	extern PC_SourceError_t PC_SourceError;

	typedef int(__cdecl * Party_GetMaxPlayers_t)(party_s* party);
	extern Party_GetMaxPlayers_t Party_GetMaxPlayers;

	typedef int(__cdecl * PartyHost_CountMembers_t)(PartyData_s* party);
	extern PartyHost_CountMembers_t PartyHost_CountMembers;

	typedef netadr_t *(__cdecl * PartyHost_GetMemberAddressBySlot_t)(int unk, void *party, const int slot);
	extern PartyHost_GetMemberAddressBySlot_t PartyHost_GetMemberAddressBySlot;

	typedef const char *(__cdecl * PartyHost_GetMemberName_t)(PartyData_s* party, const int clientNum);
	extern PartyHost_GetMemberName_t PartyHost_GetMemberName;

	typedef void(__cdecl * Playlist_ParsePlaylists_t)(const char* data);
	extern Playlist_ParsePlaylists_t Playlist_ParsePlaylists;

	typedef Font_s* (__cdecl * R_RegisterFont_t)(const char* asset, int safe);
	extern R_RegisterFont_t R_RegisterFont;

	typedef void(__cdecl * R_AddCmdDrawText_t)(const char *text, int maxChars, Font_s *font, float x, float y, float xScale, float yScale, float rotation, const float *color, int style);
	extern R_AddCmdDrawText_t R_AddCmdDrawText;

	typedef void(_cdecl * R_AddCmdDrawStretchPic_t)(float x, float y, float w, float h, float xScale, float yScale, float xay, float yay, const float *color, Game::Material* material);
	extern R_AddCmdDrawStretchPic_t R_AddCmdDrawStretchPic;

	typedef void* (__cdecl * R_AllocStaticIndexBuffer_t)(IDirect3DIndexBuffer9** store, int length);
	extern R_AllocStaticIndexBuffer_t R_AllocStaticIndexBuffer;

	typedef bool(__cdecl * R_Cinematic_StartPlayback_Now_t)();
	extern R_Cinematic_StartPlayback_Now_t R_Cinematic_StartPlayback_Now;

	typedef void(__cdecl * R_LoadGraphicsAssets_t)();
	extern R_LoadGraphicsAssets_t R_LoadGraphicsAssets;

	typedef int(__cdecl * R_TextWidth_t)(const char* text, int maxlength, Font_s* font);
	extern R_TextWidth_t R_TextWidth;

	typedef int(__cdecl * R_TextHeight_t)(Font_s* font);
	extern R_TextHeight_t R_TextHeight;

	typedef void(__cdecl * R_FlushSun_t)();
	extern R_FlushSun_t R_FlushSun;

	typedef GfxWorld*(__cdecl * R_SortWorldSurfaces_t)();
	extern R_SortWorldSurfaces_t R_SortWorldSurfaces;

	typedef void(__cdecl * RemoveRefToObject_t)(unsigned int id);
	extern RemoveRefToObject_t RemoveRefToObject;

	typedef void(__cdecl * Scr_AddEntity_t)(gentity_s const*);
	extern Scr_AddEntity_t Scr_AddEntity;

	typedef void(__cdecl * Scr_AddString_t)(const char* str);
	extern Scr_AddString_t Scr_AddString;

	typedef void(__cdecl * Scr_AddInt_t)(int num);
	extern Scr_AddInt_t Scr_AddInt;

	typedef void(__cdecl * Scr_AddFloat_t)(float);
	extern Scr_AddFloat_t Scr_AddFloat;

	typedef void(__cdecl * Scr_AddObject_t)(unsigned int id);
	extern Scr_AddObject_t Scr_AddObject;

	typedef void(__cdecl * Scr_ShutdownAllocNode_t)();
	extern Scr_ShutdownAllocNode_t Scr_ShutdownAllocNode;

	typedef int(__cdecl * Scr_LoadGameType_t)();
	extern Scr_LoadGameType_t Scr_LoadGameType;

	typedef int(__cdecl * Scr_LoadScript_t)(const char*);
	extern Scr_LoadScript_t Scr_LoadScript;

	typedef char* (__cdecl * Scr_GetString_t)(int);
	extern Scr_GetString_t Scr_GetString;

	typedef float(__cdecl * Scr_GetFloat_t)(int);
	extern Scr_GetFloat_t Scr_GetFloat;

	typedef int(__cdecl * Scr_GetInt_t)(int);
	extern Scr_GetInt_t Scr_GetInt;

	typedef int(__cdecl * Scr_GetType_t)(int);
	extern Scr_GetType_t Scr_GetType;

	typedef void(__cdecl * Scr_Error_t)(const char*);
	extern Scr_Error_t Scr_Error;

	typedef unsigned int(__cdecl * Scr_GetObject_t)(int);
	extern Scr_GetObject_t Scr_GetObject;

	typedef int(__cdecl * Scr_GetNumParam_t)();
	extern Scr_GetNumParam_t Scr_GetNumParam;

	typedef int(__cdecl * Scr_GetFunctionHandle_t)(const char*, const char*);
	extern Scr_GetFunctionHandle_t Scr_GetFunctionHandle;

	typedef int(__cdecl * Scr_ExecThread_t)(int, int);
	extern Scr_ExecThread_t Scr_ExecThread;

	typedef int(__cdecl * Scr_FreeThread_t)(int);
	extern Scr_FreeThread_t Scr_FreeThread;

	typedef void(__cdecl * Scr_Notify_t)(gentity_t *ent, unsigned __int16 stringValue, unsigned int paramcount);
	extern Scr_Notify_t Scr_Notify;

	typedef void(__cdecl * Scr_NotifyLevel_t)(unsigned __int16 stringValue, unsigned int paramcount);
	extern Scr_NotifyLevel_t Scr_NotifyLevel;

	typedef void(__cdecl * Scr_ClearOutParams_t)();
	extern Scr_ClearOutParams_t Scr_ClearOutParams;

	typedef void(__cdecl * Scr_RegisterFunction_t)(scr_function_t function);
	extern Scr_RegisterFunction_t Scr_RegisterFunction;

	typedef bool(__cdecl * Scr_IsSystemActive_t)();
	extern Scr_IsSystemActive_t Scr_IsSystemActive;

	typedef script_t* (__cdecl * Script_Alloc_t)(int length);
	extern Script_Alloc_t Script_Alloc;

	typedef void(__cdecl * Script_SetupTokens_t)(script_t* script, void* tokens);
	extern Script_SetupTokens_t Script_SetupTokens;

	typedef int(__cdecl * Script_CleanString_t)(char* buffer);
	extern Script_CleanString_t Script_CleanString;

	typedef char* (__cdecl * SE_Load_t)(const char* file, int Unk);
	extern SE_Load_t SE_Load;

	typedef char* (__cdecl * SEH_StringEd_GetString_t)(const char* string);
	extern SEH_StringEd_GetString_t SEH_StringEd_GetString;

	typedef char* (__cdecl * SL_ConvertToString_t)(unsigned short stringValue);
	extern SL_ConvertToString_t SL_ConvertToString;

	typedef short(__cdecl * SL_GetString_t)(const char *str, unsigned int user);
	extern SL_GetString_t SL_GetString;

	typedef void(__cdecl * SND_Init_t)(int a1, int a2, int a3);
	extern SND_Init_t SND_Init;

	typedef void(__cdecl * SND_InitDriver_t)();
	extern SND_InitDriver_t SND_InitDriver;

	typedef void(__cdecl * SockadrToNetadr_t)(sockaddr *s, netadr_t *a);
	extern SockadrToNetadr_t SockadrToNetadr;

	typedef void(__cdecl * Steam_JoinLobby_t)(SteamID, char);
	extern Steam_JoinLobby_t Steam_JoinLobby;

	typedef const char*(__cdecl * StringTable_Lookup_t)(StringTable *table, const int comparisonColumn, const char *value, const int valueColumn);
	extern StringTable_Lookup_t StringTable_Lookup;

	typedef int(__cdecl * StringTable_HashString_t)(const char* string);
	extern StringTable_HashString_t StringTable_HashString;

	typedef gentity_t*(__cdecl* SV_AddTestClient_t)();
	extern SV_AddTestClient_t SV_AddTestClient;

	typedef void(__cdecl* SV_ClientThink_t)(client_s*, usercmd_s*);
	extern SV_ClientThink_t SV_ClientThink;

	typedef int(__cdecl* SV_GameClientNum_Score_t)(int clientID);
	extern SV_GameClientNum_Score_t SV_GameClientNum_Score;

	typedef void(__cdecl * SV_GameSendServerCommand_t)(int clientNum, /*svscmd_type*/int type, const char* text);
	extern SV_GameSendServerCommand_t SV_GameSendServerCommand;

	typedef void(__cdecl * SV_Cmd_TokenizeString_t)(const char* string);
	extern SV_Cmd_TokenizeString_t SV_Cmd_TokenizeString;

	typedef void(__cdecl * SV_Cmd_EndTokenizedString_t)();
	extern SV_Cmd_EndTokenizedString_t SV_Cmd_EndTokenizedString;

	typedef void(__cdecl * SV_SetConfigstring_t)(int index, const char* string);
	extern SV_SetConfigstring_t SV_SetConfigstring;

	typedef void(__cdecl * SV_DirectConnect_t)(netadr_t adr);
	extern SV_DirectConnect_t SV_DirectConnect;

	typedef bool(__cdecl * SV_Loaded_t)();
	extern SV_Loaded_t SV_Loaded;

	typedef int(__cdecl * Sys_Error_t)(int, char *, ...);
	extern Sys_Error_t Sys_Error;

	typedef void(__cdecl * Sys_FreeFileList_t)(char** list);
	extern Sys_FreeFileList_t Sys_FreeFileList;

	typedef bool(__cdecl * Sys_IsDatabaseReady_t)();
	extern Sys_IsDatabaseReady_t Sys_IsDatabaseReady;

	typedef bool(__cdecl * Sys_IsDatabaseReady2_t)();
	extern Sys_IsDatabaseReady2_t Sys_IsDatabaseReady2;

	typedef bool(__cdecl * Sys_IsMainThread_t)();
	extern Sys_IsMainThread_t Sys_IsMainThread;

	typedef bool(__cdecl * Sys_IsRenderThread_t)();
	extern Sys_IsRenderThread_t Sys_IsRenderThread;

	typedef bool(__cdecl * Sys_IsServerThread_t)();
	extern Sys_IsServerThread_t Sys_IsServerThread;

	typedef bool(__cdecl * Sys_IsDatabaseThread_t)();
	extern Sys_IsDatabaseThread_t Sys_IsDatabaseThread;

	typedef char** (__cdecl * Sys_ListFiles_t)(const char *directory, const char *extension, const char *filter, int *numfiles, int wantsubs);
	extern Sys_ListFiles_t Sys_ListFiles;

	typedef int(__cdecl * Sys_Milliseconds_t)();
	extern Sys_Milliseconds_t Sys_Milliseconds;

	typedef void(__cdecl * TeleportPlayer_t)(gentity_t* entity, float* pos, float* orientation);
	extern TeleportPlayer_t TeleportPlayer;

	typedef bool(__cdecl * Sys_SendPacket_t)(netsrc_t sock, size_t len, const char *format, netadr_t adr);
	extern Sys_SendPacket_t Sys_SendPacket;

	typedef void(__cdecl * Sys_ShowConsole_t)();
	extern Sys_ShowConsole_t Sys_ShowConsole;

	typedef void(__cdecl * Sys_SuspendOtherThreads_t)();
	extern Sys_SuspendOtherThreads_t Sys_SuspendOtherThreads;

	typedef void(__cdecl * UI_AddMenuList_t)(UiContext *dc, MenuList *menuList, int close);
	extern UI_AddMenuList_t UI_AddMenuList;

	typedef char* (__cdecl * UI_CheckStringTranslation_t)(char*, char*);
	extern UI_CheckStringTranslation_t UI_CheckStringTranslation;

	typedef MenuList *(__cdecl * UI_LoadMenus_t)(const char *menuFile, int imageTrack);
	extern UI_LoadMenus_t UI_LoadMenus;

	typedef void(__cdecl * UI_UpdateArenas_t)();
	extern UI_UpdateArenas_t UI_UpdateArenas;

	typedef void(__cdecl * UI_SortArenas_t)();
	extern UI_SortArenas_t UI_SortArenas;

	typedef void(__cdecl * UI_DrawHandlePic_t)(/*ScreenPlacement*/void *scrPlace, float x, float y, float w, float h, int horzAlign, int vertAlign, const float *color, Material *material);
	extern UI_DrawHandlePic_t UI_DrawHandlePic;

	typedef void* (__cdecl * UI_GetContext_t)(void*);
	extern UI_GetContext_t UI_GetContext;

	typedef int(__cdecl * UI_TextWidth_t)(const char *text, int maxChars, Font_s *font, float scale);
	extern UI_TextWidth_t UI_TextWidth;

	typedef void(__cdecl * UI_DrawText_t)(void* scrPlace, const char *text, int maxChars, Font_s *font, float x, float y, int horzAlign, int vertAlign, float scale, const float *color, int style);
	extern UI_DrawText_t UI_DrawText;

	typedef const char * (__cdecl * Win_GetLanguage_t)();
	extern Win_GetLanguage_t Win_GetLanguage;

	typedef void (__cdecl * Vec3UnpackUnitVec_t)(PackedUnitVec, vec3_t *);
	extern Vec3UnpackUnitVec_t Vec3UnpackUnitVec;

	typedef void(__cdecl * unzClose_t)(void* handle);
	extern unzClose_t unzClose;


	typedef void(__cdecl* NET_DeferPacketToClient_t)(netadr_t *, msg_t *);
	extern NET_DeferPacketToClient_t NET_DeferPacketToClient;
	

	extern XAssetHeader* DB_XAssetPool;
	extern unsigned int* g_poolSize;

	extern DWORD* cmd_id;
	extern DWORD* cmd_argc;
	extern char*** cmd_argv;

	extern DWORD* cmd_id_sv;
	extern DWORD* cmd_argc_sv;
	extern char*** cmd_argv_sv;

	extern cmd_function_t** cmd_functions;

	extern int* deferredQueueSize;

	extern int* svs_numclients;
	extern client_t* svs_clients;

	extern int* svs_time;

	extern source_t **sourceFiles;
	extern keywordHash_t **menuParseKeywordHash;

	extern UiContext *uiContext;

	extern int* arenaCount;
	extern mapArena_t* arenas;

	extern int* gameTypeCount;
	extern gameTypeName_t* gameTypes;

	extern searchpath_t** fs_searchpaths;

	extern XBlock** g_streamBlocks;
	extern int* g_streamPos;
	extern int* g_streamPosIndex;

	extern bool* g_lobbyCreateInProgress;
	extern party_t** partyIngame;
	extern PartyData_s** partyData;

	extern int* numIP;
	extern netIP_t* localIP;

	extern int* demoFile;
	extern int* demoPlaying;
	extern int* demoRecording;
	extern int* serverMessageSequence;

	extern gentity_t* g_entities;

	extern netadr_t* connectedHost;
	extern SOCKET* ip_socket;

	extern uint32_t* com_frameTime;

	extern SafeArea* safeArea;

	extern SpawnVar* spawnVars;
	extern MapEnts** marMapEntsPtr;

	extern IDirect3D9** d3d9;
	extern IDirect3DDevice9** dx_ptr;

	extern mapname_t* mapnames;

	extern char*** varXString;
	extern TracerDef*** varTracerDefPtr;
	extern XModel*** varXModelPtr;
	extern XModel** varXModel;
	extern PathData** varPathData;
	extern const char** varConstChar;
	extern Material*** varMaterialHandle;
	extern FxEffectDef*** varFxEffectDefHandle;
	extern PhysCollmap*** varPhysCollmapPtr;
	extern PhysPreset*** varPhysPresetPtr;
	extern Game::MaterialPass** varMaterialPass;
	extern snd_alias_list_t*** varsnd_alias_list_name;

	extern FxElemField* s_elemFields;

	extern infoParm_t* infoParams;

	extern XZone* g_zones;
	extern unsigned short* db_hashTable;

	extern ScriptContainer* scriptContainer;

	XAssetHeader ReallocateAssetPool(XAssetType type, unsigned int newSize);
	void Menu_FreeItemMemory(Game::itemDef_s* item);
	const char* TableLookup(StringTable* stringtable, int row, int column);
	const char* UI_LocalizeMapName(const char* mapName);
	const char* UI_LocalizeGameType(const char* gameType);
	float UI_GetScoreboardLeft(void*);

	const char *DB_GetXAssetName(XAsset *asset);
	XAssetType DB_GetXAssetNameType(const char* name);
	int DB_GetZoneIndex(const std::string& name);
	bool DB_IsZoneLoaded(const char* zone);
	void DB_EnumXAssetEntries(XAssetType type, std::function<void(XAssetEntry*)> callback, bool overrides, bool lock);
	XAssetHeader DB_FindXAssetDefaultHeaderInternal(XAssetType type);
	XAssetEntry* DB_FindXAssetEntry(XAssetType type, const char* name);

	void FS_AddLocalizedGameDirectory(const char *path, const char *dir);

	void ShowMessageBox(const std::string& message, const std::string& title);

	unsigned int R_HashString(const char* string);
	void R_LoadSunThroughDvars(const char* mapname, sunflare_t* sun);
	void R_SetSunFromDvars(sunflare_t* sun);

	void SV_KickClient(client_t* client, const char* reason);
	void SV_KickClientError(client_t* client, const std::string& reason);

	void Scr_iPrintLn(int clientNum, const std::string& message);
	void Scr_iPrintLnBold(int clientNum, const std::string& message);
	void Scr_NotifyId(unsigned int id, unsigned __int16 stringValue, unsigned int paramcount);

	void IN_KeyUp(kbutton_t* button);
	void IN_KeyDown(kbutton_t* button);

	int FS_FOpenFileReadCurrentThread(const char* file, int* fh);

	void Load_IndexBuffer(void* data, IDirect3DIndexBuffer9** storeHere, int count);
	void Load_VertexBuffer(void* data, IDirect3DVertexBuffer9** where, int len);

	char* Com_GetParseThreadInfo();
	void Com_SetParseNegativeNumbers(int parse);

	int CL_GetMaxXP();

	void Image_Setup(GfxImage* image, unsigned int width, unsigned int height, unsigned int depth, unsigned int flags, _D3DFORMAT format);

	void Vec3Normalize(vec3_t& vec);
	void Vec2UnpackTexCoords(const PackedTexCoords in, vec2_t* out);
	void MatrixVecMultiply(const float(&mulMat)[3][3], const vec3_t& mulVec, vec3_t& solution);

	void SortWorldSurfaces(GfxWorld* world);
	void R_AddDebugLine(float* color, float* v1, float* v2);
	void R_AddDebugString(float *color, float *pos, float scale, const char *str);
	void R_AddDebugBounds(float* color, Bounds* b);
}
