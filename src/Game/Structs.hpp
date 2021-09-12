#pragma once

#define PROTOCOL 0x95
#define NUM_CUSTOM_CLASSES 15
#define SEMANTIC_WATER_MAP 11
#define FX_ELEM_FIELD_COUNT 90

// This allows us to compile our structures in IDA, for easier reversing :3
#ifndef __cplusplus
#define IDA
#endif

#ifndef IDA
namespace Game
{
#endif

	typedef float vec_t;
	typedef vec_t vec2_t[2];
	typedef vec_t vec3_t[3];
	typedef vec_t vec4_t[4];

	typedef unsigned int scr_entref_t;
	typedef void(__cdecl * scr_function_t)(scr_entref_t);

	enum XAssetType
	{
		ASSET_TYPE_PHYSPRESET = 0x0,
		ASSET_TYPE_PHYSCOLLMAP = 0x1,
		ASSET_TYPE_XANIMPARTS = 0x2,
		ASSET_TYPE_XMODEL_SURFS = 0x3,
		ASSET_TYPE_XMODEL = 0x4,
		ASSET_TYPE_MATERIAL = 0x5,
		ASSET_TYPE_PIXELSHADER = 0x6,
		ASSET_TYPE_VERTEXSHADER = 0x7,
		ASSET_TYPE_VERTEXDECL = 0x8,
		ASSET_TYPE_TECHNIQUE_SET = 0x9,
		ASSET_TYPE_IMAGE = 0xA,
		ASSET_TYPE_SOUND = 0xB,
		ASSET_TYPE_SOUND_CURVE = 0xC,
		ASSET_TYPE_LOADED_SOUND = 0xD,
		ASSET_TYPE_CLIPMAP_SP = 0xE,
		ASSET_TYPE_CLIPMAP_MP = 0xF,
		ASSET_TYPE_COMWORLD = 0x10,
		ASSET_TYPE_GAMEWORLD_SP = 0x11,
		ASSET_TYPE_GAMEWORLD_MP = 0x12,
		ASSET_TYPE_MAP_ENTS = 0x13,
		ASSET_TYPE_FXWORLD = 0x14,
		ASSET_TYPE_GFXWORLD = 0x15,
		ASSET_TYPE_LIGHT_DEF = 0x16,
		ASSET_TYPE_UI_MAP = 0x17,
		ASSET_TYPE_FONT = 0x18,
		ASSET_TYPE_MENULIST = 0x19,
		ASSET_TYPE_MENU = 0x1A,
		ASSET_TYPE_LOCALIZE_ENTRY = 0x1B,
		ASSET_TYPE_WEAPON = 0x1C,
		ASSET_TYPE_SNDDRIVER_GLOBALS = 0x1D,
		ASSET_TYPE_FX = 0x1E,
		ASSET_TYPE_IMPACT_FX = 0x1F,
		ASSET_TYPE_AITYPE = 0x20,
		ASSET_TYPE_MPTYPE = 0x21,
		ASSET_TYPE_CHARACTER = 0x22,
		ASSET_TYPE_XMODELALIAS = 0x23,
		ASSET_TYPE_RAWFILE = 0x24,
		ASSET_TYPE_STRINGTABLE = 0x25,
		ASSET_TYPE_LEADERBOARD = 0x26,
		ASSET_TYPE_STRUCTURED_DATA_DEF = 0x27,
		ASSET_TYPE_TRACER = 0x28,
		ASSET_TYPE_VEHICLE = 0x29,
		ASSET_TYPE_ADDON_MAP_ENTS = 0x2A,
		ASSET_TYPE_COUNT = 0x2B,
		ASSET_TYPE_STRING = 0x2B,
		ASSET_TYPE_ASSETLIST = 0x2C,
		ASSET_TYPE_INVALID = -1,
	};

	typedef enum : unsigned int
	{
		DVAR_FLAG_NONE = 0x0,			//no flags
		DVAR_FLAG_SAVED = 0x1,			//saves in config_mp.cfg for clients
		DVAR_FLAG_LATCHED = 0x2,			//no changing apart from initial value (although it might apply on a map reload, I think)
		DVAR_FLAG_CHEAT = 0x4,			//cheat
		DVAR_FLAG_REPLICATED = 0x8,			//on change, this is sent to all clients (if you are host)
		DVAR_FLAG_UNKNOWN10 = 0x10,			//unknown
		DVAR_FLAG_UNKNOWN20 = 0x20,			//unknown
		DVAR_FLAG_UNKNOWN40 = 0x40,			//unknown
		DVAR_FLAG_UNKNOWN80 = 0x80,			//unknown
		DVAR_FLAG_USERCREATED = 0x100,		//a 'set' type command created it
		DVAR_FLAG_USERINFO = 0x200,		//userinfo?
		DVAR_FLAG_SERVERINFO = 0x400,		//in the getstatus oob
		DVAR_FLAG_WRITEPROTECTED = 0x800,		//write protected
		DVAR_FLAG_UNKNOWN1000 = 0x1000,		//unknown
		DVAR_FLAG_READONLY = 0x2000,		//read only (same as 0x800?)
		DVAR_FLAG_UNKNOWN4000 = 0x4000,		//unknown
		DVAR_FLAG_UNKNOWN8000 = 0x8000,		//unknown
		DVAR_FLAG_UNKNOWN10000 = 0x10000,		//unknown
		DVAR_FLAG_DEDISAVED = 0x1000000,		//unknown
		DVAR_FLAG_NONEXISTENT = 0xFFFFFFFF	//no such dvar
	} dvar_flag;

	enum ImageCategory : char
	{
		IMG_CATEGORY_UNKNOWN = 0x0,
		IMG_CATEGORY_AUTO_GENERATED = 0x1,
		IMG_CATEGORY_LIGHTMAP = 0x2,
		IMG_CATEGORY_LOAD_FROM_FILE = 0x3,
		IMG_CATEGORY_RAW = 0x4,
		IMG_CATEGORY_FIRST_UNMANAGED = 0x5,
		IMG_CATEGORY_WATER = 0x5,
		IMG_CATEGORY_RENDERTARGET = 0x6,
		IMG_CATEGORY_TEMP = 0x7,
	} ;

	enum DvarSetSource
	{
		DVAR_SOURCE_INTERNAL = 0x0,
		DVAR_SOURCE_EXTERNAL = 0x1,
		DVAR_SOURCE_SCRIPT = 0x2,
		DVAR_SOURCE_DEVGUI = 0x3,
	};

	typedef enum : char
	{
		DVAR_TYPE_BOOL = 0x0,
		DVAR_TYPE_FLOAT = 0x1,
		DVAR_TYPE_FLOAT_2 = 0x2,
		DVAR_TYPE_FLOAT_3 = 0x3,
		DVAR_TYPE_FLOAT_4 = 0x4,
		DVAR_TYPE_INT = 0x5,
		DVAR_TYPE_ENUM = 0x6,
		DVAR_TYPE_STRING = 0x7,
		DVAR_TYPE_COLOR = 0x8,
		DVAR_TYPE_FLOAT_3_COLOR = 0x9,
		DVAR_TYPE_COUNT = 0xA,
	} dvar_type;

	typedef enum
	{
		CS_FREE = 0x0,
		CS_UNKNOWN1 = 0x1,
		CS_UNKNOWN2 = 0x2,
		CS_CONNECTED = 0x3,
		CS_CLIENTLOADING = 0x4,
		CS_ACTIVE = 0x5,
	} clientstate_t;

	struct FxEffectDef;
	struct pathnode_t;
	struct pathnode_tree_t;
	struct GfxPortal;
	struct Statement_s;
	struct MenuEventHandlerSet;
	struct menuDef_t;

	typedef struct cmd_function_s
	{
		cmd_function_s *next;
		const char *name;
		const char *autoCompleteDir;
		const char *autoCompleteExt;
		void(__cdecl *function)();
		int flags;
	} cmd_function_t;

#pragma pack(push, 4)
	struct kbutton_t
	{
		int down[2];
		unsigned int downtime;
		unsigned int msec;
		bool active;
		bool wasPressed;
	};
#pragma pack(pop)

	struct __declspec(align(4)) PhysPreset
	{
		const char *name;
		int type;
		float mass;
		float bounce;
		float friction;
		float bulletForceScale;
		float explosiveForceScale;
		const char *sndAliasPrefix;
		float piecesSpreadFraction;
		float piecesUpwardVelocity;
		bool tempDefaultToCylinder;
		bool perSurfaceSndAlias;
	};

	struct Bounds
	{
		float midPoint[3];
		float halfSize[3];
	};

	struct cplane_s
	{
		float normal[3];
		float dist;
		char type;
		char pad[3];
	};

	struct cbrushside_t
	{
		cplane_s *plane;
		unsigned __int16 materialNum;
		char firstAdjacentSideOffset;
		char edgeCount;
	};

	struct cbrush_t
	{
		unsigned __int16 numsides;
		unsigned __int16 glassPieceIndex;
		cbrushside_t *sides;
		char *baseAdjacentSide;
		__int16 axialMaterialNum[2][3];
		char firstAdjacentSideOffsets[2][3];
		char edgeCount[2][3];
	};

	struct BrushWrapper
	{
		Bounds bounds;
		cbrush_t brush;
		int totalEdgeCount;
		cplane_s *planes;
	};

	struct PhysGeomInfo
	{
		BrushWrapper *brushWrapper;
		int type;
		float orientation[3][3];
		Bounds bounds;
	};

	struct PhysMass
	{
		float centerOfMass[3];
		float momentsOfInertia[3];
		float productsOfInertia[3];
	};

	struct PhysCollmap
	{
		const char *name;
		unsigned int count;
		PhysGeomInfo *geoms;
		PhysMass mass;
		Bounds bounds;
	};

	union XAnimIndices
	{
		char *_1;
		unsigned __int16 *_2;
		void *data;
	};

	struct XAnimNotifyInfo
	{
		unsigned __int16 name;
		float time;
	};

	union XAnimDynamicFrames
	{
		char(*_1)[3];
		unsigned __int16(*_2)[3];
	};

	union XAnimDynamicIndices
	{
		char _1[1];
		unsigned __int16 _2[1];
	};

	struct __declspec(align(4)) XAnimPartTransFrames
	{
		float mins[3];
		float size[3];
		XAnimDynamicFrames frames;
		XAnimDynamicIndices indices;
	};

	union XAnimPartTransData
	{
		XAnimPartTransFrames frames;
		float frame0[3];
	};

	struct XAnimPartTrans
	{
		unsigned __int16 size;
		char smallTrans;
		XAnimPartTransData u;
	};

	struct __declspec(align(4)) XAnimDeltaPartQuatDataFrames2
	{
		__int16(*frames)[2];
		XAnimDynamicIndices indices;
	};

	union XAnimDeltaPartQuatData2
	{
		XAnimDeltaPartQuatDataFrames2 frames;
		__int16 frame0[2];
	};

	struct XAnimDeltaPartQuat2
	{
		unsigned __int16 size;
		XAnimDeltaPartQuatData2 u;
	};

	struct __declspec(align(4)) XAnimDeltaPartQuatDataFrames
	{
		__int16(*frames)[4];
		XAnimDynamicIndices indices;
	};

	union XAnimDeltaPartQuatData
	{
		XAnimDeltaPartQuatDataFrames frames;
		__int16 frame0[4];
	};

	struct XAnimDeltaPartQuat
	{
		unsigned __int16 size;
		XAnimDeltaPartQuatData u;
	};

	struct XAnimDeltaPart
	{
		XAnimPartTrans *trans;
		XAnimDeltaPartQuat2 *quat2;
		XAnimDeltaPartQuat *quat;
	};

	struct XAnimParts
	{
		const char *name;
		unsigned __int16 dataByteCount;
		unsigned __int16 dataShortCount;
		unsigned __int16 dataIntCount;
		unsigned __int16 randomDataByteCount;
		unsigned __int16 randomDataIntCount;
		unsigned __int16 numframes;
		char flags;
		char boneCount[10];
		char notifyCount;
		char assetType;
		bool isDefault;
		unsigned int randomDataShortCount;
		unsigned int indexCount;
		float framerate;
		float frequency;
		unsigned __int16 *names;
		char *dataByte;
		__int16 *dataShort;
		int *dataInt;
		__int16 *randomDataShort;
		char *randomDataByte;
		int *randomDataInt;
		XAnimIndices indices;
		XAnimNotifyInfo *notify;
		XAnimDeltaPart *deltaPart;
	};

	struct XSurfaceVertexInfo
	{
		__int16 vertCount[4];
		unsigned __int16 *vertsBlend;
	};

	union GfxColor
	{
		unsigned int packed;
		char array[4];
	};

	union PackedTexCoords
	{
		unsigned int packed;
	};

	union PackedUnitVec
	{
		unsigned int packed;
		char array[4];
	};

	struct GfxPackedVertex
	{
		float xyz[3];
		float binormalSign;
		GfxColor color;
		PackedTexCoords texCoord;
		PackedUnitVec normal;
		PackedUnitVec tangent;
	};

	struct XSurfaceCollisionAabb
	{
		unsigned __int16 mins[3];
		unsigned __int16 maxs[3];
	};

	struct XSurfaceCollisionNode
	{
		XSurfaceCollisionAabb aabb;
		unsigned __int16 childBeginIndex;
		unsigned __int16 childCount;
	};

	struct XSurfaceCollisionLeaf
	{
		unsigned __int16 triangleBeginIndex;
	};

	struct XSurfaceCollisionTree
	{
		float trans[3];
		float scale[3];
		unsigned int nodeCount;
		XSurfaceCollisionNode *nodes;
		unsigned int leafCount;
		XSurfaceCollisionLeaf *leafs;
	};

	struct XRigidVertList
	{
		unsigned __int16 boneOffset;
		unsigned __int16 vertCount;
		unsigned __int16 triOffset;
		unsigned __int16 triCount;
		XSurfaceCollisionTree *collisionTree;
	};

	struct XSurface
	{
		char tileMode;
		bool deformed;
		unsigned __int16 vertCount;
		unsigned __int16 triCount;
		char zoneHandle;
		unsigned __int16 baseTriIndex;
		unsigned __int16 baseVertIndex;
		unsigned __int16 *triIndices;
		XSurfaceVertexInfo vertInfo;
		GfxPackedVertex *verts0;
		unsigned int vertListCount;
		XRigidVertList *vertList;
		int partBits[6];
	};

	struct XModelSurfs
	{
		const char *name;
		XSurface *surfs;
		unsigned __int16 numsurfs;
		int partBits[6];
	};

	struct DObjAnimMat
	{
		float quat[4];
		float trans[3];
		float transWeight;
	};

	struct GfxDrawSurfFields
	{
		unsigned __int64 objectId : 16;
		unsigned __int64 reflectionProbeIndex : 8;
		unsigned __int64 hasGfxEntIndex : 1;
		unsigned __int64 customIndex : 5;
		unsigned __int64 materialSortedIndex : 12;
		unsigned __int64 prepass : 2;
		unsigned __int64 useHeroLighting : 1;
		unsigned __int64 sceneLightIndex : 8;
		unsigned __int64 surfType : 4;
		unsigned __int64 primarySortKey : 6;
		unsigned __int64 unused : 1;
	};

	union GfxDrawSurf
	{
		GfxDrawSurfFields fields;
		unsigned __int64 packed;
	};

	struct /*__declspec(align(4))*/ MaterialInfo
	{
		const char *name;
		char gameFlags;
		char sortKey;
		char textureAtlasRowCount;
		char textureAtlasColumnCount;
		GfxDrawSurf drawSurf;
		unsigned int surfaceTypeBits;
		unsigned __int16 hashIndex;
	};

	struct MaterialStreamRouting
	{
		char source;
		char dest;
	};

	struct MaterialVertexStreamRouting
	{
		MaterialStreamRouting data[13];
		IDirect3DVertexDeclaration9 *decl[16];
	};

	struct MaterialVertexDeclaration
	{
		const char *name;
		char streamCount;
		bool hasOptionalSource;
		MaterialVertexStreamRouting routing;
	};

	struct GfxVertexShaderLoadDef
	{
		unsigned int *program;
		unsigned __int16 programSize;
		unsigned __int16 loadForRenderer;
	};

	struct MaterialVertexShaderProgram
	{
		IDirect3DVertexShader9 *vs;
		GfxVertexShaderLoadDef loadDef;
	};

	struct MaterialVertexShader
	{
		const char *name;
		MaterialVertexShaderProgram prog;
	};

	struct GfxPixelShaderLoadDef
	{
		unsigned int *program;
		unsigned __int16 programSize;
		unsigned __int16 loadForRenderer;
	};

	struct MaterialPixelShaderProgram
	{
		IDirect3DPixelShader9 *ps;
		GfxPixelShaderLoadDef loadDef;
	};

	struct MaterialPixelShader
	{
		const char *name;
		MaterialPixelShaderProgram prog;
	};

	struct MaterialArgumentCodeConst
	{
		unsigned __int16 index;
		char firstRow;
		char rowCount;
	};

	union MaterialArgumentDef
	{
		const float *literalConst;
		MaterialArgumentCodeConst codeConst;
		unsigned int codeSampler;
		unsigned int nameHash;
	};

	struct MaterialShaderArgument
	{
		unsigned __int16 type;
		unsigned __int16 dest;
		MaterialArgumentDef u;
	};

	struct MaterialPass
	{
		MaterialVertexDeclaration *vertexDecl;
		MaterialVertexShader *vertexShader;
		MaterialPixelShader *pixelShader;
		char perPrimArgCount;
		char perObjArgCount;
		char stableArgCount;
		char customSamplerFlags;
		MaterialShaderArgument *args;
	};

	struct MaterialTechnique
	{
		const char *name;
		unsigned __int16 flags;
		unsigned __int16 passCount;
		MaterialPass passArray[1];
	};

	struct MaterialTechniqueSet
	{
		const char *name;
		char worldVertFormat;
		bool hasBeenUploaded;
		char unused[1];
		MaterialTechniqueSet *remappedTechniqueSet;
		MaterialTechnique *techniques[48];
	};

	struct GfxImageLoadDefIW3
	{
		char levelCount;
		char flags;
		__int16 dimensions[3];
		int format;
		int resourceSize;
		char data[1];
	};

	struct __declspec(align(4)) GfxImageLoadDef
	{
		char levelCount;
		char pad[3];
		int flags;
		int format;
		int resourceSize;
		char data[1];
	};

	union GfxTexture
	{
		IDirect3DBaseTexture9 *basemap;
		IDirect3DTexture9 *map;
		IDirect3DVolumeTexture9 *volmap;
		IDirect3DCubeTexture9 *cubemap;
		GfxImageLoadDef *loadDef;
	};

	struct Picmip
	{
		char platform[2];
	};

	struct CardMemory
	{
		int platform[2];
	};

	struct GfxImage
	{
		GfxTexture texture;
		char mapType;
		char semantic;
		char category;
		bool useSrgbReads;
		Picmip picmip;
		bool noPicmip;
		char track;
		CardMemory cardMemory;
		unsigned __int16 width;
		unsigned __int16 height;
		unsigned __int16 depth;
		bool delayLoadPixels;
		const char *name;
	};

	struct WaterWritable
	{
		float floatTime;
	};

	struct complex_s
	{
		float real;
		float imag;
	};

	struct water_t
	{
		WaterWritable writable;
		complex_s *H0;
		float *wTerm;
		int M;
		int N;
		float Lx;
		float Lz;
		float gravity;
		float windvel;
		float winddir[2];
		float amplitude;
		float codeConstant[4];
		GfxImage *image;
	};

	union MaterialTextureDefInfo
	{
		GfxImage *image;
		water_t *water;
	};

	struct MaterialTextureDef
	{
		unsigned int nameHash;
		char nameStart;
		char nameEnd;
		char samplerState;
		char semantic;
		MaterialTextureDefInfo u;
	};

	struct MaterialConstantDef
	{
		unsigned int nameHash;
		char name[12];
		float literal[4];
	};

	struct GfxStateBits
	{
		unsigned int loadBits[2];
	};

	struct Material
	{
		MaterialInfo info;
		char stateBitsEntry[48];
		char textureCount;
		char constantCount;
		char stateBitsCount;
		char stateFlags;
		char cameraRegion;
		MaterialTechniqueSet *techniqueSet;
		MaterialTextureDef *textureTable;
		MaterialConstantDef *constantTable;
		GfxStateBits *stateBitsTable;
	};

	struct XModelLodInfo
	{
		float dist;
		unsigned __int16 numsurfs;
		unsigned __int16 surfIndex;
		XModelSurfs *modelSurfs;
		int partBits[6];
		XSurface *surfs;
		char lod;
		char smcBaseIndexPlusOne;
		char smcSubIndexMask;
		char smcBucket;
	};

	struct XModelCollTri_s
	{
		float plane[4];
		float svec[4];
		float tvec[4];
	};

	struct XModelCollSurf_s
	{
		XModelCollTri_s *collTris;
		int numCollTris;
		Bounds bounds;
		int boneIdx;
		int contents;
		int surfFlags;
	};

	struct XBoneInfo
	{
		Bounds bounds;
		float radiusSquared;
	};

	struct XModel
	{
		const char *name;
		char numBones;
		char numRootBones;
		unsigned char numsurfs;
		char lodRampType;
		float scale;
		unsigned int noScalePartBits[6];
		unsigned __int16 *boneNames;
		char *parentList;
		__int16 *quats;
		float *trans;
		char *partClassification;
		DObjAnimMat *baseMat;
		Material **materialHandles;
		XModelLodInfo lodInfo[4];
		char maxLoadedLod;
		char numLods;
		char collLod;
		char flags;
		XModelCollSurf_s *collSurfs;
		int numCollSurfs;
		int contents;
		XBoneInfo *boneInfo;
		float radius;
		Bounds bounds;
		int memUsage;
		bool bad;
		PhysPreset *physPreset;
		PhysCollmap *physCollmap;
	};

	struct _AILSOUNDINFO
	{
		int format;
		const void *data_ptr;
		unsigned int data_len;
		unsigned int rate;
		int bits;
		int channels;
		unsigned int samples;
		unsigned int block_size;
		const void *initial_ptr;
	};

	struct MssSound
	{
		_AILSOUNDINFO info;
		char *data;
	};

	struct LoadedSound
	{
		const char *name;
		MssSound sound;
	};

	struct StreamFileNameRaw
	{
		const char *dir;
		const char *name;
	};

	union StreamFileInfo
	{
		StreamFileNameRaw raw;
	};

	struct StreamFileName
	{
		StreamFileInfo info;
	};

	struct StreamedSound
	{
		StreamFileName filename;
	};

	union SoundFileRef
	{
		LoadedSound *loadSnd;
		StreamedSound streamSnd;
	};

	enum snd_alias_type_t
	{
		SAT_UNKNOWN = 0x0,
		SAT_LOADED = 0x1,
		SAT_STREAMED = 0x2,
		SAT_VOICED = 0x3,
		SAT_COUNT = 0x4,
	};

	struct SoundFile
	{
		char type;
		char exists;
		SoundFileRef u;
	};

	union $C8D87EB0090687D323381DFB7A82089C
	{
		float slavePercentage;
		float masterPercentage;
	};

	struct SndCurve
	{
		const char *filename;
		unsigned __int16 knotCount;
		float knots[16][2];
	};

	struct MSSSpeakerLevels
	{
		int speaker;
		int numLevels;
		float levels[2];
	};

	struct MSSChannelMap
	{
		unsigned int speakerCount;
		MSSSpeakerLevels speakers[6];
	};

	struct SpeakerMap
	{
		bool isDefault;
		const char *name;
		MSSChannelMap channelMaps[2][2];
	};

	const struct snd_alias_t
	{
		const char *aliasName;
		const char *subtitle;
		const char *secondaryAliasName;
		const char *chainAliasName;
		const char *mixerGroup;
		SoundFile *soundFile;
		int sequence;
		float volMin;
		float volMax;
		float pitchMin;
		float pitchMax;
		float distMin;
		float distMax;
		float velocityMin;
		int flags;
		$C8D87EB0090687D323381DFB7A82089C ___u15;
		float probability;
		float lfePercentage;
		float centerPercentage;
		int startDelay;
		SndCurve *volumeFalloffCurve;
		float envelopMin;
		float envelopMax;
		float envelopPercentage;
		SpeakerMap *speakerMap;
	};

	struct snd_alias_list_t
	{
		const char *aliasName;
		snd_alias_t *head;
		unsigned int count;
	};

	struct cStaticModel_s
	{
		XModel *xmodel;
		float origin[3];
		float invScaledAxis[3][3];
		Bounds absBounds;
	};

	struct ClipMaterial
	{
		const char *name;
		int surfaceFlags;
		int contents;
	};

	struct cNode_t
	{
		cplane_s *plane;
		__int16 children[2];
	};

	struct cLeaf_t
	{
		unsigned __int16 firstCollAabbIndex;
		unsigned __int16 collAabbCount;
		int brushContents;
		int terrainContents;
		Bounds bounds;
		int leafBrushNode;
	};

	struct cLeafBrushNodeLeaf_t
	{
		unsigned __int16 *brushes;
	};

	struct cLeafBrushNodeChildren_t
	{
		float dist;
		float range;
		unsigned __int16 childOffset[2];
	};

	union cLeafBrushNodeData_t
	{
		cLeafBrushNodeLeaf_t leaf;
		cLeafBrushNodeChildren_t children;
	};

	struct cLeafBrushNode_s
	{
		char axis;
		__int16 leafBrushCount;
		int contents;
		cLeafBrushNodeData_t data;
	};

	struct CollisionBorder
	{
		float distEq[3];
		float zBase;
		float zSlope;
		float start;
		float length;
	};

	struct CollisionPartition
	{
		char triCount;
		char borderCount;
		char firstVertSegment;
		int firstTri;
		CollisionBorder *borders;
	};

	union CollisionAabbTreeIndex
	{
		int firstChildIndex;
		int partitionIndex;
	};

	struct CollisionAabbTree
	{
		float midPoint[3];
		unsigned __int16 materialIndex;
		unsigned __int16 childCount;
		float halfSize[3];
		CollisionAabbTreeIndex u;
	};

	struct cmodel_t
	{
		Bounds bounds;
		float radius;
		cLeaf_t leaf;
	};

	struct TriggerModel
	{
		int contents;
		unsigned __int16 hullCount;
		unsigned __int16 firstHull;
	};

	struct TriggerHull
	{
		Bounds bounds;
		int contents;
		unsigned __int16 slabCount;
		unsigned __int16 firstSlab;
	};

	struct TriggerSlab
	{
		float dir[3];
		float midPoint;
		float halfSize;
	};

	struct MapTriggers
	{
		unsigned int count;
		TriggerModel *models;
		unsigned int hullCount;
		TriggerHull *hulls;
		unsigned int slabCount;
		TriggerSlab *slabs;
	};

	struct /*__declspec(align(2))*/ Stage
	{
		const char *name;
		float origin[3];
		unsigned __int16 triggerIndex;
		char sunPrimaryLightIndex;
	};

	struct __declspec(align(4)) MapEnts
	{
		const char *name;
		char *entityString;
		int numEntityChars;
		MapTriggers trigger;
		Stage *stages;
		char stageCount;
	};

	struct SModelAabbNode
	{
		Bounds bounds;
		unsigned __int16 firstChild;
		unsigned __int16 childCount;
	};

	enum DynEntityType
	{
		DYNENT_TYPE_INVALID = 0x0,
		DYNENT_TYPE_CLUTTER = 0x1,
		DYNENT_TYPE_DESTRUCT = 0x2,
		DYNENT_TYPE_COUNT = 0x3,
	};

	struct GfxPlacement
	{
		float quat[4];
		float origin[3];
	};

	struct FxSpawnDefLooping
	{
		int intervalMsec;
		int count;
	};

	struct FxIntRange
	{
		int base;
		int amplitude;
	};

	struct FxSpawnDefOneShot
	{
		FxIntRange count;
	};

	union FxSpawnDef
	{
		FxSpawnDefLooping looping;
		FxSpawnDefOneShot oneShot;
	};

	struct FxFloatRange
	{
		float base;
		float amplitude;
	};

	struct FxElemAtlas
	{
		char behavior;
		char index;
		char fps;
		char loopCount;
		char colIndexBits;
		char rowIndexBits;
		__int16 entryCount;
	};

	struct FxElemVec3Range
	{
		float base[3];
		float amplitude[3];
	};

	struct FxElemVelStateInFrame
	{
		FxElemVec3Range velocity;
		FxElemVec3Range totalDelta;
	};

	const struct FxElemVelStateSample
	{
		FxElemVelStateInFrame local;
		FxElemVelStateInFrame world;
	};

	struct FxElemVisualState
	{
		char color[4];
		float rotationDelta;
		float rotationTotal;
		float size[2];
		float scale;
	};

	const struct FxElemVisStateSample
	{
		FxElemVisualState base;
		FxElemVisualState amplitude;
	};

	struct FxElemMarkVisuals
	{
		Material *materials[2];
	};

	union FxEffectDefRef
	{
		FxEffectDef *handle;
		const char *name;
	};

	union FxElemVisuals
	{
		const void *anonymous;
		Material *material;
		XModel *model;
		FxEffectDefRef effectDef;
		const char *soundName;
	};

	union FxElemDefVisuals
	{
		FxElemMarkVisuals *markArray;
		FxElemVisuals *array;
		FxElemVisuals instance;
	};

	struct FxTrailVertex
	{
		float pos[2];
		float normal[2];
		float texCoord;
	};

	struct FxTrailDef
	{
		int scrollTimeMsec;
		int repeatDist;
		float invSplitDist;
		float invSplitArcDist;
		float invSplitTime;
		int vertCount;
		FxTrailVertex *verts;
		int indCount;
		unsigned __int16 *inds;
	};

	struct FxSparkFountainDef
	{
		float gravity;
		float bounceFrac;
		float bounceRand;
		float sparkSpacing;
		float sparkLength;
		int sparkCount;
		float loopTime;
		float velMin;
		float velMax;
		float velConeFrac;
		float restSpeed;
		float boostTime;
		float boostFactor;
	};

	union FxElemExtendedDefPtr
	{
		FxTrailDef *trailDef;
		FxSparkFountainDef *sparkFountainDef;
		void *unknownDef;
	};

	const struct FxElemDef
	{
		int flags;
		FxSpawnDef spawn;
		FxFloatRange spawnRange;
		FxFloatRange fadeInRange;
		FxFloatRange fadeOutRange;
		float spawnFrustumCullRadius;
		FxIntRange spawnDelayMsec;
		FxIntRange lifeSpanMsec;
		FxFloatRange spawnOrigin[3];
		FxFloatRange spawnOffsetRadius;
		FxFloatRange spawnOffsetHeight;
		FxFloatRange spawnAngles[3];
		FxFloatRange angularVelocity[3];
		FxFloatRange initialRotation;
		FxFloatRange gravity;
		FxFloatRange reflectionFactor;
		FxElemAtlas atlas;
		char elemType;
		char visualCount;
		char velIntervalCount;
		char visStateIntervalCount;
		FxElemVelStateSample *velSamples;
		FxElemVisStateSample *visSamples;
		FxElemDefVisuals visuals;
		Bounds collBounds;
		FxEffectDefRef effectOnImpact;
		FxEffectDefRef effectOnDeath;
		FxEffectDefRef effectEmitted;
		FxFloatRange emitDist;
		FxFloatRange emitDistVariance;
		FxElemExtendedDefPtr extended;
		char sortOrder;
		char lightingFrac;
		char useItemClip;
		char fadeInfo;
	};

	struct FxEffectDef
	{
		const char *name;
		int flags;
		int totalSize;
		int msecLoopingLife;
		int elemDefCountLooping;
		int elemDefCountOneShot;
		int elemDefCountEmission;
		FxElemDef *elemDefs;
	};

	struct DynEntityDef
	{
		DynEntityType type;
		GfxPlacement pose;
		XModel *xModel;
		unsigned __int16 brushModel;
		unsigned __int16 physicsBrushModel;
		FxEffectDef *destroyFx;
		PhysPreset *physPreset;
		int health;
		PhysMass mass;
		int contents;
	};

	struct DynEntityPose
	{
		GfxPlacement pose;
		float radius;
	};

	struct DynEntityClient
	{
		int physObjId;
		unsigned __int16 flags;
		unsigned __int16 lightingHandle;
		int health;
	};

	struct DynEntityColl
	{
		unsigned __int16 sector;
		unsigned __int16 nextEntInSector;
		float linkMins[2];
		float linkMaxs[2];
	};

#pragma warning(push)
#pragma warning(disable: 4324)
	struct __declspec(align(64)) clipMap_t
	{
		const char *name;
		int isInUse;
		int planeCount;
		cplane_s *planes;
		unsigned int numStaticModels;
		cStaticModel_s *staticModelList;
		unsigned int numMaterials;
		ClipMaterial *materials;
		unsigned int numBrushSides;
		cbrushside_t *brushsides;
		unsigned int numBrushEdges;
		char *brushEdges;
		unsigned int numNodes;
		cNode_t *nodes;
		unsigned int numLeafs;
		cLeaf_t *leafs;
		unsigned int leafbrushNodesCount;
		cLeafBrushNode_s *leafbrushNodes;
		unsigned int numLeafBrushes;
		unsigned __int16 *leafbrushes;
		unsigned int numLeafSurfaces;
		unsigned int *leafsurfaces;
		unsigned int vertCount;
		float(*verts)[3];
		int triCount;
		unsigned __int16 *triIndices;
		char *triEdgeIsWalkable;
		int borderCount;
		CollisionBorder *borders;
		int partitionCount;
		CollisionPartition *partitions;
		int aabbTreeCount;
		CollisionAabbTree *aabbTrees;
		unsigned int numSubModels;
		cmodel_t *cmodels;
		unsigned __int16 numBrushes;
		cbrush_t *brushes;
		Bounds *brushBounds;
		int *brushContents;
		MapEnts *mapEnts;
		unsigned __int16 smodelNodeCount;
		SModelAabbNode *smodelNodes;
		unsigned __int16 dynEntCount[2];
		DynEntityDef *dynEntDefList[2];
		DynEntityPose *dynEntPoseList[2];
		DynEntityClient *dynEntClientList[2];
		DynEntityColl *dynEntCollList[2];
		unsigned int checksum;
	};
#pragma warning(pop)

	struct ComPrimaryLight
	{
		char type;
		char canUseShadowMap;
		char exponent;
		char unused;
		float color[3];
		float dir[3];
		float origin[3];
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		float cosHalfFovExpanded;
		float rotationLimit;
		float translationLimit;
		const char *defName;
	};

	struct ComWorld
	{
		const char *name;
		int isInUse;
		unsigned int primaryLightCount;
		ComPrimaryLight *primaryLights;
	};

	enum nodeType
	{
		NODE_ERROR = 0x0,
		NODE_PATHNODE = 0x1,
		NODE_COVER_STAND = 0x2,
		NODE_COVER_CROUCH = 0x3,
		NODE_COVER_CROUCH_WINDOW = 0x4,
		NODE_COVER_PRONE = 0x5,
		NODE_COVER_RIGHT = 0x6,
		NODE_COVER_LEFT = 0x7,
		NODE_AMBUSH = 0x8,
		NODE_EXPOSED = 0x9,
		NODE_CONCEALMENT_STAND = 0xA,
		NODE_CONCEALMENT_CROUCH = 0xB,
		NODE_CONCEALMENT_PRONE = 0xC,
		NODE_DOOR = 0xD,
		NODE_DOOR_INTERIOR = 0xE,
		NODE_SCRIPTED = 0xF,
		NODE_NEGOTIATION_BEGIN = 0x10,
		NODE_NEGOTIATION_END = 0x11,
		NODE_TURRET = 0x12,
		NODE_GUARD = 0x13,
		NODE_NUMTYPES = 0x14,
		NODE_DONTLINK = 0x14,
	};

	enum PathNodeErrorCode
	{
		PNERR_NONE = 0x0,
		PNERR_INSOLID = 0x1,
		PNERR_FLOATING = 0x2,
		PNERR_NOLINK = 0x3,
		PNERR_DUPLICATE = 0x4,
		PNERR_NOSTANCE = 0x5,
		PNERR_INVALIDDOOR = 0x6,
		PNERR_NOANGLES = 0x7,
		PNERR_BADPLACEMENT = 0x8,
		NUM_PATH_NODE_ERRORS = 0x9,
	};

	union $23305223CFD097B6F79557BDD2047E6C
	{
		float minUseDistSq;
		PathNodeErrorCode error;
	};

	struct pathlink_s
	{
		float fDist;
		unsigned __int16 nodeNum;
		char disconnectCount;
		char negotiationLink;
		char flags;
		char ubBadPlaceCount[3];
	};

	struct pathnode_constant_t
	{
		nodeType type;
		unsigned __int16 spawnflags;
		unsigned __int16 targetname;
		unsigned __int16 script_linkName;
		unsigned __int16 script_noteworthy;
		unsigned __int16 target;
		unsigned __int16 animscript;
		int animscriptfunc;
		float vOrigin[3];
		float fAngle;
		float forward[2];
		float fRadius;
		$23305223CFD097B6F79557BDD2047E6C ___u12;
		__int16 wOverlapNode[2];
		unsigned __int16 totalLinkCount;
		pathlink_s *Links;
	};

	struct pathnode_dynamic_t
	{
		void *pOwner;
		int iFreeTime;
		int iValidTime[3];
		int dangerousNodeTime[3];
		int inPlayerLOSTime;
		__int16 wLinkCount;
		__int16 wOverlapCount;
		__int16 turretEntNumber;
		char userCount;
		bool hasBadPlaceLink;
	};

	union $73F238679C0419BE2C31C6559E8604FC
	{
		float nodeCost;
		int linkIndex;
	};

	struct pathnode_transient_t
	{
		int iSearchFrame;
		pathnode_t *pNextOpen;
		pathnode_t *pPrevOpen;
		pathnode_t *pParent;
		float fCost;
		float fHeuristic;
		$73F238679C0419BE2C31C6559E8604FC ___u6;
	};

	struct pathnode_t
	{
		pathnode_constant_t constant;
		pathnode_dynamic_t dynamic;
		pathnode_transient_t transient;
	};

	struct pathbasenode_t
	{
		float vOrigin[3];
		unsigned int type;
	};

	struct pathnode_tree_nodes_t
	{
		int nodeCount;
		unsigned __int16 *nodes;
	};

	union pathnode_tree_info_t
	{
		pathnode_tree_t *child[2];
		pathnode_tree_nodes_t s;
	};

	struct pathnode_tree_t
	{
		int axis;
		float dist;
		pathnode_tree_info_t u;
	};

	struct PathData
	{
		unsigned int nodeCount;
		pathnode_t *nodes;
		pathbasenode_t *basenodes;
		unsigned int chainNodeCount;
		unsigned __int16 *chainNodeForNode;
		unsigned __int16 *nodeForChainNode;
		int visBytes;
		char *pathVis;
		int nodeTreeCount;
		pathnode_tree_t *nodeTree;
	};

	struct VehicleTrackObstacle
	{
		float origin[2];
		float radius;
	};

	struct VehicleTrackSector
	{
		float startEdgeDir[2];
		float startEdgeDist;
		float leftEdgeDir[2];
		float leftEdgeDist;
		float rightEdgeDir[2];
		float rightEdgeDist;
		float sectorLength;
		float sectorWidth;
		float totalPriorLength;
		float totalFollowingLength;
		VehicleTrackObstacle *obstacles;
		unsigned int obstacleCount;
	};

	struct VehicleTrackSegment
	{
		const char *targetName;
		VehicleTrackSector *sectors;
		unsigned int sectorCount;
		VehicleTrackSegment **nextBranches;
		unsigned int nextBranchesCount;
		VehicleTrackSegment **prevBranches;
		unsigned int prevBranchesCount;
		float endEdgeDir[2];
		float endEdgeDist;
		float totalLength;
	};

	struct VehicleTrack
	{
		VehicleTrackSegment *segments;
		unsigned int segmentCount;
	};

	struct /*__declspec(align(2))*/ G_GlassPiece
	{
		unsigned __int16 damageTaken;
		unsigned __int16 collapseTime;
		int lastStateChangeTime;
		char impactDir;
		char impactPos[2];
	};

	struct G_GlassName
	{
		char *nameStr;
		unsigned __int16 name;
		unsigned __int16 pieceCount;
		unsigned __int16 *pieceIndices;
	};

	struct G_GlassData
	{
		G_GlassPiece *glassPieces;
		unsigned int pieceCount;
		unsigned __int16 damageToWeaken;
		unsigned __int16 damageToDestroy;
		unsigned int glassNameCount;
		G_GlassName *glassNames;
		char pad[108];
	};

	struct GameWorldSp
	{
		const char *name;
		PathData path;
		VehicleTrack vehicleTrack;
		G_GlassData *g_glassData;
	};

	struct GameWorldMp
	{
		const char *name;
		G_GlassData *g_glassData;
	};

	struct FxGlassDef
	{
		float halfThickness;
		float texVecs[2][2];
		GfxColor color;
		Material *material;
		Material *materialShattered;
		PhysPreset *physPreset;
	};

	struct FxSpatialFrame
	{
		float quat[4];
		float origin[3];
	};

	struct $E43DBA5037697D705289B74D87E76C70
	{
		FxSpatialFrame frame;
		float radius;
	};

	union FxGlassPiecePlace
	{
		$E43DBA5037697D705289B74D87E76C70 __s0;
		unsigned int nextFree;
	};

	struct FxGlassPieceState
	{
		float texCoordOrigin[2];
		unsigned int supportMask;
		unsigned __int16 initIndex;
		unsigned __int16 geoDataStart;
		char defIndex;
		char pad[5];
		char vertCount;
		char holeDataCount;
		char crackDataCount;
		char fanDataCount;
		unsigned __int16 flags;
		float areaX2;
	};

	struct FxGlassPieceDynamics
	{
		int fallTime;
		int physObjId;
		int physJointId;
		float vel[3];
		float avel[3];
	};

	struct FxGlassVertex
	{
		__int16 x;
		__int16 y;
	};

	struct FxGlassHoleHeader
	{
		unsigned __int16 uniqueVertCount;
		char touchVert;
		char pad[1];
	};

	struct FxGlassCrackHeader
	{
		unsigned __int16 uniqueVertCount;
		char beginVertIndex;
		char endVertIndex;
	};

	union FxGlassGeometryData
	{
		FxGlassVertex vert;
		FxGlassHoleHeader hole;
		FxGlassCrackHeader crack;
		char asBytes[4];
		__int16 anonymous[2];
	};

	struct FxGlassInitPieceState
	{
		FxSpatialFrame frame;
		float radius;
		float texCoordOrigin[2];
		unsigned int supportMask;
		float areaX2;
		char defIndex;
		char vertCount;
		char fanDataCount;
		char pad[1];
	};

	struct FxGlassSystem
	{
		int time;
		int prevTime;
		unsigned int defCount;
		unsigned int pieceLimit;
		unsigned int pieceWordCount;
		unsigned int initPieceCount;
		unsigned int cellCount;
		unsigned int activePieceCount;
		unsigned int firstFreePiece;
		unsigned int geoDataLimit;
		unsigned int geoDataCount;
		unsigned int initGeoDataCount;
		FxGlassDef *defs;
		FxGlassPiecePlace *piecePlaces;
		FxGlassPieceState *pieceStates;
		FxGlassPieceDynamics *pieceDynamics;
		FxGlassGeometryData *geoData;
		unsigned int *isInUse;
		unsigned int *cellBits;
		char *visData;
		float(*linkOrg)[3];
		float *halfThickness;
		unsigned __int16 *lightingHandles;
		FxGlassInitPieceState *initPieceStates;
		FxGlassGeometryData *initGeoData;
		bool needToCompactData;
		char initCount;
		float effectChanceAccum;
		int lastPieceDeletionTime;
	};

	struct FxWorld
	{
		const char *name;
		FxGlassSystem glassSys;
	};

	struct __declspec(align(4)) GfxSky
	{
		int skySurfCount;
		int *skyStartSurfs;
		GfxImage *skyImage;
		char skySamplerState;
	};

	struct GfxWorldDpvsPlanes
	{
		int cellCount;
		cplane_s *planes;
		unsigned __int16 *nodes;
		unsigned int *sceneEntCellBits;
	};

	struct GfxCellTreeCount
	{
		int aabbTreeCount;
	};

	struct GfxAabbTree
	{
		Bounds bounds;
		unsigned __int16 childCount;
		unsigned __int16 surfaceCount;
		unsigned __int16 startSurfIndex;
		unsigned __int16 surfaceCountNoDecal;
		unsigned __int16 startSurfIndexNoDecal;
		unsigned __int16 smodelIndexCount;
		unsigned __int16 *smodelIndexes;
		int childrenOffset;
	};

	struct GfxCellTree
	{
		GfxAabbTree *aabbTree;
	};

	struct GfxPortalWritable
	{
		bool isQueued;
		bool isAncestor;
		char recursionDepth;
		char hullPointCount;
		float(*hullPoints)[2];
		GfxPortal *queuedParent;
	};

	struct DpvsPlane
	{
		float coeffs[4];
	};

	struct GfxPortal
	{
		GfxPortalWritable writable;
		DpvsPlane plane;
		float(*vertices)[3];
		unsigned __int16 cellIndex;
		char vertexCount;
		float hullAxis[2][3];
	};

	struct GfxCell
	{
		Bounds bounds;
		int portalCount;
		GfxPortal *portals;
		char reflectionProbeCount;
		char *reflectionProbes;
	};

	struct GfxReflectionProbe
	{
		float origin[3];
	};

	struct GfxLightmapArray
	{
		GfxImage *primary;
		GfxImage *secondary;
	};

	struct GfxWorldVertex
	{
		float xyz[3];
		float binormalSign;
		GfxColor color;
		float texCoord[2];
		float lmapCoord[2];
		PackedUnitVec normal;
		PackedUnitVec tangent;
	};

	struct GfxWorldVertexData
	{
		GfxWorldVertex *vertices;
		IDirect3DVertexBuffer9 *worldVb;
	};

	struct GfxWorldVertexLayerData
	{
		char *data;
		IDirect3DVertexBuffer9 *layerVb;
	};

	struct GfxWorldDraw
	{
		unsigned int reflectionProbeCount;
		GfxImage **reflectionProbes;
		GfxReflectionProbe *reflectionProbeOrigins;
		GfxTexture *reflectionProbeTextures;
		int lightmapCount;
		GfxLightmapArray *lightmaps;
		GfxTexture *lightmapPrimaryTextures;
		GfxTexture *lightmapSecondaryTextures;
		GfxImage *lightmapOverridePrimary;
		GfxImage *lightmapOverrideSecondary;
		unsigned int vertexCount;
		GfxWorldVertexData vd;
		unsigned int vertexLayerDataSize;
		GfxWorldVertexLayerData vld;
		unsigned int indexCount;
		unsigned __int16 *indices;
	};

	struct GfxLightGridEntry
	{
		unsigned __int16 colorsIndex;
		char primaryLightIndex;
		char needsTrace;
	};

	struct GfxLightGridColors
	{
		char rgb[56][3];
	};

	struct GfxLightGrid
	{
		bool hasLightRegions;
		unsigned int lastSunPrimaryLightIndex;
		unsigned __int16 mins[3];
		unsigned __int16 maxs[3];
		unsigned int rowAxis;
		unsigned int colAxis;
		unsigned __int16 *rowDataStart;
		unsigned int rawRowDataSize;
		char *rawRowData;
		unsigned int entryCount;
		GfxLightGridEntry *entries;
		unsigned int colorCount;
		GfxLightGridColors *colors;
	};

	struct GfxBrushModelWritable
	{
		Bounds bounds;
	};

	struct __declspec(align(4)) GfxBrushModel
	{
		GfxBrushModelWritable writable;
		Bounds bounds;
		float radius;
		unsigned __int16 surfaceCount;
		unsigned __int16 startSurfIndex;
		unsigned __int16 surfaceCountNoDecal;
	};

	struct MaterialMemory
	{
		Material *material;
		int memory;
	};

	struct sunflare_t
	{
		bool hasValidData;
		Material *spriteMaterial;
		Material *flareMaterial;
		float spriteSize;
		float flareMinSize;
		float flareMinDot;
		float flareMaxSize;
		float flareMaxDot;
		float flareMaxAlpha;
		int flareFadeInTime;
		int flareFadeOutTime;
		float blindMinDot;
		float blindMaxDot;
		float blindMaxDarken;
		int blindFadeInTime;
		int blindFadeOutTime;
		float glareMinDot;
		float glareMaxDot;
		float glareMaxLighten;
		int glareFadeInTime;
		int glareFadeOutTime;
		float sunFxPosition[3];
	};

	struct XModelDrawInfo
	{
		char hasGfxEntIndex;
		char lod;
		unsigned __int16 surfId;
	};

	struct GfxSceneDynModel
	{
		XModelDrawInfo info;
		unsigned __int16 dynEntId;
	};

	struct BModelDrawInfo
	{
		unsigned __int16 surfId;
	};

	struct GfxSceneDynBrush
	{
		BModelDrawInfo info;
		unsigned __int16 dynEntId;
	};

	struct GfxShadowGeometry
	{
		unsigned __int16 surfaceCount;
		unsigned __int16 smodelCount;
		unsigned __int16 *sortedSurfIndex;
		unsigned __int16 *smodelIndex;
	};

	struct GfxLightRegionAxis
	{
		float dir[3];
		float midPoint;
		float halfSize;
	};

	struct GfxLightRegionHull
	{
		float kdopMidPoint[9];
		float kdopHalfSize[9];
		unsigned int axisCount;
		GfxLightRegionAxis *axis;
	};

	struct GfxLightRegion
	{
		unsigned int hullCount;
		GfxLightRegionHull *hulls;
	};

	struct GfxStaticModelInst
	{
		Bounds bounds;
		float lightingOrigin[3];
	};

	struct srfTriangles_t
	{
		unsigned int vertexLayerData;
		unsigned int firstVertex;
		unsigned __int16 vertexCount;
		unsigned __int16 triCount;
		unsigned int baseIndex;
	};

	struct GfxSurfaceLightingAndFlagsFields
	{
		char lightmapIndex;
		char reflectionProbeIndex;
		char primaryLightIndex;
		char flags;
	};

	union GfxSurfaceLightingAndFlags
	{
		GfxSurfaceLightingAndFlagsFields fields;
		unsigned int packed;
	};

	struct GfxSurface
	{
		srfTriangles_t tris;
		Material *material;
		GfxSurfaceLightingAndFlags laf;
	};

	struct GfxSurfaceBounds
	{
		Bounds bounds;
	};

	struct GfxPackedPlacement
	{
		float origin[3];
		float axis[3][3];
		float scale;
	};

	struct GfxStaticModelDrawInst
	{
		GfxPackedPlacement placement;
		XModel *model;
		unsigned __int16 cullDist;
		unsigned __int16 lightingHandle;
		char reflectionProbeIndex;
		char primaryLightIndex;
		char flags;
		char firstMtlSkinIndex;
		GfxColor groundLighting;
		unsigned __int16 cacheId[4];
	};

	struct GfxWorldDpvsStatic
	{
		unsigned int smodelCount;
		unsigned int staticSurfaceCount;
		unsigned int staticSurfaceCountNoDecal;
		unsigned int litOpaqueSurfsBegin;
		unsigned int litOpaqueSurfsEnd;
		unsigned int litTransSurfsBegin;
		unsigned int litTransSurfsEnd;
		unsigned int shadowCasterSurfsBegin;
		unsigned int shadowCasterSurfsEnd;
		unsigned int emissiveSurfsBegin;
		unsigned int emissiveSurfsEnd;
		unsigned int smodelVisDataCount;
		unsigned int surfaceVisDataCount;
		char *smodelVisData[3];
		char *surfaceVisData[3];
		unsigned __int16 *sortedSurfIndex;
		GfxStaticModelInst *smodelInsts;
		GfxSurface *surfaces;
		GfxSurfaceBounds *surfacesBounds;
		GfxStaticModelDrawInst *smodelDrawInsts;
		GfxDrawSurf *surfaceMaterials;
		unsigned int *surfaceCastsSunShadow;
		volatile int usageCount;
	};

	struct GfxWorldDpvsDynamic
	{
		unsigned int dynEntClientWordCount[2];
		unsigned int dynEntClientCount[2];
		unsigned int *dynEntCellBits[2];
		char *dynEntVisData[2][3];
	};

	struct GfxHeroOnlyLight
	{
		char type;
		char unused[3];
		float color[3];
		float dir[3];
		float origin[3];
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		int exponent;
	};

	struct __declspec(align(4)) GfxWorld
	{
		const char *name;
		const char *baseName;
		int planeCount;
		int nodeCount;
		unsigned int surfaceCount;
		int skyCount;
		GfxSky *skies;
		unsigned int lastSunPrimaryLightIndex;
		unsigned int primaryLightCount;
		unsigned int sortKeyLitDecal;
		unsigned int sortKeyEffectDecal;
		unsigned int sortKeyEffectAuto;
		unsigned int sortKeyDistortion;
		GfxWorldDpvsPlanes dpvsPlanes;
		GfxCellTreeCount *aabbTreeCounts;
		GfxCellTree *aabbTrees;
		GfxCell *cells;
		GfxWorldDraw draw;
		GfxLightGrid lightGrid;
		int modelCount;
		GfxBrushModel *models;
		Bounds bounds;
		unsigned int checksum;
		int materialMemoryCount;
		MaterialMemory *materialMemory;
		sunflare_t sun;
		float outdoorLookupMatrix[4][4];
		GfxImage *outdoorImage;
		unsigned int *cellCasterBits;
		unsigned int *cellHasSunLitSurfsBits;
		GfxSceneDynModel *sceneDynModel;
		GfxSceneDynBrush *sceneDynBrush;
		unsigned int *primaryLightEntityShadowVis;
		unsigned int *primaryLightDynEntShadowVis[2];
		char *nonSunPrimaryLightForModelDynEnt;
		GfxShadowGeometry *shadowGeom;
		GfxLightRegion *lightRegion;
		GfxWorldDpvsStatic dpvs;
		GfxWorldDpvsDynamic dpvsDyn;
		unsigned int mapVtxChecksum;
		unsigned int heroOnlyLightCount;
		GfxHeroOnlyLight *heroOnlyLights;
		char fogTypesAllowed;
	};

	struct __declspec(align(4)) GfxLightImage
	{
		GfxImage *image;
		char samplerState;
	};

	struct GfxLightDef
	{
		const char *name;
		GfxLightImage attenuation;
		int lmapLookupStart;
	};

	struct Glyph
	{
		unsigned __int16 letter;
		char x0;
		char y0;
		char dx;
		char pixelWidth;
		char pixelHeight;
		float s0;
		float t0;
		float s1;
		float t1;
	};

	struct Font_s
	{
		const char *fontName;
		int pixelHeight;
		int glyphCount;
		Material *material;
		Material *glowMaterial;
		Glyph *glyphs;
	};

	struct __declspec(align(4)) rectDef_s
	{
		float x;
		float y;
		float w;
		float h;
		char horzAlign;
		char vertAlign;
	};

	struct windowDef_t
	{
		const char *name;
		rectDef_s rect;
		rectDef_s rectClient;
		const char *group;
		int style;
		int border;
		int ownerDraw;
		int ownerDrawFlags;
		float borderSize;
		int staticFlags;
		int dynamicFlags[1];
		int nextTime;
		float foreColor[4];
		float backColor[4];
		float borderColor[4];
		float outlineColor[4];
		float disableColor[4];
		Material *background;
	};

	enum expDataType
	{
		VAL_INT = 0x0,
		VAL_FLOAT = 0x1,
		VAL_STRING = 0x2,
		NUM_INTERNAL_DATATYPES = 0x3,
		VAL_FUNCTION = 0x3,
		NUM_DATATYPES = 0x4,
	};

	struct ExpressionString
	{
		const char *string;
	};

	union operandInternalDataUnion
	{
		int intVal;
		float floatVal;
		ExpressionString stringVal;
		Statement_s *function;
	};

	struct Operand
	{
		expDataType dataType;
		operandInternalDataUnion internals;
	};

	// This enum is unknown
	enum operationEnum
	{
		BLUB
	};

	union entryInternalData
	{
		operationEnum op;
		Operand operand;
	};

	struct expressionEntry
	{
		int type;
		entryInternalData data;
	};

	struct UIFunctionList
	{
		int totalFunctions;
		Statement_s **functions;
	};

	union DvarValue
	{
		bool enabled;
		int integer;
		unsigned int unsignedInt;
		float value;
		float vector[4];
		const char *string;
		unsigned char color[4];
	};

	struct $BFBB53559BEAC4289F32B924847E59CB
	{
		int stringCount;
		const char **strings;
	};

	struct $9CA192F9DB66A3CB7E01DE78A0DEA53D
	{
		int min;
		int max;
	};

	struct $251C2428A496074035CACA7AAF3D55BD
	{
		float min;
		float max;
	};

	union DvarLimits
	{
		$BFBB53559BEAC4289F32B924847E59CB enumeration;
		$9CA192F9DB66A3CB7E01DE78A0DEA53D integer;
		$251C2428A496074035CACA7AAF3D55BD value;
		$251C2428A496074035CACA7AAF3D55BD vector;
	};

	struct dvar_t
	{
		const char *name;
		const char *description;
		unsigned int flags;
		dvar_type type;
		bool modified;
		DvarValue current;
		DvarValue latched;
		DvarValue reset;
		DvarLimits domain;
		bool(__cdecl *domainFunc)(dvar_t *, DvarValue);
		dvar_t *hashNext;
	};

	struct StaticDvar
	{
		dvar_t *dvar;
		char *dvarName;
	};

	struct StaticDvarList
	{
		int numStaticDvars;
		StaticDvar **staticDvars;
	};

	struct StringList
	{
		int totalStrings;
		const char **strings;
	};

	struct ExpressionSupportingData
	{
		UIFunctionList uifunctions;
		StaticDvarList staticDvarList;
		StringList uiStrings;
	};

	struct Statement_s
	{
		int numEntries;
		expressionEntry *entries;
		ExpressionSupportingData *supportingData;
		int lastExecuteTime;
		Operand lastResult;
	};

	struct ConditionalScript
	{
		MenuEventHandlerSet *eventHandlerSet;
		Statement_s *eventExpression;
	};

	struct SetLocalVarData
	{
		const char *localVarName;
		Statement_s *expression;
	};

	union EventData
	{
		const char *unconditionalScript;
		ConditionalScript *conditionalScript;
		MenuEventHandlerSet *elseScript;
		SetLocalVarData *setLocalVarData;
	};

	struct __declspec(align(4)) MenuEventHandler
	{
		EventData eventData;
		char eventType;
	};

	struct MenuEventHandlerSet
	{
		int eventHandlerCount;
		MenuEventHandler **eventHandlers;
	};

	struct ItemKeyHandler
	{
		int key;
		MenuEventHandlerSet *action;
		ItemKeyHandler *next;
	};

	struct columnInfo_s
	{
		int pos;
		int width;
		int maxChars;
		int alignment;
	};

	struct listBoxDef_s
	{
		int mousePos;
		int startPos[1];
		int endPos[1];
		int drawPadding;
		float elementWidth;
		float elementHeight;
		int elementStyle;
		int numColumns;
		columnInfo_s columnInfo[16];
		MenuEventHandlerSet *onDoubleClick;
		int notselectable;
		int noScrollBars;
		int usePaging;
		float selectBorder[4];
		Material *selectIcon;
	};

	struct editFieldDef_s
	{
		float minVal;
		float maxVal;
		float defVal;
		float range;
		int maxChars;
		int maxCharsGotoNext;
		int maxPaintChars;
		int paintOffset;
	};

	struct multiDef_s
	{
		const char *dvarList[32];
		const char *dvarStr[32];
		float dvarValue[32];
		int count;
		int strDef;
	};

	struct newsTickerDef_s
	{
		int feedId;
		int speed;
		int spacing;
		int lastTime;
		int start;
		int end;
		float x;
	};

	struct textScrollDef_s
	{
		int startTime;
	};

	union itemDefData_t
	{
		listBoxDef_s *listBox;
		editFieldDef_s *editField;
		multiDef_s *multi;
		const char *enumDvarName;
		newsTickerDef_s *ticker;
		textScrollDef_s *scroll;
		void *data;
	};

	struct ItemFloatExpression
	{
		int target;
		Statement_s *expression;
	};

	struct itemDef_s
	{
		windowDef_t window;
		rectDef_s textRect[1];
		int type;
		int dataType;
		int alignment;
		int fontEnum;
		int textAlignMode;
		float textalignx;
		float textaligny;
		float textscale;
		int textStyle;
		int gameMsgWindowIndex;
		int gameMsgWindowMode;
		const char *text;
		int itemFlags;
		menuDef_t *parent;
		MenuEventHandlerSet *mouseEnterText;
		MenuEventHandlerSet *mouseExitText;
		MenuEventHandlerSet *mouseEnter;
		MenuEventHandlerSet *mouseExit;
		MenuEventHandlerSet *action;
		MenuEventHandlerSet *accept;
		MenuEventHandlerSet *onFocus;
		MenuEventHandlerSet *leaveFocus;
		const char *dvar;
		const char *dvarTest;
		ItemKeyHandler *onKey;
		const char *enableDvar;
		const char *localVar;
		int dvarFlags;
		snd_alias_list_t *focusSound;
		float special;
		int cursorPos[1];
		itemDefData_t typeData;
		int imageTrack;
		int floatExpressionCount;
		ItemFloatExpression *floatExpressions;
		Statement_s *visibleExp;
		Statement_s *disabledExp;
		Statement_s *textExp;
		Statement_s *materialExp;
		float glowColor[4];
		bool decayActive;
		int fxBirthTime;
		int fxLetterTime;
		int fxDecayStartTime;
		int fxDecayDuration;
		int lastSoundPlayedTime;
	};

	struct menuTransition
	{
		int transitionType;
		int targetField;
		int startTime;
		float startVal;
		float endVal;
		float time;
		int endTriggerType;
	};

	struct menuDef_t
	{
		windowDef_t window;
		const char *font;
		int fullScreen;
		int itemCount;
		int fontIndex;
		int cursorItem[1];
		int fadeCycle;
		float fadeClamp;
		float fadeAmount;
		float fadeInAmount;
		float blurRadius;
		MenuEventHandlerSet *onOpen;
		MenuEventHandlerSet *onCloseRequest;
		MenuEventHandlerSet *onClose;
		MenuEventHandlerSet *onESC;
		ItemKeyHandler *onKey;
		Statement_s *visibleExp;
		const char *allowedBinding;
		const char *soundName;
		int imageTrack;
		float focusColor[4];
		Statement_s *rectXExp;
		Statement_s *rectYExp;
		Statement_s *rectWExp;
		Statement_s *rectHExp;
		Statement_s *openSoundExp;
		Statement_s *closeSoundExp;
		itemDef_s **items;
		menuTransition scaleTransition[1];
		menuTransition alphaTransition[1];
		menuTransition xTransition[1];
		menuTransition yTransition[1];
		ExpressionSupportingData *expressionData;
	};

	struct MenuList
	{
		const char *name;
		int menuCount;
		menuDef_t **menus;
	};

	struct LocalizeEntry
	{
		const char *value;
		const char *name;
	};

	enum weapType_t
	{
		WEAPTYPE_BULLET = 0x0,
		WEAPTYPE_GRENADE = 0x1,
		WEAPTYPE_PROJECTILE = 0x2,
		WEAPTYPE_RIOTSHIELD = 0x3,
		WEAPTYPE_NUM = 0x4,
	};

	enum weapClass_t
	{
		WEAPCLASS_RIFLE = 0x0,
		WEAPCLASS_SNIPER = 0x1,
		WEAPCLASS_MG = 0x2,
		WEAPCLASS_SMG = 0x3,
		WEAPCLASS_SPREAD = 0x4,
		WEAPCLASS_PISTOL = 0x5,
		WEAPCLASS_GRENADE = 0x6,
		WEAPCLASS_ROCKETLAUNCHER = 0x7,
		WEAPCLASS_TURRET = 0x8,
		WEAPCLASS_THROWINGKNIFE = 0x9,
		WEAPCLASS_NON_PLAYER = 0xA,
		WEAPCLASS_ITEM = 0xB,
		WEAPCLASS_NUM = 0xC,
	};

	enum PenetrateType
	{
		PENETRATE_TYPE_NONE = 0x0,
		PENETRATE_TYPE_SMALL = 0x1,
		PENETRATE_TYPE_MEDIUM = 0x2,
		PENETRATE_TYPE_LARGE = 0x3,
		PENETRATE_TYPE_COUNT = 0x4,
	};

	enum weapInventoryType_t
	{
		WEAPINVENTORY_PRIMARY = 0x0,
		WEAPINVENTORY_OFFHAND = 0x1,
		WEAPINVENTORY_ITEM = 0x2,
		WEAPINVENTORY_ALTMODE = 0x3,
		WEAPINVENTORY_EXCLUSIVE = 0x4,
		WEAPINVENTORY_SCAVENGER = 0x5,
		WEAPINVENTORYCOUNT = 0x6,
	};

	enum weapFireType_t
	{
		WEAPON_FIRETYPE_FULLAUTO = 0x0,
		WEAPON_FIRETYPE_SINGLESHOT = 0x1,
		WEAPON_FIRETYPE_BURSTFIRE2 = 0x2,
		WEAPON_FIRETYPE_BURSTFIRE3 = 0x3,
		WEAPON_FIRETYPE_BURSTFIRE4 = 0x4,
		WEAPON_FIRETYPE_DOUBLEBARREL = 0x5,
		WEAPON_FIRETYPECOUNT = 0x6,
		WEAPON_FIRETYPE_BURSTFIRE_FIRST = 0x2,
		WEAPON_FIRETYPE_BURSTFIRE_LAST = 0x4,
	};

	enum OffhandClass
	{
		OFFHAND_CLASS_NONE = 0x0,
		OFFHAND_CLASS_FRAG_GRENADE = 0x1,
		OFFHAND_CLASS_SMOKE_GRENADE = 0x2,
		OFFHAND_CLASS_FLASH_GRENADE = 0x3,
		OFFHAND_CLASS_THROWINGKNIFE = 0x4,
		OFFHAND_CLASS_OTHER = 0x5,
		OFFHAND_CLASS_COUNT = 0x6,
	};

	enum weapStance_t
	{
		WEAPSTANCE_STAND = 0x0,
		WEAPSTANCE_DUCK = 0x1,
		WEAPSTANCE_PRONE = 0x2,
		WEAPSTANCE_NUM = 0x3,
	};

	enum activeReticleType_t
	{
		VEH_ACTIVE_RETICLE_NONE = 0x0,
		VEH_ACTIVE_RETICLE_PIP_ON_A_STICK = 0x1,
		VEH_ACTIVE_RETICLE_BOUNCING_DIAMOND = 0x2,
		VEH_ACTIVE_RETICLE_COUNT = 0x3,
	};

	enum weaponIconRatioType_t
	{
		WEAPON_ICON_RATIO_1TO1 = 0x0,
		WEAPON_ICON_RATIO_2TO1 = 0x1,
		WEAPON_ICON_RATIO_4TO1 = 0x2,
		WEAPON_ICON_RATIO_COUNT = 0x3,
	};

	enum ammoCounterClipType_t
	{
		AMMO_COUNTER_CLIP_NONE = 0x0,
		AMMO_COUNTER_CLIP_MAGAZINE = 0x1,
		AMMO_COUNTER_CLIP_SHORTMAGAZINE = 0x2,
		AMMO_COUNTER_CLIP_SHOTGUN = 0x3,
		AMMO_COUNTER_CLIP_ROCKET = 0x4,
		AMMO_COUNTER_CLIP_BELTFED = 0x5,
		AMMO_COUNTER_CLIP_ALTWEAPON = 0x6,
		AMMO_COUNTER_CLIP_COUNT = 0x7,
	};

	enum weapOverlayReticle_t
	{
		WEAPOVERLAYRETICLE_NONE = 0x0,
		WEAPOVERLAYRETICLE_CROSSHAIR = 0x1,
		WEAPOVERLAYRETICLE_NUM = 0x2,
	};

	enum WeapOverlayInteface_t
	{
		WEAPOVERLAYINTERFACE_NONE = 0x0,
		WEAPOVERLAYINTERFACE_JAVELIN = 0x1,
		WEAPOVERLAYINTERFACE_TURRETSCOPE = 0x2,
		WEAPOVERLAYINTERFACECOUNT = 0x3,
	};

	enum weapProjExposion_t
	{
		WEAPPROJEXP_GRENADE = 0x0,
		WEAPPROJEXP_ROCKET = 0x1,
		WEAPPROJEXP_FLASHBANG = 0x2,
		WEAPPROJEXP_NONE = 0x3,
		WEAPPROJEXP_DUD = 0x4,
		WEAPPROJEXP_SMOKE = 0x5,
		WEAPPROJEXP_HEAVY = 0x6,
		WEAPPROJEXP_NUM = 0x7,
	};

	enum WeapStickinessType
	{
		WEAPSTICKINESS_NONE = 0x0,
		WEAPSTICKINESS_ALL = 0x1,
		WEAPSTICKINESS_ALL_ORIENT = 0x2,
		WEAPSTICKINESS_GROUND = 0x3,
		WEAPSTICKINESS_GROUND_WITH_YAW = 0x4,
		WEAPSTICKINESS_KNIFE = 0x5,
		WEAPSTICKINESS_COUNT = 0x6,
	};

	enum guidedMissileType_t
	{
		MISSILE_GUIDANCE_NONE = 0x0,
		MISSILE_GUIDANCE_SIDEWINDER = 0x1,
		MISSILE_GUIDANCE_HELLFIRE = 0x2,
		MISSILE_GUIDANCE_JAVELIN = 0x3,
		MISSILE_GUIDANCE_COUNT = 0x4,
	};

	struct TracerDef
	{
		const char *name;
		Material *material;
		unsigned int drawInterval;
		float speed;
		float beamLength;
		float beamWidth;
		float screwRadius;
		float screwDist;
		float colors[5][4];
	};

	struct __declspec(align(4)) WeaponDef
	{
		const char *szOverlayName;
		XModel **gunXModel;
		XModel *handXModel;
		const char **szXAnimsRightHanded;
		const char **szXAnimsLeftHanded;
		const char *szModeName;
		unsigned __int16 *notetrackSoundMapKeys;
		unsigned __int16 *notetrackSoundMapValues;
		unsigned __int16 *notetrackRumbleMapKeys;
		unsigned __int16 *notetrackRumbleMapValues;
		int playerAnimType;
		weapType_t weapType;
		weapClass_t weapClass;
		PenetrateType penetrateType;
		weapInventoryType_t inventoryType;
		weapFireType_t fireType;
		OffhandClass offhandClass;
		weapStance_t stance;
		FxEffectDef *viewFlashEffect;
		FxEffectDef *worldFlashEffect;
		snd_alias_list_t *pickupSound;
		snd_alias_list_t *pickupSoundPlayer;
		snd_alias_list_t *ammoPickupSound;
		snd_alias_list_t *ammoPickupSoundPlayer;
		snd_alias_list_t *projectileSound;
		snd_alias_list_t *pullbackSound;
		snd_alias_list_t *pullbackSoundPlayer;
		snd_alias_list_t *fireSound;
		snd_alias_list_t *fireSoundPlayer;
		snd_alias_list_t *fireSoundPlayerAkimbo;
		snd_alias_list_t *fireLoopSound;
		snd_alias_list_t *fireLoopSoundPlayer;
		snd_alias_list_t *fireStopSound;
		snd_alias_list_t *fireStopSoundPlayer;
		snd_alias_list_t *fireLastSound;
		snd_alias_list_t *fireLastSoundPlayer;
		snd_alias_list_t *emptyFireSound;
		snd_alias_list_t *emptyFireSoundPlayer;
		snd_alias_list_t *meleeSwipeSound;
		snd_alias_list_t *meleeSwipeSoundPlayer;
		snd_alias_list_t *meleeHitSound;
		snd_alias_list_t *meleeMissSound;
		snd_alias_list_t *rechamberSound;
		snd_alias_list_t *rechamberSoundPlayer;
		snd_alias_list_t *reloadSound;
		snd_alias_list_t *reloadSoundPlayer;
		snd_alias_list_t *reloadEmptySound;
		snd_alias_list_t *reloadEmptySoundPlayer;
		snd_alias_list_t *reloadStartSound;
		snd_alias_list_t *reloadStartSoundPlayer;
		snd_alias_list_t *reloadEndSound;
		snd_alias_list_t *reloadEndSoundPlayer;
		snd_alias_list_t *detonateSound;
		snd_alias_list_t *detonateSoundPlayer;
		snd_alias_list_t *nightVisionWearSound;
		snd_alias_list_t *nightVisionWearSoundPlayer;
		snd_alias_list_t *nightVisionRemoveSound;
		snd_alias_list_t *nightVisionRemoveSoundPlayer;
		snd_alias_list_t *altSwitchSound;
		snd_alias_list_t *altSwitchSoundPlayer;
		snd_alias_list_t *raiseSound;
		snd_alias_list_t *raiseSoundPlayer;
		snd_alias_list_t *firstRaiseSound;
		snd_alias_list_t *firstRaiseSoundPlayer;
		snd_alias_list_t *putawaySound;
		snd_alias_list_t *putawaySoundPlayer;
		snd_alias_list_t *scanSound;
		snd_alias_list_t **bounceSound;
		FxEffectDef *viewShellEjectEffect;
		FxEffectDef *worldShellEjectEffect;
		FxEffectDef *viewLastShotEjectEffect;
		FxEffectDef *worldLastShotEjectEffect;
		Material *reticleCenter;
		Material *reticleSide;
		int iReticleCenterSize;
		int iReticleSideSize;
		int iReticleMinOfs;
		activeReticleType_t activeReticleType;
		float vStandMove[3];
		float vStandRot[3];
		float strafeMove[3];
		float strafeRot[3];
		float vDuckedOfs[3];
		float vDuckedMove[3];
		float vDuckedRot[3];
		float vProneOfs[3];
		float vProneMove[3];
		float vProneRot[3];
		float fPosMoveRate;
		float fPosProneMoveRate;
		float fStandMoveMinSpeed;
		float fDuckedMoveMinSpeed;
		float fProneMoveMinSpeed;
		float fPosRotRate;
		float fPosProneRotRate;
		float fStandRotMinSpeed;
		float fDuckedRotMinSpeed;
		float fProneRotMinSpeed;
		XModel **worldModel;
		XModel *worldClipModel;
		XModel *rocketModel;
		XModel *knifeModel;
		XModel *worldKnifeModel;
		Material *hudIcon;
		weaponIconRatioType_t hudIconRatio;
		Material *pickupIcon;
		weaponIconRatioType_t pickupIconRatio;
		Material *ammoCounterIcon;
		weaponIconRatioType_t ammoCounterIconRatio;
		ammoCounterClipType_t ammoCounterClip;
		int iStartAmmo;
		const char *szAmmoName;
		int iAmmoIndex;
		const char *szClipName;
		int iClipIndex;
		int iMaxAmmo;
		int shotCount;
		const char *szSharedAmmoCapName;
		int iSharedAmmoCapIndex;
		int iSharedAmmoCap;
		int damage;
		int playerDamage;
		int iMeleeDamage;
		int iDamageType;
		int iFireDelay;
		int iMeleeDelay;
		int meleeChargeDelay;
		int iDetonateDelay;
		int iRechamberTime;
		int rechamberTimeOneHanded;
		int iRechamberBoltTime;
		int iHoldFireTime;
		int iDetonateTime;
		int iMeleeTime;
		int meleeChargeTime;
		int iReloadTime;
		int reloadShowRocketTime;
		int iReloadEmptyTime;
		int iReloadAddTime;
		int iReloadStartTime;
		int iReloadStartAddTime;
		int iReloadEndTime;
		int iDropTime;
		int iRaiseTime;
		int iAltDropTime;
		int quickDropTime;
		int quickRaiseTime;
		int iBreachRaiseTime;
		int iEmptyRaiseTime;
		int iEmptyDropTime;
		int sprintInTime;
		int sprintLoopTime;
		int sprintOutTime;
		int stunnedTimeBegin;
		int stunnedTimeLoop;
		int stunnedTimeEnd;
		int nightVisionWearTime;
		int nightVisionWearTimeFadeOutEnd;
		int nightVisionWearTimePowerUp;
		int nightVisionRemoveTime;
		int nightVisionRemoveTimePowerDown;
		int nightVisionRemoveTimeFadeInStart;
		int fuseTime;
		int aiFuseTime;
		float autoAimRange;
		float aimAssistRange;
		float aimAssistRangeAds;
		float aimPadding;
		float enemyCrosshairRange;
		float moveSpeedScale;
		float adsMoveSpeedScale;
		float sprintDurationScale;
		float fAdsZoomInFrac;
		float fAdsZoomOutFrac;
		Material *overlayMaterial;
		Material *overlayMaterialLowRes;
		Material *overlayMaterialEMP;
		Material *overlayMaterialEMPLowRes;
		weapOverlayReticle_t overlayReticle;
		WeapOverlayInteface_t overlayInterface;
		float overlayWidth;
		float overlayHeight;
		float overlayWidthSplitscreen;
		float overlayHeightSplitscreen;
		float fAdsBobFactor;
		float fAdsViewBobMult;
		float fHipSpreadStandMin;
		float fHipSpreadDuckedMin;
		float fHipSpreadProneMin;
		float hipSpreadStandMax;
		float hipSpreadDuckedMax;
		float hipSpreadProneMax;
		float fHipSpreadDecayRate;
		float fHipSpreadFireAdd;
		float fHipSpreadTurnAdd;
		float fHipSpreadMoveAdd;
		float fHipSpreadDuckedDecay;
		float fHipSpreadProneDecay;
		float fHipReticleSidePos;
		float fAdsIdleAmount;
		float fHipIdleAmount;
		float adsIdleSpeed;
		float hipIdleSpeed;
		float fIdleCrouchFactor;
		float fIdleProneFactor;
		float fGunMaxPitch;
		float fGunMaxYaw;
		float swayMaxAngle;
		float swayLerpSpeed;
		float swayPitchScale;
		float swayYawScale;
		float swayHorizScale;
		float swayVertScale;
		float swayShellShockScale;
		float adsSwayMaxAngle;
		float adsSwayLerpSpeed;
		float adsSwayPitchScale;
		float adsSwayYawScale;
		float adsSwayHorizScale;
		float adsSwayVertScale;
		float adsViewErrorMin;
		float adsViewErrorMax;
		PhysCollmap *physCollmap;
		float dualWieldViewModelOffset;
		weaponIconRatioType_t killIconRatio;
		int iReloadAmmoAdd;
		int iReloadStartAdd;
		int ammoDropStockMin;
		int ammoDropClipPercentMin;
		int ammoDropClipPercentMax;
		int iExplosionRadius;
		int iExplosionRadiusMin;
		int iExplosionInnerDamage;
		int iExplosionOuterDamage;
		float damageConeAngle;
		float bulletExplDmgMult;
		float bulletExplRadiusMult;
		int iProjectileSpeed;
		int iProjectileSpeedUp;
		int iProjectileSpeedForward;
		int iProjectileActivateDist;
		float projLifetime;
		float timeToAccelerate;
		float projectileCurvature;
		XModel *projectileModel;
		weapProjExposion_t projExplosion;
		FxEffectDef *projExplosionEffect;
		FxEffectDef *projDudEffect;
		snd_alias_list_t *projExplosionSound;
		snd_alias_list_t *projDudSound;
		WeapStickinessType stickiness;
		float lowAmmoWarningThreshold;
		float ricochetChance;
		float *parallelBounce;
		float *perpendicularBounce;
		FxEffectDef *projTrailEffect;
		FxEffectDef *projBeaconEffect;
		float vProjectileColor[3];
		guidedMissileType_t guidedMissileType;
		float maxSteeringAccel;
		int projIgnitionDelay;
		FxEffectDef *projIgnitionEffect;
		snd_alias_list_t *projIgnitionSound;
		float fAdsAimPitch;
		float fAdsCrosshairInFrac;
		float fAdsCrosshairOutFrac;
		int adsGunKickReducedKickBullets;
		float adsGunKickReducedKickPercent;
		float fAdsGunKickPitchMin;
		float fAdsGunKickPitchMax;
		float fAdsGunKickYawMin;
		float fAdsGunKickYawMax;
		float fAdsGunKickAccel;
		float fAdsGunKickSpeedMax;
		float fAdsGunKickSpeedDecay;
		float fAdsGunKickStaticDecay;
		float fAdsViewKickPitchMin;
		float fAdsViewKickPitchMax;
		float fAdsViewKickYawMin;
		float fAdsViewKickYawMax;
		float fAdsViewScatterMin;
		float fAdsViewScatterMax;
		float fAdsSpread;
		int hipGunKickReducedKickBullets;
		float hipGunKickReducedKickPercent;
		float fHipGunKickPitchMin;
		float fHipGunKickPitchMax;
		float fHipGunKickYawMin;
		float fHipGunKickYawMax;
		float fHipGunKickAccel;
		float fHipGunKickSpeedMax;
		float fHipGunKickSpeedDecay;
		float fHipGunKickStaticDecay;
		float fHipViewKickPitchMin;
		float fHipViewKickPitchMax;
		float fHipViewKickYawMin;
		float fHipViewKickYawMax;
		float fHipViewScatterMin;
		float fHipViewScatterMax;
		float fightDist;
		float maxDist;
		const char *accuracyGraphName[2];
		float(*originalAccuracyGraphKnots[2])[2];
		unsigned __int16 originalAccuracyGraphKnotCount[2];
		int iPositionReloadTransTime;
		float leftArc;
		float rightArc;
		float topArc;
		float bottomArc;
		float accuracy;
		float aiSpread;
		float playerSpread;
		float minTurnSpeed[2];
		float maxTurnSpeed[2];
		float pitchConvergenceTime;
		float yawConvergenceTime;
		float suppressTime;
		float maxRange;
		float fAnimHorRotateInc;
		float fPlayerPositionDist;
		const char *szUseHintString;
		const char *dropHintString;
		int iUseHintStringIndex;
		int dropHintStringIndex;
		float horizViewJitter;
		float vertViewJitter;
		float scanSpeed;
		float scanAccel;
		int scanPauseTime;
		const char *szScript;
		float fOOPosAnimLength[2];
		int minDamage;
		int minPlayerDamage;
		float fMaxDamageRange;
		float fMinDamageRange;
		float destabilizationRateTime;
		float destabilizationCurvatureMax;
		int destabilizeDistance;
		float *locationDamageMultipliers;
		const char *fireRumble;
		const char *meleeImpactRumble;
		TracerDef *tracerType;
		float turretScopeZoomRate;
		float turretScopeZoomMin;
		float turretScopeZoomMax;
		float turretOverheatUpRate;
		float turretOverheatDownRate;
		float turretOverheatPenalty;
		snd_alias_list_t *turretOverheatSound;
		FxEffectDef *turretOverheatEffect;
		const char *turretBarrelSpinRumble;
		float turretBarrelSpinSpeed;
		float turretBarrelSpinUpTime;
		float turretBarrelSpinDownTime;
		snd_alias_list_t *turretBarrelSpinMaxSnd;
		snd_alias_list_t *turretBarrelSpinUpSnd[4];
		snd_alias_list_t *turretBarrelSpinDownSnd[4];
		snd_alias_list_t *missileConeSoundAlias;
		snd_alias_list_t *missileConeSoundAliasAtBase;
		float missileConeSoundRadiusAtTop;
		float missileConeSoundRadiusAtBase;
		float missileConeSoundHeight;
		float missileConeSoundOriginOffset;
		float missileConeSoundVolumescaleAtCore;
		float missileConeSoundVolumescaleAtEdge;
		float missileConeSoundVolumescaleCoreSize;
		float missileConeSoundPitchAtTop;
		float missileConeSoundPitchAtBottom;
		float missileConeSoundPitchTopSize;
		float missileConeSoundPitchBottomSize;
		float missileConeSoundCrossfadeTopSize;
		float missileConeSoundCrossfadeBottomSize;
		bool sharedAmmo;
		bool lockonSupported;
		bool requireLockonToFire;
		bool bigExplosion;
		bool noAdsWhenMagEmpty;
		bool avoidDropCleanup;
		bool inheritsPerks;
		bool crosshairColorChange;
		bool bRifleBullet;
		bool armorPiercing;
		bool bBoltAction;
		bool aimDownSight;
		bool bRechamberWhileAds;
		bool bBulletExplosiveDamage;
		bool bCookOffHold;
		bool bClipOnly;
		bool noAmmoPickup;
		bool adsFireOnly;
		bool cancelAutoHolsterWhenEmpty;
		bool disableSwitchToWhenEmpty;
		bool suppressAmmoReserveDisplay;
		bool laserSightDuringNightvision;
		bool markableViewmodel;
		bool noDualWield;
		bool flipKillIcon;
		bool bNoPartialReload;
		bool bSegmentedReload;
		bool blocksProne;
		bool silenced;
		bool isRollingGrenade;
		bool projExplosionEffectForceNormalUp;
		bool bProjImpactExplode;
		bool stickToPlayers;
		bool hasDetonator;
		bool disableFiring;
		bool timedDetonation;
		bool rotate;
		bool holdButtonToThrow;
		bool freezeMovementWhenFiring;
		bool thermalScope;
		bool altModeSameWeapon;
		bool turretBarrelSpinEnabled;
		bool missileConeSoundEnabled;
		bool missileConeSoundPitchshiftEnabled;
		bool missileConeSoundCrossfadeEnabled;
		bool offhandHoldIsCancelable;
	};

	enum ImpactType
	{
		IMPACT_TYPE_NONE = 0x0,
		IMPACT_TYPE_BULLET_SMALL = 0x1,
		IMPACT_TYPE_BULLET_LARGE = 0x2,
		IMPACT_TYPE_BULLET_AP = 0x3,
		IMPACT_TYPE_BULLET_EXPLODE = 0x4,
		IMPACT_TYPE_SHOTGUN = 0x5,
		IMPACT_TYPE_SHOTGUN_EXPLODE = 0x6,
		IMPACT_TYPE_GRENADE_BOUNCE = 0x7,
		IMPACT_TYPE_GRENADE_EXPLODE = 0x8,
		IMPACT_TYPE_ROCKET_EXPLODE = 0x9,
		IMPACT_TYPE_PROJECTILE_DUD = 0xA,
		IMPACT_TYPE_COUNT = 0xB,
	};

	struct /*__declspec(align(2))*/ WeaponCompleteDef
	{
		const char *szInternalName;
		WeaponDef *weapDef;
		const char *szDisplayName;
		unsigned __int16 *hideTags;
		const char **szXAnims;
		float fAdsZoomFov;
		int iAdsTransInTime;
		int iAdsTransOutTime;
		int iClipSize;
		ImpactType impactType;
		int iFireTime;
		weaponIconRatioType_t dpadIconRatio;
		float penetrateMultiplier;
		float fAdsViewKickCenterSpeed;
		float fHipViewKickCenterSpeed;
		const char *szAltWeaponName;
		unsigned int altWeaponIndex;
		int iAltRaiseTime;
		Material *killIcon;
		Material *dpadIcon;
		int fireAnimLength;
		int iFirstRaiseTime;
		int ammoDropStockMax;
		float adsDofStart;
		float adsDofEnd;
		unsigned __int16 accuracyGraphKnotCount[2];
		float(*accuracyGraphKnots[2])[2];
		bool motionTracker;
		bool enhanced;
		bool dpadIconShowsAmmo;
	};

	struct SndDriverGlobals
	{
		const char *name;
	};

	struct FxImpactEntry
	{
		FxEffectDef *nonflesh[31];
		FxEffectDef *flesh[4];
	};

	struct FxImpactTable
	{
		const char *name;
		FxImpactEntry *table;
	};

	struct RawFile
	{
		const char *name;
		int compressedLen;
		int len;
		const char *buffer;
	};

	struct StringTableCell
	{
		const char *string;
		int hash;
	};

	struct StringTable
	{
		const char *name;
		int columnCount;
		int rowCount;
		StringTableCell *values;
	};

	enum LbColType
	{
		LBCOL_TYPE_NUMBER = 0x0,
		LBCOL_TYPE_TIME = 0x1,
		LBCOL_TYPE_LEVELXP = 0x2,
		LBCOL_TYPE_PRESTIGE = 0x3,
		LBCOL_TYPE_BIGNUMBER = 0x4,
		LBCOL_TYPE_PERCENT = 0x5,
		LBCOL_TYPE_COUNT = 0x6,
	};

	enum LbAggType
	{
		LBAGG_TYPE_MIN = 0x0,
		LBAGG_TYPE_MAX = 0x1,
		LBAGG_TYPE_SUM = 0x2,
		LBAGG_TYPE_LAST = 0x3,
		LBAGG_TYPE_COUNT = 0x4,
	};

	struct LbColumnDef
	{
		const char *name;
		int id;
		int propertyId;
		bool hidden;
		const char *statName;
		LbColType type;
		int precision;
		LbAggType agg;
	};

	struct LeaderboardDef
	{
		const char *name;
		int id;
		int columnCount;
		int xpColId;
		int prestigeColId;
		LbColumnDef *columns;
	};

	struct __declspec(align(4)) StructuredDataEnumEntry
	{
		const char *string;
		unsigned __int16 index;
	};

	struct StructuredDataEnum
	{
		int entryCount;
		int reservedEntryCount;
		StructuredDataEnumEntry *entries;
	};

	enum StructuredDataTypeCategory
	{
		DATA_INT = 0x0,
		DATA_BYTE = 0x1,
		DATA_BOOL = 0x2,
		DATA_STRING = 0x3,
		DATA_ENUM = 0x4,
		DATA_STRUCT = 0x5,
		DATA_INDEXED_ARRAY = 0x6,
		DATA_ENUM_ARRAY = 0x7,
		DATA_FLOAT = 0x8,
		DATA_SHORT = 0x9,
		DATA_COUNT = 0xA,
	};

	union StructuredDataTypeUnion
	{
		unsigned int stringDataLength;
		int enumIndex;
		int structIndex;
		int indexedArrayIndex;
		int enumedArrayIndex;
	};

	struct StructuredDataType
	{
		StructuredDataTypeCategory type;
		StructuredDataTypeUnion u;
	};

	struct StructuredDataStructProperty
	{
		const char *name;
		StructuredDataType type;
		unsigned int offset;
	};

	struct StructuredDataStruct
	{
		int propertyCount;
		StructuredDataStructProperty *properties;
		int size;
		unsigned int bitOffset;
	};

	struct StructuredDataIndexedArray
	{
		int arraySize;
		StructuredDataType elementType;
		unsigned int elementSize;
	};

	struct StructuredDataEnumedArray
	{
		int enumIndex;
		StructuredDataType elementType;
		unsigned int elementSize;
	};

	struct StructuredDataDef
	{
		int version;
		unsigned int formatChecksum;
		int enumCount;
		StructuredDataEnum *enums;
		int structCount;
		StructuredDataStruct *structs;
		int indexedArrayCount;
		StructuredDataIndexedArray *indexedArrays;
		int enumedArrayCount;
		StructuredDataEnumedArray *enumedArrays;
		StructuredDataType rootType;
		unsigned int size;
	};

	struct StructuredDataDefSet
	{
		const char *name;
		unsigned int defCount;
		StructuredDataDef *defs;
	};

	struct StructuredDataBuffer
	{
		char *data;
		unsigned int size;
	};

	enum VehicleType
	{
		VEH_WHEELS_4 = 0x0,
		VEH_TANK = 0x1,
		VEH_PLANE = 0x2,
		VEH_BOAT = 0x3,
		VEH_ARTILLERY = 0x4,
		VEH_HELICOPTER = 0x5,
		VEH_SNOWMOBILE = 0x6,
		VEH_TYPE_COUNT = 0x7,
	};

	enum VehicleAxleType
	{
		VEH_AXLE_FRONT = 0x0,
		VEH_AXLE_REAR = 0x1,
		VEH_AXLE_ALL = 0x2,
		VEH_AXLE_COUNT = 0x3,
	};

	struct VehiclePhysDef
	{
		int physicsEnabled;
		const char *physPresetName;
		PhysPreset *physPreset;
		const char *accelGraphName;
		VehicleAxleType steeringAxle;
		VehicleAxleType powerAxle;
		VehicleAxleType brakingAxle;
		float topSpeed;
		float reverseSpeed;
		float maxVelocity;
		float maxPitch;
		float maxRoll;
		float suspensionTravelFront;
		float suspensionTravelRear;
		float suspensionStrengthFront;
		float suspensionDampingFront;
		float suspensionStrengthRear;
		float suspensionDampingRear;
		float frictionBraking;
		float frictionCoasting;
		float frictionTopSpeed;
		float frictionSide;
		float frictionSideRear;
		float velocityDependentSlip;
		float rollStability;
		float rollResistance;
		float pitchResistance;
		float yawResistance;
		float uprightStrengthPitch;
		float uprightStrengthRoll;
		float targetAirPitch;
		float airYawTorque;
		float airPitchTorque;
		float minimumMomentumForCollision;
		float collisionLaunchForceScale;
		float wreckedMassScale;
		float wreckedBodyFriction;
		float minimumJoltForNotify;
		float slipThresholdFront;
		float slipThresholdRear;
		float slipFricScaleFront;
		float slipFricScaleRear;
		float slipFricRateFront;
		float slipFricRateRear;
		float slipYawTorque;
	};

	struct VehicleDef
	{
		const char *name;
		VehicleType type;
		const char *useHintString;
		int health;
		int quadBarrel;
		float texScrollScale;
		float topSpeed;
		float accel;
		float rotRate;
		float rotAccel;
		float maxBodyPitch;
		float maxBodyRoll;
		float fakeBodyAccelPitch;
		float fakeBodyAccelRoll;
		float fakeBodyVelPitch;
		float fakeBodyVelRoll;
		float fakeBodySideVelPitch;
		float fakeBodyPitchStrength;
		float fakeBodyRollStrength;
		float fakeBodyPitchDampening;
		float fakeBodyRollDampening;
		float fakeBodyBoatRockingAmplitude;
		float fakeBodyBoatRockingPeriod;
		float fakeBodyBoatRockingRotationPeriod;
		float fakeBodyBoatRockingFadeoutSpeed;
		float boatBouncingMinForce;
		float boatBouncingMaxForce;
		float boatBouncingRate;
		float boatBouncingFadeinSpeed;
		float boatBouncingFadeoutSteeringAngle;
		float collisionDamage;
		float collisionSpeed;
		float killcamOffset[3];
		int playerProtected;
		int bulletDamage;
		int armorPiercingDamage;
		int grenadeDamage;
		int projectileDamage;
		int projectileSplashDamage;
		int heavyExplosiveDamage;
		VehiclePhysDef vehPhysDef;
		float boostDuration;
		float boostRechargeTime;
		float boostAcceleration;
		float suspensionTravel;
		float maxSteeringAngle;
		float steeringLerp;
		float minSteeringScale;
		float minSteeringSpeed;
		int camLookEnabled;
		float camLerp;
		float camPitchInfluence;
		float camRollInfluence;
		float camFovIncrease;
		float camFovOffset;
		float camFovSpeed;
		const char *turretWeaponName;
		WeaponCompleteDef *turretWeapon;
		float turretHorizSpanLeft;
		float turretHorizSpanRight;
		float turretVertSpanUp;
		float turretVertSpanDown;
		float turretRotRate;
		snd_alias_list_t *turretSpinSnd;
		snd_alias_list_t *turretStopSnd;
		int trophyEnabled;
		float trophyRadius;
		float trophyInactiveRadius;
		int trophyAmmoCount;
		float trophyReloadTime;
		unsigned __int16 trophyTags[4];
		Material *compassFriendlyIcon;
		Material *compassEnemyIcon;
		int compassIconWidth;
		int compassIconHeight;
		snd_alias_list_t *idleLowSnd;
		snd_alias_list_t *idleHighSnd;
		snd_alias_list_t *engineLowSnd;
		snd_alias_list_t *engineHighSnd;
		float engineSndSpeed;
		snd_alias_list_t *engineStartUpSnd;
		int engineStartUpLength;
		snd_alias_list_t *engineShutdownSnd;
		snd_alias_list_t *engineIdleSnd;
		snd_alias_list_t *engineSustainSnd;
		snd_alias_list_t *engineRampUpSnd;
		int engineRampUpLength;
		snd_alias_list_t *engineRampDownSnd;
		int engineRampDownLength;
		snd_alias_list_t *suspensionSoftSnd;
		float suspensionSoftCompression;
		snd_alias_list_t *suspensionHardSnd;
		float suspensionHardCompression;
		snd_alias_list_t *collisionSnd;
		float collisionBlendSpeed;
		snd_alias_list_t *speedSnd;
		float speedSndBlendSpeed;
		const char *surfaceSndPrefix;
		snd_alias_list_t *surfaceSnds[31];
		float surfaceSndBlendSpeed;
		float slideVolume;
		float slideBlendSpeed;
		float inAirPitch;
	};

	struct AddonMapEnts
	{
		const char *name;
		char *entityString;
		int numEntityChars;
		MapTriggers trigger;
	};

	union XAssetHeader
	{
		void *data;
		PhysPreset *physPreset;
		PhysCollmap *physCollmap;
		XAnimParts *parts;
		XModelSurfs *modelSurfs;
		XModel *model;
		Material *material;
		MaterialPixelShader *pixelShader;
		MaterialVertexShader *vertexShader;
		MaterialVertexDeclaration *vertexDecl;
		MaterialTechniqueSet *techniqueSet;
		GfxImage *image;
		snd_alias_list_t *sound;
		SndCurve *sndCurve;
		LoadedSound *loadSnd;
		clipMap_t *clipMap;
		ComWorld *comWorld;
		GameWorldSp *gameWorldSp;
		GameWorldMp *gameWorldMp;
		MapEnts *mapEnts;
		FxWorld *fxWorld;
		GfxWorld *gfxWorld;
		GfxLightDef *lightDef;
		Font_s *font;
		MenuList *menuList;
		menuDef_t *menu;
		LocalizeEntry *localize;
		WeaponCompleteDef *weapon;
		SndDriverGlobals *sndDriverGlobals;
		FxEffectDef *fx;
		FxImpactTable *impactFx;
		RawFile *rawfile;
		StringTable *stringTable;
		LeaderboardDef *leaderboardDef;
		StructuredDataDefSet *structuredDataDefSet;
		TracerDef *tracerDef;
		VehicleDef *vehDef;
		AddonMapEnts *addonMapEnts;
	};

	struct XAsset
	{
		XAssetType type;
		XAssetHeader header;
	};

	struct XBlock
	{
		char *data;
		unsigned int size;
	};

	struct XAssetEntry
	{
		XAsset asset;
		char zoneIndex;
		volatile char inuse;
		unsigned __int16 nextHash;
		unsigned __int16 nextOverride;
		unsigned __int16 usageFrame;
	};

	enum XFileLanguage : unsigned char
	{
		XLANG_NONE = 0x00,
		XLANG_ENGLISH = 0x01,
		XLANG_FRENCH = 0x02,
		XLANG_GERMAN = 0x03,
		XLANG_ITALIAN = 0x04,
		XLANG_SPANISH = 0x05,
		XLANG_BRITISH = 0x06,
		XLANG_RUSSIAN = 0x07,
		XLANG_POLISH = 0x08,
		XLANG_KOREAN = 0x09,
		XLANG_TAIWANESE = 0x0A,
		XLANG_JAPANESE = 0x0B,
		XLANG_CHINESE = 0x0C,
		XLANG_THAI = 0x0D,
		XLANG_LEET = 0x0E, // Wat?
		XLANG_CZECH = 0x0F,
	};

#pragma pack(push, 1)
	struct XFileHeader
	{
		unsigned __int64 magic;
		unsigned int version;
		XFileLanguage language;
		DWORD highDateTime;
		DWORD lowDateTime;
	};
#pragma pack(pop)

	enum XFILE_BLOCK_TYPES
	{
		XFILE_BLOCK_TEMP = 0x0,
		XFILE_BLOCK_PHYSICAL = 0x1,
		XFILE_BLOCK_RUNTIME = 0x2,
		XFILE_BLOCK_VIRTUAL = 0x3,
		XFILE_BLOCK_LARGE = 0x4,

		// Those are probably incorrect
		XFILE_BLOCK_CALLBACK,
		XFILE_BLOCK_VERTEX,
		XFILE_BLOCK_INDEX,

		MAX_XFILE_COUNT,

		XFILE_BLOCK_INVALID = -1
	};

	struct XFile
	{
		unsigned int size;
		unsigned int externalSize;
		unsigned int blockSize[MAX_XFILE_COUNT];
	};

	struct ScriptStringList
	{
		int count;
		const char **strings;
	};

	struct XAssetList
	{
		ScriptStringList stringList;
		int assetCount;
		XAsset *assets;
	};

	struct ZoneHeader
	{
		XFile xFile;
		XAssetList assetList;
	};

	struct XZoneMemory
	{
		XBlock blocks[MAX_XFILE_COUNT];
		char *lockedVertexData;
		char *lockedIndexData;
		void *vertexBuffer;
		void *indexBuffer;
	};

	struct XZone
	{
		int unk;
		char name[64];
		int flags;
		int allocType;
		XZoneMemory mem;
		int fileSize;
		char modZone;
	};

	struct XNKID
	{
		char ab[8];
	};

	struct XNADDR
	{
		in_addr ina;
		in_addr inaOnline;
		unsigned __int16 wPortOnline;
		char abEnet[6];
		char abOnline[20];
	};

	struct XNKEY
	{
		char ab[16];
	};

	struct _XSESSION_INFO
	{
		XNKID sessionID;
		XNADDR hostAddress;
		XNKEY keyExchangeKey;
	};

	struct mapArena_t
	{
		char uiName[32];
		char mapName[16];
		char description[32];
		char mapimage[32];
		char keys[32][16];
		char values[32][64];
		char pad[144];
	};

	struct newMapArena_t
	{
		char uiName[32];
		char oldMapName[16];
		char description[32];
		char mapimage[32];
		char keys[32][16];
		char values[32][64];
		char other[144];
		char mapName[32];
	};

	struct gameTypeName_t
	{
		char gameType[12];
		char uiName[32];
	};

	typedef struct party_s
	{
		unsigned char pad1[544];
		int privateSlots;
		int publicSlots;
	} party_t;

	typedef struct PartyData_s
	{
		DWORD unk;
	} PartyData_t;

	struct fileInIwd_s
	{
		unsigned int pos;
		char *name;
		fileInIwd_s *next;
	};

	struct iwd_t
	{
		char iwdFilename[256];
		char iwdBasename[256];
		char iwdGamename[256];
		char *handle;
		int checksum;
		int pure_checksum;
		volatile int hasOpenFile;
		int numfiles;
		char referenced;
		unsigned int hashSize;
		fileInIwd_s **hashTable;
		fileInIwd_s *buildBuffer;
	};

#ifdef IDA
	typedef void _iobuf;
#endif

	union qfile_gus
	{
		_iobuf *o;
		char *z;
	};

	struct qfile_us
	{
		qfile_gus file;
		int iwdIsClone;
	};

	struct fileHandleData_t
	{
		qfile_us handleFiles;
		int handleSync;
		int fileSize;
		int zipFilePos;
		iwd_t *zipFile;
		int streamed;
		char name[256];
	};

	typedef struct {
		char		path[256];		// c:\quake3
		char		gamedir[256];	// baseq3
	} directory_t;

	typedef struct searchpath_s
	{
		searchpath_s* next;
		iwd_t *iwd;
		directory_t* dir;
		int bLocalized;
		int ignore;
		int ignorePureCheck;
		int language;
	} searchpath_t;

	struct SafeArea
	{
		int fontHeight;
		int textHeight;
		int textWidth;
		float left;
		float top;
		float right;
		float bottom;
	};

#pragma pack(push, 4)
	struct SpawnVar
	{
		bool spawnVarsValid;
		int numSpawnVars;
		char *spawnVars[64][2];
		int numSpawnVarChars;
		char spawnVarChars[2048];
	};
#pragma pack(pop)

#pragma pack(push, 4)
	struct usercmd_s
	{
		int serverTime;
		int buttons;
		int angles[3];
		unsigned __int16 weapon;
		unsigned __int16 primaryWeaponForAltMode;
		unsigned __int16 offHandIndex;
		char forwardmove;
		char rightmove;
		float meleeChargeYaw;
		char meleeChargeDist;
		char selectedLoc[2];
		char selectedLocAngle;
		char remoteControlAngles[2];
	};
#pragma pack(pop)

	typedef char mapname_t[40];

	struct traceWork_t
	{
		/*TraceExtents*/int extents;
		float delta[3];
		float deltaLen;
		float deltaLenSq;
		float delta2DLen;
		float delta2DLenSq;
		float size[3];
		Bounds bounds;
		int contents;
		bool isPoint;
		bool axialCullOnly;
		float radius;
		float offset[3];
		float radiusOffset[3];
		float boundingRadius;
		/*TraceThreadInfo*/ int threadInfo;
		/*CM_WorldTraceCallbacks*/ void *callbacks;
	};

	struct gameState_t
	{
		int stringOffsets[4139];
		char stringData[131072];
		int dataCount;
	} gameState;

	struct VariableStackBuffer
	{
		const char *pos;
		unsigned __int16 size;
		unsigned __int16 bufLen;
		unsigned __int16 localId;
		char time;
		char buf[1];
	};

	enum VariableType
	{
		VAR_UNDEFINED = 0x0,
		VAR_BEGIN_REF = 0x1,
		VAR_POINTER = 0x1,
		VAR_STRING = 0x2,
		VAR_ISTRING = 0x3,
		VAR_VECTOR = 0x4,
		VAR_END_REF = 0x5,
		VAR_FLOAT = 0x5,
		VAR_INTEGER = 0x6,
		VAR_CODEPOS = 0x7,
		VAR_PRECODEPOS = 0x8,
		VAR_FUNCTION = 0x9,
		VAR_BUILTIN_FUNCTION = 0xA,
		VAR_BUILTIN_METHOD = 0xB,
		VAR_STACK = 0xC,
		VAR_ANIMATION = 0xD,
		VAR_PRE_ANIMATION = 0xE,
		VAR_THREAD = 0xF,
		VAR_NOTIFY_THREAD = 0x10,
		VAR_TIME_THREAD = 0x11,
		VAR_CHILD_THREAD = 0x12,
		VAR_OBJECT = 0x13,
		VAR_DEAD_ENTITY = 0x14,
		VAR_ENTITY = 0x15,
		VAR_ARRAY = 0x16,
		VAR_DEAD_THREAD = 0x17,
		VAR_COUNT = 0x18,
		VAR_FREE = 0x18,
		VAR_THREAD_LIST = 0x19,
		VAR_ENDON_LIST = 0x1A,
		VAR_TOTAL_COUNT = 0x1B,
	};

	union VariableUnion
	{
		int intValue;
		unsigned int uintValue;
		float floatValue;
		unsigned int stringValue;
		const float *vectorValue;
		const char *codePosValue;
		unsigned int pointerValue;
		VariableStackBuffer *stackValue;
		unsigned int entityOffset;
	};

	struct VariableValue
	{
		VariableUnion u;
		int type;
	};

	struct ScriptContainer
	{
		VariableValue* stack;
		char unk1;
		char unk2;
		char unk3;
		char pad;
		DWORD unk4;
		int numParam;
	};

	enum UILocalVarType
	{
		UILOCALVAR_INT = 0x0,
		UILOCALVAR_FLOAT = 0x1,
		UILOCALVAR_STRING = 0x2,
	};

	union $B42A88463653BDCDFC5664844B4491DA
	{
		int integer;
		float value;
		const char *string;
	};

	struct UILocalVar
	{
		UILocalVarType type;
		const char *name;
		$B42A88463653BDCDFC5664844B4491DA u;
	};

	struct UILocalVarContext
	{
		UILocalVar table[256];
	};

	struct $1942E78D15753E2013144570239460A8
	{
		float x;
		float y;
		int lastMoveTime;
	};

	struct UiContext
	{
		int localClientNum;
		float bias;
		int realTime;
		int frameTime;
		$1942E78D15753E2013144570239460A8 cursor;
		int isCursorVisible;
		int paintFull;
		int screenWidth;
		int screenHeight;
		float screenAspect;
		float FPS;
		float blurRadiusOut;
		menuDef_t *Menus[640];
		int menuCount;
		menuDef_t *menuStack[16];
		int openMenuCount;
		UILocalVarContext localVars;
	};

	struct msg_t
	{
		int overflowed;
		int readOnly;
		char *data;
		char *splitData;
		int maxsize;
		int cursize;
		int splitSize;
		int readcount;
		int bit;
		int lastEntityRef;
	};

	struct XZoneInfo
	{
		const char *name;
		int allocFlags;
		int freeFlags;
	};

	enum FsListBehavior_e
	{
		FS_LIST_PURE_ONLY = 0x0,
		FS_LIST_ALL = 0x1,
	};

	enum netsrc_t
	{
		NS_CLIENT1 = 0x0,
		NS_SERVER = 0x1,
		NS_MAXCLIENTS = 0x1,
		NS_PACKET = 0x2,
	};

	enum netadrtype_t
	{
		NA_BOT = 0x0,
		NA_BAD = 0x1,
		NA_LOOPBACK = 0x2,
		NA_BROADCAST = 0x3,
		NA_IP = 0x4,
		NA_IPX = 0x5,
		NA_BROADCAST_IPX = 0x6,
	};

	typedef union
	{
		unsigned char bytes[4];
		DWORD full;
	} netIP_t;

	struct netadr_t
	{
		netadrtype_t type;
		//char ip[4];
		netIP_t ip;
		unsigned __int16 port;
		char ipx[10];
	};

	struct netProfileInfo_t
	{
		char __pad0[0x5E0];
	};

	static_assert(sizeof(netProfileInfo_t) == 0x5E0);

	struct netchan_t
	{
		int outgoingSequence;
		netsrc_t sock;
		int dropped;
		int incomingSequence;
		netadr_t remoteAddress;
		int qport;
		int fragmentSequence;
		int fragmentLength;
		char* fragmentBuffer;
		int fragmentBufferSize;
		int unsentFragments;
		int unsentFragmentStart;
		int unsentLength;
		char* unsentBuffer;
		int unsentBufferSize;
		netProfileInfo_t prof;
	};

	static_assert(sizeof(netchan_t) == 0x62C);

	struct FxEditorElemAtlas
	{
		int behavior;
		int index;
		int fps;
		int loopCount;
		int colIndexBits;
		int rowIndexBits;
		int entryCount;
	};

	struct FxCurve
	{
		int dimensionCount;
		int keyCount;
		float keys[1];
	};

	union $81775853B5F1E1C6748A82ED93FC367C
	{
		FxElemVisuals visuals[32];
		FxElemMarkVisuals markVisuals[16];
	};

	struct FxEditorTrailDef
	{
		FxTrailVertex verts[64];
		int vertCount;
		unsigned __int16 inds[128];
		int indCount;
	};

	struct FxEditorElemDef
	{
		char name[48];
		int editorFlags;
		int flags;
		FxFloatRange spawnRange;
		FxFloatRange fadeInRange;
		FxFloatRange fadeOutRange;
		float spawnFrustumCullRadius;
		FxSpawnDefLooping spawnLooping;
		FxSpawnDefOneShot spawnOneShot;
		FxIntRange spawnDelayMsec;
		FxIntRange lifeSpanMsec;
		FxFloatRange spawnOrigin[3];
		FxFloatRange spawnOffsetRadius;
		FxFloatRange spawnOffsetHeight;
		FxFloatRange spawnAngles[3];
		FxFloatRange angularVelocity[3];
		FxFloatRange initialRotation;
		FxFloatRange gravity;
		FxFloatRange elasticity;
		FxEditorElemAtlas atlas;
		float velScale[2][3];
		FxCurve *velShape[2][3][2];
		float rotationScale;
		FxCurve *rotationShape[2];
		float sizeScale[2];
		FxCurve *sizeShape[2][2];
		float scaleScale;
		FxCurve *scaleShape[2];
		FxCurve *color[2];
		FxCurve *alpha[2];
		float lightingFrac;
		float decalFadeInTime;
		float collOffset[3];
		float collRadius;
		FxEffectDef *effectOnImpact;
		FxEffectDef *effectOnDeath;
		int sortOrder;
		FxEffectDef *emission;
		FxFloatRange emitDist;
		FxFloatRange emitDistVariance;
		char elemType;
		int visualCount;
		$81775853B5F1E1C6748A82ED93FC367C ___u42;
		int trailSplitDist;
		int trailSplitArcDist;
		int trailSplitTime;
		int trailRepeatDist;
		float trailScrollTime;
		FxEditorTrailDef trailDef;
		float sparkFountainGravity;
		float sparkFountainBounceFrac;
		float sparkFountainBounceRand;
		float sparkFountainSparkSpacing;
		float sparkFountainSparkLength;
		int sparkFountainSparkCount;
		float sparkFountainLoopTime;
		float sparkFountainVelMin;
		float sparkFountainVelMax;
		float sparkFountainVelConeAngle;
		float sparkFountainRestSpeed;
		float sparkFountainBoostTime;
		float sparkFountainBoostFactor;
	};

	struct FxEditorEffectDef
	{
		char name[64];
		int elemCount;
		FxEditorElemDef elems[32];
	};

	struct FxElemField
	{
		const char *keyName;
		bool(__cdecl *handler)(const char **, FxEditorElemDef *);
	};

	enum $18B36A54AD92993D0728595D3504B7CB
	{
		FX_ELEM_TYPE_SPRITE_BILLBOARD = 0x0,
		FX_ELEM_TYPE_SPRITE_ORIENTED = 0x1,
		FX_ELEM_TYPE_TAIL = 0x2,
		FX_ELEM_TYPE_TRAIL = 0x3,
		FX_ELEM_TYPE_CLOUD = 0x4,
		FX_ELEM_TYPE_SPARK_CLOUD = 0x5,
		FX_ELEM_TYPE_SPARK_FOUNTAIN = 0x6,
		FX_ELEM_TYPE_MODEL = 0x7,
		FX_ELEM_TYPE_OMNI_LIGHT = 0x8,
		FX_ELEM_TYPE_SPOT_LIGHT = 0x9,
		FX_ELEM_TYPE_SOUND = 0xA,
		FX_ELEM_TYPE_DECAL = 0xB,
		FX_ELEM_TYPE_RUNNER = 0xC,
		FX_ELEM_TYPE_COUNT = 0xD,
		FX_ELEM_TYPE_LAST_SPRITE = 0x3,
		FX_ELEM_TYPE_LAST_DRAWN = 0x9,
	};

	struct infoParm_t
	{
		char *name;
		int clearSolid;
		int surfaceFlags;
		int contents;
		int toolFlags;
	};

	struct GfxImageFileHeader
	{
		char tag[3];
		char version;
		unsigned int flags;
		char format;
		char unused;
		__int16 dimensions[3];
		int fileSizeForPicmip[4];
	};

	enum $1FA877C9772E9F0892A93F52A91453E9
	{
		MAPTYPE_NONE = 0x0,
		MAPTYPE_INVALID1 = 0x1,
		MAPTYPE_1D = 0x2,
		MAPTYPE_2D = 0x3,
		MAPTYPE_3D = 0x4,
		MAPTYPE_CUBE = 0x5,
		MAPTYPE_COUNT = 0x6,
	};

	enum GfxImageFileFormat
	{
		IMG_FORMAT_INVALID = 0x0,
		IMG_FORMAT_BITMAP_RGBA = 0x1,
		IMG_FORMAT_BITMAP_RGB = 0x2,
		IMG_FORMAT_BITMAP_LUMINANCE_ALPHA = 0x3,
		IMG_FORMAT_BITMAP_LUMINANCE = 0x4,
		IMG_FORMAT_BITMAP_ALPHA = 0x5,
		IMG_FORMAT_WAVELET_RGBA = 0x6,
		IMG_FORMAT_WAVELET_RGB = 0x7,
		IMG_FORMAT_WAVELET_LUMINANCE_ALPHA = 0x8,
		IMG_FORMAT_WAVELET_LUMINANCE = 0x9,
		IMG_FORMAT_WAVELET_ALPHA = 0xA,
		IMG_FORMAT_DXT1 = 0xB,
		IMG_FORMAT_DXT3 = 0xC,
		IMG_FORMAT_DXT5 = 0xD,
		IMG_FORMAT_DXN = 0xE,
		IMG_FORMAT_DXT3A_AS_LUMINANCE = 0xF,
		IMG_FORMAT_DXT5A_AS_LUMINANCE = 0x10,
		IMG_FORMAT_DXT3A_AS_ALPHA = 0x11,
		IMG_FORMAT_DXT5A_AS_ALPHA = 0x12,
		IMG_FORMAT_DXT1_AS_LUMINANCE_ALPHA = 0x13,
		IMG_FORMAT_DXN_AS_LUMINANCE_ALPHA = 0x14,
		IMG_FORMAT_DXT1_AS_LUMINANCE = 0x15,
		IMG_FORMAT_DXT1_AS_ALPHA = 0x16,
		IMG_FORMAT_COUNT = 0x17,
	};

	enum $25EF9448C800B18F0C83DB367159AFD6
	{
		PART_TYPE_NO_QUAT = 0x0,
		PART_TYPE_HALF_QUAT = 0x1,
		PART_TYPE_FULL_QUAT = 0x2,
		PART_TYPE_HALF_QUAT_NO_SIZE = 0x3,
		PART_TYPE_FULL_QUAT_NO_SIZE = 0x4,
		PART_TYPE_SMALL_TRANS = 0x5,
		PART_TYPE_TRANS = 0x6,
		PART_TYPE_TRANS_NO_SIZE = 0x7,
		PART_TYPE_NO_TRANS = 0x8,
		PART_TYPE_ALL = 0x9,
		PART_TYPE_COUNT = 0xA,
	};

	enum $EDD38D6A57EF43793B1F773859FC039A
	{
		FS_SEEK_CUR = 0x0,
		FS_SEEK_END = 0x1,
		FS_SEEK_SET = 0x2,
	};

#pragma region CUSTOM

	enum itemTextStyle
	{
		ITEM_TEXTSTYLE_NORMAL = 0,   // normal text
		ITEM_TEXTSTYLE_SHADOWED = 3,   // drop shadow ( need a color for this )
		ITEM_TEXTSTYLE_SHADOWEDMORE = 6,   // drop shadow ( need a color for this )
		ITEM_TEXTSTYLE_BORDERED = 7,   // border (stroke)
		ITEM_TEXTSTYLE_BORDEREDMORE = 8,   // more border :P
		ITEM_TEXTSTYLE_MONOSPACE = 128,
		ITEM_TEXTSTYLE_MONOSPACESHADOWED = 132,
	};

	enum $53C66D4FC2874B6934A17E4ED449BCEB
	{
		// 		DB_ZONE_COMMON = 0x1,
		// 		DB_ZONE_UI = 0x2,
		// 		DB_ZONE_GAME = 0x4,
		// 		DB_ZONE_LOAD = 0x8,
		// 		DB_ZONE_DEV = 0x10,
		// 		DB_ZONE_TRANSIENT = 0x20,
		DB_ZONE_CODE_LOC = 0x0,
		DB_ZONE_COMMON_LOC = 0x1,
		DB_ZONE_CODE = 0x2,
		DB_ZONE_COMMON = 0x4,
		DB_ZONE_GAME = 0x8,
		DB_ZONE_MOD = 0x10,
		DB_ZONE_LOAD = 0x20,
		DB_ZONE_DEV = 0x40
	};

	enum playerFlag
	{
		PLAYER_FLAG_NOCLIP = 1 << 0,
		PLAYER_FLAG_UFO = 1 << 1,
		PLAYER_FLAG_FROZEN = 1 << 2,
	};

	typedef struct gclient_s
	{
		unsigned char pad[12764];
		unsigned int team;
		char pad2[436];
		int flags;
		char pad3[724];
	} gclient_t;

	struct LerpEntityState
	{
		char pad[0x70];
	};

	struct clientLinkInfo_t
	{
		__int16 parentId;
		char tagName;
		char flags;
	};

	struct entityState_s
	{
		int number;
		int eType;
		LerpEntityState lerp;
		int time2;
		int otherEntityNum;
		int attackerEntityNum;
		int groundEntityNum;
		int loopSound;
		int surfType;

		union
		{
			int brushModel;
			int triggerModel;
			int item;
			int xmodel;
			int primaryLight;
		} index;

		int clientNum;
		int iHeadIcon;
		int iHeadIconTeam;
		int solid;
		unsigned int eventParm;
		int eventSequence;
		int events[4];
		unsigned int eventParms[4];
		unsigned __int16 weapon;
		int legsAnim;
		int torsoAnim;
		int un1;
		int un2;
		clientLinkInfo_t clientLinkInfo;
		unsigned int partBits[6];
		int clientMask[1];
	};

	struct EntHandle
	{
		unsigned __int16 number;
		unsigned __int16 infoIndex;
	};

	struct entityShared_t
	{
		char isLinked;
		char modelType;
		char svFlags;
		char isInUse;
		Bounds box;
		int contents;
		Bounds absBox;
		float currentOrigin[3];
		float currentAngles[3];
		EntHandle ownerNum;
		int eventTime;
	};

	enum EntHandler
	{
		ENT_HANDLER_NULL = 0x0,
		ENT_HANDLER_TRIGGER_MULTIPLE = 0x1,
		ENT_HANDLER_TRIGGER_HURT = 0x2,
		ENT_HANDLER_TRIGGER_HURT_TOUCH = 0x3,
		ENT_HANDLER_TRIGGER_DAMAGE = 0x4,
		ENT_HANDLER_SCRIPT_MOVER = 0x5,
		ENT_HANDLER_SCRIPT_MODEL = 0x6,
		ENT_HANDLER_GRENADE = 0x7,
		ENT_HANDLER_TIMED_OBJECT = 0x8,
		ENT_HANDLER_ROCKET = 0x9,
		ENT_HANDLER_CLIENT = 0xA,
		ENT_HANDLER_CLIENT_SPECTATOR = 0xB,
		ENT_HANDLER_CLIENT_DEAD = 0xC,
		ENT_HANDLER_PLAYER_CLONE = 0xD,
		ENT_HANDLER_TURRET_INIT = 0xE,
		ENT_HANDLER_TURRET = 0xF,
		ENT_HANDLER_DROPPED_ITEM = 0x10,
		ENT_HANDLER_ITEM_INIT = 0x11,
		ENT_HANDLER_ITEM = 0x12,
		ENT_HANDLER_PRIMARY_LIGHT = 0x13,
		ENT_HANDLER_PLAYER_BLOCK = 0x14,
		ENT_HANDLER_VEHICLE = 0x15,

		ENT_HANDLER_COUNT
	};

	typedef struct gentity_s
	{
		entityState_s s;
		entityShared_t r;
		gclient_t* client; // 344
		void /*Turret*/* turret;
		void /*Vehicle*/* vehicle;
		int physObjId;
		unsigned __int16 model;
		char physicsObject;
		char takedamage;
		char active;
		char handler;
		char team;
		bool freeAfterEvent;
		__int16 padding_short;
		short classname;
		unsigned __int16 script_classname;
		unsigned __int16 script_linkName;
		unsigned __int16 target;
		unsigned __int16 targetname;
		unsigned int attachIgnoreCollision;
		int spawnflags;
		int flags;
		int eventTime;
		int clipmask;
		int processedFrame;
		EntHandle parent;
		int nextthink;
		int health;
		int maxHealth;
		int damage;
		int count;
		EntHandle missileTargetEnt;
		EntHandle remoteControlledOwner;
		gentity_s* tagChildren;
		unsigned __int16 attachModelNames[19];
		unsigned __int16 attachTagNames[19];
		int useCount;
		gentity_s* nextFree;
		int birthTime;
		char pad[100];
	} gentity_t;

#pragma pack(push, 1)
	typedef struct client_s
	{
		clientstate_t state; // 0
		char __pad0[4]; // 4
		int deltaMessage; // 8
		char __pad1[12]; // 12
		netchan_t netchan; // 24
		char __pad2[20]; // 1604
		const char* delayDropReason; // 1624
		char connectInfoString[1024]; // 1628
		char __pad3[132096]; // 2652
		int reliableSequence; // 134748
		int reliableAcknowledge; // 134752
		int reliableSent; // 134756
		int messageAcknowledge; // 134760
		char __pad4[1080]; // 134764
		char name[16]; // 135844
		char __pad5[12]; // 135860
		int snapNum; // 135872
		int __pad6; // 135876
		short ping; // 135880
		char __pad7[133158]; // 135882
		int isBot; // 269040
		int serverID; // 269044
		char __pad8[9224]; // 269048
		unsigned __int64 steamID; // 278272
		char __pad9[403592]; // 278280
	} client_t;
#pragma pack(pop)

	static_assert(sizeof(client_t) == 0xA6790);

	struct CModelAllocData
	{
		void* mainArray;
		void* vertexBuffer;
		void* indexBuffer;
	};

	struct CModelSectionHeader
	{
		int size;
		int offset;
		int fixupStart;
		int fixupCount;
		void* buffer;
	};

	enum CModelSection
	{
		SECTION_MAIN = 0,
		SECTION_INDEX = 1,
		SECTION_VERTEX = 2,
		SECTION_FIXUP = 3,
	};

	struct CModelHeader
	{
		int version;
		unsigned int signature;
		CModelSectionHeader sectionHeader[4];
	};

	typedef struct punctuation_s
	{
		char *p;						//punctuation character(s)
		int n;							//punctuation indication
		struct punctuation_s *next;		//next punctuation
	} punctuation_t;

#define MAX_TOKEN 1024
#define MAX_TOKENLENGTH 1024

	typedef struct token_s
	{
		char string[MAX_TOKEN];			//available token
		int type;						//last read token type
		int subtype;					//last read token sub type
		unsigned long int intvalue;	//integer value
		long double floatvalue;			//floating point value
		char *whitespace_p;				//start of white space before token
		char *endwhitespace_p;			//start of white space before token
		int line;						//line the token was on
		int linescrossed;				//lines crossed in white space
		struct token_s *next;			//next token in chain
	} token_t;

	typedef struct script_s
	{
		char filename[64];				//file name of the script
		char *buffer;					//buffer containing the script
		char *script_p;					//current pointer in the script
		char *end_p;					//pointer to the end of the script
		char *lastscript_p;				//script pointer before reading token
		char *whitespace_p;				//begin of the white space
		char *endwhitespace_p;			//end of the white space
		int length;						//length of the script in bytes
		int line;						//current line in script
		int lastline;					//line before reading token
		int tokenavailable;				//set by UnreadLastToken
		int flags;						//several script flags
		punctuation_t *punctuations;	//the punctuations used in the script
		punctuation_t **punctuationtable;
		token_t token;					//available token
		struct script_s *next;			//next script in a chain
	} script_t;

	typedef struct define_s
	{
		char *name;							//define name
		int flags;							//define flags
		int builtin;						// > 0 if builtin define
		int numparms;						//number of define parameters
		token_t *parms;						//define parameters
		token_t *tokens;					//macro tokens (possibly containing parm tokens)
		struct define_s *next;				//next defined macro in a list
		struct define_s *hashnext;			//next define in the hash chain
	} define_t;

	typedef struct indent_s
	{
		int type;								//indent type
		int skip;								//true if skipping current indent
		script_t *script;						//script the indent was in
		struct indent_s *next;					//next indent on the indent stack
	} indent_t;

	typedef struct source_s
	{
		char filename[64];					//file name of the script
		char includepath[64];					//path to include files
		punctuation_t *punctuations;			//punctuations to use
		script_t *scriptstack;					//stack with scripts of the source
		token_t *tokens;						//tokens to read first
		define_t *defines;						//list with macro definitions
		define_t **definehash;					//hash chain with defines
		indent_t *indentstack;					//stack with indents
		int skip;								// > 0 if skipping conditional code
		token_t token;							//last read token
	} source_t;

	typedef struct pc_token_s
	{
		int type;
		int subtype;
		int intvalue;
		float floatvalue;
		char string[MAX_TOKENLENGTH];
	} pc_token_t;

	typedef struct keywordHash_s
	{
		char *keyword;
		bool(*func)(menuDef_t *item, int handle);
		//struct keywordHash_s *next;
	} keywordHash_t;

	enum MaterialTechniqueType
	{
		TECHNIQUE_DEPTH_PREPASS = 0x0,
		TECHNIQUE_BUILD_FLOAT_Z = 0x1,
		TECHNIQUE_BUILD_SHADOWMAP_DEPTH = 0x2,
		TECHNIQUE_BUILD_SHADOWMAP_COLOR = 0x3,
		TECHNIQUE_UNLIT = 0x4,
		TECHNIQUE_EMISSIVE = 0x5,
		TECHNIQUE_EMISSIVE_DFOG = 0x6,
		TECHNIQUE_EMISSIVE_SHADOW = 0x7,
		TECHNIQUE_EMISSIVE_SHADOW_DFOG = 0x8,
		TECHNIQUE_LIT_BEGIN = 0x9,
		TECHNIQUE_LIT = 0x9,
		TECHNIQUE_LIT_DFOG = 0xA,
		TECHNIQUE_LIT_SUN = 0xB,
		TECHNIQUE_LIT_SUN_DFOG = 0xC,
		TECHNIQUE_LIT_SUN_SHADOW = 0xD,
		TECHNIQUE_LIT_SUN_SHADOW_DFOG = 0xE,
		TECHNIQUE_LIT_SPOT = 0xF,
		TECHNIQUE_LIT_SPOT_DFOG = 0x10,
		TECHNIQUE_LIT_SPOT_SHADOW = 0x11,
		TECHNIQUE_LIT_SPOT_SHADOW_DFOG = 0x12,
		TECHNIQUE_LIT_OMNI = 0x13,
		TECHNIQUE_LIT_OMNI_DFOG = 0x14,
		TECHNIQUE_LIT_OMNI_SHADOW = 0x15,
		TECHNIQUE_LIT_OMNI_SHADOW_DFOG = 0x16,
		TECHNIQUE_LIT_INSTANCED = 0x17,
		TECHNIQUE_LIT_INSTANCED_DFOG = 0x18,
		TECHNIQUE_LIT_INSTANCED_SUN = 0x19,
		TECHNIQUE_LIT_INSTANCED_SUN_DFOG = 0x1A,
		TECHNIQUE_LIT_INSTANCED_SUN_SHADOW = 0x1B,
		TECHNIQUE_LIT_INSTANCED_SUN_SHADOW_DFOG = 0x1C,
		TECHNIQUE_LIT_INSTANCED_SPOT = 0x1D,
		TECHNIQUE_LIT_INSTANCED_SPOT_DFOG = 0x1E,
		TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW = 0x1F,
		TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW_DFOG = 0x20,
		TECHNIQUE_LIT_INSTANCED_OMNI = 0x21,
		TECHNIQUE_LIT_INSTANCED_OMNI_DFOG = 0x22,
		TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW = 0x23,
		TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW_DFOG = 0x24,
		TECHNIQUE_LIT_END = 0x25,
		TECHNIQUE_LIGHT_SPOT = 0x25,
		TECHNIQUE_LIGHT_OMNI = 0x26,
		TECHNIQUE_LIGHT_SPOT_SHADOW = 0x27,
		TECHNIQUE_FAKELIGHT_NORMAL = 0x28,
		TECHNIQUE_FAKELIGHT_VIEW = 0x29,
		TECHNIQUE_SUNLIGHT_PREVIEW = 0x2A,
		TECHNIQUE_CASE_TEXTURE = 0x2B,
		TECHNIQUE_WIREFRAME_SOLID = 0x2C,
		TECHNIQUE_WIREFRAME_SHADED = 0x2D,
		TECHNIQUE_DEBUG_BUMPMAP = 0x2E,
		TECHNIQUE_DEBUG_BUMPMAP_INSTANCED = 0x2F,
		TECHNIQUE_COUNT = 0x30,
		TECHNIQUE_TOTAL_COUNT = 0x31,
		TECHNIQUE_NONE = 0x32,
	};

	enum MaterialVertexDeclType
	{
		VERTDECL_GENERIC = 0x0,
		VERTDECL_PACKED = 0x1,
		VERTDECL_WORLD = 0x2,
		VERTDECL_WORLD_T1N0 = 0x3,
		VERTDECL_WORLD_T1N1 = 0x4,
		VERTDECL_WORLD_T2N0 = 0x5,
		VERTDECL_WORLD_T2N1 = 0x6,
		VERTDECL_WORLD_T2N2 = 0x7,
		VERTDECL_WORLD_T3N0 = 0x8,
		VERTDECL_WORLD_T3N1 = 0x9,
		VERTDECL_WORLD_T3N2 = 0xA,
		VERTDECL_WORLD_T4N0 = 0xB,
		VERTDECL_WORLD_T4N1 = 0xC,
		VERTDECL_WORLD_T4N2 = 0xD,
		VERTDECL_POS_TEX = 0xE,
		VERTDECL_STATICMODELCACHE = 0xF,
		VERTDECL_COUNT = 0x10,
	};

	struct $A6DFE8F2BEFD3E7315B44D22E582538B
	{
		unsigned int stride;
		IDirect3DVertexBuffer9 *vb;
		unsigned int offset;
	};

	struct GfxCmdBufPrimState
	{
		IDirect3DDevice9 *device;
		IDirect3DIndexBuffer9 *indexBuffer;
		MaterialVertexDeclType vertDeclType;
		$A6DFE8F2BEFD3E7315B44D22E582538B streams[2];
		IDirect3DVertexDeclaration9 *vertexDecl;
	};

	enum GfxDepthRangeType
	{
		GFX_DEPTH_RANGE_SCENE = 0x0,
		GFX_DEPTH_RANGE_VIEWMODEL = 0x2,
		GFX_DEPTH_RANGE_FULL = 0xFFFFFFFF,
	};

	struct GfxViewport
	{
		int x;
		int y;
		int width;
		int height;
	};

	enum GfxRenderTargetId
	{
		R_RENDERTARGET_SAVED_SCREEN = 0x0,
		R_RENDERTARGET_FRAME_BUFFER = 0x1,
		R_RENDERTARGET_SCENE = 0x2,
		R_RENDERTARGET_RESOLVED_POST_SUN = 0x3,
		R_RENDERTARGET_RESOLVED_SCENE = 0x4,
		R_RENDERTARGET_FLOAT_Z = 0x5,
		R_RENDERTARGET_PINGPONG_0 = 0x6,
		R_RENDERTARGET_PINGPONG_1 = 0x7,
		R_RENDERTARGET_POST_EFFECT_0 = 0x8,
		R_RENDERTARGET_POST_EFFECT_1 = 0x9,
		R_RENDERTARGET_SHADOWMAP_LARGE = 0xA,
		R_RENDERTARGET_SHADOWMAP_SMALL = 0xB,
		R_RENDERTARGET_COUNT = 0xC,
		R_RENDERTARGET_NONE = 0xD,
	};

	struct /*__declspec(align(16))*/ GfxCmdBufState
	{
		char refSamplerState[16];
		unsigned int samplerState[16];
		GfxTexture *samplerTexture[16];
		GfxCmdBufPrimState prim;
		Material *material;
		MaterialTechniqueType techType;
		MaterialTechnique *technique;
		MaterialPass *pass;
		unsigned int passIndex;
		GfxDepthRangeType depthRangeType;
		float depthRangeNear;
		float depthRangeFar;
		unsigned __int64 vertexShaderConstState[64];
		unsigned __int64 pixelShaderConstState[256];
		char alphaRef;
		unsigned int refStateBits[2];
		unsigned int activeStateBits[2];
		MaterialPixelShader *pixelShader;
		MaterialVertexShader *vertexShader;
		unsigned int scissorX;
		unsigned int scissorY;
		unsigned int scissorW;
		unsigned int scissorH;
		unsigned int pixPrimarySortKey;
		unsigned int pixSceneLightIndex;
		Material *pixMaterial;
		MaterialTechnique *pixTechnique;
		int pixCombine;
		GfxViewport viewport;
		GfxRenderTargetId renderTargetId;
		Material *origMaterial;
		MaterialTechniqueType origTechType;
	};

	struct GfxCmdBufContext
	{
		/*GfxCmdBufSourceState*/ void *source;
		GfxCmdBufState *state;
	};

	struct GfxDrawGroupSetupFields
	{
		unsigned __int16 materialSortedIndex : 15;
		unsigned __int16 useHeroLighting : 1;
		char sceneLightIndex;
		char surfType;
	};

	union GfxDrawGroupSetup
	{
		GfxDrawGroupSetupFields fields;
		unsigned int packed;
	};

	struct GfxMarkSurfLightingFields
	{
		char lmapIndex;
		char reflectionProbeIndex;
		unsigned __int16 modelIndex;
	};

	union GfxMarkSurfLighting
	{
		GfxMarkSurfLightingFields fields;
		unsigned int packed;
	};

	struct GfxMarkSurf
	{
		GfxDrawGroupSetup drawGroup;
		unsigned __int16* indices;
		unsigned __int16 triCount;
		char modelType;
		char pad;
		GfxMarkSurfLighting lighting;
	};

	struct GfxCodeSurf
	{
		GfxDrawGroupSetup drawGroup;
		unsigned int triCount;
		unsigned __int16* indices;
		unsigned __int16 argOffset;
		unsigned __int16 argCount;
	};

	struct __declspec(align(4)) GfxGlassSurf
	{
		GfxDrawGroupSetup drawGroup;
		char pad;
		char reflectionProbeIndex;
		unsigned __int16 triCount;
		unsigned __int16* indices;
		unsigned __int16 lightingHandle;
	};

	struct GfxCloudSurfFields
	{
		unsigned __int16 materialSortedIndex;
		char cloudDataIndex;
		char surfType;
	};

	union GfxCloudSurf
	{
		GfxCloudSurfFields fields;
		unsigned int packed;
	};

	struct GfxSparkSurfFields
	{
		unsigned __int16 materialSortedIndex;
		unsigned __int16 sparkDataIndex;
	};

	union GfxSparkSurf
	{
		GfxSparkSurfFields fields;
		unsigned int packed;
	};

	struct GfxSceneDef
	{
		int time;
		float floatTime;
		float viewOffset[3];
		GfxImage* sunShadowImage;
		float sunShadowPixelAdjust[4];
	};

	struct GfxLight
	{
		char type;
		char canUseShadowMap;
		char unused[2];
		float color[3];
		float dir[3];
		float origin[3];
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		int exponent;
		unsigned int spotShadowIndex;
		GfxLightDef* def;
	};

	struct GfxVisibleLight
	{
		char pad[0x2004];
	};

	struct GfxEntity
	{
		unsigned int renderFxFlags;
		float materialTime;
	};

	struct GfxSkinnedXModelSurfs
	{
		void* firstSurf;
	};

	struct GfxSceneEntityCull
	{
		volatile unsigned int state;
		Bounds bounds;
		GfxSkinnedXModelSurfs skinnedSurfs;
	};

	union GfxSceneEntityInfo
	{
		void/*cpose_t*/* pose;
		unsigned __int16* cachedLightingHandle;
	};

	struct DSkelPartBits
	{
		int anim[6];
		int control[6];
		int worldCtrl[6];
		int skel[6];
	};

	struct DSkel
	{
		DSkelPartBits partBits;
		int timeStamp;
		/*DObjAnimMat*/void* mat;
	};

	struct DObj
	{
		/*XAnimTree_s*/ void* tree;
		unsigned __int16 duplicateParts;
		unsigned __int16 entnum;
		char duplicatePartsSize;
		char numModels;
		char numBones;
		char flags;
		unsigned int ignoreCollision;
		volatile int locked;
		DSkel skel;
		float radius;
		unsigned int hidePartBits[6];
		XModel** models;
	};

	struct GfxSceneEntity
	{
		float lightingOrigin[3];
		GfxPlacement placement;
		GfxSceneEntityCull cull;
		char lods[32];
		unsigned __int32 gfxEntIndex : 7;
		unsigned __int32 entnum : 12;
		unsigned __int32 renderFxFlags : 13;
		DObj* obj;
		GfxSceneEntityInfo info;
		char reflectionProbeIndex;
	};

	struct GfxScaledPlacement
	{
		GfxPlacement base;
		float scale;
	};

	struct GfxSceneModel
	{
		XModelDrawInfo info;
		XModel* model;
		DObj* obj;
		GfxScaledPlacement placement;
		unsigned __int32 gfxEntIndex : 7;
		unsigned __int32 entnum : 12;
		unsigned __int32 renderFxFlags : 13;
		float radius;
		unsigned __int16* cachedLightingHandle;
		float lightingOrigin[3];
		char reflectionProbeIndex;
		char lod;
	};

	struct __declspec(align(4)) GfxSceneBrush
	{
		BModelDrawInfo info;
		unsigned __int16 entnum;
		GfxBrushModel* bmodel;
		GfxPlacement placement;
		char reflectionProbeIndex;
	};

	union GfxSceneGlass
	{
		struct
		{
			bool rendered;
			char reflectionProbeIndex;
			unsigned __int16 lightingHandle;
		};
		unsigned int packed;
	};

	union GfxEntCellRefInfo
	{
		float radius;
		GfxBrushModel* bmodel;
	};

	struct GfxSceneDpvs
	{
		unsigned int localClientNum;
		char* entVisData[7];
		unsigned __int16* sceneXModelIndex;
		unsigned __int16* sceneDObjIndex;
		GfxEntCellRefInfo* entInfo[4];
	};

	struct __declspec(align(64)) GfxScene
	{
		GfxCodeSurf codeEmissiveSurfs[2048];
		GfxCodeSurf codeTransSurfs[640];
		GfxMarkSurf markSurfs[1536];
		GfxGlassSurf glassSurfs[768];
		GfxCloudSurf cloudSurfs[256];
		GfxDrawSurf drawSurfsDepthHack[32];
		GfxDrawSurf drawSurfsLitOpaque[8192];
		GfxDrawSurf drawSurfsLitTrans[2048];
		GfxDrawSurf drawSurfsEmissive[8192];
		GfxDrawSurf drawSurfsSunShadow0[4096];
		GfxDrawSurf drawSurfsSunShadow1[8192];
		GfxDrawSurf drawSurfsSpotShadow0[896];
		GfxDrawSurf drawSurfsSpotShadow1[896];
		GfxDrawSurf drawSurfsSpotShadow2[896];
		GfxDrawSurf drawSurfsSpotShadow3[896];
		unsigned int sceneLightIsUsed[32];
		unsigned int cachedSceneLightIsUsed[4][32];
		GfxSparkSurf sparkSurfs[64];
		unsigned int drawSurfLimit[10];
		volatile int drawSurfCount[10];
		GfxDrawSurf* drawSurfs[10];
		volatile int codeSurfUser[2];
		volatile int markMeshGuard;
		unsigned int codeEmissiveSurfCount;
		unsigned int codeTransSurfCount;
		unsigned int markSurfCount;
		unsigned int glassSurfCount;
		GfxSceneDef def;
		unsigned int addedLightCount;
		GfxLight addedLight[32];
		bool isAddedLightCulled[32];
		float dynamicSpotLightNearPlaneOffset;
		float dynamicSpotLightLength;
		GfxVisibleLight visLight[4];
		GfxVisibleLight visLightShadow[1];
		unsigned int* entOverflowedDrawBuf;
		volatile int gfxEntCount;
		GfxEntity gfxEnts[128];
		int sceneDObjCount;
		int preClientSceneDObjCount;
		int sceneDObjCountAtMark;
		GfxSceneEntity sceneDObj[520];
		char sceneDObjVisData[7][512];
		int sceneDObjMarkableViewmodelIndex;
		unsigned int sceneDObjFirstViewmodelIndex;
		unsigned int sceneDObjViewmodelCount;
		volatile int sceneModelCount;
		int sceneModelCountAtMark;
		int sceneDObjModelCount;
		GfxSceneModel sceneModel[1024];
		char sceneModelVisData[7][1024];
		volatile int sceneBrushCount;
		int sceneBrushCountAtMark;
		GfxSceneBrush sceneBrush[512];
		char sceneBrushVisData[3][512];
		GfxSceneGlass sceneGlass[1024];
		unsigned int sceneDynModelCount;
		unsigned int sceneDynBrushCount;
		int gfxEntCountAtMark;
		GfxSceneDpvs dpvs;
		int updateSound;
		int allowAddDObj;
	};

#pragma endregion

#ifndef IDA
}
#endif
