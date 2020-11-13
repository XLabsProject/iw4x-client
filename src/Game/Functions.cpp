#include "STDInclude.hpp"

namespace Game
{
	std::vector<std::string> Sys_ListFilesWrapper(const std::string& directory, const std::string& extension)
	{
		auto fileCount = 0;
		auto files = Game::Sys_ListFiles(directory.data(), extension.data(), 0, &fileCount, 0);

		std::vector<std::string> result;

		for (auto i = 0; i < fileCount; i++)
		{
			if (files[i])
			{
				result.push_back(files[i]);
			}
		}

		Game::FS_FreeFileList(files);

		return result;
	}
	
	AddRefToObject_t AddRefToObject = AddRefToObject_t(0x61C360);
	AllocObject_t AllocObject = AllocObject_t(0x434320);

	AngleVectors_t AngleVectors = AngleVectors_t(0x4691A0);

	BG_GetNumWeapons_t BG_GetNumWeapons = BG_GetNumWeapons_t(0x4F5CC0);
	BG_GetWeaponName_t BG_GetWeaponName = BG_GetWeaponName_t(0x4E6EC0);
	BG_LoadWeaponDef_LoadObj_t BG_LoadWeaponDef_LoadObj = BG_LoadWeaponDef_LoadObj_t(0x57B5F0);

	Cbuf_AddServerText_t Cbuf_AddServerText = Cbuf_AddServerText_t(0x4BB9B0);
	Cbuf_AddText_t Cbuf_AddText = Cbuf_AddText_t(0x404B20);

	CG_GetClientNum_t CG_GetClientNum = CG_GetClientNum_t(0x433700);
	CG_PlayBoltedEffect_t CG_PlayBoltedEffect = CG_PlayBoltedEffect_t(0x00430E10);
	CG_GetBoneIndex_t CG_GetBoneIndex = CG_GetBoneIndex_t(0x00504F20);
	
	CL_GetClientName_t CL_GetClientName = CL_GetClientName_t(0x4563D0);
	CL_IsCgameInitialized_t CL_IsCgameInitialized = CL_IsCgameInitialized_t(0x43EB20);
	CL_ConnectFromParty_t CL_ConnectFromParty = CL_ConnectFromParty_t(0x433D30);
	CL_DownloadsComplete_t CL_DownloadsComplete = CL_DownloadsComplete_t(0x42CE90);
	CL_DrawStretchPicPhysical_t CL_DrawStretchPicPhysical = CL_DrawStretchPicPhysical_t(0x4FC120);
	CL_GetConfigString_t CL_GetConfigString = CL_GetConfigString_t(0x44ADB0);
	CL_GetMaxRank_t CL_GetMaxRank = CL_GetMaxRank_t(0x44BA30);
	CL_GetRankForXP_t CL_GetRankForXP = CL_GetRankForXP_t(0x4FF8A0);
	CL_GetRankIcon_t CL_GetRankIcon = CL_GetRankIcon_t(0x4A7B30);
	CL_HandleRelayPacket_t CL_HandleRelayPacket = CL_HandleRelayPacket_t(0x5A8C70);
	CL_ResetViewport_t CL_ResetViewport = CL_ResetViewport_t(0x4A8830);
	CL_SelectStringTableEntryInDvar_f_t CL_SelectStringTableEntryInDvar_f = CL_SelectStringTableEntryInDvar_f_t(0x4A4560);

	Cmd_AddCommand_t Cmd_AddCommand = Cmd_AddCommand_t(0x470090);
	Cmd_AddServerCommand_t Cmd_AddServerCommand = Cmd_AddServerCommand_t(0x4DCE00);
	Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand = Cmd_ExecuteSingleCommand_t(0x609540);
	Com_ClientPacketEvent_t Com_ClientPacketEvent = Com_ClientPacketEvent_t(0x49F0B0);

	Com_Error_t Com_Error = Com_Error_t(0x4B22D0);
	Com_Printf_t Com_Printf = Com_Printf_t(0x402500);
	Com_PrintMessage_t Com_PrintMessage = Com_PrintMessage_t(0x4AA830);
	Com_EndParseSession_t Com_EndParseSession = Com_EndParseSession_t(0x4B80B0);
	Com_BeginParseSession_t Com_BeginParseSession = Com_BeginParseSession_t(0x4AAB80);
	Com_SetSpaceDelimited_t Com_SetSpaceDelimited = Com_SetSpaceDelimited_t(0x4FC710);
	Com_Parse_t Com_Parse = Com_Parse_t(0x474D60);
	Com_MatchToken_t Com_MatchToken = Com_MatchToken_t(0x447130);
	Com_SetSlowMotion_t Com_SetSlowMotion = Com_SetSlowMotion_t(0x446E20);

	Con_DrawMiniConsole_t Con_DrawMiniConsole = Con_DrawMiniConsole_t(0x464F30);
	Con_DrawSolidConsole_t Con_DrawSolidConsole = Con_DrawSolidConsole_t(0x5A5040);

	DB_AllocStreamPos_t DB_AllocStreamPos = DB_AllocStreamPos_t(0x418380);
	DB_PushStreamPos_t DB_PushStreamPos = DB_PushStreamPos_t(0x458A20);
	DB_PopStreamPos_t DB_PopStreamPos = DB_PopStreamPos_t(0x4D1D60);

	DB_BeginRecoverLostDevice_t DB_BeginRecoverLostDevice = DB_BeginRecoverLostDevice_t(0x4BFF90);
	DB_EndRecoverLostDevice_t DB_EndRecoverLostDevice = DB_EndRecoverLostDevice_t(0x46B660);
	DB_EnumXAssets_t DB_EnumXAssets = DB_EnumXAssets_t(0x4B76D0);
	DB_EnumXAssets_Internal_t DB_EnumXAssets_Internal = DB_EnumXAssets_Internal_t(0x5BB0A0);
	DB_FindXAssetHeader_t DB_FindXAssetHeader = DB_FindXAssetHeader_t(0x407930);
	DB_GetRawBuffer_t DB_GetRawBuffer = DB_GetRawBuffer_t(0x4CDC50);
	DB_GetRawFileLen_t DB_GetRawFileLen = DB_GetRawFileLen_t(0x4DAA80);
	DB_GetLoadedFraction_t DB_GetLoadedFraction = DB_GetLoadedFraction_t(0x468380);
	DB_GetXAssetNameHandler_t* DB_GetXAssetNameHandlers = reinterpret_cast<DB_GetXAssetNameHandler_t*>(0x799328);
	DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandlers = reinterpret_cast<DB_GetXAssetSizeHandler_t*>(0x799488);
	DB_GetXAssetTypeName_t DB_GetXAssetTypeName = DB_GetXAssetTypeName_t(0x4CFCF0);
	DB_IsXAssetDefault_t DB_IsXAssetDefault = DB_IsXAssetDefault_t(0x48E6A0);
	DB_LoadXAssets_t DB_LoadXAssets = DB_LoadXAssets_t(0x4E5930);
	DB_LoadXFileData_t DB_LoadXFileData = DB_LoadXFileData_t(0x445460);
	DB_ReadXFile_t DB_ReadXFile = DB_ReadXFile_t(0x445460);
	DB_ReadXFileUncompressed_t DB_ReadXFileUncompressed = DB_ReadXFileUncompressed_t(0x4705E0);
	DB_ReleaseXAssetHandler_t* DB_ReleaseXAssetHandlers = reinterpret_cast<DB_ReleaseXAssetHandler_t*>(0x799AB8);
	DB_SetXAssetName_t DB_SetXAssetName = DB_SetXAssetName_t(0x453580);
	DB_SetXAssetNameHandler_t* DB_SetXAssetNameHandlers = reinterpret_cast<DB_SetXAssetNameHandler_t*>(0x7993D8);
	DB_XModelSurfsFixup_t DB_XModelSurfsFixup = DB_XModelSurfsFixup_t(0x5BAC50);

	Dvar_RegisterBool_t Dvar_RegisterBool = Dvar_RegisterBool_t(0x4CE1A0);
	Dvar_RegisterFloat_t Dvar_RegisterFloat = Dvar_RegisterFloat_t(0x648440);
	Dvar_RegisterVec2_t Dvar_RegisterVec2 = Dvar_RegisterVec2_t(0x4F6070);
	Dvar_RegisterVec3_t Dvar_RegisterVec3 = Dvar_RegisterVec3_t(0x4EF8E0);
	Dvar_RegisterVec4_t Dvar_RegisterVec4 = Dvar_RegisterVec4_t(0x4F28E0);
	Dvar_RegisterInt_t Dvar_RegisterInt = Dvar_RegisterInt_t(0x479830);
	Dvar_RegisterEnum_t Dvar_RegisterEnum = Dvar_RegisterEnum_t(0x412E40);
	Dvar_RegisterString_t Dvar_RegisterString = Dvar_RegisterString_t(0x4FC7E0);
	Dvar_RegisterColor_t Dvar_RegisterColor = Dvar_RegisterColor_t(0x4F28E0);//0x471500;

	Dvar_GetUnpackedColorByName_t Dvar_GetUnpackedColorByName = Dvar_GetUnpackedColorByName_t(0x406530);
	Dvar_FindVar_t Dvar_FindVar = Dvar_FindVar_t(0x4D5390);
	Dvar_InfoString_Big_t Dvar_InfoString_Big = Dvar_InfoString_Big_t(0x4D98A0);
	Dvar_SetCommand_t Dvar_SetCommand = Dvar_SetCommand_t(0x4EE430);

	Encode_Init_t Encode_Init = Encode_Init_t(0x462AB0);

	Field_Clear_t Field_Clear = Field_Clear_t(0x437EB0);

	FreeMemory_t FreeMemory = FreeMemory_t(0x4D6640);

	FS_FileExists_t FS_FileExists = FS_FileExists_t(0x4DEFA0);
	FS_FreeFile_t FS_FreeFile = FS_FreeFile_t(0x4416B0);
	FS_ReadFile_t FS_ReadFile = FS_ReadFile_t(0x4F4B90);
	FS_GetFileList_t FS_GetFileList = FS_GetFileList_t(0x441BB0);
	FS_FreeFileList_t FS_FreeFileList = FS_FreeFileList_t(0x4A5DE0);
	FS_FOpenFileAppend_t FS_FOpenFileAppend = FS_FOpenFileAppend_t(0x410BB0);
	FS_FOpenFileAppend_t FS_FOpenFileWrite = FS_FOpenFileAppend_t(0x4BA530);
	FS_FOpenFileRead_t FS_FOpenFileRead = FS_FOpenFileRead_t(0x46CBF0);
	FS_FOpenFileRead_t FS_FOpenFileReadDatabase = FS_FOpenFileRead_t(0x42ECA0);
	FS_FOpenFileReadForThread_t FS_FOpenFileReadForThread = FS_FOpenFileReadForThread_t(0x643270);
	FS_FCloseFile_t FS_FCloseFile = FS_FCloseFile_t(0x462000);
	FS_WriteFile_t FS_WriteFile = FS_WriteFile_t(0x426450);
	FS_Write_t FS_Write = FS_Write_t(0x4C06E0);
	FS_Read_t FS_Read = FS_Read_t(0x4A04C0);
	FS_Seek_t FS_Seek = FS_Seek_t(0x4A63D0);
	FS_FTell_t FS_FTell = FS_FTell_t(0x4E6760);
	FS_Remove_t FS_Remove = FS_Remove_t(0x4660F0);
	FS_Restart_t FS_Restart = FS_Restart_t(0x461A50);
	FS_BuildPathToFile_t FS_BuildPathToFile = FS_BuildPathToFile_t(0x4702C0);
	FS_IsShippedIWD_t FS_IsShippedIWD = FS_IsShippedIWD_t(0x642440);

	G_SpawnEntitiesFromString_t G_SpawnEntitiesFromString = G_SpawnEntitiesFromString_t(0x4D8840);
	G_GetWeaponIndexForName_t G_GetWeaponIndexForName = G_GetWeaponIndexForName_t(0x49E540);

	GScr_LoadGameTypeScript_t GScr_LoadGameTypeScript = GScr_LoadGameTypeScript_t(0x4ED9A0);

	Image_LoadFromFileWithReader_t Image_LoadFromFileWithReader = Image_LoadFromFileWithReader_t(0x53ABF0);
	Image_Release_t Image_Release = Image_Release_t(0x51F010);

	Info_ValueForKey_t Info_ValueForKey = Info_ValueForKey_t(0x47C820);

	Key_SetCatcher_t Key_SetCatcher = Key_SetCatcher_t(0x43BD00);

	LargeLocalInit_t LargeLocalInit = LargeLocalInit_t(0x4A62A0);

	Load_Stream_t Load_Stream = Load_Stream_t(0x470E30);
	Load_XString_t Load_XString = Load_XString_t(0x47FDA0);
	Load_XModelPtr_t Load_XModelPtr = Load_XModelPtr_t(0x4FCA70);
	Load_XModelSurfsFixup_t Load_XModelSurfsFixup = Load_XModelSurfsFixup_t(0x40D7A0);
	Load_XStringArray_t Load_XStringArray = Load_XStringArray_t(0x4977F0);
	Load_XStringCustom_t Load_XStringCustom = Load_XStringCustom_t(0x4E0DD0);
	Load_FxEffectDefHandle_t Load_FxEffectDefHandle = Load_FxEffectDefHandle_t(0x4D9B90);
	Load_FxElemDef_t Load_FxElemDef = Load_FxElemDef_t(0x45AD90);
	Load_GfxImagePtr_t Load_GfxImagePtr = Load_GfxImagePtr_t(0x4C13D0);
	Load_Texture_t Load_Texture = Load_Texture_t(0x51F4E0);
	Load_GfxTextureLoad_t Load_GfxTextureLoad = Load_GfxTextureLoad_t(0x4D3210);
	Load_SndAliasCustom_t Load_SndAliasCustom = Load_SndAliasCustom_t(0x49B6B0);
	Load_MaterialHandle_t Load_MaterialHandle = Load_MaterialHandle_t(0x403960);
	Load_PhysCollmapPtr_t Load_PhysCollmapPtr = Load_PhysCollmapPtr_t(0x47E990);
	Load_PhysPresetPtr_t Load_PhysPresetPtr = Load_PhysPresetPtr_t(0x4FAD30);
	Load_TracerDefPtr_t Load_TracerDefPtr = Load_TracerDefPtr_t(0x493090);
	Load_snd_alias_list_nameArray_t Load_snd_alias_list_nameArray = Load_snd_alias_list_nameArray_t(0x4499F0);

	Menus_CloseAll_t Menus_CloseAll = Menus_CloseAll_t(0x4BA5B0);
    Menus_CloseRequest_t Menus_CloseRequest = Menus_CloseRequest_t(0x430D50);
	Menus_OpenByName_t Menus_OpenByName = Menus_OpenByName_t(0x4CCE60);
	Menus_FindByName_t Menus_FindByName = Menus_FindByName_t(0x487240);
	Menu_IsVisible_t Menu_IsVisible = Menu_IsVisible_t(0x4D77D0);
	Menus_MenuIsInStack_t Menus_MenuIsInStack = Menus_MenuIsInStack_t(0x47ACB0);

	MSG_Init_t MSG_Init = MSG_Init_t(0x45FCA0);
	MSG_ReadData_t MSG_ReadData = MSG_ReadData_t(0x4527C0);
	MSG_ReadLong_t MSG_ReadLong = MSG_ReadLong_t(0x4C9550);
	MSG_ReadShort_t MSG_ReadShort = MSG_ReadShort_t(0x40BDD0);
	MSG_ReadInt64_t MSG_ReadInt64 = MSG_ReadInt64_t(0x4F1850);
	MSG_ReadString_t MSG_ReadString = MSG_ReadString_t(0x60E2B0);
	MSG_ReadStringLine_t MSG_ReadStringLine = MSG_ReadStringLine_t(0x4FEF30);
	MSG_WriteByte_t MSG_WriteByte = MSG_WriteByte_t(0x48C520);
	MSG_WriteData_t MSG_WriteData = MSG_WriteData_t(0x4F4120);
	MSG_WriteLong_t MSG_WriteLong = MSG_WriteLong_t(0x41CA20);
	MSG_WriteShort_t MSG_WriteShort = MSG_WriteShort_t(0x503B90);
	MSG_WriteString_t MSG_WriteString = MSG_WriteString_t(0x463820);
	MSG_WriteBitsCompress_t MSG_WriteBitsCompress = MSG_WriteBitsCompress_t(0x4319D0);
	MSG_ReadByte_t MSG_ReadByte = MSG_ReadByte_t(0x4C1C20);
	MSG_ReadBitsCompress_t MSG_ReadBitsCompress = MSG_ReadBitsCompress_t(0x4DCC30);

	NetadrToSockadr_t NetadrToSockadr = NetadrToSockadr_t(0x4B4B40);

	NET_AdrToString_t NET_AdrToString = NET_AdrToString_t(0x469880);
	NET_CompareAdr_t NET_CompareAdr = NET_CompareAdr_t(0x4D0AA0);
	NET_ErrorString_t NET_ErrorString = NET_ErrorString_t(0x4E7720);
	NET_Init_t NET_Init = NET_Init_t(0x491860);
	NET_IsLocalAddress_t NET_IsLocalAddress = NET_IsLocalAddress_t(0x402BD0);
	NET_StringToAdr_t NET_StringToAdr = NET_StringToAdr_t(0x409010);
	NET_OutOfBandPrint_t NET_OutOfBandPrint = NET_OutOfBandPrint_t(0x4AEF00);
	NET_OutOfBandData_t NET_OutOfBandData = NET_OutOfBandData_t(0x49C7E0);

	Live_MPAcceptInvite_t Live_MPAcceptInvite = Live_MPAcceptInvite_t(0x420A6D);
	Live_GetMapIndex_t Live_GetMapIndex = Live_GetMapIndex_t(0x4F6440);
	Live_GetPrestige_t Live_GetPrestige = Live_GetPrestige_t(0x430F90);
	Live_GetXp_t Live_GetXp = Live_GetXp_t(0x404C60);

	LoadModdableRawfile_t LoadModdableRawfile = LoadModdableRawfile_t(0x61ABC0);

	PC_ReadToken_t PC_ReadToken = PC_ReadToken_t(0x4ACCD0);
	PC_ReadTokenHandle_t PC_ReadTokenHandle = PC_ReadTokenHandle_t(0x4D2060);
	PC_SourceError_t PC_SourceError = PC_SourceError_t(0x467A00);

	Party_GetMaxPlayers_t Party_GetMaxPlayers = Party_GetMaxPlayers_t(0x4F5D60);
	PartyHost_CountMembers_t PartyHost_CountMembers = PartyHost_CountMembers_t(0x497330);
	PartyHost_GetMemberAddressBySlot_t PartyHost_GetMemberAddressBySlot = PartyHost_GetMemberAddressBySlot_t(0x44E100);
	PartyHost_GetMemberName_t PartyHost_GetMemberName = PartyHost_GetMemberName_t(0x44BE90);

	Playlist_ParsePlaylists_t Playlist_ParsePlaylists = Playlist_ParsePlaylists_t(0x4295A0);

	R_AddCmdDrawStretchPic_t R_AddCmdDrawStretchPic = R_AddCmdDrawStretchPic_t(0x509770);
	R_AllocStaticIndexBuffer_t R_AllocStaticIndexBuffer = R_AllocStaticIndexBuffer_t(0x51E7A0);
	R_Cinematic_StartPlayback_Now_t R_Cinematic_StartPlayback_Now = R_Cinematic_StartPlayback_Now_t(0x51C5B0);
	R_RegisterFont_t R_RegisterFont = R_RegisterFont_t(0x505670);
	R_AddCmdDrawText_t R_AddCmdDrawText = R_AddCmdDrawText_t(0x509D80);
	R_LoadGraphicsAssets_t R_LoadGraphicsAssets = R_LoadGraphicsAssets_t(0x506AC0);
	R_TextWidth_t R_TextWidth = R_TextWidth_t(0x5056C0);
	R_TextHeight_t R_TextHeight = R_TextHeight_t(0x505770);
	R_FlushSun_t R_FlushSun = R_FlushSun_t(0x53FB50);
	R_SortWorldSurfaces_t R_SortWorldSurfaces = R_SortWorldSurfaces_t(0x53DC10);

	RemoveRefToObject_t RemoveRefToObject = RemoveRefToObject_t(0x437190);

	Scr_LoadGameType_t Scr_LoadGameType = Scr_LoadGameType_t(0x4D9520);

	Scr_LoadScript_t Scr_LoadScript = Scr_LoadScript_t(0x45D940);
	Scr_GetFunctionHandle_t Scr_GetFunctionHandle = Scr_GetFunctionHandle_t(0x4234F0);

	Scr_GetString_t Scr_GetString = Scr_GetString_t(0x425900);
	Scr_GetFloat_t Scr_GetFloat = Scr_GetFloat_t(0x443140);
	Scr_GetInt_t Scr_GetInt = Scr_GetInt_t(0x4F31D0);
	Scr_GetObject_t Scr_GetObject = Scr_GetObject_t(0x462100);
	Scr_GetNumParam_t Scr_GetNumParam = Scr_GetNumParam_t(0x4B0E90);

	Scr_ExecThread_t Scr_ExecThread = Scr_ExecThread_t(0x4AD0B0);
	Scr_FreeThread_t Scr_FreeThread = Scr_FreeThread_t(0x4BD320);

	Scr_AddEntity_t Scr_AddEntity = Scr_AddEntity_t(0x4BFB40);
	Scr_AddString_t Scr_AddString = Scr_AddString_t(0x412310);
	Scr_AddInt_t Scr_AddInt = Scr_AddInt_t(0x41D7D0);
	Scr_AddFloat_t Scr_AddFloat = Scr_AddFloat_t(0x61E860);
	Scr_AddObject_t Scr_AddObject = Scr_AddObject_t(0x430F40);
	Scr_Notify_t Scr_Notify = Scr_Notify_t(0x4A4750);
	Scr_NotifyLevel_t Scr_NotifyLevel = Scr_NotifyLevel_t(0x4D9C30);

	Scr_Error_t Scr_Error = Scr_Error_t(0x61E8B0);
	Scr_GetType_t Scr_GetType = Scr_GetType_t(0x422900);

	Scr_ClearOutParams_t Scr_ClearOutParams = Scr_ClearOutParams_t(0x4386E0);

	Scr_RegisterFunction_t Scr_RegisterFunction = Scr_RegisterFunction_t(0x492D50);
	Scr_ShutdownAllocNode_t Scr_ShutdownAllocNode = Scr_ShutdownAllocNode_t(0x441650);
	Scr_IsSystemActive_t Scr_IsSystemActive = Scr_IsSystemActive_t(0x4B24E0);

	Script_Alloc_t Script_Alloc = Script_Alloc_t(0x422E70);
	Script_SetupTokens_t Script_SetupTokens = Script_SetupTokens_t(0x4E6950);
	Script_CleanString_t Script_CleanString = Script_CleanString_t(0x498220);

	SE_Load_t SE_Load = SE_Load_t(0x502A30);

	SEH_StringEd_GetString_t SEH_StringEd_GetString = SEH_StringEd_GetString_t(0x44BB30);

	Dvar_SetFromStringByName_t Dvar_SetFromStringByName = Dvar_SetFromStringByName_t(0x4F52E0);
	Dvar_SetFromStringByNameFromSource_t Dvar_SetFromStringByNameFromSource = Dvar_SetFromStringByNameFromSource_t(0x4FC770);
	Dvar_SetStringByName_t Dvar_SetStringByName = Dvar_SetStringByName_t(0x44F060);

	SL_ConvertToString_t SL_ConvertToString = SL_ConvertToString_t(0x4EC1D0);
	SL_GetString_t SL_GetString = SL_GetString_t(0x4CDC10);

	SND_Init_t SND_Init = SND_Init_t(0x46A630);
	SND_InitDriver_t SND_InitDriver = SND_InitDriver_t(0x4F5090);

	SockadrToNetadr_t SockadrToNetadr = SockadrToNetadr_t(0x4F8460);

	Steam_JoinLobby_t Steam_JoinLobby = Steam_JoinLobby_t(0x49CF70);

	StringTable_Lookup_t StringTable_Lookup = StringTable_Lookup_t(0x42F0E0);
	StringTable_HashString_t StringTable_HashString = StringTable_HashString_t(0x475EB0);

	SV_AddTestClient_t SV_AddTestClient = SV_AddTestClient_t(0x48AD30);
	SV_GameClientNum_Score_t SV_GameClientNum_Score = SV_GameClientNum_Score_t(0x469AC0);
	SV_GameSendServerCommand_t SV_GameSendServerCommand = SV_GameSendServerCommand_t(0x4BC3A0);
	SV_Cmd_TokenizeString_t SV_Cmd_TokenizeString = SV_Cmd_TokenizeString_t(0x4B5780);
	SV_Cmd_EndTokenizedString_t SV_Cmd_EndTokenizedString = SV_Cmd_EndTokenizedString_t(0x464750);
	SV_DirectConnect_t SV_DirectConnect = SV_DirectConnect_t(0x460480);
	SV_SetConfigstring_t SV_SetConfigstring = SV_SetConfigstring_t(0x4982E0);
	SV_Loaded_t SV_Loaded = SV_Loaded_t(0x4EE3E0);

	NET_DeferPacketToClient_t NET_DeferPacketToClient = NET_DeferPacketToClient_t(0x4C8AA0);

	SV_ClientThink_t SV_ClientThink = SV_ClientThink_t(0x44ADD0);

	Sys_Error_t Sys_Error = Sys_Error_t(0x4E0200);
	Sys_FreeFileList_t Sys_FreeFileList = Sys_FreeFileList_t(0x4D8580);
	Sys_IsDatabaseReady_t Sys_IsDatabaseReady = Sys_IsDatabaseReady_t(0x4CA4A0);
	Sys_IsDatabaseReady2_t Sys_IsDatabaseReady2 = Sys_IsDatabaseReady2_t(0x441280);
	Sys_IsMainThread_t Sys_IsMainThread = Sys_IsMainThread_t(0x4C37D0);
	Sys_IsRenderThread_t Sys_IsRenderThread = Sys_IsRenderThread_t(0x4B20E0);
	Sys_IsServerThread_t Sys_IsServerThread = Sys_IsServerThread_t(0x4B0270);
	Sys_IsDatabaseThread_t Sys_IsDatabaseThread = Sys_IsDatabaseThread_t(0x4C6020);
	Sys_SendPacket_t Sys_SendPacket = Sys_SendPacket_t(0x60FDC0);
	Sys_ShowConsole_t Sys_ShowConsole = Sys_ShowConsole_t(0x4305E0);
	Sys_SuspendOtherThreads_t Sys_SuspendOtherThreads = Sys_SuspendOtherThreads_t(0x45A190);
	Sys_ListFiles_t Sys_ListFiles = Sys_ListFiles_t(0x45A660);
	Sys_Milliseconds_t Sys_Milliseconds = Sys_Milliseconds_t(0x42A660);

	TeleportPlayer_t TeleportPlayer = TeleportPlayer_t(0x496850);

	UI_AddMenuList_t UI_AddMenuList = UI_AddMenuList_t(0x4533C0);
	UI_CheckStringTranslation_t UI_CheckStringTranslation = UI_CheckStringTranslation_t(0x4FB010);
	UI_LoadMenus_t UI_LoadMenus = UI_LoadMenus_t(0x641460);
	UI_UpdateArenas_t UI_UpdateArenas = UI_UpdateArenas_t(0x4A95B0);
	UI_SortArenas_t UI_SortArenas = UI_SortArenas_t(0x630AE0);
	UI_DrawHandlePic_t UI_DrawHandlePic = UI_DrawHandlePic_t(0x4D0EA0);
	UI_GetContext_t UI_GetContext = UI_GetContext_t(0x4F8940);
	UI_TextWidth_t UI_TextWidth = UI_TextWidth_t(0x6315C0);
	UI_DrawText_t UI_DrawText = UI_DrawText_t(0x49C0D0);

	Win_GetLanguage_t Win_GetLanguage = Win_GetLanguage_t(0x45CBA0);

	Vec3UnpackUnitVec_t Vec3UnpackUnitVec = Vec3UnpackUnitVec_t(0x45CA90);

	unzClose_t unzClose = unzClose_t(0x41BF20);

	XAssetHeader* DB_XAssetPool = reinterpret_cast<XAssetHeader*>(0x7998A8);
	unsigned int* g_poolSize = reinterpret_cast<unsigned int*>(0x7995E8);

	DWORD* cmd_id = reinterpret_cast<DWORD*>(0x1AAC5D0);
	DWORD* cmd_argc = reinterpret_cast<DWORD*>(0x1AAC614);
	char*** cmd_argv = reinterpret_cast<char***>(0x1AAC634);

	DWORD* cmd_id_sv = reinterpret_cast<DWORD*>(0x1ACF8A0);
	DWORD* cmd_argc_sv = reinterpret_cast<DWORD*>(0x1ACF8E4);
	char*** cmd_argv_sv = reinterpret_cast<char***>(0x1ACF904);

	cmd_function_t** cmd_functions = reinterpret_cast<cmd_function_t**>(0x1AAC658);

	source_t **sourceFiles = reinterpret_cast<source_t **>(0x7C4A98);
	keywordHash_t **menuParseKeywordHash = reinterpret_cast<keywordHash_t **>(0x63AE928);

	int* deferredQueueSize = reinterpret_cast<int*>(0x1CC85EC);

	int* svs_numclients = reinterpret_cast<int*>(0x31D938C);
	client_t* svs_clients = reinterpret_cast<client_t*>(0x31D9390);

	int* svs_time = reinterpret_cast<int*>(0x31D9384);

	UiContext *uiContext = reinterpret_cast<UiContext *>(0x62E2858);

	int* arenaCount = reinterpret_cast<int*>(0x62E6930);
	mapArena_t* arenas = reinterpret_cast<mapArena_t*>(0x62E6934);

	int* gameTypeCount = reinterpret_cast<int*>(0x62E50A0);
	gameTypeName_t* gameTypes = reinterpret_cast<gameTypeName_t*>(0x62E50A4);

	searchpath_t** fs_searchpaths = reinterpret_cast<searchpath_t**>(0x63D96E0);

	XBlock** g_streamBlocks = reinterpret_cast<XBlock**>(0x16E554C);
	int* g_streamPos = reinterpret_cast<int*>(0x16E5554);
	int* g_streamPosIndex = reinterpret_cast<int*>(0x16E5578);

	bool* g_lobbyCreateInProgress = reinterpret_cast<bool*>(0x66C9BC2);
	party_t** partyIngame = reinterpret_cast<party_t**>(0x1081C00);
	PartyData_s** partyData = reinterpret_cast<PartyData_s**>(0x107E500);

	int* numIP = reinterpret_cast<int*>(0x64A1E68);
	netIP_t* localIP = reinterpret_cast<netIP_t*>(0x64A1E28);

	int* demoFile = reinterpret_cast<int*>(0xA5EA1C);
	int* demoPlaying = reinterpret_cast<int*>(0xA5EA0C);
	int* demoRecording = reinterpret_cast<int*>(0xA5EA08);
	int* serverMessageSequence = reinterpret_cast<int*>(0xA3E9B4);

	gentity_t* g_entities = reinterpret_cast<gentity_t*>(0x18835D8);

	netadr_t* connectedHost = reinterpret_cast<netadr_t*>(0xA1E888);

	SOCKET* ip_socket = reinterpret_cast<SOCKET*>(0x64A3008);

	uint32_t* com_frameTime = reinterpret_cast<uint32_t*>(0x1AD8F3C);

	SafeArea* safeArea = reinterpret_cast<SafeArea*>(0xA15F3C);

	SpawnVar* spawnVars = reinterpret_cast<SpawnVar*>(0x1A83DE8);
	MapEnts** marMapEntsPtr = reinterpret_cast<MapEnts**>(0x112AD34);

	IDirect3D9** d3d9 = reinterpret_cast<IDirect3D9**>(0x66DEF84);
	IDirect3DDevice9** dx_ptr = reinterpret_cast<IDirect3DDevice9**>(0x66DEF88);

	mapname_t* mapnames = reinterpret_cast<mapname_t*>(0x7471D0);

	char*** varXString = reinterpret_cast<char***>(0x112B340);
	TracerDef*** varTracerDefPtr = reinterpret_cast<TracerDef***>(0x112B3BC);
	XModel*** varXModelPtr = reinterpret_cast<XModel***>(0x112A934);
	XModel** varXModel = reinterpret_cast<XModel**>(0x112AE14);
	PathData** varPathData = reinterpret_cast<PathData**>(0x112AD7C);
	const char** varConstChar = reinterpret_cast<const char**>(0x112A774);
	Material*** varMaterialHandle = reinterpret_cast<Material***>(0x112A878);
	FxEffectDef*** varFxEffectDefHandle = reinterpret_cast<FxEffectDef***>(0x112ACC0);
	PhysCollmap*** varPhysCollmapPtr = reinterpret_cast<PhysCollmap***>(0x112B440);
	PhysPreset*** varPhysPresetPtr = reinterpret_cast<PhysPreset***>(0x112B378);
	Game::MaterialPass** varMaterialPass = reinterpret_cast<Game::MaterialPass**>(0x112A960);
	snd_alias_list_t*** varsnd_alias_list_name = reinterpret_cast<snd_alias_list_t***>(0x112AF38);

	FxElemField* s_elemFields = reinterpret_cast<FxElemField*>(0x73B848);

	infoParm_t* infoParams = reinterpret_cast<infoParm_t*>(0x79D260); // Count 0x1E

	XZone* g_zones = reinterpret_cast<XZone*>(0x14C0F80);
	unsigned short* db_hashTable = reinterpret_cast<unsigned short*>(0x12412B0);

	ScriptContainer* scriptContainer = reinterpret_cast<ScriptContainer*>(0x2040D00);

	XAssetHeader ReallocateAssetPool(XAssetType type, unsigned int newSize)
	{
		int elSize = DB_GetXAssetSizeHandlers[type]();
		XAssetHeader poolEntry = { Utils::Memory::GetAllocator()->allocate(newSize * elSize) };
		DB_XAssetPool[type] = poolEntry;
		g_poolSize[type] = newSize;
		return poolEntry;
	}

	const char* TableLookup(StringTable* stringtable, int row, int column)
	{
		if (!stringtable || !stringtable->values || row >= stringtable->rowCount || column >= stringtable->columnCount) return "";

		const char* value = stringtable->values[row * stringtable->columnCount + column].string;
		if (!value) value = "";

		return value;
	}

	const char* UI_LocalizeMapName(const char* mapName)
	{
		for (int i = 0; i < *arenaCount; ++i)
		{
			if (!_stricmp(Components::ArenaLength::NewArenas[i].mapName, mapName))
			{
				char* uiName = &Components::ArenaLength::NewArenas[i].uiName[0];
				if ((uiName[0] == 'M' && uiName[1] == 'P') || (uiName[0] == 'P' && uiName[1] == 'A')) // MPUI/PATCH
				{
					return SEH_StringEd_GetString(uiName);
				}

				return uiName;
			}
		}

		return mapName;
	}

	const char* UI_LocalizeGameType(const char* gameType)
	{
		if (!gameType || !*gameType)
		{
			return "";
		}

		for (int i = 0; i < *gameTypeCount; ++i)
		{
			if (!_stricmp(gameTypes[i].gameType, gameType))
			{
				return SEH_StringEd_GetString(gameTypes[i].uiName);
			}
		}

		return gameType;
	}

	const char *DB_GetXAssetName(XAsset *asset)
	{
		if (!asset) return "";
		return DB_GetXAssetNameHandlers[asset->type](&asset->header);
	}

	XAssetType DB_GetXAssetNameType(const char* name)
	{
		for (int i = 0; i < ASSET_TYPE_COUNT; ++i)
		{
			XAssetType type = static_cast<XAssetType>(i);
			if (!_stricmp(DB_GetXAssetTypeName(type), name))
			{
				// Col map workaround!
				if (type == Game::XAssetType::ASSET_TYPE_CLIPMAP_SP)
				{
					return Game::XAssetType::ASSET_TYPE_CLIPMAP_MP;
				}

				return type;
			}
		}

		return ASSET_TYPE_INVALID;
	}

	int DB_GetZoneIndex(const std::string& name)
	{
		for (int i = 0; i < 32; ++i)
		{
			if (Game::g_zones[i].name == name)
			{
				return i;
			}
		}

		return -1;
	}

	bool DB_IsZoneLoaded(const char* zone)
	{
		int zoneCount = Utils::Hook::Get<int>(0x1261BCC);
		char* zoneIndices = reinterpret_cast<char*>(0x16B8A34);
		char* zoneData = reinterpret_cast<char*>(0x14C0F80);

		for (int i = 0; i < zoneCount; ++i)
		{
			std::string name = zoneData + 4 + 0xA4 * (zoneIndices[i] & 0xFF);

			if (name == zone)
			{
				return true;
			}
		}

		return false;
	}

	void DB_EnumXAssetEntries(XAssetType type, std::function<void(XAssetEntry*)> callback, bool overrides, bool lock)
	{
		volatile long* lockVar = reinterpret_cast<volatile long*>(0x16B8A54);
		if (lock) InterlockedIncrement(lockVar);

		while (lock && *reinterpret_cast<volatile long*>(0x16B8A58)) std::this_thread::sleep_for(1ms);

		unsigned int index = 0;
		do
		{
			unsigned short hashIndex = db_hashTable[index];
			if (hashIndex)
			{
				do
				{
					XAssetEntry* asset = &Components::Maps::GetAssetEntryPool()[hashIndex];
					hashIndex = asset->nextHash;
					if (asset->asset.type == type)
					{
						callback(asset);
						if (overrides)
						{
							unsigned short overrideIndex = asset->nextOverride;
							if (asset->nextOverride)
							{
								do
								{
									asset = &Components::Maps::GetAssetEntryPool()[overrideIndex];
									callback(asset);
									overrideIndex = asset->nextOverride;
								} while (overrideIndex);
							}
						}
					}
				} while (hashIndex);
			}
			++index;
		} while (index < 74000);

		if(lock) InterlockedDecrement(lockVar);
	}

	// this cant be MessageBox because windows.h has a define that converts it to MessageBoxW. which is just stupid
	void ShowMessageBox(const std::string& message, const std::string& title)
	{
		if (!Game::CL_IsCgameInitialized())
		{
			Dvar_SetStringByName("com_errorMessage", message.data());
			Dvar_SetStringByName("com_errorTitle", title.data());
			Cbuf_AddText(0, "openmenu error_popmenu_lobby");
		}
	}

	unsigned int R_HashString(const char* string)
	{
		unsigned int hash = 0;

		while (*string)
		{
			hash = (*string | 0x20) ^ (33 * hash);
			++string;
		}

		return hash;
	}

	void SV_KickClientError(client_t* client, const std::string& reason)
	{
		if (client->state < 5)
		{
			Components::Network::SendCommand(client->addr, "error", reason);
		}

		SV_KickClient(client, reason.data());
	}

	void Scr_iPrintLn(int clientNum, const std::string& message)
	{
		Game::SV_GameSendServerCommand(clientNum, 0, Utils::String::VA("%c \"%s\"", 0x66, message.data()));
	}

	void Scr_iPrintLnBold(int clientNum, const std::string& message)
	{
		Game::SV_GameSendServerCommand(clientNum, 0, Utils::String::VA("%c \"%s\"", 0x67, message.data()));
	}

	int FS_FOpenFileReadCurrentThread(const char* file, int* fh)
	{
		if (GetCurrentThreadId() == *reinterpret_cast<DWORD*>(0x1CDE7FC))
		{
			return FS_FOpenFileRead(file, fh);
		}
		else if (GetCurrentThreadId() == *reinterpret_cast<DWORD*>(0x1CDE814))
		{
			return FS_FOpenFileReadDatabase(file, fh);
		}
		else
		{
			*fh = NULL;
			return -1;
		}
	}

	void Load_IndexBuffer(void* data, IDirect3DIndexBuffer9** storeHere, int count)
	{
		if (Components::Dvar::Var("r_loadForRenderer").get<bool>())
		{
			void* buffer = R_AllocStaticIndexBuffer(storeHere, 2 * count);
			std::memcpy(buffer, data, 2 * count);

			if (storeHere && *storeHere)
			{
				(*storeHere)->Unlock();
			}
		}
	}

	char* Com_GetParseThreadInfo()
	{
		if (Game::Sys_IsMainThread())
		{
			return reinterpret_cast<char*>(0x6466628);
		}
		else if (Game::Sys_IsRenderThread())
		{
			return reinterpret_cast<char*>(0x646AC34);
		}
		else if (Game::Sys_IsServerThread())
		{
			return reinterpret_cast<char*>(0x646F240);
		}
		else if(Game::Sys_IsDatabaseThread())
		{
			return reinterpret_cast<char*>(0x647384C);
		}
		else
		{
			return nullptr;
		}
	}

	void Com_SetParseNegativeNumbers(int parse)
	{
		char* g_parse = Com_GetParseThreadInfo();

		if (g_parse)
		{
			g_parse[1056 * *(reinterpret_cast<DWORD*>(g_parse) + 4224) + 1032] = parse != 0;
		}
	}

	int CL_GetMaxXP()
	{
		StringTable* rankTable = DB_FindXAssetHeader(ASSET_TYPE_STRINGTABLE, "mp/rankTable.csv").stringTable;
		const char* maxrank = StringTable_Lookup(rankTable, 0, "maxrank", 1);
		return atoi(StringTable_Lookup(rankTable, 0, maxrank, 7));
	}

	void Vec3Normalize(vec3_t& vec)
	{
		const auto length = std::sqrt(std::pow(vec[0], 2) + std::pow(vec[1], 2) + std::pow(vec[2], 2));
		vec[0] /= length;
		vec[1] /= length;
		vec[2] /= length;
	}

	void Vec2UnpackTexCoords(const PackedTexCoords in, vec2_t* out)
	{
		unsigned int v3; // xmm1_4

		if (LOWORD(in.packed))
			v3 = ((in.packed & 0x8000) << 16) | (((((in.packed & 0x3FFF) << 14) - (~(LOWORD(in.packed) << 14) & 0x10000000)) ^ 0x80000001) >> 1);
		else
			v3 = 0;

		(*out)[0] = *reinterpret_cast<float*>(&v3);

		if (HIWORD(in.packed))
			v3 = ((HIWORD(in.packed) & 0x8000) << 16) | (((((HIWORD(in.packed) & 0x3FFF) << 14)
				- (~(HIWORD(in.packed) << 14) & 0x10000000)) ^ 0x80000001) >> 1);
		else
			v3 = 0;

		(*out)[1] = *reinterpret_cast<float*>(&v3);
	}

	void MatrixVecMultiply(const float (& mulMat)[3][3], const vec3_t& mulVec, vec3_t& solution)
	{
		vec3_t res;
		res[0] = mulMat[0][0] * mulVec[0] + mulMat[1][0] * mulVec[1] + mulMat[2][0] * mulVec[2];
		res[1] = mulMat[0][1] * mulVec[0] + mulMat[1][1] * mulVec[1] + mulMat[2][1] * mulVec[2];
		res[2] = mulMat[0][2] * mulVec[0] + mulMat[1][2] * mulVec[1] + mulMat[2][2] * mulVec[2];
		std::memmove(&solution[0], &res[0], sizeof(res));
	}

	void SortWorldSurfaces(GfxWorld* world)
	{
		DWORD* specular1 = reinterpret_cast<DWORD*>(0x69F105C);
		DWORD* specular2 = reinterpret_cast<DWORD*>(0x69F92D4);
		DWORD saveSpecular1 = *specular1;
		DWORD saveSpecular2 = *specular2;

		GfxWorld** gameWorld = reinterpret_cast<GfxWorld**>(0x66DEE94);
		GfxWorld* saveWorld = *gameWorld;

		*specular1 = 1;
		*specular2 = 1;
		*gameWorld = world;

		R_SortWorldSurfaces();

		*gameWorld = saveWorld;
		*specular1 = saveSpecular1;
		*specular2 = saveSpecular2;
	}

	void R_AddDebugBounds(float* color, Bounds* b)
	{
		Game::vec3_t v1, v2, v3, v4, v5, v6, v7, v8;
		float* center = b->midPoint;
		float* halfSize = b->halfSize;

		v1[0] = center[0] - halfSize[0];
		v1[1] = center[1] - halfSize[1];
		v1[2] = center[2] - halfSize[2];

		v2[0] = center[0] + halfSize[0];
		v2[1] = center[1] - halfSize[1];
		v2[2] = center[2] - halfSize[2];

		v3[0] = center[0] - halfSize[0];
		v3[1] = center[1] + halfSize[1];
		v3[2] = center[2] - halfSize[2];

		v4[0] = center[0] + halfSize[0];
		v4[1] = center[1] + halfSize[1];
		v4[2] = center[2] - halfSize[2];

		v5[0] = center[0] - halfSize[0];
		v5[1] = center[1] - halfSize[1];
		v5[2] = center[2] + halfSize[2];

		v6[0] = center[0] + halfSize[0];
		v6[1] = center[1] - halfSize[1];
		v6[2] = center[2] + halfSize[2];

		v7[0] = center[0] - halfSize[0];
		v7[1] = center[1] + halfSize[1];
		v7[2] = center[2] + halfSize[2];

		v8[0] = center[0] + halfSize[0];
		v8[1] = center[1] + halfSize[1];
		v8[2] = center[2] + halfSize[2];

		// bottom
		Game::R_AddDebugLine(color, v1, v2);
		Game::R_AddDebugLine(color, v2, v4);
		Game::R_AddDebugLine(color, v4, v3);
		Game::R_AddDebugLine(color, v3, v1);

		// top
		Game::R_AddDebugLine(color, v5, v6);
		Game::R_AddDebugLine(color, v6, v8);
		Game::R_AddDebugLine(color, v8, v7);
		Game::R_AddDebugLine(color, v7, v5);

		// verticals
		Game::R_AddDebugLine(color, v1, v5);
		Game::R_AddDebugLine(color, v2, v6);
		Game::R_AddDebugLine(color, v3, v7);
		Game::R_AddDebugLine(color, v4, v8);
	}

#pragma optimize("", off)
	__declspec(naked) float UI_GetScoreboardLeft(void* /*a1*/)
	{
		__asm
		{
			push eax
			pushad

			mov ecx, 590390h
			mov eax, [esp + 28h]
			call ecx
			mov [esp + 20h], eax
			popad
			pop eax

			retn
		}
	}

	__declspec(naked) XAssetHeader DB_FindXAssetDefaultHeaderInternal(XAssetType /*type*/)
	{
		__asm
		{
			push eax
			pushad

			mov eax, 5BB210h
			mov edi, [esp + 28h]
			call eax

			mov [esp + 20h], eax
			popad
			pop eax

			retn
		}
	}

	__declspec(naked) XAssetEntry* DB_FindXAssetEntry(XAssetType /*type*/, const char* /*name*/)
	{
		__asm
		{
			push eax
			pushad

			mov edi, [esp + 2Ch] // name
			push edi

			mov edi, [esp + 2Ch] // type

			mov eax, 5BB1B0h
			call eax

			add esp, 4h

			mov [esp + 20h], eax
			popad
			pop eax

			retn
		}
	}

	__declspec(naked) void FS_AddLocalizedGameDirectory(const char* /*path*/, const char* /*dir*/)
	{
		__asm
		{
			pushad

			mov ebx, [esp + 24h]
			mov eax, [esp + 28h]
			mov ecx, 642EF0h
			call ecx

			popad

			retn
		}
	}

	__declspec(naked) void R_LoadSunThroughDvars(const char* /*mapname*/, sunflare_t* /*sun*/)
	{
		__asm
		{
			pushad
			push [esp + 28h]
			mov eax, [esp + 28h]

			mov ecx, 53F990h
			call ecx

			add esp, 4h
			popad
			retn
		}
	}

	__declspec(naked) void R_SetSunFromDvars(sunflare_t* /*sun*/)
	{
		__asm
		{
			pushad
			mov esi, [esp + 24h]

			mov eax, 53F6D0h
			call eax

			popad
			retn
		}
	}

	__declspec(naked) void SV_KickClient(client_t* /*client*/, const char* /*reason*/)
	{
		__asm
		{
			pushad

			mov edi, 0
			mov esi, [esp + 24h]
			push [esp + 28h]
			push 0
			push 0

			mov eax, 6249A0h
			call eax
			add esp, 0Ch

			popad

			retn
		}
	}

	__declspec(naked) void Scr_NotifyId(unsigned int /*id*/, unsigned __int16 /*stringValue*/, unsigned int /*paramcount*/)
	{
		__asm
		{
			pushad

			mov eax, [esp + 2Ch] // paramcount

			push [esp + 28h] // stringValue
			push [esp + 28h] // id

			mov edx, 61E670h // Scr_NotifyId
			call edx

			add esp, 8h

			popad
			retn
		}
	}

	__declspec(naked) void IN_KeyUp(kbutton_t* /*button*/)
	{
		__asm
		{
			pushad
			mov esi, [esp + 24h]
			mov eax, 5A5580h
			call eax
			popad
			retn
		}
	}

	__declspec(naked) void IN_KeyDown(kbutton_t* /*button*/)
	{
		__asm
		{
			pushad
			mov esi, [esp + 24h]
			mov eax, 5A54E0h
			call eax
			popad
			retn
		}
	}

	__declspec(naked) void Load_VertexBuffer(void* /*data*/, IDirect3DVertexBuffer9** /*where*/, int /*len*/)
	{
		__asm
		{
			pushad

			mov eax, [esp + 2Ch]
			mov edi, [esp + 28h]
			push [esp + 24h]

			mov ebx, 5112C0h
			call ebx
			add esp, 4

			popad

			retn
		}
	}

	__declspec(naked) void Image_Setup(GfxImage* /*image*/, unsigned int /*width*/, unsigned int /*height*/, unsigned int /*depth*/, unsigned int /*flags*/, _D3DFORMAT /*format*/)
	{
		__asm
		{
			pushad

			mov eax, [esp + 24h] // image
			mov edi, [esp + 28h] // width
			push [esp + 38h]     // format
			push [esp + 38h]     // flags
			push [esp + 38h]     // depth
			push [esp + 38h]     // height

			mov ecx, 54AF50h
			call ecx

			add esp, 10h

			popad
			retn
		}
	}

	__declspec(naked) void Menu_FreeItemMemory(itemDef_s* /*item*/)
	{
		__asm
		{
			pushad
			mov edi, [esp + 24h]
			mov eax, 63D880h
			call eax
			popad
			retn
		}
	}

	__declspec(naked) void R_AddDebugLine(float* /*color*/, float* /*v1*/, float* /*v2*/)
	{
		__asm
		{
			pushad

			mov eax, [esp + 2Ch] // v2
			push eax

			mov eax, [esp + 2Ch] // v1
			push eax

			// debugglobals
			mov esi, ds:66DAD78h
			add esi, 268772

			mov edi, [esp + 2Ch] // color

			mov eax, 51CEB0h
			call eax

			add esp, 8h

			popad
			retn
		}
	}

	__declspec(naked) void R_AddDebugString(float* /*color*/, float* /*pos*/, float /*scale*/, const char* /*string*/)
	{
		__asm
		{
			pushad

			mov eax, [esp + 30h] // string
			push eax

			mov eax, [esp + 30h] // scale
			push eax

			mov eax, [esp + 2Ch] // color
			push eax

			mov eax, [esp + 34h] // pos
			push eax

			// debugglobals
			mov edi, ds:66DAD78h
			add edi, 268772

			mov eax, 51D0A0h
			call eax

			add esp, 10h

			popad
			retn
		}
	}
#pragma optimize("", on)
}
