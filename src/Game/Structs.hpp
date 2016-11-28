#define PROTOCOL 0x92

// This allows us to compile our structures in IDA, for easier reversing :3
#ifdef __cplusplus
namespace Game
{
#endif

	typedef float vec_t;
	typedef vec_t vec2_t[2];
	typedef vec_t vec3_t[3];
	typedef vec_t vec4_t[4];

	typedef enum
	{
		ASSET_TYPE_PHYSPRESET = 0,
		ASSET_TYPE_PHYS_COLLMAP = 1,
		ASSET_TYPE_XANIM = 2,
		ASSET_TYPE_XMODELSURFS = 3,
		ASSET_TYPE_XMODEL = 4,
		ASSET_TYPE_MATERIAL = 5,
		ASSET_TYPE_PIXELSHADER = 6,
		ASSET_TYPE_VERTEXSHADER = 7,
		ASSET_TYPE_VERTEXDECL = 8,
		ASSET_TYPE_TECHSET = 9,
		ASSET_TYPE_IMAGE = 10,
		ASSET_TYPE_SOUND = 11,
		ASSET_TYPE_SNDCURVE = 12,
		ASSET_TYPE_LOADED_SOUND = 13,
		ASSET_TYPE_COL_MAP_SP = 14,
		ASSET_TYPE_COL_MAP_MP = 15,
		ASSET_TYPE_COM_MAP = 16,
		ASSET_TYPE_GAME_MAP_SP = 17,
		ASSET_TYPE_GAME_MAP_MP = 18,
		ASSET_TYPE_MAP_ENTS = 19,
		ASSET_TYPE_FX_MAP = 20,
		ASSET_TYPE_GFX_MAP = 21,
		ASSET_TYPE_LIGHTDEF = 22,
		ASSET_TYPE_UI_MAP = 23,
		ASSET_TYPE_FONT = 24,
		ASSET_TYPE_MENUFILE = 25,
		ASSET_TYPE_MENU = 26,
		ASSET_TYPE_LOCALIZE = 27,
		ASSET_TYPE_WEAPON = 28,
		ASSET_TYPE_SNDDRIVERGLOBALS = 29,
		ASSET_TYPE_FX = 30,
		ASSET_TYPE_IMPACTFX = 31,
		ASSET_TYPE_AITYPE = 32,
		ASSET_TYPE_MPTYPE = 33,
		ASSET_TYPE_CHARACTER = 34,
		ASSET_TYPE_XMODELALIAS = 35,
		ASSET_TYPE_RAWFILE = 36,
		ASSET_TYPE_STRINGTABLE = 37,
		ASSET_TYPE_LEADERBOARDDEF = 38,
		ASSET_TYPE_STRUCTUREDDATADEF = 39,
		ASSET_TYPE_TRACER = 40,
		ASSET_TYPE_VEHICLE = 41,
		ASSET_TYPE_ADDON_MAP_ENTS = 42,

		ASSET_TYPE_COUNT,
		ASSET_TYPE_INVALID = -1,
	} XAssetType;

	typedef enum
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

	typedef enum
	{
		DVAR_TYPE_BOOL = 0,
		DVAR_TYPE_FLOAT = 1,
		DVAR_TYPE_FLOAT_2 = 2,
		DVAR_TYPE_FLOAT_3 = 3,
		DVAR_TYPE_FLOAT_4 = 4,
		DVAR_TYPE_INT = 5,
		DVAR_TYPE_ENUM = 6,
		DVAR_TYPE_STRING = 7,
		DVAR_TYPE_COLOR = 8,
		//DVAR_TYPE_INT64	= 9 only in Tx
	} dvar_type;
	// 67/72 bytes figured out
	union dvar_value_t {
		char*	string;
		int		integer;
		float	value;
		bool	boolean;
		float	vec2[2];
		float	vec3[3];
		float	vec4[4];
		unsigned char	color[4]; //to get float: multiply by 0.003921568859368563 - BaberZz
		//__int64 integer64; only in Tx
	};
	union dvar_maxmin_t {
		int i;
		float f;
	};
	typedef struct dvar_t
	{
		//startbyte:endbyte
		const char* name; //0:3
		const char*	description; //4:7
		unsigned int flags; //8:11
		char type; //12:12
		bool modified; //13:15
		dvar_value_t current; //16:31
		dvar_value_t latched; //32:47
		dvar_value_t _default; //48:64
		dvar_maxmin_t min; //65:67
		dvar_maxmin_t max; //68:72 woooo
	} dvar_t;

	typedef struct cmd_function_s
	{
		cmd_function_s *next;
		const char *name;
		const char *autoCompleteDir;
		const char *autoCompleteExt;
		void(__cdecl *function)();
		bool isKey; // Looks like this is true when the command is a key/button
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

	typedef struct
	{
		char type;
		char pad[3];
		const char* folder;
		const char* file;
	} StreamFile;

	typedef struct
	{
		char pad[20];
		StreamFile* stream;
		char pad2[76];
	} snd_alias_t;

	typedef struct
	{
		const char* name;
		snd_alias_t* aliases;
		int numAliases;
	} snd_alias_list_t;

	typedef struct
	{
		const char *name;
		int allocFlags;
		int freeFlags;
	} XZoneInfo;

	struct expression_s;
	struct statement_s;
	struct menuDef_t;
	enum operationEnum;

	enum IWI_COMPRESSION
	{
		IWI_INVALID = 0x0,
		IWI_ARGB = 0x1,
		IWI_RGB8 = 0x2,
		IWI_DXT1 = 0xB,
		IWI_DXT3 = 0xC,
		IWI_DXT5 = 0xD,
	};

	struct GfxImageFileHeader
	{
		char tag[3];
		char version;
		int flags;
		char format;
		short dimensions[3];
		int fileSizeForPicmip[4];
	};

	struct GfxImageLoadDef // actually a IDirect3DTexture* but this is easier
	{
		char mipLevels;
		char flags;
		short dimensions[3];
		int format; // usually the compression Magic
		int dataSize; // set to zero to load from IWD
		char data[1]; 
	};

	struct GfxImage
	{
		union 
		{
			GfxImageLoadDef* loadDef;
#ifdef __cplusplus
			IDirect3DTexture9* texture;
#endif
		};
		
		char mapType; // 5 is cube, 4 is 3d, 3 is 2d
		char semantic;
		char category;
		char flags;
		int cardMemory;
		int dataLen1;
		int dataLen2;
		short height;
		short width;
		short depth;
		char loaded;
		char pad;
		char* name;
	};

	struct water_t
	{
		float floatTime;
		float *H0X;		// Count = M * N
		float *H0Y;		// Count = M * N
		//float *wTerm;		// Count = M * N
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

	#define SEMANTIC_WATER_MAP 11

	union MaterialTextureDefInfo
	{
		GfxImage *image;	// MaterialTextureDef->semantic != SEMANTIC_WATER_MAP
		water_t *water;		// MaterialTextureDef->semantic == SEMANTIC_WATER_MAP
	};

	struct MaterialTextureDef
	{
		unsigned int nameHash;
		char nameStart;
		char nameEnd;
		char sampleState;
		char semantic;
		MaterialTextureDefInfo info;
	};

	struct MaterialShaderArgument
	{
		short type;
		short dest;
		short paramID;
		short more;
	};

	struct MaterialVertexDeclaration
	{
		const char* name;
		int unknown;
		char pad[28];
		/*IDirect3DVertexDeclaration9**/void* declarations[16];
	};

	struct GfxPixelShaderLoadDef
	{
		char *cachedPart;
		char *physicalPart;
		unsigned __int16 cachedPartSize;
		unsigned __int16 physicalPartSize;
	};

	struct MaterialPixelShader
	{
		const char* name;
		GfxPixelShaderLoadDef loadDef;
	};

	struct GfxVertexShaderLoadDef
	{
		char *cachedPart;
		char *physicalPart;
		unsigned __int16 cachedPartSize;
		unsigned __int16 physicalPartSize;
	};

	struct MaterialVertexShader
	{
		const char* name;
		GfxVertexShaderLoadDef loadDef;
	};

	struct MaterialPass
	{
		MaterialVertexDeclaration* vertexDecl;
		MaterialVertexShader* vertexShader;
		MaterialPixelShader* pixelShader;
		char argCount1;
		char argCount2;
		char argCount3;
		char unk;
		MaterialShaderArgument* argumentDef;
	};

	struct MaterialTechnique
	{
		char* name;
		short pad2;
		short numPasses;
		MaterialPass passes[1];
	};

	struct MaterialTechniqueSet
	{
		const char* name;
		char pad[4];
		MaterialTechniqueSet* remappedTechniques;
		MaterialTechnique* techniques[48];
	};

	struct MaterialConstantDef
	{
		int nameHash;
		char name[12];
		vec4_t literal;
	};

	struct Material
	{
		const char *name;
		char gameFlags;
		char sortKey;
		char textureAtlasRowCount;
		char textureAtlasColumnCount;
		char drawSurf[12];
		int surfaceTypeBits;
		char stateBitsEntry[48];
		char textureCount;
		char constantCount;
		char stateBitsCount;
		char stateFlags;
		char cameraRegion;
		MaterialTechniqueSet *techniqueSet;
		MaterialTextureDef *textureTable;
		MaterialConstantDef *constantTable;
		void *stateBitTable;
	};

	struct TracerDef
	{
		const char * name;
		Material * material;
		unsigned int drawInterval;
		float speed;
		float beamLength;
		float beamWidth;
		float screwRadius;
		float screwDist;
		float colors[5][4];
	};

	struct keyname_t
	{
		const char *name;
		int keynum;
	};

	struct ItemFloatExpressionEntry
	{
		int target;
		const char *s1;
		const char *s2;
	};

	// Taken from menudefinition.h
	enum itemTextStyle
	{
		ITEM_TEXTSTYLE_NORMAL            = 0,   // normal text
		ITEM_TEXTSTYLE_SHADOWED          = 3,   // drop shadow ( need a color for this )
		ITEM_TEXTSTYLE_SHADOWEDMORE      = 6,   // drop shadow ( need a color for this )
		ITEM_TEXTSTYLE_BORDERED          = 7,   // border (stroke)
		ITEM_TEXTSTYLE_BORDEREDMORE      = 8,   // more border :P
		ITEM_TEXTSTYLE_MONOSPACE         = 128,
		ITEM_TEXTSTYLE_MONOSPACESHADOWED = 132,		
	};

#define ITEM_TYPE_TEXT				0		// simple text
#define ITEM_TYPE_BUTTON			1		// button, basically text with a border
#define ITEM_TYPE_RADIOBUTTON		2		// toggle button, may be grouped
#define ITEM_TYPE_CHECKBOX			3		// check box
#define ITEM_TYPE_EDITFIELD 		4		// editable text, associated with a dvar
#define ITEM_TYPE_COMBO 			5		// drop down list
#define ITEM_TYPE_LISTBOX			6		// scrollable list
#define ITEM_TYPE_MODEL 			7		// model
#define ITEM_TYPE_OWNERDRAW 		8		// owner draw, name specs what it is
#define ITEM_TYPE_NUMERICFIELD		9		// editable text, associated with a dvar
#define ITEM_TYPE_SLIDER			10		// mouse speed, volume, etc.
#define ITEM_TYPE_YESNO 			11		// yes no dvar setting
#define ITEM_TYPE_MULTI 			12		// multiple list setting, enumerated
#define ITEM_TYPE_DVARENUM 			13		// multiple list setting, enumerated from a dvar
#define ITEM_TYPE_BIND				14		// bind
#define ITEM_TYPE_MENUMODEL 		15		// special menu model
#define ITEM_TYPE_VALIDFILEFIELD	16		// text must be valid for use in a dos filename
#define ITEM_TYPE_DECIMALFIELD		17		// editable text, associated with a dvar, which allows decimal input
#define ITEM_TYPE_UPREDITFIELD		18		// editable text, associated with a dvar
#define ITEM_TYPE_GAME_MESSAGE_WINDOW 19	// game message window
#define ITEM_TYPE_NEWSTICKER		20		// horizontal scrollbox
#define ITEM_TYPE_TEXTSCROLL		21		// vertical scrollbox
#define ITEM_TYPE_EMAILFIELD		22
#define ITEM_TYPE_PASSWORDFIELD		23

	struct MenuEventHandlerSet;
	struct Statement_s;

	struct UIFunctionList
	{
		int totalFunctions;
		Statement_s **functions;
	};

	struct StaticDvar
	{
		/*dvar_t*/
		void *dvar;
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

	enum expDataType : int
	{
		VAL_INT = 0x0,
		VAL_FLOAT = 0x1,
		VAL_STRING = 0x2,
		VAL_FUNCTION = 0x3,
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

	union entryInternalData
	{
		//operationEnum op;
		Operand operand;
	};

	/* expressionEntry->type */
#define OPERATOR	0
#define OPERAND		1

	struct expressionEntry	// 0xC
	{
		int type;
		entryInternalData data;
	};

	struct Statement_s	// 0x18
	{
		int numEntries;
		expressionEntry *entries;
		ExpressionSupportingData *supportingData;
		char unknown[0xC];	// ?
	};

	struct SetLocalVarData
	{
		const char *localVarName;
		Statement_s *expression;
	};

	struct ConditionalScript
	{
		MenuEventHandlerSet *eventHandlerSet;
		Statement_s *eventExpression;  // loads this first
	};

	union EventData
	{
		const char *unconditionalScript;
		ConditionalScript *conditionalScript;
		MenuEventHandlerSet *elseScript;
		SetLocalVarData *setLocalVarData;
	};

	enum EventType
	{
		EVENT_UNCONDITIONAL = 0x0,
		EVENT_IF = 0x1,
		EVENT_ELSE = 0x2,
		EVENT_SET_LOCAL_VAR_BOOL = 0x3,
		EVENT_SET_LOCAL_VAR_INT = 0x4,
		EVENT_SET_LOCAL_VAR_FLOAT = 0x5,
		EVENT_SET_LOCAL_VAR_STRING = 0x6,
		EVENT_COUNT = 0x7,
	};

	struct MenuEventHandler
	{
		EventData eventData;
		EventType eventType;
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

#pragma pack(push, 4)
	struct rectDef_s
	{
		float x;
		float y;
		float w;
		float h;
		char horzAlign;
		char vertAlign;
	};
#pragma pack(pop)

	/* windowDef_t->dynamicFlags */
	// 0x1
#define WINDOWDYNAMIC_HASFOCUS		0x00000002
#define WINDOWDYNAMIC_VISIBLE		0x00000004
#define WINDOWDYNAMIC_FADEOUT		0x00000010
#define WINDOWDYNAMIC_FADEIN		0x00000020
	// 0x40
	// 0x80
#define WINDOWDYNAMIC_CLOSED		0x00000800
	// 0x2000
#define WINDOWDYNAMIC_BACKCOLOR		0x00008000
#define WINDOWDYNAMIC_FORECOLOR		0x00010000

	/* windowDef_t->staticFlags */
#define WINDOWSTATIC_DECORATION				0x00100000
#define WINDOWSTATIC_HORIZONTALSCROLL			0x00200000
#define WINDOWSTATIC_SCREENSPACE				0x00400000
#define WINDOWSTATIC_AUTOWRAPPED				0x00800000
#define WINDOWSTATIC_POPUP						0x01000000
#define WINDOWSTATIC_OUTOFBOUNDSCLICK			0x02000000
#define WINDOWSTATIC_LEGACYSPLITSCREENSCALE	0x04000000
#define WINDOWSTATIC_HIDDENDURINGFLASH			0x10000000
#define WINDOWSTATIC_HIDDENDURINGSCOPE			0x20000000
#define WINDOWSTATIC_HIDDENDURINGUI			0x40000000
#define WINDOWSTATIC_TEXTONLYFOCUS				0x80000000

	struct windowDef_t // 0xA4
	{
		const char *name;	// 0x00
		rectDef_s rect;
		rectDef_s rectClient;
		char *group;		// 0x2C
		int style;			// 0x30
		int border;			// 0x34
		int ownerDraw;		// 0x38
		int ownerDrawFlags;	// 0x3C
		float borderSize;	// 0x40
		int staticFlags;	// 0x44
		int dynamicFlags;	// 0x48
		int nextTime;		// 0x4C
		float foreColor[4];	// 0x50
		float backColor[4];	// 0x60
		float borderColor[4];// 0x70
		float outlineColor[4];// 0x80
		float disableColor[4];// 0x90
		Material *background;	// 0xA0
	};

	enum ItemFloatExpressionTarget
	{
		ITEM_FLOATEXP_TGT_RECT_X = 0x0,
		ITEM_FLOATEXP_TGT_RECT_Y = 0x1,
		ITEM_FLOATEXP_TGT_RECT_W = 0x2,
		ITEM_FLOATEXP_TGT_RECT_H = 0x3,
		ITEM_FLOATEXP_TGT_FORECOLOR_R = 0x4,
		ITEM_FLOATEXP_TGT_FORECOLOR_G = 0x5,
		ITEM_FLOATEXP_TGT_FORECOLOR_B = 0x6,
		ITEM_FLOATEXP_TGT_FORECOLOR_RGB = 0x7,
		ITEM_FLOATEXP_TGT_FORECOLOR_A = 0x8,
		ITEM_FLOATEXP_TGT_GLOWCOLOR_R = 0x9,
		ITEM_FLOATEXP_TGT_GLOWCOLOR_G = 0xA,
		ITEM_FLOATEXP_TGT_GLOWCOLOR_B = 0xB,
		ITEM_FLOATEXP_TGT_GLOWCOLOR_RGB = 0xC,
		ITEM_FLOATEXP_TGT_GLOWCOLOR_A = 0xD,
		ITEM_FLOATEXP_TGT_BACKCOLOR_R = 0xE,
		ITEM_FLOATEXP_TGT_BACKCOLOR_G = 0xF,
		ITEM_FLOATEXP_TGT_BACKCOLOR_B = 0x10,
		ITEM_FLOATEXP_TGT_BACKCOLOR_RGB = 0x11,
		ITEM_FLOATEXP_TGT_BACKCOLOR_A = 0x12,
		ITEM_FLOATEXP_TGT__COUNT = 0x13,
	};

	struct ItemFloatExpression
	{
		ItemFloatExpressionTarget target;
		Statement_s *expression;
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

	struct multiDef_s	// 0x188
	{
		const char *dvarList[32];
		const char *dvarStr[32];
		float dvarValue[32];
		int count;
		int strDef;
	};

	struct columnInfo_s
	{
		int xpos;
		int width;
		int maxChars;
		int alignment;
	};

	struct listBoxDef_s	// 0x144
	{
		// somethings not right here
		int startPos[2];
		int endPos[2];
		float elementWidth;
		float elementHeight;
		int elementStyle;
		int numColumns;
		columnInfo_s columnInfo[16];
		MenuEventHandlerSet *doubleClick;	// 0xC8
		int notselectable;
		int noscrollbars;
		int usepaging;
		float selectBorder[4];
		Material *selectIcon;
	};

	struct newsTickerDef_s
	{
		int feedId;
		int speed;
		int spacing;
	};

	struct textScrollDef_s
	{
		int startTime;
	};

	union itemDefData_t
	{
		listBoxDef_s *listBox;
		editFieldDef_s *editField;
		newsTickerDef_s *ticker;
		multiDef_s *multiDef;
		const char *enumDvarName;
		textScrollDef_s *scroll;
		void *data;
	};

	struct itemDef_t
	{
		windowDef_t window;
		rectDef_s textRect;
		int type;
		int dataType;
		int alignment;
		int fontEnum;
		int textAlignMode;
		float textAlignX;
		float textAlignY;
		float textScale;
		int textStyle;
		int gameMsgWindowIndex;
		int gameMsgWindowMode;
		const char *text;
		int textSaveGameInfo;
		int parent;
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
		const char *focusSound;
		float special;
		int cursorPos;
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

	struct menuTransition	// 0x18
	{
		int transitionType;
		int startTime;
		float startVal;
		float endVal;
		float time;
		int endTriggerType;
	};

	struct menuDef_t
	{
		windowDef_t window;
		int font;
		int fullscreen;
		int itemCount;
		int fontIndex;
		int cursorItems;
		int fadeCycle;
		float fadeClamp;
		float fadeAmount;
		float fadeInAmount;
		float blurRadius;
		MenuEventHandlerSet *onOpen;
		MenuEventHandlerSet *onRequestClose;
		MenuEventHandlerSet *onClose;
		MenuEventHandlerSet *onEsc;
		ItemKeyHandler *onKey;
		Statement_s *visibleExp;
		const char *allowedBinding;
		const char *soundLoop;
		int imageTrack;
		float focusColor[4];
		Statement_s *rectXExp;
		Statement_s *rectYExp;
		Statement_s *rectHExp;
		Statement_s *rectWExp;
		Statement_s *openSoundExp;
		Statement_s *closeSoundExp;
		itemDef_t **items;
		char unknown[112];
		ExpressionSupportingData *expressionData;
	};

	struct MenuList
	{
		char *name;
		int menuCount;
		menuDef_t **menus;
	};

#define FS_SEEK_CUR 0
#define FS_SEEK_END 1
#define FS_SEEK_SET 2

	enum FsListBehavior_e
	{
		FS_LIST_PURE_ONLY = 0x0,
		FS_LIST_ALL = 0x1,
	};

	typedef enum 
	{
		NA_BOT,
		NA_BAD,					// an address lookup failed
		NA_LOOPBACK,
		NA_BROADCAST,
		NA_IP,
		NA_IP6, // custom type
	} netadrtype_t;

	typedef enum 
	{
		NS_CLIENT,
		NS_SERVER
	} netsrc_t;

	typedef union
	{
		unsigned char bytes[4];
		DWORD full;
	} netIP_t;

	typedef struct 
	{
		netadrtype_t type;
		netIP_t ip;
		unsigned short port;
		unsigned char ipx[8];
	} netadr_t;

	typedef struct
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
	} msg_t;

	enum playerFlag
	{
		PLAYER_FLAG_NOCLIP = 1,
		PLAYER_FLAG_UFO = 2,
		PLAYER_FLAG_FROZEN = 4,
	};

	typedef struct gclient_s 
	{
		unsigned char pad[12764];
		unsigned int team;
		char pad2[436];
		int flags;
		char pad3[724];
	} gclient_t;

	typedef struct gentity_s 
	{
		unsigned char pad[312]; // 0
		float origin[3]; // 312
		float angles[3]; // 324
		char pad2[8];
		gclient_t* client; // 344
		unsigned char pad3[28];
		short classname;
		short pad4;
		unsigned char pad5[248];
	} gentity_t;

#pragma pack(push, 1)
	typedef struct client_s
	{
		// 0
		int state;
		// 4
		char pad[36];
		// 40
		netadr_t addr;
		// 60
		char pad1[1568];
		// 1628
		char connectInfoString[1024];
		// 2652
		char pad2[133192];
		// 135844
		char name[16];
		// 135860
		char pad3[12];
		// 135872
		int snapNum;
		// 135876
		int pad4;
		// 135880
		short ping;
		// 135882
		//char pad5[142390];
		char pad5[133158];
		// 269040
		int isBot;
		// 269044
		char pad6[9228];
		// 278272
		unsigned __int64 steamid;
		// 278280
		char pad7[403592];
	} client_t;
#pragma pack(pop)

	// Q3TA precompiler code

	//undef if binary numbers of the form 0b... or 0B... are not allowed
#define BINARYNUMBERS
	//undef if not using the token.intvalue and token.floatvalue
#define NUMBERVALUE
	//use dollar sign also as punctuation
#define DOLLAR

	//maximum token length
#define MAX_TOKEN					1024

	//punctuation
	typedef struct punctuation_s
	{
		char *p;						//punctuation character(s)
		int n;							//punctuation indication
		struct punctuation_s *next;		//next punctuation
	} punctuation_t;

	//token
	typedef struct token_s
	{
		char string[MAX_TOKEN];			//available token
		int type;						//last read token type
		int subtype;					//last read token sub type
#ifdef NUMBERVALUE
		unsigned long int intvalue;	//integer value
		long double floatvalue;			//floating point value
#endif //NUMBERVALUE
		char *whitespace_p;				//start of white space before token
		char *endwhitespace_p;			//start of white space before token
		int line;						//line the token was on
		int linescrossed;				//lines crossed in white space
		struct token_s *next;			//next token in chain
	} token_t;

	//script file
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

	//macro definitions
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

	//indents
	//used for conditional compilation directives:
	//#if, #else, #elif, #ifdef, #ifndef
	typedef struct indent_s
	{
		int type;								//indent type
		int skip;								//true if skipping current indent
		script_t *script;						//script the indent was in
		struct indent_s *next;					//next indent on the indent stack
	} indent_t;

	//source file
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

#define MAX_TOKENLENGTH		1024

	typedef struct pc_token_s
	{
		int type;
		int subtype;
		int intvalue;
		float floatvalue;
		char string[MAX_TOKENLENGTH];
	} pc_token_t;

	//token types
#define TT_STRING					1			// string
#define TT_LITERAL					2			// literal
#define TT_NUMBER					3			// number
#define TT_NAME						4			// name
#define TT_PUNCTUATION				5			// punctuation

#define KEYWORDHASH_SIZE	512

	typedef struct keywordHash_s
	{
		char *keyword;
		bool(*func)(menuDef_t *item, int handle);
		//struct keywordHash_s *next;
	} keywordHash_t;

	enum UILocalVarType
	{
		UILOCALVAR_INT = 0x0,
		UILOCALVAR_FLOAT = 0x1,
		UILOCALVAR_STRING = 0x2,
	};

	struct UILocalVar
	{
		UILocalVarType type;
		const char *name;
		union
		{
			int integer;
			float value;
			const char *string;
		};
	};

	struct UILocalVarContext
	{
		UILocalVar table[256];
	};

	struct UiContext
	{
// 		int localClientNum;
// 		float bias;
// 		int realTime;
// 		int frameTime;
// 		int cursorx;
// 		int cursory;
// 		int debug;
// 		int screenWidth;
// 		int screenHeight;
// 		float screenAspect;
// 		float FPS;
// 		float blurRadiusOut;
		char pad[56];
		menuDef_t *menus[512];
		char pad2[512];
		int menuCount;
		// Unsure if below is correct
		menuDef_t *menuStack[16];
		int openMenuCount;
		UILocalVarContext localVars;
	};

	struct LocalizedEntry
	{
		const char* value;
		const char* name;
	};

	struct Bounds
	{
		vec3_t midPoint;
		vec3_t halfSize;
	};

	struct TriggerModel
	{
		int contents;
		unsigned short hullCount;
		unsigned short firstHull;
	};

	struct TriggerHull
	{
		Bounds bounds;
		int contents;
		unsigned short slabCount;
		unsigned short firstSlab;
	};

	struct TriggerSlab
	{
		vec3_t dir;
		float midPoint;
		float halfSize;
	};

	struct MapTriggers
	{
		int modelCount;
		TriggerModel* models; // sizeof 8
		int hullCount;
		TriggerHull* hulls; // sizeof 32
		int slabCount;
		TriggerSlab* slabs; // sizeof 20
	};

	struct Stage
	{
		char * stageName;
		float offset[3];
		int flags;
	};

	struct MapEnts
	{
		const char *name;
		char *entityString;
		int numEntityChars;
		MapTriggers trigger;
		Stage * stages;
		char stageCount;
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

	struct RawFile
	{
		const char* name;
		int sizeCompressed;
		int sizeUnCompressed;
		char * compressedData;
	};

	struct FontEntry
	{
		unsigned short character;
		unsigned char padLeft;
		unsigned char padTop;
		unsigned char padRight;
		unsigned char width;
		unsigned char height;
		unsigned char const0;
		float uvLeft;
		float uvTop;
		float uvRight;
		float uvBottom;
	};

	typedef struct Font_s
	{
		char* name;
		int size;
		int entries;
		Material* image;
		Material* glowImage;
		FontEntry* characters;
	} Font;

	typedef enum
	{
		STRUCTURED_DATA_INT = 0,
		STRUCTURED_DATA_BYTE = 1,
		STRUCTURED_DATA_BOOL = 2,
		STRUCTURED_DATA_STRING = 3,
		STRUCTURED_DATA_ENUM = 4,
		STRUCTURED_DATA_STRUCT = 5,
		STRUCTURED_DATA_INDEXEDARR = 6,
		STRUCTURED_DATA_ENUMARR = 7,
		STRUCTURED_DATA_FLOAT = 8,
		STRUCTURED_DATA_SHORT = 9
	} StructuredDataType;

	typedef struct
	{
		StructuredDataType type;
		union
		{
			int index;
		};
		int offset;
	} StructuredDataItem;

	typedef struct
	{
		const char* name;
		StructuredDataItem item;
	} StructuredDataStructProperty;

	typedef struct
	{
		int numProperties;
		StructuredDataStructProperty* property;
		int unknown1;
		int unknown2;
	} StructuredDataStruct;

	typedef struct
	{
		int enumIndex;
		StructuredDataItem item;
	} StructuredDataEnumedArray;

	typedef struct
	{
		const char* key;
		int index;
	} StructuredDataEnumEntry;

	typedef struct
	{
		int numIndices;
		int unknown;
		StructuredDataEnumEntry* indices;
	} StructuredDataEnum;

	typedef struct
	{
		int numItems;
		StructuredDataItem item;
	} StructuredDataIndexedArray;

	typedef struct
	{
		int version;
		unsigned int hash;
		int numEnums;
		StructuredDataEnum* enums;
		int numStructs;
		StructuredDataStruct* structs;
		int numIndexedArrays;
		StructuredDataIndexedArray* indexedArrays;
		int numEnumArrays;
		StructuredDataEnumedArray* enumArrays;
		StructuredDataItem rootItem;
	} StructuredDataDef;

	typedef struct
	{
		const char* name;
		int count;
		StructuredDataDef* data;
	} StructuredDataDefSet;

	typedef struct
	{
		StructuredDataDef* data;
		StructuredDataItem* item;
		int offset;
		int error;
	} structuredDataFindState_t;

	struct XModelAngle
	{
		short x;
		short y;
		short z;
		short base; // defines the 90-degree point for the shorts
	};

	struct XModelTagPos
	{
		float x;
		float y;
		float z;
	};

	struct XSurfaceCollisionTree
	{
		float trans[3];
		float scale[3];
		int numNode;
		char* node; // el size 16
		int numLeaf;
		short* leaf;
	};

	struct XRigidVertList
	{
		unsigned short boneOffset;
		unsigned short vertCount;
		unsigned short triOffset;
		unsigned short triCount;
		XSurfaceCollisionTree* entry;
	};

	struct GfxPackedVertex
	{
		float x;
		float y;
		float z;
		DWORD color;
		WORD texCoords[2];
		float normal[3];
	};

	struct Face
	{
		unsigned short v1;
		unsigned short v2;
		unsigned short v3;
	};

	struct XSurface
	{
		char tileMode;
		char deformed;
		unsigned short numVertices; // +2
		unsigned short numPrimitives; // +4
		unsigned char streamHandle; // something to do with buffers, +6
		char pad2; // +7
		int pad3; // +8
		Face* indexBuffer; // +12
		short blendNum1; // +16
		short blendNum2; // +18
		short blendNum3; // +20
		short blendNum4; // +22
		char* blendInfo; // +24
		GfxPackedVertex* vertexBuffer; // +28
		int numCT; // +32
		XRigidVertList* ct; // +36
		short something;
		unsigned short flags;
		int something2;
		char pad5[16]; // +40
					   // pad5 matches XModelSurfaces pad
					   // total size, 64
	};

	struct XModelSurfs
	{
		const char* name;
		XSurface* surfaces;
		int numSurfaces;
		char pad[24];
	};

	struct XModelLodInfo
	{
		float dist;
		short numSurfs; // +4
		short maxSurfs;// +6
		XModelSurfs* surfaces; // +8
		int partBits[4]; // +12
		char pad3[8]; 
		XSurface* surfs;
		char pad4[4]; 
	};

	struct cplane_t
	{
		vec3_t a;
		float dist;
		int type;
	};

	struct cbrushside_t
	{
		cplane_t* side;
		short texInfo;
		short dispInfo;
	};

	struct cbrushWrapper_t
	{
		short count;
		cbrushside_t* brushSide;
		char * brushEdge;
		char pad[24];
	};

#pragma pack(push, 4)
	struct BrushWrapper
	{
		float mins[3];
		float maxs[3];
		cbrushWrapper_t brush;
		int totalEdgeCount;
		cplane_t *planes;
	};
#pragma pack(pop)

	struct PhysGeomInfo
	{
		BrushWrapper *brush;
		int type;
		float orientation[3][3];
		float offset[3];
		float halfLengths[3];
	};

	struct PhysMass
	{
		float centerOfMass[3];
		float momentsOfInertia[3];
		float productsOfInertia[3];
	};

	struct PhysCollmap
	{
		const char * name;
		unsigned int count;
		PhysGeomInfo *geoms;
		char unknown[0x18];
		PhysMass mass;
	};

	struct DObjAnimMat
	{
		float quat[4];
		float trans[3];
		float transWeight;
	};

	struct XModelCollSurf
	{
		void* tris; // +0, sizeof 48
		int count; // +4
		char pad[36]; // +8
	}; // +44

	struct PhysPreset
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
	};

	struct XBoneInfo
	{
		float offset[3];
		float bounds[3];
		float radiusSquared;
	};

	struct XModel
	{
		const char* name; // +0
		char numBones; // +4
		char numRootBones; // +5
		char numSurfaces; // +6
		char pad2; // +7
		char pad3[28]; // +8
		short* boneNames; // +36
		char* parentList; // +40
		XModelAngle* tagAngles; // +44, element size 8
		XModelTagPos* tagPositions; // +48, element size 12
		char* partClassification; // +52
		DObjAnimMat* animMatrix; // +56, element size 32
		Material** materials; // +60
		XModelLodInfo lods[4]; // +64
		char pad4;
		char numLods;
		short collLod;
		XModelCollSurf* colSurf; // +244
		int numColSurfs; // +248
		int contents;
		XBoneInfo* boneInfo; // bone count, +256, element size 28
		char pad7[36];
		PhysPreset* physPreset;
		PhysCollmap* physCollmap;
	}; // total size 304

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

	struct DSkelPartBits
	{
		int anim[4];
		int control[4];
		int skel[4];
	};

	struct DSkel
	{
		DSkelPartBits partBits;
		int timeStamp;
		DObjAnimMat *mat;
	};

#pragma pack(push, 2)
	struct DObj
	{
		/*XAnimTree_s*/void *tree;
		unsigned __int16 duplicateParts;
		unsigned __int16 entnum;
		char duplicatePartsSize;
		char numModels;
		char numBones;
		char pad;
		unsigned int ignoreCollision;
		volatile int locked;
		DSkel skel;
		float radius;
		int hidePartBits[4];
		char pad2[56];
		XModel **models;
	};
#pragma pack(pop)

	union XAnimDynamicIndices
	{
		char _1[1];
		unsigned __int16 _2[1];
	};

	union XAnimDynamicFrames
	{
		char(*_1)[3];
		unsigned __int16(*_2)[3];
	};

	struct XAnimNotifyInfo
	{
		unsigned __int16 name;
		float time;
	};

	union XAnimIndices
	{
		char* _1;
		unsigned short *_2;
		void* data;
	};

	struct XAnimPartTransFrames
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

	struct XAnimDeltaPartQuatDataFrames2
	{
		__int16 *frames;
		char indices[1];
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

	struct XAnimDeltaPartQuatDataFrames
	{
		__int16 *frames;
		char indices[1];
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

	enum XAnimPartType
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
	};

	enum XAnimFlags
	{
		XANIM_LOOP_SYNC_TIME = 0x1,
		XANIM_NONLOOP_SYNC_TIME = 0x2,
		XANIM_SYNC_ROOT = 0x4,
		XANIM_COMPLETE = 0x8,
		XANIM_ADDITIVE = 0x10,
		XANIM_CLIENT = 0x20,
		XANIM_SEPARATE = 0x40,
		XANIM_FORCELOAD = 0x80,
		XANIM_PROPOGATE_FLAGS = 0x63,
	};

	struct XAnimParts
	{
		const char * name; // 0
		unsigned short dataByteCount; // 4
		unsigned short dataShortCount; // 6
		unsigned short dataIntCount; // 8
		unsigned short randomDataByteCount; // 10 - 0xA
		unsigned short randomDataIntCount;// 12 - 0xC
		unsigned short framecount; // 14 - 0xE
		char bLoop; // 16
		char boneCount[10]; // 17
		char notetrackCount; // 27
		bool pad1; // 28
		bool bDelta; // 29
		char assetType; // 30
		char pad2; // 31
		int randomDataShortCount; // 32 - 0x20
		int indexcount; // 36 - 0x24
		float framerate; // 40 - 0x28
		float frequency; // 44 - 0x2C
		short * tagnames; // 48 - 0x30
		char *dataByte;// 52 - 0x34
		short *dataShort; // 56 - 0x38
		int *dataInt; // 60 - 0x3C
		short *randomDataShort; // 64 - 0x40
		char *randomDataByte; // 68 - 0x44
		int *randomDataInt; // 72 - 0x48
		XAnimIndices indices; // 76 - 0x4C
		XAnimNotifyInfo* notetracks; // 80 - 0x50
		XAnimDeltaPart * delta; // 84 - 0x54
								// 88 - 0x58
	};


	/* FxEffectDef::flags */
#define FX_ELEM_LOOPING				0x1
#define FX_ELEM_USE_RAND_COLOR			0x2
#define FX_ELEM_USE_RAND_ALPHA			0x4
#define FX_ELEM_USE_RAND_SIZE0			0x8
#define FX_ELEM_USE_RAND_SIZE1			0x10
#define FX_ELEM_USE_RAND_SCALE			0x20
#define FX_ELEM_USE_RAND_ROT_DELTA		0x40
#define FX_ELEM_MOD_COLOR_BY_ALPHA		0x80
#define FX_ELEM_USE_RAND_VEL0			0x100
#define FX_ELEM_USE_RAND_VEL1			0x200
#define FX_ELEM_USE_BACK_COMPAT_VEL		0x400
#define FX_ELEM_ABS_VEL0			0x800
#define FX_ELEM_ABS_VEL1			0x1000
#define FX_ELEM_PLAY_ON_TOUCH			0x2000
#define FX_ELEM_PLAY_ON_DEATH			0x4000
#define FX_ELEM_PLAY_ON_RUN			0x8000
#define FX_ELEM_BOUNDING_SPHERE			0x10000
#define FX_ELEM_USE_ITEM_CLIP			0x20000
#define FX_ELEM_DISABLED			0x80000000
#define FX_ELEM_DECAL_FADE_IN			0x40000

	/* FxElemDef::flags */
#define FX_ELEM_SPAWN_RELATIVE_TO_EFFECT	0x2
#define FX_ELEM_SPAWN_FRUSTUM_CULL		0x4
#define FX_ELEM_RUNNER_USES_RAND_ROT		0x8
#define FX_ELEM_SPAWN_OFFSET_NONE		0x0
#define FX_ELEM_SPAWN_OFFSET_SPHERE		0x10
#define FX_ELEM_SPAWN_OFFSET_CYLINDER		0x20
#define FX_ELEM_SPAWN_OFFSET_MASK		0x30
#define FX_ELEM_RUN_RELATIVE_TO_WORLD		0x0
#define FX_ELEM_RUN_RELATIVE_TO_SPAWN		0x40
#define FX_ELEM_RUN_RELATIVE_TO_EFFECT		0x80
#define FX_ELEM_RUN_RELATIVE_TO_OFFSET		0xC0
#define FX_ELEM_RUN_MASK			0xC0
#define FX_ELEM_USE_COLLISION			0x100
#define FX_ELEM_DIE_ON_TOUCH			0x200
#define FX_ELEM_DRAW_PAST_FOG			0x400
#define FX_ELEM_DRAW_WITH_VIEWMODEL		0x800
#define FX_ELEM_BLOCK_SIGHT			0x1000
#define FX_ELEM_HAS_VELOCITY_GRAPH_LOCAL	0x1000000
#define FX_ELEM_HAS_VELOCITY_GRAPH_WORLD	0x2000000
#define FX_ELEM_HAS_GRAVITY			0x4000000
#define FX_ELEM_USE_MODEL_PHYSICS		0x8000000
#define FX_ELEM_NONUNIFORM_SCALE		0x10000000
#define FX_ELEM_CLOUD_SHAPE_CUBE		0x0
#define FX_ELEM_CLOUD_SHAPE_SPHERE_LARGE	0x20000000
#define FX_ELEM_CLOUD_SHAPE_SPHERE_MEDIUM	0x40000000
#define FX_ELEM_CLOUD_SHAPE_SPHERE_SMALL	0x60000000
#define FX_ELEM_CLOUD_MASK			0x60000000
#define FX_ELEM_DISABLE_FOUNTAIN_COLLISION	0x80000000
#define FX_ELEM_DRAW_IN_THERMAL_ONLY		0x2000
#define FX_ELEM_TRAIL_ORIENT_BY_VELOCITY	0x4000
#define FX_ELEM_EMIT_ORIENT_BY_ELEM		0x8000

	/* FxElemAtlas::behavior */
#define FX_ATLAS_START_MASK		0x3
#define FX_ATLAS_START_FIXED		0x0
#define FX_ATLAS_START_RANDOM		0x1
#define FX_ATLAS_START_INDEXED		0x2
#define FX_ATLAS_PLAY_OVER_LIFE		0x4
#define FX_ATLAS_LOOP_ONLY_N_TIMES	0x8

	enum FxElemType : char
	{
		FX_ELEM_TYPE_SPRITE_BILLBOARD = 0x0,
		FX_ELEM_TYPE_SPRITE_ORIENTED = 0x1,
		FX_ELEM_TYPE_TAIL = 0x2,
		FX_ELEM_TYPE_TRAIL = 0x3,
		FX_ELEM_TYPE_CLOUD = 0x4,
		FX_ELEM_TYPE_SPARKCLOUD = 0x5,
		FX_ELEM_TYPE_SPARKFOUNTAIN = 0x6,
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

	struct FxElemVec3Range
	{
		float base[3];
		float amplitude[3];
	};

	struct FxIntRange
	{
		int base;
		int amplitude;
	};

	struct FxFloatRange
	{
		float base;
		float amplitude;
	};

	struct FxSpawnDefLooping
	{
		int intervalMsec;
		int count;
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

	struct FxEffectDef;

	union FxEffectDefRef
	{
		FxEffectDef *handle;
		const char *name;
	};

	union FxElemVisuals
	{
		const void *anonymous;
		Material *material;
		XModel *xmodel;
		FxEffectDefRef *effectDef;
		const char *soundName;
	};

	struct FxElemMarkVisuals
	{
		Material* data[2];
	};

	union FxElemDefVisuals
	{
		FxElemVisuals instance;
		//If parent FxElemDef::elemType == 0x7, use xmodel
		//If parent FxElemDef::elemType == 0xC, use effectDef
		//If parent FxElemDef::elemType == 0xA, use soundName
		//If parent FxElemDef::elemType != 0x9 || 0x8, use material

		FxElemVisuals *array;			//Total count = parent FxElemDef::visualCount
		FxElemMarkVisuals *markArray;		//Total count = parent FxElemDef::visualCount
	};

	struct FxTrailVertex
	{
		/*
		float pos[2];
		float normal[2];
		float texCoord[2];
		*/
		char pad[20];
	};

	struct FxTrailDef
	{
		int scrollTimeMsec;
		int repeatDist;
		float splitArcDist;
		int splitDist;
		int splitTime;
		int vertCount;
		FxTrailVertex *verts;
		int indCount;
		unsigned __int16 *inds;
	};

	struct FxSparkFountain
	{
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

	union FxElemExtendedDef
	{
		char *unknownBytes;
		FxSparkFountain *sparkFountain;
		FxTrailDef *trailDef;
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

	struct FxElemVelStateInFrame
	{
		FxElemVec3Range velocity;
		FxElemVec3Range totalDelta;
	};

	struct FxElemVelStateSample
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

	struct FxElemVisStateSample
	{
		FxElemVisualState base;
		FxElemVisualState amplitude;
	};

	struct FxElemDef	// 0xFC
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
		FxElemVelStateSample *velSamples;	// count = velIntervalCount
		FxElemVisStateSample *visSamples;	// count = visStateIntervalCount
		FxElemDefVisuals visuals;
		//If elemType is 0xB, then use markVisuals
		//If elemType is not 0xB and visualCount == 1, then use visual
		//If elemType is not 0xB and visualCount != 1, then use visualsArray
		vec3_t collMins;
		vec3_t collMaxs;
		FxEffectDefRef *effectOnImpact;
		FxEffectDefRef *effectOnDeath;
		FxEffectDefRef *effectEmitted;
		FxFloatRange emitDist;
		FxFloatRange emitDistVariance;
		FxElemExtendedDef extendedDef;
		//If elemType == 3, then use trailDef
		//If elemType == 6, then use sparkFountain
		//If elemType != 3 && elemType != 6 use unknownBytes (size = 1)
		char sortOrder;
		char lightingFrac;
		char unused[2];
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
		FxElemDef *elemDefs;		//Count = elemDefCountOneShot + elemDefCountEmission + elemDefCountLooping
	};

	struct cPlane
	{
		vec3_t a;
		float dist;
		int type;
	};

	struct cStaticModel
	{
		XModel *xmodel;
		float origin[3];
		float invScaledAxis[3][3];
		float absmin[3];
		float absmax[3];
	};

	struct dMaterial
	{
		char* name;
		int unk;
		int unk2;
	};

	struct cNode
	{
		cPlane* plane;
		short children[2];
	};

	struct cBrushSide
	{
		cPlane* side;
		short texInfo, dispInfo;
	};

	struct cBrush
	{
		int count;
		cBrushSide * brushSide;
		char * brushEdge;
		char pad[24];
	};

	struct cLeaf
	{
		unsigned __int16 firstCollAabbIndex;
		unsigned __int16 collAabbCount;
		int brushContents;
		int terrainContents;
		float mins[3];
		float maxs[3];
		int leafBrushNode;
	};

	struct cLeafBrushNodeLeaf
	{
		unsigned __int16 *brushes;
	};

	struct cLeafBrushNodeChildren
	{
		float dist;
		float range;
		unsigned __int16 childOffset[6];
	};

	union cLeafBrushNodeData
	{
		cLeafBrushNodeLeaf leaf;
		cLeafBrushNodeChildren children;
	};

	struct cLeafBrushNode
	{
		char axis;
		__int16 leafBrushCount;
		int contents;
		cLeafBrushNodeData data;
	};

	struct cModel
	{
		float mins[3];
		float maxs[3];
		float radius;
		cLeaf leaf;
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
		float origin[3];
		float halfSize[3];
		unsigned __int16 materialIndex;
		unsigned __int16 childCount;
		CollisionAabbTreeIndex u;
	};

	struct clipMap_t
	{
		const char* name;
		int unknown1; // +8
		int numCPlanes; // +8
		cPlane* cPlanes; // sizeof 20, +12
		int numStaticModels; // +16
		cStaticModel* staticModelList; // sizeof 76, +20
		int numMaterials; // +24
		dMaterial* materials; // sizeof 12 with a string (possibly name?), +28
		int numCBrushSides; // +32
		cBrushSide* cBrushSides; // sizeof 8, +36
		int numCBrushEdges; // +40
		char* cBrushEdges; // +44
		int numCNodes; // +48
		cNode * cNodes; // sizeof 8, +52
		int numCLeaf; // +56
		cLeaf* cLeaf; // +60
		int numCLeafBrushNodes; // +64
		cLeafBrushNode* cLeafBrushNodes; // +68
		int numLeafBrushes; // +72
		short* leafBrushes; // +76
		int numLeafSurfaces; // +80
		int* leafSurfaces; // +84
		int numVerts; // +88
		vec3_t* verts; // +92
		int numTriIndices; // +96
		short* triIndices; // +100
		bool* triEdgeIsWalkable; // +104
		int numCollisionBorders; // +108
		CollisionBorder* collisionBorders;// sizeof 28, +112
		int numCollisionPartitions; // +116
		CollisionPartition* collisionPartitions; // sizeof 12, +120
		int numCollisionAABBTrees; // +124
		CollisionAabbTree* collisionAABBTrees;// sizeof 32, +128
		int numCModels; // +132
		cModel* cModels; // sizeof 68, +136
		short numCBrushes; // +140
		short pad2; // +142
		cBrush * cBrushes; // sizeof 36, +144
		void* unknown2; // same count as cBrushes, +148
		int * unknown3; // same count as cBrushes, +152
		MapEnts * mapEnts; // +156
		int unkCount4; // +160
		void* unknown4; // +164
		unsigned __int16 dynEntCount[2];
		DynEntityDef *dynEntDefList[2];
		DynEntityPose *dynEntPoseList[2];
		DynEntityClient *dynEntClientList[2];
		DynEntityColl *dynEntCollList[2];
		unsigned int checksum;
		char unknown5[0x30];
	}; // +256

	struct GameMap_Data
	{
		void* unk1;
		int unkCount1;
		int unkCount2;
		void* unk2;
		char pad[112];
	};

	struct PathData
	{
		char pad[40];
	};

	struct VehicleTrack
	{
		char pad[8];
	};

	struct GameWorldSp
	{
		const char* name;
		PathData pathData;
		VehicleTrack vehicleTrack;
		GameMap_Data* data;
	};


	struct GameWorldMp
	{
		const char* name;
		GameMap_Data* data;
	};

	struct VehicleDef
	{
		const char* name;
		char pad[408];
		void* weaponDef;
		char pad2[304];
	};

	struct XModelDrawInfo
	{
		unsigned __int16 lod;
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

	struct GfxStreamingAabbTree
	{
		unsigned __int16 firstItem;
		unsigned __int16 itemCount;
		unsigned __int16 firstChild;
		unsigned __int16 childCount;
		float mins[3];
		float maxs[3];
	};

	struct GfxWorldStreamInfo
	{
		int aabbTreeCount;
		GfxStreamingAabbTree *aabbTrees;
		int leafRefCount;
		int *leafRefs;
	};

	union GfxColor
	{
		unsigned int packed;
		char array[4];
	};

	union PackedUnitVec
	{
		unsigned int packed;
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
		void/*IDirect3DVertexBuffer9*/* worldVb;
	};

#pragma pack(push, 4)
	struct GfxLightImage
	{
		GfxImage *image;
		char samplerState;
	};
#pragma pack(pop)

	struct GfxLightDef
	{
		const char *name;
		GfxLightImage attenuation;
		int lmapLookupStart;
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
		GfxLightDef *def;
	};

	struct GfxReflectionProbe
	{
		float offset[3];
	};

	struct cplane_s;

	struct GfxWorldDpvsPlanes
	{
		int cellCount;
		cplane_s *planes;
		unsigned __int16 *nodes;
		unsigned int *sceneEntCellBits; //Size = cellCount << 11
	};

	struct GfxAabbTree
	{
		float mins[3];
		float maxs[3];
		int pad;
		unsigned __int16 childCount;
		unsigned __int16 surfaceCount;
		unsigned __int16 startSurfIndex;
		unsigned __int16 smodelIndexCount;
		unsigned __int16 *smodelIndexes;
		int childrenOffset;
	};

	struct GfxCellTree
	{
		GfxAabbTree *aabbTree;
	};

	struct GfxCellTreeCount
	{
		int aabbTreeCount;
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

	struct GfxStaticModelInst
	{
		float mins[3];
		float maxs[3];
		GfxColor groundLighting;
	};

	enum surfaceType_t
	{
		SF_TRIANGLES = 0x0,
		SF_TRIANGLES_NO_SUN_SHADOW = 0x1,
		SF_BEGIN_STATICMODEL = 0x2,
		SF_STATICMODEL_RIGID = 0x2,
		SF_STATICMODEL_INSTANCED = 0x3,
		SF_STATICMODEL_RIGID_NO_SUN_SHADOW = 0x4,
		SF_STATICMODEL_INSTANCED_NO_SUN_SHADOW = 0x5,
		SF_END_STATICMODEL = 0x6,
		SF_BMODEL = 0x6,
		SF_BEGIN_XMODEL = 0x7,
		SF_XMODEL_RIGID = 0x7,
		SF_XMODEL_SKINNED = 0x8,
		SF_END_XMODEL = 0x9,
		SF_BEGIN_FX = 0x9,
		SF_CODE = 0x9,
		SF_GLASS = 0xA,
		SF_MARK = 0xB,
		SF_SPARK = 0xC,
		SF_PARTICLE_CLOUD = 0xD,
		SF_PARTICLE_SPARK_CLOUD = 0xE,
		SF_END_FX = 0xF,
		SF_NUM_SURFACE_TYPES = 0xF,
		SF_FORCE_32_BITS = 0xFFFFFFFF,
	};

	struct srfTriangles_t
	{
		int vertexLayerData;
		int firstVertex;
		unsigned __int16 vertexCount;
		unsigned __int16 triCount;
		int baseIndex;
	};

	struct GfxSurface
	{
		srfTriangles_t tris;
		Material *material;
		char lightmapIndex;
		char reflectionProbeIndex;
		char primaryLightIndex;
		char castsSunShadow;
	};

	struct GfxSurfaceBounds
	{
		Bounds bounds;
		char flags;
	};

	struct GfxDrawSurfFields
	{
		__int64 _bf0;
	};

	union GfxDrawSurf
	{
		GfxDrawSurfFields fields;
		unsigned __int64 packed;
	};

	struct GfxStaticModelDrawInst;

	struct GfxWorldDpvsStatic
	{
		unsigned int smodelCount;
		unsigned int staticSurfaceCount;
		unsigned int litSurfsBegin;
		unsigned int litSurfsEnd;
		char unknown1[0x20];
		int sunShadowCount;
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

#pragma pack(push, 4)
	struct GfxPackedPlacement
	{
		float origin[3];
		PackedUnitVec axis[3];
		float scale;
	};

	struct GfxStaticModelDrawInst
	{
		GfxPackedPlacement placement;

		char pad[24];

		XModel *model; // 52
		float cullDist;
		char reflectionProbeIndex;
		char primaryLightIndex;
		unsigned __int16 lightingHandle;
		char flags;

		char pad2[8];
	};

	struct cplane_s
	{
		float normal[3];
		float dist;
		char type;
		char signbits;
	};

	struct GfxPortalWritable
	{
		bool isQueued;
		bool isAncestor;
		char recursionDepth;
		char hullPointCount;
		float(*hullPoints)[2];
	};

	struct DpvsPlane
	{
		float coeffs[4];
		char side[3];
	};

	struct GfxPortal
	{
		GfxPortalWritable writable;
		DpvsPlane plane;
		float(*vertices)[3];
		char unknown[2];
		char vertexCount;
		float hullAxis[2][3];
	};

	struct GfxCell
	{
		float mins[3];
		float maxs[3];
		int portalCount;
		GfxPortal *portals;
		char reflectionProbeCount;
		char *reflectionProbes;
	};

	struct GfxLightmapArray
	{
		GfxImage *primary;
		GfxImage *secondary;
	};

	struct GfxLightGrid
	{
		bool hasLightRegions;
		unsigned int sunPrimaryLightIndex;
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
		float mins[3];
		float maxs[3];
	};

	struct GfxBrushModel
	{
		GfxBrushModelWritable writable;
		float bounds[2][3];
		unsigned int surfaceCount;
		unsigned int startSurfIndex;
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

	struct GfxWorldDpvsDynamic
	{
		unsigned int dynEntClientWordCount[2];
		unsigned int dynEntClientCount[2];
		unsigned int *dynEntCellBits[2];
		char *dynEntVisData[2][3];
	};

	struct SunLightParseParams
	{
		char name[64];
		float ambientScale;
		float ambientColor[3];
		float diffuseFraction;
		float sunLight;
		float sunColor[3];
		float diffuseColor[3];
		bool diffuseColorHasBeenSet;
		float angles[3];
	};

	struct GfxWorldVertexLayerData
	{
		char *data;
		void/*IDirect3DVertexBuffer9*/* layerVb;
	};

	typedef char GfxTexture[0x34];

	struct GfxWorldDraw
	{
		unsigned int reflectionProbeCount;
		GfxImage * * reflectionImages;
		GfxReflectionProbe *reflectionProbes;
		GfxTexture * reflectionProbeTextures; //Count = refelctionProbeCount
		int lightmapCount;
		GfxLightmapArray *lightmaps;
		GfxTexture * lightmapPrimaryTextures; //Count = lightmapCount
		GfxTexture * lightmapSecondaryTextures; //Count = lightmapCount
		GfxImage *skyImage;
		GfxImage *outdoorImage;
		unsigned int vertexCount;
		GfxWorldVertexData vd;
		unsigned int vertexLayerDataSize;
		GfxWorldVertexLayerData vld;
		int indexCount;
		unsigned __int16 *indices;
	};

	struct GfxSky
	{
		int skySurfCount;
		int * skyStartSurfs;
		GfxImage * skyImage;
		int skySamplerState;
	};

	struct GfxWorld
	{
		const char *name;
		const char *baseName;
		int planeCount;
		int nodeCount;
		int unknown2;
		unsigned int skyCount;
		GfxSky* skies;
		char unknown1[0x18];
		GfxWorldDpvsPlanes dpvsPlanes; //The following rely on the count in this
		GfxCellTreeCount *aabbTreeCounts;
		GfxCellTree *aabbTrees;
		GfxCell *cells;
		GfxWorldDraw worldDraw;
		GfxLightGrid lightGrid;
		int modelCount;
		GfxBrushModel *models;
		float mins[3];
		float maxs[3];
		unsigned int checksum;
		int materialMemoryCount;
		MaterialMemory *materialMemory;
		sunflare_t sun;
		unsigned int *cellCasterBits[2];
		GfxSceneDynModel *sceneDynModel;
		GfxSceneDynBrush *sceneDynBrush;
		unsigned int *primaryLightEntityShadowVis;
		unsigned int *primaryLightDynEntShadowVis[2];
		char *primaryLightForModelDynEnt;
		GfxShadowGeometry *shadowGeom;
		GfxLightRegion *lightRegion;

		char pad[24];
		GfxImage* unknownImage;
		char* unknown6;
		char* unknown7;
		char* unknown8;
		char* unknown9;
		char* unknown10;
		char* unknown11;
		char* unknown12;
		char* unknown13;
		char* unknown14;
		char* unknown15;

		GfxWorldDpvsStatic dpvs;
		GfxWorldDpvsDynamic dpvsDyn;

		char pad2[4];

		unsigned int heroOnlyLightCount;
		char * heroOnlyLight;
		int unknown5;
	};
#pragma pack(pop)

	struct rgpStruct
	{
		int pad[2117];
		GfxWorld* world;
	};

	union XAssetHeader
	{
		void *data;

		// This is only for debugging
		const char* string;

		MenuList *menuList;
		menuDef_t *menu;
		Material *material;
		snd_alias_list_t *aliasList;
		LocalizedEntry *localize;
		StringTable *stringTable;
		MapEnts* mapEnts;
		RawFile* rawfile;
		GfxImage* image;
		Font* font;
		MaterialTechniqueSet *materialTechset;
		MaterialVertexDeclaration *vertexDecl;
		MaterialVertexShader *vertexShader;
		MaterialPixelShader *pixelShader;
		StructuredDataDefSet* structuredData;
		XModel* model;
		PhysPreset* physPreset;
		PhysCollmap* physCollmap;
		XModelSurfs* surfaces;
		XAnimParts* xanim;
		clipMap_t* clipMap;
		FxEffectDef* fx;
		GameWorldMp* gameMapMP;
		GameWorldSp* gameMapSP;
		TracerDef* tracer;
		VehicleDef* vehicle;
		GfxWorld* gfxMap;
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
		bool inuse;
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
	struct  XFileHeader
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

	typedef struct fileInPack_s
	{
		DWORD idk;
		char* name;
		DWORD idk2;
	} fileInPack_t;

	typedef struct
	{
		char			pakFilename[256];			// c:\quake3\baseq3\pak0.pk3
		char			pakBasename[256];			// pak0
		char			pakGamename[256];			// baseq3
		void*			handle;						// handle to zip file
		int				checksum;
		int				pure_checksum;				// checksum for pure
		int				idk;						// ?
		int				numfiles;					// number of files in pk3
		int				referenced;					// referenced file flags
		int				hashSize;					// hash table size (power of 2)
		fileInPack_t*	*hashTable;					// hash table
		fileInPack_t*	buildBuffer;
	} pack_t;

	typedef struct {
		char		path[256];		// c:\quake3
		char		gamedir[256];	// baseq3
	} directory_t;

	typedef struct searchpath_s
	{
		searchpath_s* next;
		pack_t* pack;
		directory_t* dir;
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

	// Probably incomplete or wrong!
#pragma pack(push, 4)
	struct usercmd_s
	{
		int serverTime;
		int buttons;
		char weapon;
		char offHandIndex;
		int angles[3];
		char forwardmove;
		char rightmove;
		float meleeChargeYaw;
		char meleeChargeDist;
	};
#pragma pack(pop)
	
		typedef char mapname_t[40];

#ifdef __cplusplus
}
#endif