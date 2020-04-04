// Generated automatically with "cito". Do not edit.
#include <stdlib.h>
#include <string.h>
#include "asap.h"

static void CiString_Assign(char **str, char *value)
{
	free(*str);
	*str = value;
}

static char *CiString_Substring(const char *str, int len)
{
	char *p = malloc(len + 1);
	memcpy(p, str, len);
	p[len] = '\0';
	return p;
}

typedef enum {
	NmiStatus_RESET,
	NmiStatus_ON_V_BLANK,
	NmiStatus_WAS_V_BLANK
} NmiStatus;

typedef enum {
	ASAPModuleType_SAP_B,
	ASAPModuleType_SAP_C,
	ASAPModuleType_SAP_D,
	ASAPModuleType_SAP_S,
	ASAPModuleType_CMC,
	ASAPModuleType_CM3,
	ASAPModuleType_CMR,
	ASAPModuleType_CMS,
	ASAPModuleType_DLT,
	ASAPModuleType_MPT,
	ASAPModuleType_RMT,
	ASAPModuleType_TMC,
	ASAPModuleType_TM2,
	ASAPModuleType_FC
} ASAPModuleType;
typedef struct DurationParser DurationParser;
typedef struct ASAPNativeModuleWriter ASAPNativeModuleWriter;
typedef struct Cpu6502 Cpu6502;

typedef enum {
	FlashPackItemType_LITERAL,
	FlashPackItemType_COPY_TWO_BYTES,
	FlashPackItemType_COPY_THREE_BYTES,
	FlashPackItemType_COPY_MANY_BYTES,
	FlashPackItemType_SET_ADDRESS,
	FlashPackItemType_END_OF_STREAM
} FlashPackItemType;
typedef struct FlashPackItem FlashPackItem;
typedef struct FlashPack FlashPack;
typedef struct PokeyChannel PokeyChannel;
typedef struct Pokey Pokey;
typedef struct PokeyPair PokeyPair;

struct Cpu6502 {
	ASAP *asap;
	uint8_t memory[65536];
	int cycle;
	int pc;
	int a;
	int x;
	int y;
	int s;
	int nz;
	int c;
	int vdi;
};

static void Cpu6502_Reset(Cpu6502 *self);

static int Cpu6502_Peek(const Cpu6502 *self, int addr);

static void Cpu6502_Poke(Cpu6502 *self, int addr, int data);

static int Cpu6502_PeekReadModifyWrite(Cpu6502 *self, int addr);

static int Cpu6502_Pull(Cpu6502 *self);

static void Cpu6502_PullFlags(Cpu6502 *self);

static void Cpu6502_Push(Cpu6502 *self, int data);

static void Cpu6502_PushPc(Cpu6502 *self);

static void Cpu6502_PushFlags(Cpu6502 *self, int b);

static void Cpu6502_AddWithCarry(Cpu6502 *self, int data);

static void Cpu6502_SubtractWithCarry(Cpu6502 *self, int data);

static int Cpu6502_ArithmeticShiftLeft(Cpu6502 *self, int addr);

static int Cpu6502_RotateLeft(Cpu6502 *self, int addr);

static int Cpu6502_LogicalShiftRight(Cpu6502 *self, int addr);

static int Cpu6502_RotateRight(Cpu6502 *self, int addr);

static int Cpu6502_Decrement(Cpu6502 *self, int addr);

static int Cpu6502_Increment(Cpu6502 *self, int addr);

static void Cpu6502_ExecuteIrq(Cpu6502 *self, int b);

static void Cpu6502_CheckIrq(Cpu6502 *self);

static void Cpu6502_Shx(Cpu6502 *self, int addr, int data);

/**
 * Runs 6502 emulation for the specified number of Atari scanlines.
 * Each scanline is 114 cycles of which 9 is taken by ANTIC for memory refresh.
 * @param self This <code>Cpu6502</code>.
 */
static void Cpu6502_DoFrame(Cpu6502 *self, int cycleLimit);

struct PokeyChannel {
	int audf;
	int audc;
	int periodCycles;
	int tickCycle;
	int timerCycle;
	int mute;
	int out;
	int delta;
};

static void PokeyChannel_Initialize(PokeyChannel *self);

static void PokeyChannel_Slope(PokeyChannel *self, Pokey *pokey, const PokeyPair *pokeys, int cycle);

static void PokeyChannel_DoTick(PokeyChannel *self, Pokey *pokey, const PokeyPair *pokeys, int cycle, int ch);

static void PokeyChannel_DoStimer(PokeyChannel *self, int cycle);

static void PokeyChannel_SetMute(PokeyChannel *self, bool enable, int mask, int cycle);

static void PokeyChannel_MuteUltrasound(PokeyChannel *self, int cycle);

static void PokeyChannel_SetAudc(PokeyChannel *self, Pokey *pokey, const PokeyPair *pokeys, int data, int cycle);

static void PokeyChannel_EndFrame(PokeyChannel *self, int cycle);

struct Pokey {
	PokeyChannel channels[4];
	int audctl;
	int skctl;
	int irqst;
	bool init;
	int divCycles;
	int reloadCycles1;
	int reloadCycles3;
	int polyIndex;
	int deltaBuffer[888];
	int iirAcc;
};

static void Pokey_StartFrame(Pokey *self);

static void Pokey_Initialize(Pokey *self);

static void Pokey_AddDelta(Pokey *self, const PokeyPair *pokeys, int cycle, int delta);

/**
 * Fills <code>DeltaBuffer</code> up to <code>cycleLimit</code> basing on current Audf/Audc/Audctl values.
 * @param self This <code>Pokey</code>.
 */
static void Pokey_GenerateUntilCycle(Pokey *self, const PokeyPair *pokeys, int cycleLimit);

static void Pokey_EndFrame(Pokey *self, const PokeyPair *pokeys, int cycle);

static bool Pokey_IsSilent(const Pokey *self);

static void Pokey_Mute(Pokey *self, int mask);

static void Pokey_InitMute(Pokey *self, int cycle);

static int Pokey_Poke(Pokey *self, const PokeyPair *pokeys, int addr, int data, int cycle);

static int Pokey_CheckIrq(Pokey *self, int cycle, int nextEventCycle);

static int Pokey_StoreSample(Pokey *self, uint8_t *buffer, int bufferOffset, int i, ASAPSampleFormat format);

static void Pokey_AccumulateTrailing(Pokey *self, int i);

struct PokeyPair {
	uint8_t poly9Lookup[511];
	uint8_t poly17Lookup[16385];
	int extraPokeyMask;
	Pokey basePokey;
	Pokey extraPokey;
	int sampleFactor;
	int sampleOffset;
	int readySamplesStart;
	int readySamplesEnd;
};
static void PokeyPair_Construct(PokeyPair *self);

static void PokeyPair_Initialize(PokeyPair *self, bool ntsc, bool stereo);

static int PokeyPair_Poke(PokeyPair *self, int addr, int data, int cycle);

static int PokeyPair_Peek(const PokeyPair *self, int addr, int cycle);

static void PokeyPair_StartFrame(PokeyPair *self);

static int PokeyPair_EndFrame(PokeyPair *self, int cycle);

/**
 * Fills buffer with samples from <code>DeltaBuffer</code>.
 * @param self This <code>PokeyPair</code>.
 */
static int PokeyPair_Generate(PokeyPair *self, uint8_t *buffer, int bufferOffset, int blocks, ASAPSampleFormat format);

static bool PokeyPair_IsSilent(const PokeyPair *self);

/**
 * Information about a music file.
 */
struct ASAPInfo {
	char *filename;
	char *author;
	char *title;
	char *date;
	int channels;
	int songs;
	int defaultSong;
	int durations[32];
	bool loops[32];
	bool ntsc;
	ASAPModuleType type;
	int fastplay;
	int music;
	int init;
	int player;
	int covoxAddr;
	int headerLen;
	uint8_t songPos[32];
};
static void ASAPInfo_Construct(ASAPInfo *self);
static void ASAPInfo_Destruct(ASAPInfo *self);

static bool ASAPInfo_IsValidChar(int c);

static bool ASAPInfo_CheckValidChar(int c);

static bool ASAPInfo_CheckValidText(const char *s);

static bool ASAPInfo_CheckTwoDateDigits(const ASAPInfo *self, int i);

static int ASAPInfo_CheckDate(const ASAPInfo *self);

static int ASAPInfo_GetTwoDateDigits(const ASAPInfo *self, int i);

static int ASAPInfo_GetWord(uint8_t const *array, int i);

static bool ASAPInfo_ParseModule(ASAPInfo *self, uint8_t const *module, int moduleLen);

static void ASAPInfo_AddSong(ASAPInfo *self, int playerCalls);

static void ASAPInfo_ParseCmcSong(ASAPInfo *self, uint8_t const *module, int pos);

static bool ASAPInfo_ParseCmc(ASAPInfo *self, uint8_t const *module, int moduleLen, ASAPModuleType type);

static bool ASAPInfo_IsDltTrackEmpty(uint8_t const *module, int pos);

static bool ASAPInfo_IsDltPatternEnd(uint8_t const *module, int pos, int i);

static void ASAPInfo_ParseDltSong(ASAPInfo *self, uint8_t const *module, bool *seen, int pos);

static bool ASAPInfo_ParseDlt(ASAPInfo *self, uint8_t const *module, int moduleLen);

static void ASAPInfo_ParseMptSong(ASAPInfo *self, uint8_t const *module, bool *globalSeen, int songLen, int pos);

static bool ASAPInfo_ParseMpt(ASAPInfo *self, uint8_t const *module, int moduleLen);

static int ASAPInfo_GetRmtInstrumentFrames(uint8_t const *module, int instrument, int volume, int volumeFrame, bool onExtraPokey);

static void ASAPInfo_ParseRmtSong(ASAPInfo *self, uint8_t const *module, bool *globalSeen, int songLen, int posShift, int pos);

static bool ASAPInfo_ValidateRmt(uint8_t const *module, int moduleLen);

static bool ASAPInfo_ParseRmt(ASAPInfo *self, uint8_t const *module, int moduleLen);

static void ASAPInfo_ParseTmcSong(ASAPInfo *self, uint8_t const *module, int pos);

static int ASAPInfo_ParseTmcTitle(uint8_t *title, int titleLen, uint8_t const *module, int moduleOffset);

static bool ASAPInfo_ParseTmc(ASAPInfo *self, uint8_t const *module, int moduleLen);

static void ASAPInfo_ParseTm2Song(ASAPInfo *self, uint8_t const *module, int pos);

static bool ASAPInfo_ParseTm2(ASAPInfo *self, uint8_t const *module, int moduleLen);

static int ASAPInfo_AfterFF(uint8_t const *module, int moduleLen, int currentOffset);

static int ASAPInfo_GetFcTrackCommand(uint8_t const *module, int const *trackPos, int n);

static bool ASAPInfo_IsFcSongEnd(uint8_t const *module, int const *trackPos);

static bool ASAPInfo_ValidateFc(uint8_t const *module, int moduleLen);

static bool ASAPInfo_ParseFc(ASAPInfo *self, uint8_t const *module, int moduleLen);

static bool ASAPInfo_HasStringAt(uint8_t const *module, int moduleIndex, const char *s);

static int ASAPInfo_ParseDec(const char *s, int minVal, int maxVal);

static int ASAPInfo_ParseHex(const char *s);

static bool ASAPInfo_ValidateSap(uint8_t const *module, int moduleLen);

static bool ASAPInfo_ParseSap(ASAPInfo *self, uint8_t const *module, int moduleLen);

static int ASAPInfo_PackExt(const char *ext);

static int ASAPInfo_GetPackedExt(const char *filename);

static bool ASAPInfo_IsOurPackedExt(int ext);

static int ASAPInfo_GuessPackedExt(uint8_t const *module, int moduleLen);

static int ASAPInfo_GetRmtSapOffset(const ASAPInfo *self, uint8_t const *module, int moduleLen);

static ASAPModuleType ASAPInfo_GetOriginalModuleType(const ASAPInfo *self, uint8_t const *module, int moduleLen);

/**
 * Atari 8-bit chip music emulator.
 * This class performs no I/O operations - all music data must be passed in byte arrays.
 */
struct ASAP {
	int nextEventCycle;
	Cpu6502 cpu;
	int nextScanlineCycle;
	NmiStatus nmist;
	int consol;
	uint8_t covox[4];
	PokeyPair pokeys;
	ASAPInfo moduleInfo;
	int nextPlayerCycle;
	int tmcPerFrameCounter;
	int currentSong;
	int currentDuration;
	int blocksPlayed;
	int silenceCycles;
	int silenceCyclesCounter;
	bool gtiaOrCovoxPlayedThisFrame;
};
static void ASAP_Construct(ASAP *self);
static void ASAP_Destruct(ASAP *self);

static int ASAP_PeekHardware(const ASAP *self, int addr);

static void ASAP_PokeHardware(ASAP *self, int addr, int data);

static void ASAP_Call6502(ASAP *self, int addr);

static void ASAP_Call6502Player(ASAP *self);

static bool ASAP_IsIrq(const ASAP *self);

static void ASAP_HandleEvent(ASAP *self);

static int ASAP_Do6502Frame(ASAP *self);

static int ASAP_DoFrame(ASAP *self);

static bool ASAP_Do6502Init(ASAP *self, int pc, int a, int x, int y);

static int ASAP_MillisecondsToBlocks(int milliseconds);

static void ASAP_PutLittleEndian(uint8_t *buffer, int offset, int value);

static void ASAP_PutLittleEndians(uint8_t *buffer, int offset, int value1, int value2);

static int ASAP_PutWavMetadata(uint8_t *buffer, int offset, int fourCC, const char *value);

static int ASAP_GenerateAt(ASAP *self, uint8_t *buffer, int bufferOffset, int bufferLen, ASAPSampleFormat format);

static uint8_t const *ASAP6502_GetPlayerRoutine(const ASAPInfo *info);

struct DurationParser {
	const char *source;
	int position;
	int length;
};

static int DurationParser_ParseDigit(DurationParser *self, int max);

static int DurationParser_Parse(DurationParser *self, const char *s);

struct ASAPNativeModuleWriter {
	ASAPWriter *writer;
	uint8_t const *sourceModule;
	int sourceOffset;
	int addressDiff;
};

static int ASAPNativeModuleWriter_GetByte(const ASAPNativeModuleWriter *self, int offset);

static int ASAPNativeModuleWriter_GetWord(const ASAPNativeModuleWriter *self, int offset);

static bool ASAPNativeModuleWriter_Copy(const ASAPNativeModuleWriter *self, int endOffset);

static bool ASAPNativeModuleWriter_RelocateBytes(const ASAPNativeModuleWriter *self, int lowOffset, int highOffset, int count, int shift);

static bool ASAPNativeModuleWriter_RelocateLowHigh(const ASAPNativeModuleWriter *self, int count);

static bool ASAPNativeModuleWriter_RelocateWords(const ASAPNativeModuleWriter *self, int count);

static bool ASAPNativeModuleWriter_Write(ASAPNativeModuleWriter *self, const ASAPInfo *info, ASAPModuleType type, int moduleLen);

/**
 * Static methods for writing modules in different formats.
 */
struct ASAPWriter {
	uint8_t *output;
	int outputOffset;
	int outputEnd;
};
static void ASAPWriter_Construct(ASAPWriter *self);

static void ASAPWriter_TwoDigitsToString(uint8_t *result, int offset, int value);

static bool ASAPWriter_SecondsToString(uint8_t *result, int offset, int value);

static bool ASAPWriter_WriteByte(ASAPWriter *self, int value);

static bool ASAPWriter_WriteWord(ASAPWriter *self, int value);

static bool ASAPWriter_WriteBytes(ASAPWriter *self, uint8_t const *array, int startIndex, int endIndex);

static bool ASAPWriter_WriteString(ASAPWriter *self, const char *s);

static bool ASAPWriter_WriteDec(ASAPWriter *self, int value);

static bool ASAPWriter_WriteTextSapTag(ASAPWriter *self, const char *tag, const char *value);

static bool ASAPWriter_WriteDecSapTag(ASAPWriter *self, const char *tag, int value);

static bool ASAPWriter_WriteHexSapTag(ASAPWriter *self, const char *tag, int value);

static bool ASAPWriter_WriteSapHeader(ASAPWriter *self, const ASAPInfo *info, int type, int init, int player);

static bool ASAPWriter_WriteExecutableHeader(ASAPWriter *self, int *initAndPlayer, const ASAPInfo *info, int type, int init, int player);

static bool ASAPWriter_WritePlaTaxLda0(ASAPWriter *self);

static bool ASAPWriter_WriteCmcInit(ASAPWriter *self, int *initAndPlayer, const ASAPInfo *info);

static bool ASAPWriter_WriteExecutableFromSap(ASAPWriter *self, int *initAndPlayer, const ASAPInfo *info, int type, uint8_t const *module, int moduleLen);

static int ASAPWriter_WriteExecutableHeaderForSongPos(ASAPWriter *self, int *initAndPlayer, const ASAPInfo *info, int player, int codeForOneSong, int codeForManySongs, int playerOffset);

static bool ASAPWriter_WriteExecutable(ASAPWriter *self, int *initAndPlayer, const ASAPInfo *info, uint8_t const *module, int moduleLen);

static int ASAPWriter_PadXexInfo(uint8_t *dest, int offset, int endColumn);

static int ASAPWriter_FormatXexInfoText(uint8_t *dest, int destLen, int endColumn, const char *src, bool author);

static bool ASAPWriter_WriteXexInfoTextDl(ASAPWriter *self, int address, int len, int verticalScrollAt);

static bool ASAPWriter_WriteXexInfo(ASAPWriter *self, const ASAPInfo *info);

static bool ASAPWriter_WriteNative(ASAPWriter *self, const ASAPInfo *info, uint8_t const *module, int moduleLen);

struct FlashPackItem {
	FlashPackItemType type;
	int value;
};

static int FlashPackItem_WriteValueTo(const FlashPackItem *self, uint8_t *buffer, int index);

struct FlashPack {
	int memory[65536];
	uint8_t compressed[65536];
	int compressedLength;
	FlashPackItem items[64];
	int itemsCount;
};

static int FlashPack_FindHole(const FlashPack *self);

static int FlashPack_GetInnerFlags(const FlashPack *self, int index);

static void FlashPack_PutItems(FlashPack *self);

static void FlashPack_PutItem(FlashPack *self, FlashPackItemType type, int value);

static bool FlashPack_IsLiteralPreferred(const FlashPack *self);

static void FlashPack_CompressMemoryArea(FlashPack *self, int startAddress, int endAddress);

static void FlashPack_PutPoke(FlashPack *self, int address, int value);

static bool FlashPack_Compress(FlashPack *self, ASAPWriter *w);

static const uint8_t CiResource_cm3_obx[2022] = {
	255, 255, 0, 5, 223, 12, 76, 18, 11, 76, 120, 5, 76, 203, 7, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 160, 227, 237, 227, 160, 240, 236, 225,
	249, 229, 242, 160, 246, 160, 178, 174, 177, 160, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255,
	255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 128, 128, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 141, 110,
	5, 142, 111, 5, 140, 112, 5, 41, 112, 74, 74, 74, 170, 189, 148, 11,
	141, 169, 5, 189, 149, 11, 141, 170, 5, 169, 3, 141, 15, 210, 216, 165,
	254, 72, 165, 255, 72, 172, 112, 5, 174, 111, 5, 173, 110, 5, 32, 178,
	5, 104, 133, 255, 104, 133, 254, 96, 173, 118, 5, 133, 254, 173, 119, 5,
	133, 255, 160, 0, 138, 240, 28, 177, 254, 201, 143, 240, 4, 201, 239, 208,
	12, 202, 208, 9, 200, 192, 84, 176, 9, 152, 170, 16, 6, 200, 192, 84,
	144, 229, 96, 142, 104, 5, 32, 123, 6, 169, 0, 162, 9, 157, 69, 5,
	202, 16, 250, 141, 103, 5, 169, 1, 141, 113, 5, 169, 255, 141, 106, 5,
	173, 114, 5, 133, 254, 173, 115, 5, 133, 255, 160, 19, 177, 254, 170, 173,
	118, 5, 133, 254, 173, 119, 5, 133, 255, 172, 104, 5, 177, 254, 201, 207,
	208, 13, 152, 24, 105, 85, 168, 177, 254, 48, 15, 170, 76, 52, 6, 201,
	143, 240, 7, 201, 239, 240, 3, 136, 16, 226, 142, 108, 5, 142, 109, 5,
	96, 41, 15, 240, 245, 142, 221, 10, 142, 243, 10, 142, 2, 11, 140, 222,
	10, 140, 244, 10, 140, 3, 11, 96, 142, 114, 5, 134, 254, 140, 115, 5,
	132, 255, 24, 138, 105, 20, 141, 116, 5, 152, 105, 0, 141, 117, 5, 142,
	118, 5, 200, 200, 140, 119, 5, 160, 19, 177, 254, 141, 108, 5, 141, 109,
	5, 162, 8, 169, 0, 141, 113, 5, 157, 0, 210, 224, 3, 176, 8, 157,
	9, 5, 169, 255, 157, 57, 5, 202, 16, 233, 169, 128, 162, 3, 157, 75,
	5, 202, 16, 250, 96, 169, 1, 141, 113, 5, 169, 0, 240, 238, 41, 3,
	201, 3, 240, 240, 224, 64, 176, 236, 192, 26, 176, 232, 170, 169, 128, 157,
	75, 5, 169, 0, 157, 57, 5, 157, 60, 5, 157, 63, 5, 173, 111, 5,
	157, 12, 5, 173, 112, 5, 10, 10, 10, 133, 254, 24, 173, 114, 5, 105,
	48, 72, 173, 115, 5, 105, 1, 168, 104, 24, 101, 254, 157, 97, 5, 152,
	105, 0, 157, 100, 5, 24, 173, 114, 5, 105, 148, 133, 254, 173, 115, 5,
	105, 0, 133, 255, 173, 112, 5, 10, 109, 112, 5, 10, 168, 177, 254, 157,
	79, 5, 200, 177, 254, 157, 82, 5, 41, 7, 141, 110, 5, 200, 177, 254,
	157, 85, 5, 200, 177, 254, 157, 88, 5, 200, 177, 254, 157, 91, 5, 200,
	177, 254, 157, 94, 5, 160, 0, 173, 110, 5, 201, 3, 208, 2, 160, 2,
	201, 7, 208, 2, 160, 4, 185, 178, 11, 133, 254, 185, 179, 11, 133, 255,
	189, 85, 5, 74, 74, 74, 74, 24, 109, 111, 5, 141, 111, 5, 141, 194,
	7, 168, 173, 110, 5, 201, 7, 208, 15, 152, 10, 168, 177, 254, 157, 45,
	5, 200, 140, 111, 5, 76, 131, 7, 177, 254, 157, 45, 5, 189, 85, 5,
	41, 15, 24, 109, 111, 5, 141, 111, 5, 172, 111, 5, 173, 110, 5, 201,
	5, 8, 177, 254, 40, 240, 8, 221, 45, 5, 208, 3, 56, 233, 1, 157,
	48, 5, 189, 79, 5, 72, 41, 3, 168, 185, 184, 11, 157, 54, 5, 104,
	74, 74, 74, 74, 160, 62, 201, 15, 240, 16, 160, 55, 201, 14, 240, 10,
	160, 48, 201, 13, 240, 4, 24, 105, 0, 168, 185, 188, 11, 157, 51, 5,
	96, 216, 165, 252, 72, 165, 253, 72, 165, 254, 72, 165, 255, 72, 173, 113,
	5, 208, 3, 76, 5, 11, 173, 78, 5, 240, 3, 76, 110, 9, 173, 108,
	5, 205, 109, 5, 240, 3, 76, 91, 9, 173, 103, 5, 240, 3, 76, 220,
	8, 162, 2, 188, 75, 5, 48, 3, 157, 75, 5, 157, 69, 5, 202, 16,
	242, 173, 118, 5, 133, 252, 173, 119, 5, 133, 253, 172, 104, 5, 132, 254,
	204, 106, 5, 208, 25, 173, 107, 5, 240, 20, 173, 104, 5, 172, 105, 5,
	140, 104, 5, 206, 107, 5, 208, 232, 141, 104, 5, 168, 16, 226, 162, 0,
	177, 252, 201, 254, 208, 14, 172, 104, 5, 200, 196, 254, 240, 67, 140, 104,
	5, 76, 26, 8, 157, 66, 5, 24, 152, 105, 85, 168, 232, 224, 3, 144,
	223, 172, 104, 5, 177, 252, 16, 122, 201, 255, 240, 118, 74, 74, 74, 41,
	14, 170, 189, 164, 11, 141, 126, 8, 189, 165, 11, 141, 127, 8, 173, 67,
	5, 133, 255, 32, 147, 8, 140, 104, 5, 192, 85, 176, 4, 196, 254, 208,
	143, 164, 254, 140, 104, 5, 76, 5, 11, 32, 148, 6, 160, 255, 96, 48,
	251, 168, 96, 48, 247, 56, 152, 229, 255, 168, 96, 48, 239, 24, 152, 101,
	255, 168, 96, 48, 231, 141, 108, 5, 141, 109, 5, 200, 96, 48, 221, 173,
	68, 5, 48, 216, 141, 107, 5, 200, 140, 105, 5, 24, 152, 101, 255, 141,
	106, 5, 96, 136, 48, 10, 177, 252, 201, 143, 240, 4, 201, 239, 208, 243,
	200, 96, 162, 2, 189, 72, 5, 240, 5, 222, 72, 5, 16, 99, 189, 75,
	5, 208, 94, 188, 66, 5, 192, 64, 176, 87, 173, 116, 5, 133, 252, 173,
	117, 5, 133, 253, 177, 252, 133, 254, 24, 152, 105, 64, 168, 177, 252, 133,
	255, 37, 254, 201, 255, 240, 58, 188, 69, 5, 177, 254, 41, 192, 208, 12,
	177, 254, 41, 63, 157, 15, 5, 254, 69, 5, 16, 235, 201, 64, 208, 19,
	177, 254, 41, 63, 141, 111, 5, 189, 15, 5, 141, 112, 5, 32, 188, 6,
	76, 72, 9, 201, 128, 208, 10, 177, 254, 41, 63, 157, 72, 5, 254, 69,
	5, 202, 16, 144, 174, 103, 5, 232, 224, 48, 144, 2, 162, 0, 142, 103,
	5, 206, 109, 5, 208, 14, 173, 108, 5, 141, 109, 5, 173, 103, 5, 208,
	3, 238, 104, 5, 172, 48, 5, 173, 82, 5, 41, 7, 201, 5, 240, 4,
	201, 6, 208, 1, 136, 140, 39, 5, 160, 0, 201, 5, 240, 4, 201, 6,
	208, 2, 160, 2, 201, 7, 208, 2, 160, 40, 140, 44, 5, 162, 2, 189,
	82, 5, 41, 224, 157, 40, 5, 189, 97, 5, 133, 252, 189, 100, 5, 133,
	253, 189, 57, 5, 201, 255, 240, 54, 201, 15, 208, 32, 189, 63, 5, 240,
	45, 222, 63, 5, 189, 63, 5, 208, 37, 188, 9, 5, 240, 1, 136, 152,
	157, 9, 5, 189, 88, 5, 157, 63, 5, 76, 232, 9, 189, 57, 5, 74,
	168, 177, 252, 144, 4, 74, 74, 74, 74, 41, 15, 157, 9, 5, 188, 45,
	5, 189, 82, 5, 41, 7, 201, 1, 208, 31, 136, 152, 200, 221, 48, 5,
	8, 169, 1, 40, 208, 2, 10, 10, 61, 60, 5, 240, 12, 188, 48, 5,
	192, 255, 208, 5, 169, 0, 157, 9, 5, 152, 157, 36, 5, 169, 1, 141,
	110, 5, 189, 57, 5, 201, 15, 240, 56, 41, 7, 168, 185, 208, 12, 133,
	254, 189, 57, 5, 41, 8, 8, 138, 40, 24, 240, 2, 105, 3, 168, 185,
	91, 5, 37, 254, 240, 27, 189, 51, 5, 157, 36, 5, 142, 110, 5, 202,
	16, 8, 141, 39, 5, 169, 0, 141, 44, 5, 232, 189, 54, 5, 157, 40,
	5, 189, 57, 5, 41, 15, 201, 15, 240, 16, 254, 57, 5, 189, 57, 5,
	201, 15, 208, 6, 189, 88, 5, 157, 63, 5, 189, 75, 5, 16, 10, 189,
	9, 5, 208, 5, 169, 64, 157, 75, 5, 254, 60, 5, 160, 0, 189, 82,
	5, 74, 74, 74, 74, 144, 1, 136, 74, 144, 1, 200, 24, 152, 125, 45,
	5, 157, 45, 5, 189, 48, 5, 201, 255, 208, 2, 160, 0, 24, 152, 125,
	48, 5, 157, 48, 5, 202, 48, 3, 76, 153, 9, 173, 40, 5, 141, 43,
	5, 173, 82, 5, 41, 7, 170, 160, 3, 173, 110, 5, 240, 3, 188, 216,
	12, 152, 72, 185, 188, 12, 8, 41, 127, 170, 152, 41, 3, 10, 168, 189,
	36, 5, 153, 0, 210, 200, 189, 9, 5, 224, 3, 208, 3, 173, 9, 5,
	29, 40, 5, 40, 16, 2, 169, 0, 153, 0, 210, 104, 168, 136, 41, 3,
	208, 207, 160, 8, 173, 44, 5, 153, 0, 210, 24, 104, 133, 255, 104, 133,
	254, 104, 133, 253, 104, 133, 252, 96, 104, 170, 240, 78, 201, 2, 240, 6,
	104, 104, 202, 208, 251, 96, 165, 20, 197, 20, 240, 252, 173, 36, 2, 201,
	137, 208, 7, 173, 37, 2, 201, 11, 240, 230, 173, 36, 2, 141, 146, 11,
	173, 37, 2, 141, 147, 11, 169, 137, 141, 36, 2, 169, 11, 141, 37, 2,
	104, 104, 240, 3, 56, 233, 1, 141, 96, 11, 104, 168, 104, 170, 169, 112,
	32, 120, 5, 169, 0, 162, 0, 76, 120, 5, 165, 20, 197, 20, 240, 252,
	173, 36, 2, 201, 137, 208, 174, 173, 37, 2, 201, 11, 208, 167, 173, 146,
	11, 141, 36, 2, 173, 147, 11, 141, 37, 2, 169, 64, 76, 120, 5, 32,
	203, 7, 144, 3, 32, 120, 11, 76, 255, 255, 178, 5, 221, 5, 168, 6,
	59, 6, 123, 6, 148, 6, 159, 6, 82, 6, 147, 8, 153, 8, 157, 8,
	165, 8, 173, 8, 183, 8, 205, 8, 188, 11, 253, 11, 62, 12, 128, 160,
	32, 64, 255, 241, 228, 215, 203, 192, 181, 170, 161, 152, 143, 135, 127, 120,
	114, 107, 101, 95, 90, 85, 80, 75, 71, 67, 63, 60, 56, 53, 50, 47,
	44, 42, 39, 37, 35, 33, 31, 29, 28, 26, 24, 23, 22, 20, 19, 18,
	17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
	1, 0, 0, 0, 0, 0, 0, 242, 233, 218, 206, 191, 182, 170, 161, 152,
	143, 137, 128, 122, 113, 107, 101, 95, 0, 86, 80, 103, 96, 90, 85, 81,
	76, 72, 67, 63, 61, 57, 52, 51, 57, 45, 42, 40, 37, 36, 33, 31,
	30, 0, 0, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3,
	2, 1, 0, 0, 56, 11, 140, 10, 0, 10, 106, 9, 232, 8, 106, 8,
	239, 7, 128, 7, 8, 7, 174, 6, 70, 6, 230, 5, 149, 5, 65, 5,
	246, 4, 176, 4, 110, 4, 48, 4, 246, 3, 187, 3, 132, 3, 82, 3,
	34, 3, 244, 2, 200, 2, 160, 2, 122, 2, 85, 2, 52, 2, 20, 2,
	245, 1, 216, 1, 189, 1, 164, 1, 141, 1, 119, 1, 96, 1, 78, 1,
	56, 1, 39, 1, 21, 1, 6, 1, 247, 0, 232, 0, 219, 0, 207, 0,
	195, 0, 184, 0, 172, 0, 162, 0, 154, 0, 144, 0, 136, 0, 127, 0,
	120, 0, 112, 0, 106, 0, 100, 0, 94, 0, 87, 0, 82, 0, 50, 0,
	10, 0, 0, 1, 2, 131, 0, 1, 2, 3, 1, 0, 2, 131, 1, 0,
	2, 3, 1, 2, 128, 3, 128, 64, 32, 16, 8, 4, 2, 1, 3, 3,
	3, 3, 7, 11, 15, 19 };
static const uint8_t CiResource_cmc_obx[2019] = {
	255, 255, 0, 5, 220, 12, 76, 15, 11, 76, 120, 5, 76, 203, 7, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 160, 227, 237, 227, 160, 240, 236, 225,
	249, 229, 242, 160, 246, 160, 178, 174, 177, 160, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255,
	255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 128, 128, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 141, 110,
	5, 142, 111, 5, 140, 112, 5, 41, 112, 74, 74, 74, 170, 189, 145, 11,
	141, 169, 5, 189, 146, 11, 141, 170, 5, 169, 3, 141, 15, 210, 216, 165,
	254, 72, 165, 255, 72, 172, 112, 5, 174, 111, 5, 173, 110, 5, 32, 178,
	5, 104, 133, 255, 104, 133, 254, 96, 173, 118, 5, 133, 254, 173, 119, 5,
	133, 255, 160, 0, 138, 240, 28, 177, 254, 201, 143, 240, 4, 201, 239, 208,
	12, 202, 208, 9, 200, 192, 84, 176, 9, 152, 170, 16, 6, 200, 192, 84,
	144, 229, 96, 142, 104, 5, 32, 123, 6, 169, 0, 162, 9, 157, 69, 5,
	202, 16, 250, 141, 103, 5, 169, 1, 141, 113, 5, 169, 255, 141, 106, 5,
	173, 114, 5, 133, 254, 173, 115, 5, 133, 255, 160, 19, 177, 254, 170, 173,
	118, 5, 133, 254, 173, 119, 5, 133, 255, 172, 104, 5, 177, 254, 201, 207,
	208, 13, 152, 24, 105, 85, 168, 177, 254, 48, 15, 170, 76, 52, 6, 201,
	143, 240, 7, 201, 239, 240, 3, 136, 16, 226, 142, 108, 5, 142, 109, 5,
	96, 41, 15, 240, 245, 142, 218, 10, 142, 240, 10, 142, 255, 10, 140, 219,
	10, 140, 241, 10, 140, 0, 11, 96, 142, 114, 5, 134, 254, 140, 115, 5,
	132, 255, 24, 138, 105, 20, 141, 116, 5, 152, 105, 0, 141, 117, 5, 142,
	118, 5, 200, 200, 140, 119, 5, 160, 19, 177, 254, 141, 108, 5, 141, 109,
	5, 162, 8, 169, 0, 141, 113, 5, 157, 0, 210, 224, 3, 176, 8, 157,
	9, 5, 169, 255, 157, 57, 5, 202, 16, 233, 169, 128, 162, 3, 157, 75,
	5, 202, 16, 250, 96, 169, 1, 141, 113, 5, 169, 0, 240, 238, 41, 3,
	201, 3, 240, 240, 224, 64, 176, 236, 192, 26, 176, 232, 170, 169, 128, 157,
	75, 5, 169, 0, 157, 57, 5, 157, 60, 5, 157, 63, 5, 173, 111, 5,
	157, 12, 5, 173, 112, 5, 10, 10, 10, 133, 254, 24, 173, 114, 5, 105,
	48, 72, 173, 115, 5, 105, 1, 168, 104, 24, 101, 254, 157, 97, 5, 152,
	105, 0, 157, 100, 5, 24, 173, 114, 5, 105, 148, 133, 254, 173, 115, 5,
	105, 0, 133, 255, 173, 112, 5, 10, 109, 112, 5, 10, 168, 177, 254, 157,
	79, 5, 200, 177, 254, 157, 82, 5, 41, 7, 141, 110, 5, 200, 177, 254,
	157, 85, 5, 200, 177, 254, 157, 88, 5, 200, 177, 254, 157, 91, 5, 200,
	177, 254, 157, 94, 5, 160, 0, 173, 110, 5, 201, 3, 208, 2, 160, 2,
	201, 7, 208, 2, 160, 4, 185, 175, 11, 133, 254, 185, 176, 11, 133, 255,
	189, 85, 5, 74, 74, 74, 74, 24, 109, 111, 5, 141, 111, 5, 141, 194,
	7, 168, 173, 110, 5, 201, 7, 208, 15, 152, 10, 168, 177, 254, 157, 45,
	5, 200, 140, 111, 5, 76, 131, 7, 177, 254, 157, 45, 5, 189, 85, 5,
	41, 15, 24, 109, 111, 5, 141, 111, 5, 172, 111, 5, 173, 110, 5, 201,
	5, 8, 177, 254, 40, 240, 8, 221, 45, 5, 208, 3, 56, 233, 1, 157,
	48, 5, 189, 79, 5, 72, 41, 3, 168, 185, 181, 11, 157, 54, 5, 104,
	74, 74, 74, 74, 160, 62, 201, 15, 240, 16, 160, 55, 201, 14, 240, 10,
	160, 48, 201, 13, 240, 4, 24, 105, 0, 168, 185, 185, 11, 157, 51, 5,
	96, 216, 165, 252, 72, 165, 253, 72, 165, 254, 72, 165, 255, 72, 173, 113,
	5, 208, 3, 76, 2, 11, 173, 78, 5, 240, 3, 76, 107, 9, 173, 108,
	5, 205, 109, 5, 240, 3, 76, 88, 9, 173, 103, 5, 240, 3, 76, 220,
	8, 162, 2, 188, 75, 5, 48, 3, 157, 75, 5, 157, 69, 5, 202, 16,
	242, 173, 118, 5, 133, 252, 173, 119, 5, 133, 253, 172, 104, 5, 132, 254,
	204, 106, 5, 208, 25, 173, 107, 5, 240, 20, 173, 104, 5, 172, 105, 5,
	140, 104, 5, 206, 107, 5, 208, 232, 141, 104, 5, 168, 16, 226, 162, 0,
	177, 252, 201, 254, 208, 14, 172, 104, 5, 200, 196, 254, 240, 67, 140, 104,
	5, 76, 26, 8, 157, 66, 5, 24, 152, 105, 85, 168, 232, 224, 3, 144,
	223, 172, 104, 5, 177, 252, 16, 122, 201, 255, 240, 118, 74, 74, 74, 41,
	14, 170, 189, 161, 11, 141, 126, 8, 189, 162, 11, 141, 127, 8, 173, 67,
	5, 133, 255, 32, 147, 8, 140, 104, 5, 192, 85, 176, 4, 196, 254, 208,
	143, 164, 254, 140, 104, 5, 76, 2, 11, 32, 148, 6, 160, 255, 96, 48,
	251, 168, 96, 48, 247, 56, 152, 229, 255, 168, 96, 48, 239, 24, 152, 101,
	255, 168, 96, 48, 231, 141, 108, 5, 141, 109, 5, 200, 96, 48, 221, 173,
	68, 5, 48, 216, 141, 107, 5, 200, 140, 105, 5, 24, 152, 101, 255, 141,
	106, 5, 96, 136, 48, 10, 177, 252, 201, 143, 240, 4, 201, 239, 208, 243,
	200, 96, 162, 2, 189, 72, 5, 240, 5, 222, 72, 5, 16, 99, 189, 75,
	5, 208, 94, 188, 66, 5, 192, 64, 176, 87, 173, 116, 5, 133, 252, 173,
	117, 5, 133, 253, 177, 252, 133, 254, 24, 152, 105, 64, 168, 177, 252, 133,
	255, 37, 254, 201, 255, 240, 58, 188, 69, 5, 177, 254, 41, 192, 208, 12,
	177, 254, 41, 63, 157, 15, 5, 254, 69, 5, 16, 235, 201, 64, 208, 19,
	177, 254, 41, 63, 141, 111, 5, 189, 15, 5, 141, 112, 5, 32, 188, 6,
	76, 72, 9, 201, 128, 208, 10, 177, 254, 41, 63, 157, 72, 5, 254, 69,
	5, 202, 16, 144, 174, 103, 5, 232, 138, 41, 63, 141, 103, 5, 206, 109,
	5, 208, 14, 173, 108, 5, 141, 109, 5, 173, 103, 5, 208, 3, 238, 104,
	5, 172, 48, 5, 173, 82, 5, 41, 7, 201, 5, 240, 4, 201, 6, 208,
	1, 136, 140, 39, 5, 160, 0, 201, 5, 240, 4, 201, 6, 208, 2, 160,
	2, 201, 7, 208, 2, 160, 40, 140, 44, 5, 162, 2, 189, 82, 5, 41,
	224, 157, 40, 5, 189, 97, 5, 133, 252, 189, 100, 5, 133, 253, 189, 57,
	5, 201, 255, 240, 54, 201, 15, 208, 32, 189, 63, 5, 240, 45, 222, 63,
	5, 189, 63, 5, 208, 37, 188, 9, 5, 240, 1, 136, 152, 157, 9, 5,
	189, 88, 5, 157, 63, 5, 76, 229, 9, 189, 57, 5, 74, 168, 177, 252,
	144, 4, 74, 74, 74, 74, 41, 15, 157, 9, 5, 188, 45, 5, 189, 82,
	5, 41, 7, 201, 1, 208, 31, 136, 152, 200, 221, 48, 5, 8, 169, 1,
	40, 208, 2, 10, 10, 61, 60, 5, 240, 12, 188, 48, 5, 192, 255, 208,
	5, 169, 0, 157, 9, 5, 152, 157, 36, 5, 169, 1, 141, 110, 5, 189,
	57, 5, 201, 15, 240, 56, 41, 7, 168, 185, 205, 12, 133, 254, 189, 57,
	5, 41, 8, 8, 138, 40, 24, 240, 2, 105, 3, 168, 185, 91, 5, 37,
	254, 240, 27, 189, 51, 5, 157, 36, 5, 142, 110, 5, 202, 16, 8, 141,
	39, 5, 169, 0, 141, 44, 5, 232, 189, 54, 5, 157, 40, 5, 189, 57,
	5, 41, 15, 201, 15, 240, 16, 254, 57, 5, 189, 57, 5, 201, 15, 208,
	6, 189, 88, 5, 157, 63, 5, 189, 75, 5, 16, 10, 189, 9, 5, 208,
	5, 169, 64, 157, 75, 5, 254, 60, 5, 160, 0, 189, 82, 5, 74, 74,
	74, 74, 144, 1, 136, 74, 144, 1, 200, 24, 152, 125, 45, 5, 157, 45,
	5, 189, 48, 5, 201, 255, 208, 2, 160, 0, 24, 152, 125, 48, 5, 157,
	48, 5, 202, 48, 3, 76, 150, 9, 173, 40, 5, 141, 43, 5, 173, 82,
	5, 41, 7, 170, 160, 3, 173, 110, 5, 240, 3, 188, 213, 12, 152, 72,
	185, 185, 12, 8, 41, 127, 170, 152, 41, 3, 10, 168, 189, 36, 5, 153,
	0, 210, 200, 189, 9, 5, 224, 3, 208, 3, 173, 9, 5, 29, 40, 5,
	40, 16, 2, 169, 0, 153, 0, 210, 104, 168, 136, 41, 3, 208, 207, 160,
	8, 173, 44, 5, 153, 0, 210, 24, 104, 133, 255, 104, 133, 254, 104, 133,
	253, 104, 133, 252, 96, 104, 170, 240, 78, 201, 2, 240, 6, 104, 104, 202,
	208, 251, 96, 165, 20, 197, 20, 240, 252, 173, 36, 2, 201, 134, 208, 7,
	173, 37, 2, 201, 11, 240, 230, 173, 36, 2, 141, 143, 11, 173, 37, 2,
	141, 144, 11, 169, 134, 141, 36, 2, 169, 11, 141, 37, 2, 104, 104, 240,
	3, 56, 233, 1, 141, 93, 11, 104, 168, 104, 170, 169, 112, 32, 120, 5,
	169, 0, 162, 0, 76, 120, 5, 165, 20, 197, 20, 240, 252, 173, 36, 2,
	201, 134, 208, 174, 173, 37, 2, 201, 11, 208, 167, 173, 143, 11, 141, 36,
	2, 173, 144, 11, 141, 37, 2, 169, 64, 76, 120, 5, 32, 203, 7, 144,
	3, 32, 117, 11, 76, 255, 255, 178, 5, 221, 5, 168, 6, 59, 6, 123,
	6, 148, 6, 159, 6, 82, 6, 147, 8, 153, 8, 157, 8, 165, 8, 173,
	8, 183, 8, 205, 8, 185, 11, 250, 11, 59, 12, 128, 160, 32, 64, 255,
	241, 228, 215, 203, 192, 181, 170, 161, 152, 143, 135, 127, 120, 114, 107, 101,
	95, 90, 85, 80, 75, 71, 67, 63, 60, 56, 53, 50, 47, 44, 42, 39,
	37, 35, 33, 31, 29, 28, 26, 24, 23, 22, 20, 19, 18, 17, 16, 15,
	14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0,
	0, 0, 0, 0, 242, 233, 218, 206, 191, 182, 170, 161, 152, 143, 137, 128,
	122, 113, 107, 101, 95, 0, 86, 80, 103, 96, 90, 85, 81, 76, 72, 67,
	63, 61, 57, 52, 51, 57, 45, 42, 40, 37, 36, 33, 31, 30, 0, 0,
	15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
	0, 56, 11, 140, 10, 0, 10, 106, 9, 232, 8, 106, 8, 239, 7, 128,
	7, 8, 7, 174, 6, 70, 6, 230, 5, 149, 5, 65, 5, 246, 4, 176,
	4, 110, 4, 48, 4, 246, 3, 187, 3, 132, 3, 82, 3, 34, 3, 244,
	2, 200, 2, 160, 2, 122, 2, 85, 2, 52, 2, 20, 2, 245, 1, 216,
	1, 189, 1, 164, 1, 141, 1, 119, 1, 96, 1, 78, 1, 56, 1, 39,
	1, 21, 1, 6, 1, 247, 0, 232, 0, 219, 0, 207, 0, 195, 0, 184,
	0, 172, 0, 162, 0, 154, 0, 144, 0, 136, 0, 127, 0, 120, 0, 112,
	0, 106, 0, 100, 0, 94, 0, 87, 0, 82, 0, 50, 0, 10, 0, 0,
	1, 2, 131, 0, 1, 2, 3, 1, 0, 2, 131, 1, 0, 2, 3, 1,
	2, 128, 3, 128, 64, 32, 16, 8, 4, 2, 1, 3, 3, 3, 3, 7,
	11, 15, 19 };
static const uint8_t CiResource_cmr_obx[2019] = {
	255, 255, 0, 5, 220, 12, 76, 15, 11, 76, 120, 5, 76, 203, 7, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 160, 227, 237, 227, 160, 240, 236, 225,
	249, 229, 242, 160, 246, 160, 178, 174, 177, 160, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255,
	255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 128, 128, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 141, 110,
	5, 142, 111, 5, 140, 112, 5, 41, 112, 74, 74, 74, 170, 189, 145, 11,
	141, 169, 5, 189, 146, 11, 141, 170, 5, 169, 3, 141, 15, 210, 216, 165,
	254, 72, 165, 255, 72, 172, 112, 5, 174, 111, 5, 173, 110, 5, 32, 178,
	5, 104, 133, 255, 104, 133, 254, 96, 173, 118, 5, 133, 254, 173, 119, 5,
	133, 255, 160, 0, 138, 240, 28, 177, 254, 201, 143, 240, 4, 201, 239, 208,
	12, 202, 208, 9, 200, 192, 84, 176, 9, 152, 170, 16, 6, 200, 192, 84,
	144, 229, 96, 142, 104, 5, 32, 123, 6, 169, 0, 162, 9, 157, 69, 5,
	202, 16, 250, 141, 103, 5, 169, 1, 141, 113, 5, 169, 255, 141, 106, 5,
	173, 114, 5, 133, 254, 173, 115, 5, 133, 255, 160, 19, 177, 254, 170, 173,
	118, 5, 133, 254, 173, 119, 5, 133, 255, 172, 104, 5, 177, 254, 201, 207,
	208, 13, 152, 24, 105, 85, 168, 177, 254, 48, 15, 170, 76, 52, 6, 201,
	143, 240, 7, 201, 239, 240, 3, 136, 16, 226, 142, 108, 5, 142, 109, 5,
	96, 41, 15, 240, 245, 142, 218, 10, 142, 240, 10, 142, 255, 10, 140, 219,
	10, 140, 241, 10, 140, 0, 11, 96, 142, 114, 5, 134, 254, 140, 115, 5,
	132, 255, 24, 138, 105, 20, 141, 116, 5, 152, 105, 0, 141, 117, 5, 142,
	118, 5, 200, 200, 140, 119, 5, 160, 19, 177, 254, 141, 108, 5, 141, 109,
	5, 162, 8, 169, 0, 141, 113, 5, 157, 0, 210, 224, 3, 176, 8, 157,
	9, 5, 169, 255, 157, 57, 5, 202, 16, 233, 169, 128, 162, 3, 157, 75,
	5, 202, 16, 250, 96, 169, 1, 141, 113, 5, 169, 0, 240, 238, 41, 3,
	201, 3, 240, 240, 224, 64, 176, 236, 192, 26, 176, 232, 170, 169, 128, 157,
	75, 5, 169, 0, 157, 57, 5, 157, 60, 5, 157, 63, 5, 173, 111, 5,
	157, 12, 5, 173, 112, 5, 10, 10, 10, 133, 254, 24, 173, 114, 5, 105,
	48, 72, 173, 115, 5, 105, 1, 168, 104, 24, 101, 254, 157, 97, 5, 152,
	105, 0, 157, 100, 5, 24, 173, 114, 5, 105, 148, 133, 254, 173, 115, 5,
	105, 0, 133, 255, 173, 112, 5, 10, 109, 112, 5, 10, 168, 177, 254, 157,
	79, 5, 200, 177, 254, 157, 82, 5, 41, 7, 141, 110, 5, 200, 177, 254,
	157, 85, 5, 200, 177, 254, 157, 88, 5, 200, 177, 254, 157, 91, 5, 200,
	177, 254, 157, 94, 5, 160, 0, 173, 110, 5, 201, 3, 208, 2, 160, 2,
	201, 7, 208, 2, 160, 4, 185, 175, 11, 133, 254, 185, 176, 11, 133, 255,
	189, 85, 5, 74, 74, 74, 74, 24, 109, 111, 5, 141, 111, 5, 141, 194,
	7, 168, 173, 110, 5, 201, 7, 208, 15, 152, 10, 168, 177, 254, 157, 45,
	5, 200, 140, 111, 5, 76, 131, 7, 177, 254, 157, 45, 5, 189, 85, 5,
	41, 15, 24, 109, 111, 5, 141, 111, 5, 172, 111, 5, 173, 110, 5, 201,
	5, 8, 177, 254, 40, 240, 8, 221, 45, 5, 208, 3, 56, 233, 1, 157,
	48, 5, 189, 79, 5, 72, 41, 3, 168, 185, 181, 11, 157, 54, 5, 104,
	74, 74, 74, 74, 160, 62, 201, 15, 240, 16, 160, 55, 201, 14, 240, 10,
	160, 48, 201, 13, 240, 4, 24, 105, 0, 168, 185, 185, 11, 157, 51, 5,
	96, 216, 165, 252, 72, 165, 253, 72, 165, 254, 72, 165, 255, 72, 173, 113,
	5, 208, 3, 76, 2, 11, 173, 78, 5, 240, 3, 76, 107, 9, 173, 108,
	5, 205, 109, 5, 240, 3, 76, 88, 9, 173, 103, 5, 240, 3, 76, 220,
	8, 162, 2, 188, 75, 5, 48, 3, 157, 75, 5, 157, 69, 5, 202, 16,
	242, 173, 118, 5, 133, 252, 173, 119, 5, 133, 253, 172, 104, 5, 132, 254,
	204, 106, 5, 208, 25, 173, 107, 5, 240, 20, 173, 104, 5, 172, 105, 5,
	140, 104, 5, 206, 107, 5, 208, 232, 141, 104, 5, 168, 16, 226, 162, 0,
	177, 252, 201, 254, 208, 14, 172, 104, 5, 200, 196, 254, 240, 67, 140, 104,
	5, 76, 26, 8, 157, 66, 5, 24, 152, 105, 85, 168, 232, 224, 3, 144,
	223, 172, 104, 5, 177, 252, 16, 122, 201, 255, 240, 118, 74, 74, 74, 41,
	14, 170, 189, 161, 11, 141, 126, 8, 189, 162, 11, 141, 127, 8, 173, 67,
	5, 133, 255, 32, 147, 8, 140, 104, 5, 192, 85, 176, 4, 196, 254, 208,
	143, 164, 254, 140, 104, 5, 76, 2, 11, 32, 148, 6, 160, 255, 96, 48,
	251, 168, 96, 48, 247, 56, 152, 229, 255, 168, 96, 48, 239, 24, 152, 101,
	255, 168, 96, 48, 231, 141, 108, 5, 141, 109, 5, 200, 96, 48, 221, 173,
	68, 5, 48, 216, 141, 107, 5, 200, 140, 105, 5, 24, 152, 101, 255, 141,
	106, 5, 96, 136, 48, 10, 177, 252, 201, 143, 240, 4, 201, 239, 208, 243,
	200, 96, 162, 2, 189, 72, 5, 240, 5, 222, 72, 5, 16, 99, 189, 75,
	5, 208, 94, 188, 66, 5, 192, 64, 176, 87, 173, 116, 5, 133, 252, 173,
	117, 5, 133, 253, 177, 252, 133, 254, 24, 152, 105, 64, 168, 177, 252, 133,
	255, 37, 254, 201, 255, 240, 58, 188, 69, 5, 177, 254, 41, 192, 208, 12,
	177, 254, 41, 63, 157, 15, 5, 254, 69, 5, 16, 235, 201, 64, 208, 19,
	177, 254, 41, 63, 141, 111, 5, 189, 15, 5, 141, 112, 5, 32, 188, 6,
	76, 72, 9, 201, 128, 208, 10, 177, 254, 41, 63, 157, 72, 5, 254, 69,
	5, 202, 16, 144, 174, 103, 5, 232, 138, 41, 63, 141, 103, 5, 206, 109,
	5, 208, 14, 173, 108, 5, 141, 109, 5, 173, 103, 5, 208, 3, 238, 104,
	5, 172, 48, 5, 173, 82, 5, 41, 7, 201, 5, 240, 4, 201, 6, 208,
	1, 136, 140, 39, 5, 160, 0, 201, 5, 240, 4, 201, 6, 208, 2, 160,
	2, 201, 7, 208, 2, 160, 40, 140, 44, 5, 162, 2, 189, 82, 5, 41,
	224, 157, 40, 5, 189, 97, 5, 133, 252, 189, 100, 5, 133, 253, 189, 57,
	5, 201, 255, 240, 54, 201, 15, 208, 32, 189, 63, 5, 240, 45, 222, 63,
	5, 189, 63, 5, 208, 37, 188, 9, 5, 240, 1, 136, 152, 157, 9, 5,
	189, 88, 5, 157, 63, 5, 76, 229, 9, 189, 57, 5, 74, 168, 177, 252,
	144, 4, 74, 74, 74, 74, 41, 15, 157, 9, 5, 188, 45, 5, 189, 82,
	5, 41, 7, 201, 1, 208, 31, 136, 152, 200, 221, 48, 5, 8, 169, 1,
	40, 208, 2, 10, 10, 61, 60, 5, 240, 12, 188, 48, 5, 192, 255, 208,
	5, 169, 0, 157, 9, 5, 152, 157, 36, 5, 169, 1, 141, 110, 5, 189,
	57, 5, 201, 15, 240, 56, 41, 7, 168, 185, 205, 12, 133, 254, 189, 57,
	5, 41, 8, 8, 138, 40, 24, 240, 2, 105, 3, 168, 185, 91, 5, 37,
	254, 240, 27, 189, 51, 5, 157, 36, 5, 142, 110, 5, 202, 16, 8, 141,
	39, 5, 169, 0, 141, 44, 5, 232, 189, 54, 5, 157, 40, 5, 189, 57,
	5, 41, 15, 201, 15, 240, 16, 254, 57, 5, 189, 57, 5, 201, 15, 208,
	6, 189, 88, 5, 157, 63, 5, 189, 75, 5, 16, 10, 189, 9, 5, 208,
	5, 169, 64, 157, 75, 5, 254, 60, 5, 160, 0, 189, 82, 5, 74, 74,
	74, 74, 144, 1, 136, 74, 144, 1, 200, 24, 152, 125, 45, 5, 157, 45,
	5, 189, 48, 5, 201, 255, 208, 2, 160, 0, 24, 152, 125, 48, 5, 157,
	48, 5, 202, 48, 3, 76, 150, 9, 173, 40, 5, 141, 43, 5, 173, 82,
	5, 41, 7, 170, 160, 3, 173, 110, 5, 240, 3, 188, 213, 12, 152, 72,
	185, 185, 12, 8, 41, 127, 170, 152, 41, 3, 10, 168, 189, 36, 5, 153,
	0, 210, 200, 189, 9, 5, 224, 3, 208, 3, 173, 9, 5, 29, 40, 5,
	40, 16, 2, 169, 0, 153, 0, 210, 104, 168, 136, 41, 3, 208, 207, 160,
	8, 173, 44, 5, 153, 0, 210, 24, 104, 133, 255, 104, 133, 254, 104, 133,
	253, 104, 133, 252, 96, 104, 170, 240, 78, 201, 2, 240, 6, 104, 104, 202,
	208, 251, 96, 165, 20, 197, 20, 240, 252, 173, 36, 2, 201, 134, 208, 7,
	173, 37, 2, 201, 11, 240, 230, 173, 36, 2, 141, 143, 11, 173, 37, 2,
	141, 144, 11, 169, 134, 141, 36, 2, 169, 11, 141, 37, 2, 104, 104, 240,
	3, 56, 233, 1, 141, 93, 11, 104, 168, 104, 170, 169, 112, 32, 120, 5,
	169, 0, 162, 0, 76, 120, 5, 165, 20, 197, 20, 240, 252, 173, 36, 2,
	201, 134, 208, 174, 173, 37, 2, 201, 11, 208, 167, 173, 143, 11, 141, 36,
	2, 173, 144, 11, 141, 37, 2, 169, 64, 76, 120, 5, 32, 203, 7, 144,
	3, 32, 117, 11, 76, 255, 255, 178, 5, 221, 5, 168, 6, 59, 6, 123,
	6, 148, 6, 159, 6, 82, 6, 147, 8, 153, 8, 157, 8, 165, 8, 173,
	8, 183, 8, 205, 8, 185, 11, 250, 11, 59, 12, 128, 160, 32, 64, 255,
	241, 228, 215, 203, 192, 181, 170, 161, 152, 143, 135, 127, 120, 114, 107, 101,
	95, 90, 85, 80, 75, 71, 67, 63, 60, 56, 53, 50, 47, 44, 42, 39,
	37, 35, 33, 31, 29, 28, 26, 24, 23, 22, 20, 19, 18, 17, 16, 15,
	14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0,
	0, 0, 0, 0, 242, 233, 218, 206, 191, 182, 170, 161, 152, 143, 137, 128,
	122, 113, 107, 101, 95, 92, 86, 80, 77, 71, 68, 65, 62, 56, 53, 136,
	127, 121, 115, 108, 103, 96, 90, 85, 81, 76, 72, 67, 63, 61, 57, 52,
	51, 48, 45, 42, 40, 37, 36, 33, 31, 30, 5, 4, 3, 2, 1, 0,
	0, 56, 11, 140, 10, 0, 10, 106, 9, 232, 8, 106, 8, 239, 7, 128,
	7, 8, 7, 174, 6, 70, 6, 230, 5, 149, 5, 65, 5, 246, 4, 176,
	4, 110, 4, 48, 4, 246, 3, 187, 3, 132, 3, 82, 3, 34, 3, 244,
	2, 200, 2, 160, 2, 122, 2, 85, 2, 52, 2, 20, 2, 245, 1, 216,
	1, 189, 1, 164, 1, 141, 1, 119, 1, 96, 1, 78, 1, 56, 1, 39,
	1, 21, 1, 6, 1, 247, 0, 232, 0, 219, 0, 207, 0, 195, 0, 184,
	0, 172, 0, 162, 0, 154, 0, 144, 0, 136, 0, 127, 0, 120, 0, 112,
	0, 106, 0, 100, 0, 94, 0, 87, 0, 82, 0, 50, 0, 10, 0, 0,
	1, 2, 131, 0, 1, 2, 3, 1, 0, 2, 131, 1, 0, 2, 3, 1,
	2, 128, 3, 128, 64, 32, 16, 8, 4, 2, 1, 3, 3, 3, 3, 7,
	11, 15, 19 };
static const uint8_t CiResource_cms_obx[2757] = {
	255, 255, 0, 5, 190, 15, 234, 234, 234, 76, 21, 8, 76, 96, 15, 35,
	5, 169, 5, 173, 5, 184, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 128, 128, 128, 128, 128, 128, 0, 0, 0, 0, 0, 0, 255,
	255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 130, 0, 0, 6, 6, 0,
	128, 20, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15,
	15, 0, 0, 0, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0, 0, 0,
	1, 2, 131, 0, 1, 2, 3, 1, 0, 2, 131, 1, 0, 2, 3, 1,
	2, 128, 3, 128, 64, 32, 16, 8, 4, 2, 1, 75, 8, 118, 8, 133,
	9, 19, 9, 80, 9, 110, 9, 124, 9, 26, 9, 128, 160, 32, 64, 255,
	241, 228, 215, 203, 192, 181, 170, 161, 152, 143, 135, 127, 120, 114, 107, 101,
	95, 90, 85, 80, 75, 71, 67, 63, 60, 56, 53, 50, 47, 44, 42, 39,
	37, 35, 33, 31, 29, 28, 26, 24, 23, 22, 20, 19, 18, 17, 16, 15,
	14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0,
	0, 0, 0, 0, 242, 233, 218, 206, 191, 182, 170, 161, 152, 143, 137, 128,
	122, 113, 107, 101, 95, 0, 86, 80, 103, 96, 90, 85, 81, 76, 72, 67,
	63, 61, 57, 52, 51, 57, 45, 42, 40, 37, 36, 33, 31, 30, 0, 0,
	15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
	0, 56, 11, 140, 10, 0, 10, 106, 9, 232, 8, 106, 8, 239, 7, 128,
	7, 8, 7, 174, 6, 70, 6, 230, 5, 149, 5, 65, 5, 246, 4, 176,
	4, 110, 4, 48, 4, 246, 3, 187, 3, 132, 3, 82, 3, 34, 3, 244,
	2, 200, 2, 160, 2, 122, 2, 85, 2, 52, 2, 20, 2, 245, 1, 216,
	1, 189, 1, 164, 1, 141, 1, 119, 1, 96, 1, 78, 1, 56, 1, 39,
	1, 21, 1, 6, 1, 247, 0, 232, 0, 219, 0, 207, 0, 195, 0, 184,
	0, 172, 0, 162, 0, 154, 0, 144, 0, 136, 0, 127, 0, 120, 0, 112,
	0, 106, 0, 100, 0, 94, 0, 87, 0, 82, 0, 50, 0, 10, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 0,
	0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 0,
	0, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 0,
	0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 0,
	0, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 5, 5, 6, 6, 0,
	0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 0,
	1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 0,
	1, 1, 2, 2, 3, 4, 4, 5, 5, 6, 7, 7, 8, 8, 9, 0,
	1, 1, 2, 3, 3, 4, 5, 5, 6, 7, 7, 8, 9, 9, 10, 0,
	1, 1, 2, 3, 4, 4, 5, 6, 7, 7, 8, 9, 10, 10, 11, 0,
	1, 2, 2, 3, 4, 5, 6, 7, 8, 9, 9, 10, 11, 11, 12, 0,
	1, 2, 3, 4, 5, 5, 6, 7, 8, 9, 10, 10, 11, 12, 13, 0,
	1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14, 0,
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 6,
	12, 12, 12, 18, 12, 28, 12, 38, 12, 50, 12, 79, 12, 233, 5, 42,
	6, 107, 6, 161, 11, 196, 11, 185, 11, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 3, 3, 3, 3, 7, 11, 15, 19, 141, 143, 5, 142, 144,
	5, 140, 145, 5, 41, 112, 74, 74, 74, 170, 169, 3, 141, 15, 210, 189,
	213, 5, 141, 73, 8, 189, 214, 5, 141, 74, 8, 169, 3, 141, 31, 210,
	169, 1, 141, 146, 5, 172, 145, 5, 174, 144, 5, 173, 143, 5, 76, 72,
	8, 173, 147, 5, 133, 252, 173, 148, 5, 133, 253, 160, 0, 138, 240, 28,
	177, 252, 201, 143, 240, 4, 201, 239, 208, 12, 202, 208, 9, 200, 192, 84,
	176, 9, 152, 170, 16, 6, 200, 192, 84, 144, 229, 96, 142, 149, 5, 169,
	0, 162, 5, 157, 17, 5, 157, 23, 5, 157, 29, 5, 202, 16, 244, 141,
	150, 5, 141, 157, 5, 160, 255, 140, 159, 5, 173, 153, 5, 133, 252, 173,
	154, 5, 133, 253, 160, 19, 177, 252, 170, 173, 147, 5, 133, 252, 173, 148,
	5, 133, 253, 172, 149, 5, 152, 72, 169, 15, 141, 169, 5, 141, 170, 5,
	177, 252, 201, 135, 208, 35, 152, 72, 24, 105, 85, 168, 177, 252, 16, 2,
	169, 15, 41, 15, 141, 169, 5, 152, 24, 105, 85, 168, 177, 252, 16, 3,
	173, 169, 5, 41, 15, 141, 170, 5, 104, 76, 243, 8, 177, 252, 201, 143,
	240, 7, 201, 239, 240, 3, 136, 16, 199, 104, 168, 177, 252, 201, 207, 208,
	13, 152, 24, 105, 85, 168, 177, 252, 48, 15, 170, 76, 19, 9, 201, 143,
	240, 7, 201, 239, 240, 3, 136, 16, 226, 142, 151, 5, 142, 152, 5, 96,
	142, 153, 5, 134, 252, 140, 154, 5, 132, 253, 24, 138, 105, 20, 141, 155,
	5, 152, 105, 0, 141, 156, 5, 24, 138, 105, 0, 141, 147, 5, 152, 105,
	2, 141, 148, 5, 160, 19, 177, 252, 141, 151, 5, 141, 152, 5, 162, 3,
	142, 31, 210, 142, 15, 210, 169, 0, 141, 146, 5, 160, 8, 169, 0, 153,
	0, 210, 153, 16, 210, 192, 6, 176, 8, 153, 35, 5, 169, 255, 153, 41,
	5, 136, 16, 233, 169, 128, 162, 5, 157, 29, 5, 202, 16, 250, 141, 157,
	5, 96, 169, 0, 240, 240, 141, 157, 5, 240, 11, 173, 143, 5, 41, 7,
	170, 169, 128, 157, 29, 5, 172, 145, 5, 173, 144, 5, 141, 143, 5, 140,
	145, 5, 169, 0, 157, 83, 5, 157, 41, 5, 157, 77, 5, 152, 10, 10,
	10, 133, 254, 24, 173, 153, 5, 105, 48, 72, 173, 154, 5, 105, 1, 168,
	104, 24, 101, 254, 157, 101, 5, 152, 105, 0, 157, 71, 5, 24, 173, 153,
	5, 105, 148, 133, 252, 173, 154, 5, 105, 0, 133, 253, 173, 145, 5, 10,
	109, 145, 5, 10, 168, 140, 145, 5, 200, 200, 200, 200, 200, 177, 252, 157,
	113, 5, 136, 177, 252, 157, 107, 5, 136, 177, 252, 157, 119, 5, 136, 136,
	177, 252, 157, 59, 5, 160, 0, 41, 7, 201, 3, 208, 2, 160, 2, 201,
	7, 208, 2, 160, 4, 185, 247, 7, 133, 254, 185, 248, 7, 133, 255, 172,
	145, 5, 200, 200, 177, 252, 74, 74, 74, 74, 24, 109, 143, 5, 141, 143,
	5, 141, 159, 10, 168, 189, 59, 5, 41, 7, 201, 7, 208, 15, 152, 10,
	168, 177, 254, 157, 125, 5, 200, 140, 143, 5, 76, 92, 10, 177, 254, 157,
	125, 5, 172, 145, 5, 200, 200, 177, 252, 41, 15, 24, 109, 143, 5, 141,
	143, 5, 172, 143, 5, 189, 59, 5, 41, 7, 201, 5, 8, 177, 254, 40,
	240, 8, 221, 125, 5, 208, 3, 56, 233, 1, 157, 89, 5, 172, 145, 5,
	177, 252, 72, 41, 3, 168, 185, 229, 5, 157, 131, 5, 104, 74, 74, 74,
	74, 160, 62, 201, 15, 240, 16, 160, 55, 201, 14, 240, 10, 160, 48, 201,
	13, 240, 4, 24, 105, 50, 168, 185, 233, 5, 157, 137, 5, 96, 216, 165,
	252, 72, 165, 253, 72, 165, 254, 72, 165, 255, 72, 173, 146, 5, 208, 3,
	76, 51, 15, 173, 157, 5, 240, 3, 76, 229, 12, 173, 152, 5, 205, 151,
	5, 176, 3, 76, 210, 12, 173, 150, 5, 240, 3, 76, 158, 11, 162, 5,
	169, 0, 188, 29, 5, 48, 3, 157, 29, 5, 157, 17, 5, 202, 16, 242,
	173, 147, 5, 133, 252, 173, 148, 5, 133, 253, 172, 149, 5, 140, 161, 5,
	204, 159, 5, 208, 25, 173, 160, 5, 240, 20, 173, 149, 5, 172, 158, 5,
	140, 149, 5, 206, 160, 5, 208, 232, 141, 149, 5, 168, 16, 226, 162, 0,
	177, 252, 201, 254, 240, 28, 157, 53, 5, 230, 253, 177, 252, 198, 253, 201,
	254, 240, 15, 157, 56, 5, 24, 152, 105, 85, 168, 232, 224, 3, 144, 224,
	176, 34, 172, 149, 5, 200, 204, 161, 5, 240, 80, 140, 149, 5, 76, 250,
	10, 104, 41, 14, 170, 189, 253, 7, 141, 135, 11, 189, 254, 7, 141, 136,
	11, 76, 129, 11, 172, 149, 5, 177, 252, 16, 57, 201, 255, 240, 53, 74,
	74, 74, 72, 41, 1, 240, 218, 104, 41, 14, 170, 189, 233, 7, 141, 135,
	11, 189, 234, 7, 141, 136, 11, 173, 54, 5, 133, 254, 32, 134, 11, 140,
	149, 5, 192, 85, 176, 5, 204, 161, 5, 208, 179, 172, 161, 5, 140, 149,
	5, 76, 51, 15, 76, 94, 12, 165, 254, 48, 18, 41, 15, 141, 169, 5,
	173, 55, 5, 16, 3, 173, 169, 5, 41, 15, 141, 170, 5, 200, 96, 165,
	254, 48, 250, 41, 1, 141, 184, 5, 200, 96, 173, 179, 5, 48, 20, 206,
	180, 5, 208, 51, 169, 50, 141, 180, 5, 206, 179, 5, 208, 41, 206, 179,
	5, 200, 96, 165, 254, 48, 214, 141, 180, 5, 238, 180, 5, 165, 254, 48,
	204, 141, 180, 5, 238, 180, 5, 173, 55, 5, 141, 179, 5, 16, 5, 169,
	0, 141, 179, 5, 238, 179, 5, 104, 104, 76, 229, 12, 32, 110, 9, 160,
	255, 96, 165, 254, 48, 249, 168, 96, 165, 254, 48, 243, 56, 152, 229, 254,
	168, 96, 165, 254, 48, 233, 24, 152, 101, 254, 168, 96, 165, 254, 48, 223,
	141, 151, 5, 141, 152, 5, 200, 96, 165, 254, 48, 211, 173, 55, 5, 48,
	206, 200, 140, 158, 5, 24, 152, 101, 254, 141, 159, 5, 173, 55, 5, 141,
	160, 5, 192, 84, 96, 136, 48, 10, 177, 252, 201, 143, 240, 4, 201, 239,
	208, 243, 200, 96, 162, 5, 189, 23, 5, 240, 5, 222, 23, 5, 16, 91,
	189, 29, 5, 208, 86, 188, 53, 5, 192, 64, 176, 79, 173, 155, 5, 133,
	252, 173, 156, 5, 133, 253, 177, 252, 133, 254, 24, 152, 105, 64, 168, 177,
	252, 133, 255, 201, 255, 176, 52, 188, 17, 5, 177, 254, 41, 192, 208, 12,
	177, 254, 41, 63, 157, 47, 5, 254, 17, 5, 16, 235, 201, 64, 208, 13,
	177, 254, 41, 63, 188, 47, 5, 32, 150, 9, 76, 194, 12, 201, 128, 208,
	10, 177, 254, 41, 63, 157, 23, 5, 254, 17, 5, 202, 16, 152, 174, 150,
	5, 232, 138, 41, 63, 141, 150, 5, 206, 152, 5, 208, 14, 173, 151, 5,
	141, 152, 5, 173, 150, 5, 208, 3, 238, 149, 5, 172, 89, 5, 173, 59,
	5, 41, 7, 201, 5, 240, 4, 201, 6, 208, 1, 136, 140, 162, 5, 160,
	0, 201, 5, 240, 4, 201, 6, 208, 2, 160, 2, 201, 7, 208, 2, 160,
	40, 140, 164, 5, 172, 92, 5, 173, 62, 5, 41, 7, 201, 5, 240, 4,
	201, 6, 208, 1, 136, 140, 163, 5, 160, 0, 201, 5, 240, 4, 201, 6,
	208, 2, 160, 2, 201, 7, 208, 2, 160, 40, 140, 165, 5, 162, 5, 189,
	59, 5, 41, 224, 157, 65, 5, 189, 101, 5, 133, 252, 189, 71, 5, 133,
	253, 189, 41, 5, 201, 255, 240, 55, 201, 15, 208, 33, 189, 77, 5, 240,
	46, 222, 77, 5, 189, 77, 5, 208, 38, 188, 35, 5, 240, 1, 136, 152,
	157, 35, 5, 189, 119, 5, 157, 77, 5, 136, 76, 137, 13, 189, 41, 5,
	74, 168, 177, 252, 144, 4, 74, 74, 74, 74, 41, 15, 157, 35, 5, 188,
	125, 5, 189, 59, 5, 41, 7, 201, 1, 208, 31, 136, 152, 200, 221, 89,
	5, 8, 169, 1, 40, 208, 2, 10, 10, 61, 83, 5, 240, 12, 188, 89,
	5, 192, 255, 208, 5, 169, 0, 157, 35, 5, 152, 157, 95, 5, 169, 1,
	141, 168, 5, 189, 41, 5, 201, 15, 240, 76, 41, 7, 168, 185, 205, 5,
	133, 254, 189, 41, 5, 41, 8, 8, 138, 40, 24, 240, 2, 105, 6, 168,
	185, 107, 5, 37, 254, 240, 47, 189, 137, 5, 157, 95, 5, 142, 168, 5,
	202, 224, 2, 240, 15, 224, 255, 208, 22, 141, 162, 5, 169, 0, 141, 164,
	5, 76, 9, 14, 173, 140, 5, 141, 163, 5, 169, 0, 141, 165, 5, 232,
	189, 131, 5, 157, 65, 5, 189, 41, 5, 41, 15, 201, 15, 240, 18, 254,
	41, 5, 189, 41, 5, 41, 15, 201, 15, 208, 6, 189, 119, 5, 157, 77,
	5, 189, 29, 5, 16, 10, 189, 35, 5, 208, 5, 169, 64, 157, 29, 5,
	254, 83, 5, 160, 0, 189, 59, 5, 74, 74, 74, 74, 144, 1, 136, 74,
	144, 1, 200, 24, 152, 125, 125, 5, 157, 125, 5, 189, 89, 5, 201, 255,
	208, 2, 160, 0, 24, 152, 125, 89, 5, 157, 89, 5, 202, 48, 3, 76,
	57, 13, 32, 127, 15, 173, 65, 5, 141, 166, 5, 173, 68, 5, 141, 167,
	5, 173, 59, 5, 41, 7, 32, 185, 15, 152, 72, 185, 185, 5, 8, 41,
	127, 170, 152, 41, 3, 10, 168, 224, 3, 208, 3, 76, 200, 14, 189, 173,
	5, 208, 39, 189, 95, 5, 153, 0, 210, 189, 35, 5, 29, 65, 5, 40,
	16, 2, 169, 0, 153, 1, 210, 104, 168, 136, 41, 3, 240, 3, 76, 131,
	14, 173, 164, 5, 141, 8, 210, 76, 232, 14, 40, 76, 177, 14, 173, 173,
	5, 208, 23, 173, 162, 5, 153, 0, 210, 173, 35, 5, 13, 166, 5, 40,
	16, 2, 169, 0, 153, 1, 210, 76, 177, 14, 40, 76, 177, 14, 173, 62,
	5, 41, 7, 32, 185, 15, 152, 72, 185, 185, 5, 8, 41, 127, 170, 152,
	41, 3, 10, 168, 224, 3, 208, 3, 76, 64, 15, 189, 176, 5, 208, 30,
	189, 98, 5, 153, 16, 210, 189, 38, 5, 29, 68, 5, 40, 16, 2, 169,
	0, 153, 17, 210, 104, 168, 136, 41, 3, 240, 7, 76, 240, 14, 40, 76,
	30, 15, 173, 165, 5, 141, 24, 210, 24, 104, 133, 255, 104, 133, 254, 104,
	133, 253, 104, 133, 252, 96, 173, 176, 5, 208, 23, 173, 163, 5, 153, 16,
	210, 173, 38, 5, 13, 167, 5, 40, 16, 2, 169, 0, 153, 17, 210, 76,
	30, 15, 40, 76, 30, 15, 32, 168, 10, 176, 25, 173, 184, 5, 240, 20,
	173, 157, 5, 141, 183, 5, 169, 1, 141, 157, 5, 32, 168, 10, 173, 183,
	5, 141, 157, 5, 96, 173, 169, 5, 10, 10, 10, 10, 141, 171, 5, 173,
	170, 5, 10, 10, 10, 10, 141, 172, 5, 162, 2, 134, 200, 173, 171, 5,
	29, 35, 5, 170, 189, 233, 6, 166, 200, 157, 35, 5, 173, 172, 5, 29,
	38, 5, 170, 189, 233, 6, 166, 200, 157, 38, 5, 202, 16, 221, 96, 168,
	185, 13, 8, 168, 96 };
static const uint8_t CiResource_dlt_obx[2125] = {
	255, 255, 0, 4, 70, 12, 255, 241, 228, 215, 203, 192, 181, 170, 161, 152,
	143, 135, 127, 121, 114, 107, 101, 95, 90, 85, 80, 75, 71, 67, 63, 60,
	56, 53, 50, 47, 44, 42, 39, 37, 35, 33, 31, 29, 28, 26, 24, 23,
	22, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6,
	5, 4, 255, 241, 228, 215, 242, 233, 218, 206, 191, 182, 170, 161, 152, 143,
	137, 128, 122, 113, 107, 101, 95, 92, 86, 80, 103, 96, 90, 85, 81, 76,
	72, 67, 63, 61, 57, 52, 51, 48, 45, 42, 40, 37, 36, 33, 31, 30,
	28, 27, 25, 0, 22, 21, 0, 10, 9, 8, 7, 6, 5, 4, 3, 2,
	1, 0, 242, 233, 218, 206, 242, 233, 218, 206, 191, 182, 170, 161, 152, 143,
	137, 128, 122, 113, 107, 101, 95, 92, 86, 80, 103, 96, 90, 85, 81, 76,
	72, 67, 63, 61, 57, 52, 51, 48, 45, 42, 40, 37, 36, 33, 31, 30,
	28, 27, 25, 0, 22, 21, 0, 10, 9, 8, 7, 6, 5, 4, 3, 2,
	1, 0, 242, 233, 218, 206, 255, 241, 228, 216, 202, 192, 181, 171, 162, 153,
	142, 135, 127, 120, 115, 108, 102, 97, 90, 85, 81, 75, 72, 67, 63, 60,
	57, 52, 51, 48, 45, 42, 40, 37, 36, 33, 31, 30, 28, 27, 25, 23,
	22, 21, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6,
	5, 4, 3, 2, 1, 255, 76, 9, 5, 76, 200, 5, 76, 183, 5, 136,
	140, 54, 3, 169, 126, 141, 53, 3, 162, 6, 142, 51, 3, 162, 1, 142,
	52, 3, 32, 51, 5, 32, 95, 5, 32, 163, 5, 32, 139, 5, 169, 1,
	141, 50, 3, 169, 3, 141, 15, 210, 96, 162, 0, 160, 32, 142, 48, 3,
	140, 49, 3, 160, 0, 173, 48, 3, 153, 0, 2, 173, 49, 3, 153, 64,
	2, 173, 48, 3, 24, 105, 128, 141, 48, 3, 144, 3, 238, 49, 3, 200,
	192, 64, 208, 225, 96, 162, 0, 160, 68, 142, 48, 3, 140, 49, 3, 160,
	0, 173, 48, 3, 153, 128, 2, 173, 49, 3, 153, 160, 2, 173, 48, 3,
	24, 105, 64, 141, 48, 3, 144, 3, 238, 49, 3, 200, 192, 32, 208, 225,
	96, 173, 0, 76, 41, 1, 74, 106, 106, 168, 162, 0, 185, 128, 4, 157,
	64, 4, 200, 232, 224, 64, 208, 244, 96, 160, 3, 169, 0, 153, 40, 3,
	153, 32, 3, 153, 36, 3, 153, 44, 3, 136, 16, 241, 96, 169, 0, 141,
	50, 3, 160, 7, 169, 0, 153, 0, 210, 136, 16, 250, 96, 96, 173, 50,
	3, 240, 250, 173, 40, 3, 13, 41, 3, 13, 42, 3, 13, 43, 3, 141,
	8, 210, 174, 36, 3, 172, 32, 3, 142, 0, 210, 140, 1, 210, 174, 37,
	3, 172, 33, 3, 142, 2, 210, 140, 3, 210, 174, 38, 3, 172, 34, 3,
	142, 4, 210, 140, 5, 210, 174, 39, 3, 172, 35, 3, 142, 6, 210, 140,
	7, 210, 206, 52, 3, 208, 74, 173, 51, 3, 141, 52, 3, 238, 53, 3,
	238, 53, 3, 16, 28, 238, 54, 3, 169, 0, 141, 53, 3, 32, 199, 6,
	173, 4, 3, 13, 5, 3, 13, 6, 3, 13, 7, 3, 208, 3, 76, 183,
	5, 173, 4, 3, 240, 3, 32, 97, 7, 173, 5, 3, 240, 3, 32, 192,
	7, 173, 6, 3, 240, 3, 32, 31, 8, 173, 7, 3, 240, 3, 32, 126,
	8, 173, 4, 3, 240, 8, 173, 44, 3, 240, 3, 32, 221, 8, 173, 5,
	3, 240, 8, 173, 45, 3, 240, 3, 32, 206, 9, 173, 6, 3, 240, 8,
	173, 46, 3, 240, 3, 32, 191, 10, 173, 7, 3, 240, 8, 173, 47, 3,
	240, 3, 32, 131, 11, 96, 192, 67, 144, 14, 169, 0, 141, 4, 3, 141,
	32, 3, 141, 40, 3, 76, 230, 6, 192, 66, 208, 15, 189, 128, 64, 141,
	51, 3, 141, 52, 3, 238, 54, 3, 76, 199, 6, 192, 65, 208, 9, 189,
	128, 64, 141, 54, 3, 76, 199, 6, 104, 104, 76, 183, 5, 174, 54, 3,
	188, 0, 64, 192, 64, 176, 191, 189, 128, 64, 141, 24, 3, 185, 0, 2,
	133, 224, 185, 64, 2, 133, 225, 169, 1, 141, 4, 3, 188, 0, 65, 192,
	64, 176, 78, 189, 128, 65, 141, 25, 3, 185, 0, 2, 133, 226, 185, 64,
	2, 133, 227, 169, 1, 141, 5, 3, 188, 0, 66, 192, 64, 176, 63, 189,
	128, 66, 141, 26, 3, 185, 0, 2, 133, 228, 185, 64, 2, 133, 229, 169,
	1, 141, 6, 3, 188, 0, 67, 192, 64, 176, 48, 189, 128, 67, 141, 27,
	3, 185, 0, 2, 133, 230, 185, 64, 2, 133, 231, 169, 1, 141, 7, 3,
	96, 169, 0, 141, 5, 3, 141, 33, 3, 141, 41, 3, 240, 186, 169, 0,
	141, 6, 3, 141, 34, 3, 141, 42, 3, 240, 201, 169, 0, 141, 7, 3,
	141, 35, 3, 141, 43, 3, 96, 172, 53, 3, 177, 224, 48, 11, 200, 177,
	224, 48, 1, 96, 104, 104, 76, 31, 6, 24, 109, 24, 3, 41, 127, 141,
	8, 3, 169, 15, 141, 0, 3, 141, 44, 3, 200, 177, 224, 170, 189, 160,
	2, 133, 233, 133, 241, 133, 249, 189, 128, 2, 133, 232, 73, 16, 133, 240,
	73, 48, 133, 248, 160, 49, 177, 232, 141, 12, 3, 160, 51, 177, 232, 41,
	127, 141, 16, 3, 169, 0, 141, 20, 3, 141, 28, 3, 160, 48, 177, 232,
	41, 213, 141, 40, 3, 96, 172, 53, 3, 177, 226, 48, 11, 200, 177, 226,
	48, 1, 96, 104, 104, 76, 31, 6, 24, 109, 25, 3, 41, 127, 141, 9,
	3, 169, 15, 141, 1, 3, 141, 45, 3, 200, 177, 226, 170, 189, 160, 2,
	133, 235, 133, 243, 133, 251, 189, 128, 2, 133, 234, 73, 16, 133, 242, 73,
	48, 133, 250, 160, 49, 177, 234, 141, 13, 3, 160, 51, 177, 234, 41, 127,
	141, 17, 3, 169, 0, 141, 21, 3, 141, 29, 3, 160, 48, 177, 234, 41,
	131, 141, 41, 3, 96, 172, 53, 3, 177, 228, 48, 11, 200, 177, 228, 48,
	1, 96, 104, 104, 76, 31, 6, 24, 109, 26, 3, 41, 127, 141, 10, 3,
	169, 15, 141, 2, 3, 141, 46, 3, 200, 177, 228, 170, 189, 160, 2, 133,
	237, 133, 245, 133, 253, 189, 128, 2, 133, 236, 73, 16, 133, 244, 73, 48,
	133, 252, 160, 49, 177, 236, 141, 14, 3, 160, 51, 177, 236, 41, 127, 141,
	18, 3, 169, 0, 141, 22, 3, 141, 30, 3, 160, 48, 177, 236, 41, 169,
	141, 42, 3, 96, 172, 53, 3, 177, 230, 48, 11, 200, 177, 230, 48, 1,
	96, 104, 104, 76, 31, 6, 24, 109, 27, 3, 41, 127, 141, 11, 3, 169,
	15, 141, 3, 3, 141, 47, 3, 200, 177, 230, 170, 189, 160, 2, 133, 239,
	133, 247, 133, 255, 189, 128, 2, 133, 238, 73, 16, 133, 246, 73, 48, 133,
	254, 160, 49, 177, 238, 141, 15, 3, 160, 51, 177, 238, 41, 127, 141, 19,
	3, 169, 0, 141, 23, 3, 141, 31, 3, 160, 48, 177, 238, 41, 129, 141,
	43, 3, 96, 172, 0, 3, 48, 70, 177, 232, 141, 32, 3, 177, 240, 208,
	9, 32, 108, 9, 206, 0, 3, 76, 79, 9, 201, 1, 240, 39, 201, 3,
	208, 16, 173, 8, 3, 24, 113, 248, 170, 173, 28, 3, 141, 55, 3, 76,
	24, 9, 173, 28, 3, 24, 113, 248, 141, 55, 3, 174, 8, 3, 32, 150,
	9, 206, 0, 3, 96, 177, 248, 141, 36, 3, 206, 0, 3, 96, 32, 108,
	9, 160, 49, 177, 232, 240, 30, 206, 12, 3, 240, 3, 76, 79, 9, 173,
	32, 3, 41, 15, 240, 11, 206, 32, 3, 177, 232, 141, 12, 3, 76, 79,
	9, 141, 44, 3, 96, 173, 28, 3, 24, 160, 50, 113, 232, 141, 28, 3,
	206, 16, 3, 208, 12, 238, 20, 3, 160, 51, 177, 232, 41, 127, 141, 16,
	3, 96, 173, 20, 3, 41, 3, 24, 105, 52, 168, 177, 232, 170, 160, 51,
	177, 232, 48, 14, 138, 109, 8, 3, 170, 173, 28, 3, 141, 55, 3, 76,
	150, 9, 138, 109, 28, 3, 141, 55, 3, 174, 8, 3, 189, 0, 4, 24,
	109, 55, 3, 141, 36, 3, 173, 40, 3, 41, 4, 208, 1, 96, 172, 0,
	3, 177, 240, 208, 21, 138, 24, 160, 0, 113, 248, 170, 189, 0, 4, 24,
	109, 55, 3, 24, 105, 255, 141, 38, 3, 96, 173, 36, 3, 24, 105, 255,
	141, 38, 3, 96, 172, 1, 3, 48, 70, 177, 234, 141, 33, 3, 177, 242,
	208, 9, 32, 93, 10, 206, 1, 3, 76, 64, 10, 201, 1, 240, 39, 201,
	3, 208, 16, 173, 9, 3, 24, 113, 250, 170, 173, 29, 3, 141, 55, 3,
	76, 9, 10, 173, 29, 3, 24, 113, 250, 141, 55, 3, 174, 9, 3, 32,
	135, 10, 206, 1, 3, 96, 177, 250, 141, 37, 3, 206, 1, 3, 96, 32,
	93, 10, 160, 49, 177, 234, 240, 30, 206, 13, 3, 240, 3, 76, 64, 10,
	173, 33, 3, 41, 15, 240, 11, 206, 33, 3, 177, 234, 141, 13, 3, 76,
	64, 10, 141, 45, 3, 96, 173, 29, 3, 24, 160, 50, 113, 234, 141, 29,
	3, 206, 17, 3, 208, 12, 238, 21, 3, 160, 51, 177, 234, 41, 127, 141,
	17, 3, 96, 173, 21, 3, 41, 3, 24, 105, 52, 168, 177, 234, 170, 160,
	51, 177, 234, 48, 14, 138, 109, 9, 3, 170, 173, 29, 3, 141, 55, 3,
	76, 135, 10, 138, 109, 29, 3, 141, 55, 3, 174, 9, 3, 189, 0, 4,
	24, 109, 55, 3, 141, 37, 3, 173, 41, 3, 41, 2, 208, 1, 96, 172,
	1, 3, 177, 242, 208, 21, 138, 24, 160, 0, 113, 250, 170, 189, 0, 4,
	24, 109, 55, 3, 24, 105, 255, 141, 39, 3, 96, 173, 37, 3, 24, 105,
	255, 141, 39, 3, 96, 172, 2, 3, 48, 70, 177, 236, 141, 34, 3, 177,
	244, 208, 9, 32, 78, 11, 206, 2, 3, 76, 49, 11, 201, 1, 240, 39,
	201, 3, 208, 16, 173, 10, 3, 24, 113, 252, 170, 173, 30, 3, 141, 55,
	3, 76, 250, 10, 173, 30, 3, 24, 113, 252, 141, 55, 3, 174, 10, 3,
	32, 120, 11, 206, 2, 3, 96, 177, 252, 141, 38, 3, 206, 2, 3, 96,
	32, 78, 11, 160, 49, 177, 236, 240, 30, 206, 14, 3, 240, 3, 76, 49,
	11, 173, 34, 3, 41, 15, 240, 11, 206, 34, 3, 177, 236, 141, 14, 3,
	76, 49, 11, 141, 46, 3, 96, 173, 30, 3, 24, 160, 50, 113, 236, 141,
	30, 3, 206, 18, 3, 208, 12, 238, 22, 3, 160, 51, 177, 236, 41, 127,
	141, 18, 3, 96, 173, 22, 3, 41, 3, 24, 105, 52, 168, 177, 236, 170,
	160, 51, 177, 236, 48, 14, 138, 109, 10, 3, 170, 173, 30, 3, 141, 55,
	3, 76, 120, 11, 138, 109, 30, 3, 141, 55, 3, 174, 10, 3, 189, 0,
	4, 24, 109, 55, 3, 141, 38, 3, 96, 172, 3, 3, 48, 70, 177, 238,
	141, 35, 3, 177, 246, 208, 9, 32, 18, 12, 206, 3, 3, 76, 245, 11,
	201, 1, 240, 39, 201, 3, 208, 16, 173, 11, 3, 24, 113, 254, 170, 173,
	31, 3, 141, 55, 3, 76, 190, 11, 173, 31, 3, 24, 113, 254, 141, 55,
	3, 174, 11, 3, 32, 60, 12, 206, 3, 3, 96, 177, 254, 141, 39, 3,
	206, 3, 3, 96, 32, 18, 12, 160, 49, 177, 238, 240, 30, 206, 15, 3,
	240, 3, 76, 245, 11, 173, 35, 3, 41, 15, 240, 11, 206, 35, 3, 177,
	238, 141, 15, 3, 76, 245, 11, 141, 47, 3, 96, 173, 31, 3, 24, 160,
	50, 113, 238, 141, 31, 3, 206, 19, 3, 208, 12, 238, 23, 3, 160, 51,
	177, 238, 41, 127, 141, 19, 3, 96, 173, 23, 3, 41, 3, 24, 105, 52,
	168, 177, 238, 170, 160, 51, 177, 238, 48, 14, 138, 109, 11, 3, 170, 173,
	31, 3, 141, 55, 3, 76, 60, 12, 138, 109, 31, 3, 141, 55, 3, 174,
	11, 3, 189, 0, 4, 24, 109, 55, 3, 141, 39, 3, 96 };
static const uint8_t CiResource_fc_obx[1220] = {
	255, 255, 0, 4, 189, 8, 76, 9, 4, 32, 16, 4, 76, 173, 5, 162,
	0, 160, 10, 32, 25, 5, 162, 8, 189, 172, 8, 157, 0, 210, 157, 16,
	210, 202, 16, 244, 96, 133, 244, 162, 0, 134, 230, 134, 232, 134, 234, 169,
	0, 133, 231, 133, 233, 133, 235, 133, 236, 133, 238, 133, 240, 133, 237, 133,
	239, 133, 241, 138, 160, 2, 162, 4, 32, 9, 5, 136, 202, 202, 16, 248,
	162, 4, 32, 160, 4, 202, 202, 16, 249, 169, 255, 197, 230, 240, 34, 197,
	232, 240, 30, 197, 234, 240, 26, 169, 254, 160, 0, 209, 224, 240, 18, 209,
	226, 240, 14, 209, 228, 240, 10, 177, 224, 49, 226, 49, 228, 201, 255, 208,
	207, 162, 4, 32, 153, 4, 202, 202, 16, 249, 166, 230, 228, 232, 176, 2,
	166, 232, 228, 234, 176, 2, 166, 234, 232, 198, 244, 208, 140, 138, 96, 181,
	231, 240, 2, 246, 230, 96, 161, 224, 201, 254, 176, 29, 181, 230, 201, 255,
	240, 23, 181, 237, 208, 17, 161, 224, 201, 64, 144, 28, 208, 3, 32, 196,
	4, 32, 196, 4, 76, 160, 4, 214, 237, 96, 246, 224, 208, 2, 246, 225,
	180, 230, 200, 240, 2, 246, 230, 96, 168, 185, 191, 8, 133, 242, 185, 31,
	9, 133, 243, 180, 231, 246, 231, 177, 242, 201, 64, 144, 22, 201, 96, 144,
	23, 201, 255, 144, 238, 169, 0, 149, 237, 149, 236, 149, 231, 32, 196, 4,
	76, 160, 4, 181, 236, 149, 237, 96, 41, 31, 149, 236, 76, 221, 4, 72,
	24, 121, 159, 9, 149, 224, 169, 0, 121, 162, 9, 149, 225, 104, 96, 134,
	252, 132, 253, 72, 160, 2, 177, 252, 162, 0, 157, 127, 9, 24, 113, 252,
	232, 224, 32, 144, 245, 165, 252, 166, 253, 24, 105, 3, 144, 1, 232, 141,
	159, 9, 142, 162, 9, 232, 141, 160, 9, 142, 163, 9, 232, 141, 161, 9,
	142, 164, 9, 232, 133, 254, 134, 255, 24, 105, 128, 144, 1, 232, 133, 252,
	134, 253, 162, 0, 160, 0, 165, 252, 153, 191, 8, 165, 253, 153, 31, 9,
	161, 252, 230, 252, 208, 2, 230, 253, 201, 255, 208, 244, 200, 192, 64, 144,
	229, 162, 0, 165, 252, 157, 255, 8, 165, 253, 157, 95, 9, 160, 0, 177,
	252, 200, 201, 255, 208, 249, 152, 24, 101, 252, 133, 252, 144, 2, 230, 253,
	232, 224, 32, 144, 222, 104, 240, 3, 32, 31, 4, 141, 190, 8, 169, 64,
	141, 170, 8, 44, 170, 8, 48, 39, 80, 53, 169, 0, 162, 11, 157, 125,
	8, 202, 16, 250, 141, 170, 8, 162, 2, 169, 8, 157, 167, 8, 173, 190,
	8, 157, 122, 8, 202, 16, 242, 169, 80, 141, 180, 8, 76, 233, 5, 169,
	0, 162, 7, 157, 172, 8, 202, 16, 250, 169, 128, 141, 170, 8, 96, 238,
	171, 8, 162, 2, 134, 30, 232, 138, 10, 168, 132, 31, 202, 189, 159, 9,
	133, 28, 189, 162, 9, 133, 29, 189, 128, 8, 240, 3, 76, 240, 6, 188,
	122, 8, 177, 28, 201, 64, 240, 14, 201, 255, 240, 24, 201, 254, 208, 45,
	141, 170, 8, 76, 203, 7, 200, 177, 28, 141, 180, 8, 200, 152, 157, 122,
	8, 76, 9, 6, 173, 190, 8, 157, 122, 8, 169, 80, 141, 180, 8, 169,
	0, 157, 128, 8, 157, 131, 8, 157, 125, 8, 76, 9, 6, 72, 41, 128,
	240, 11, 254, 122, 8, 104, 41, 15, 157, 167, 8, 16, 178, 104, 168, 185,
	191, 8, 133, 28, 185, 31, 9, 133, 29, 189, 131, 8, 157, 128, 8, 169,
	0, 157, 158, 8, 188, 125, 8, 177, 28, 48, 7, 10, 48, 36, 74, 76,
	177, 6, 10, 10, 168, 177, 254, 157, 137, 8, 200, 177, 254, 157, 143, 8,
	200, 177, 254, 157, 146, 8, 200, 177, 254, 157, 149, 8, 254, 125, 8, 76,
	110, 6, 74, 41, 31, 170, 189, 127, 9, 166, 30, 157, 128, 8, 157, 131,
	8, 254, 125, 8, 76, 110, 6, 157, 155, 8, 24, 125, 167, 8, 168, 224,
	0, 208, 15, 185, 242, 7, 141, 172, 8, 185, 50, 8, 141, 174, 8, 76,
	215, 6, 185, 196, 7, 157, 134, 8, 164, 31, 153, 172, 8, 169, 0, 157,
	140, 8, 254, 125, 8, 188, 125, 8, 177, 28, 201, 255, 208, 8, 169, 0,
	157, 125, 8, 254, 122, 8, 189, 137, 8, 168, 185, 255, 8, 141, 6, 7,
	185, 95, 9, 141, 7, 7, 166, 30, 188, 140, 8, 185, 255, 255, 201, 255,
	240, 39, 16, 16, 164, 31, 153, 173, 8, 169, 0, 153, 172, 8, 254, 140,
	8, 76, 148, 7, 164, 31, 254, 140, 8, 29, 143, 8, 153, 173, 8, 224,
	0, 240, 6, 189, 134, 8, 153, 172, 8, 189, 146, 8, 240, 35, 173, 171,
	8, 41, 1, 208, 5, 189, 146, 8, 208, 2, 169, 0, 141, 78, 7, 24,
	189, 155, 8, 105, 2, 125, 167, 8, 168, 185, 196, 7, 164, 31, 153, 172,
	8, 189, 149, 8, 240, 52, 48, 32, 188, 152, 8, 254, 152, 8, 185, 181,
	8, 201, 31, 208, 7, 169, 0, 157, 152, 8, 240, 236, 24, 125, 134, 8,
	164, 31, 153, 172, 8, 76, 148, 7, 189, 134, 8, 201, 255, 240, 11, 254,
	134, 8, 254, 134, 8, 164, 31, 153, 172, 8, 189, 158, 8, 240, 41, 189,
	161, 8, 240, 6, 222, 161, 8, 76, 194, 7, 189, 158, 8, 16, 10, 24,
	189, 134, 8, 125, 164, 8, 76, 186, 7, 56, 189, 134, 8, 253, 164, 8,
	157, 134, 8, 164, 31, 153, 172, 8, 222, 128, 8, 202, 48, 3, 76, 238,
	5, 96, 255, 241, 228, 215, 203, 192, 181, 170, 161, 152, 143, 135, 127, 120,
	114, 107, 101, 95, 90, 85, 80, 75, 71, 67, 63, 60, 56, 53, 50, 47,
	44, 42, 39, 37, 35, 33, 31, 29, 28, 26, 23, 22, 19, 15, 7, 0,
	56, 140, 0, 106, 232, 106, 239, 128, 8, 174, 70, 230, 149, 65, 246, 176,
	110, 48, 246, 187, 132, 82, 34, 244, 200, 160, 122, 85, 52, 20, 245, 216,
	189, 164, 141, 119, 96, 78, 56, 39, 21, 6, 247, 232, 219, 207, 195, 184,
	172, 162, 154, 144, 136, 127, 120, 112, 106, 100, 94, 87, 82, 50, 10, 0,
	11, 10, 10, 9, 8, 8, 7, 7, 7, 6, 6, 5, 5, 5, 4, 4,
	4, 4, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 80, 0, 0, 1, 1, 0,
	0, 255, 255, 31 };
static const uint8_t CiResource_mpt_obx[2233] = {
	255, 255, 0, 5, 178, 13, 76, 205, 11, 173, 46, 7, 208, 1, 96, 169,
	0, 141, 28, 14, 238, 29, 14, 173, 23, 14, 205, 187, 13, 144, 80, 206,
	21, 14, 240, 3, 76, 197, 5, 162, 0, 142, 23, 14, 169, 0, 157, 237,
	13, 157, 245, 13, 189, 179, 13, 133, 236, 189, 183, 13, 133, 237, 172, 22,
	14, 177, 236, 200, 201, 255, 240, 7, 201, 254, 208, 15, 76, 42, 12, 177,
	236, 48, 249, 10, 168, 140, 22, 14, 76, 59, 5, 157, 233, 13, 177, 236,
	157, 213, 13, 232, 224, 4, 208, 196, 200, 140, 22, 14, 76, 197, 5, 206,
	21, 14, 16, 87, 173, 188, 13, 141, 21, 14, 162, 3, 222, 245, 13, 16,
	68, 189, 233, 13, 10, 168, 185, 255, 255, 133, 236, 200, 185, 255, 255, 133,
	237, 5, 236, 240, 48, 189, 237, 13, 141, 31, 14, 32, 62, 7, 172, 31,
	14, 200, 152, 157, 237, 13, 189, 241, 13, 157, 245, 13, 224, 2, 208, 21,
	189, 197, 13, 73, 15, 10, 10, 10, 10, 105, 69, 141, 161, 13, 169, 10,
	105, 0, 141, 162, 13, 202, 16, 180, 238, 23, 14, 162, 1, 173, 27, 14,
	201, 2, 240, 2, 162, 3, 173, 27, 14, 201, 2, 208, 5, 236, 25, 14,
	240, 3, 76, 118, 6, 181, 240, 61, 114, 6, 240, 18, 160, 40, 177, 236,
	24, 125, 225, 13, 32, 117, 9, 56, 125, 1, 14, 157, 203, 13, 202, 16,
	213, 169, 3, 141, 15, 210, 165, 241, 41, 16, 240, 15, 172, 226, 13, 185,
	198, 9, 141, 201, 13, 185, 5, 10, 141, 202, 13, 173, 201, 13, 141, 0,
	210, 173, 202, 13, 141, 2, 210, 173, 203, 13, 141, 4, 210, 173, 204, 13,
	141, 6, 210, 173, 193, 13, 162, 255, 172, 27, 14, 192, 1, 208, 5, 174,
	25, 14, 240, 3, 141, 1, 210, 173, 194, 13, 224, 1, 240, 3, 141, 3,
	210, 192, 2, 240, 20, 173, 195, 13, 224, 2, 240, 3, 141, 5, 210, 173,
	196, 13, 224, 3, 240, 3, 141, 7, 210, 165, 240, 5, 241, 5, 242, 5,
	243, 13, 28, 14, 141, 8, 210, 96, 4, 2, 0, 0, 189, 217, 13, 133,
	236, 189, 221, 13, 133, 237, 5, 236, 208, 8, 157, 193, 13, 149, 240, 76,
	248, 5, 180, 244, 192, 32, 240, 66, 177, 236, 56, 253, 197, 13, 44, 58,
	7, 240, 2, 41, 240, 157, 193, 13, 200, 177, 236, 141, 30, 14, 200, 148,
	244, 41, 7, 240, 60, 168, 185, 126, 9, 141, 203, 6, 185, 133, 9, 141,
	204, 6, 173, 30, 14, 74, 74, 74, 74, 74, 9, 40, 168, 177, 236, 24,
	32, 255, 255, 169, 0, 149, 240, 76, 248, 5, 189, 9, 14, 240, 18, 222,
	13, 14, 208, 13, 157, 13, 14, 189, 193, 13, 41, 15, 240, 3, 222, 193,
	13, 160, 35, 177, 236, 149, 240, 189, 17, 14, 24, 105, 37, 168, 41, 3,
	157, 17, 14, 136, 177, 236, 125, 209, 13, 157, 225, 13, 32, 119, 9, 157,
	201, 13, 189, 5, 14, 240, 6, 222, 5, 14, 76, 223, 5, 189, 189, 13,
	141, 30, 7, 16, 254, 76, 194, 8, 0, 76, 229, 8, 0, 76, 251, 8,
	0, 76, 21, 9, 0, 76, 37, 9, 0, 76, 56, 9, 0, 76, 66, 9,
	16, 76, 72, 9, 169, 0, 157, 197, 13, 172, 31, 14, 136, 200, 177, 236,
	201, 254, 208, 4, 140, 31, 14, 96, 201, 224, 144, 8, 173, 187, 13, 141,
	23, 14, 208, 233, 201, 208, 144, 10, 41, 15, 141, 188, 13, 141, 21, 14,
	16, 219, 201, 192, 144, 9, 41, 15, 73, 15, 157, 197, 13, 16, 206, 201,
	128, 144, 7, 41, 63, 157, 241, 13, 16, 195, 201, 64, 144, 27, 200, 140,
	31, 14, 41, 31, 157, 229, 13, 10, 168, 185, 255, 255, 157, 217, 13, 200,
	185, 255, 255, 157, 221, 13, 76, 62, 7, 140, 31, 14, 141, 30, 14, 24,
	125, 213, 13, 157, 209, 13, 173, 27, 14, 240, 66, 201, 2, 240, 58, 189,
	229, 13, 201, 31, 208, 55, 173, 30, 14, 56, 233, 1, 41, 15, 168, 177,
	254, 133, 253, 152, 9, 16, 168, 177, 254, 133, 248, 160, 1, 5, 253, 208,
	2, 160, 0, 140, 26, 14, 169, 0, 133, 252, 157, 217, 13, 157, 221, 13,
	138, 10, 141, 24, 14, 142, 25, 14, 96, 224, 2, 176, 99, 189, 217, 13,
	133, 238, 189, 221, 13, 133, 239, 5, 238, 240, 74, 160, 32, 177, 238, 41,
	15, 157, 249, 13, 177, 238, 41, 112, 74, 74, 157, 189, 13, 200, 177, 238,
	10, 10, 72, 41, 63, 157, 5, 14, 104, 41, 192, 157, 205, 13, 200, 177,
	238, 157, 9, 14, 157, 13, 14, 169, 0, 149, 244, 157, 17, 14, 157, 253,
	13, 157, 1, 14, 189, 209, 13, 157, 225, 13, 32, 117, 9, 157, 201, 13,
	236, 25, 14, 240, 1, 96, 160, 255, 140, 25, 14, 200, 140, 26, 14, 96,
	224, 2, 208, 51, 172, 211, 13, 185, 69, 11, 141, 121, 13, 185, 129, 11,
	141, 127, 13, 169, 0, 133, 249, 133, 250, 173, 231, 13, 41, 15, 168, 177,
	254, 133, 251, 152, 9, 16, 168, 177, 254, 141, 137, 13, 5, 251, 208, 6,
	141, 121, 13, 141, 127, 13, 96, 173, 232, 13, 41, 15, 168, 177, 254, 133,
	253, 152, 9, 16, 168, 177, 254, 5, 253, 240, 15, 177, 254, 56, 229, 253,
	133, 248, 169, 0, 133, 252, 169, 141, 208, 2, 169, 173, 141, 97, 13, 141,
	56, 13, 169, 24, 141, 7, 210, 96, 173, 29, 14, 41, 7, 74, 74, 144,
	18, 208, 24, 189, 249, 13, 24, 157, 1, 14, 125, 201, 13, 157, 201, 13,
	76, 223, 5, 169, 0, 157, 1, 14, 76, 223, 5, 189, 201, 13, 56, 253,
	249, 13, 157, 201, 13, 56, 169, 0, 253, 249, 13, 157, 1, 14, 76, 223,
	5, 189, 253, 13, 24, 157, 1, 14, 125, 201, 13, 157, 201, 13, 24, 189,
	253, 13, 125, 249, 13, 157, 253, 13, 76, 223, 5, 189, 225, 13, 56, 253,
	253, 13, 157, 225, 13, 32, 117, 9, 76, 5, 9, 169, 0, 56, 253, 253,
	13, 157, 1, 14, 189, 201, 13, 56, 253, 253, 13, 76, 5, 9, 189, 225,
	13, 24, 125, 253, 13, 76, 28, 9, 32, 85, 9, 76, 208, 8, 32, 85,
	9, 24, 125, 225, 13, 32, 155, 9, 76, 223, 5, 188, 253, 13, 189, 249,
	13, 48, 2, 200, 200, 136, 152, 157, 253, 13, 221, 249, 13, 208, 8, 189,
	249, 13, 73, 255, 157, 249, 13, 189, 253, 13, 96, 41, 63, 29, 205, 13,
	168, 185, 255, 255, 96, 148, 145, 152, 165, 173, 180, 192, 9, 9, 9, 9,
	9, 9, 9, 64, 0, 32, 0, 125, 201, 13, 157, 201, 13, 96, 125, 209,
	13, 157, 225, 13, 32, 117, 9, 157, 201, 13, 96, 157, 201, 13, 189, 141,
	9, 16, 12, 157, 201, 13, 169, 128, 208, 5, 157, 201, 13, 169, 1, 13,
	28, 14, 141, 28, 14, 96, 45, 10, 210, 157, 201, 13, 96, 242, 51, 150,
	226, 56, 140, 0, 106, 232, 106, 239, 128, 8, 174, 70, 230, 149, 65, 246,
	176, 110, 48, 246, 187, 132, 82, 34, 244, 200, 160, 122, 85, 52, 20, 245,
	216, 189, 164, 141, 119, 96, 78, 56, 39, 21, 6, 247, 232, 219, 207, 195,
	184, 172, 162, 154, 144, 136, 127, 120, 112, 106, 100, 94, 13, 13, 12, 11,
	11, 10, 10, 9, 8, 8, 7, 7, 7, 6, 6, 5, 5, 5, 4, 4,
	4, 4, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 24, 24, 24, 24,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 22, 22, 23, 23, 23,
	23, 24, 24, 24, 24, 24, 25, 25, 25, 25, 26, 21, 21, 22, 22, 22,
	23, 23, 24, 24, 24, 25, 25, 26, 26, 26, 27, 20, 21, 21, 22, 22,
	23, 23, 24, 24, 24, 25, 25, 26, 26, 27, 27, 20, 20, 21, 21, 22,
	22, 23, 23, 24, 25, 25, 26, 26, 27, 27, 28, 19, 20, 20, 21, 22,
	22, 23, 23, 24, 25, 25, 26, 26, 27, 28, 28, 19, 19, 20, 21, 21,
	22, 23, 23, 24, 25, 25, 26, 27, 27, 28, 29, 18, 19, 20, 20, 21,
	22, 23, 23, 24, 25, 25, 26, 27, 28, 28, 29, 18, 19, 19, 20, 21,
	22, 22, 23, 24, 25, 26, 26, 27, 28, 29, 29, 18, 18, 19, 20, 21,
	22, 22, 23, 24, 25, 26, 26, 27, 28, 29, 30, 17, 18, 19, 20, 21,
	22, 22, 23, 24, 25, 26, 26, 27, 28, 29, 30, 17, 18, 19, 20, 21,
	21, 22, 23, 24, 25, 26, 27, 27, 28, 29, 30, 17, 18, 19, 20, 20,
	21, 22, 23, 24, 25, 26, 27, 28, 28, 29, 30, 17, 18, 19, 19, 20,
	21, 22, 23, 24, 25, 26, 27, 28, 29, 29, 30, 17, 18, 18, 19, 20,
	21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 30, 16, 17, 18, 19, 20,
	21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 34, 36, 38, 41,
	43, 46, 48, 51, 55, 58, 61, 65, 69, 73, 77, 82, 87, 92, 97, 103,
	110, 116, 123, 130, 138, 146, 155, 164, 174, 184, 195, 207, 220, 233, 246, 5,
	21, 37, 55, 73, 93, 113, 135, 159, 184, 210, 237, 11, 42, 75, 110, 147,
	186, 227, 15, 62, 112, 164, 219, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3,
	3, 3, 3, 229, 42, 64, 89, 100, 238, 8, 166, 11, 12, 12, 12, 12,
	12, 13, 13, 142, 50, 7, 140, 54, 7, 41, 7, 168, 185, 189, 11, 141,
	227, 11, 185, 197, 11, 141, 228, 11, 76, 255, 255, 173, 54, 7, 174, 50,
	7, 141, 148, 7, 141, 155, 7, 142, 149, 7, 142, 156, 7, 24, 105, 64,
	141, 129, 5, 141, 135, 5, 144, 1, 232, 142, 130, 5, 142, 136, 5, 24,
	105, 128, 141, 124, 9, 144, 1, 232, 142, 125, 9, 232, 141, 31, 12, 142,
	32, 12, 162, 9, 189, 255, 255, 157, 179, 13, 202, 16, 247, 206, 188, 13,
	169, 0, 141, 46, 7, 162, 98, 157, 189, 13, 202, 16, 250, 162, 8, 157,
	0, 210, 202, 16, 250, 96, 32, 42, 12, 173, 50, 7, 10, 141, 22, 14,
	173, 187, 13, 141, 23, 14, 169, 1, 141, 21, 14, 141, 46, 7, 96, 173,
	54, 7, 133, 254, 173, 50, 7, 133, 255, 96, 173, 54, 7, 41, 3, 170,
	173, 50, 7, 32, 198, 7, 173, 26, 14, 240, 238, 14, 54, 7, 32, 190,
	12, 169, 1, 141, 27, 14, 173, 26, 14, 240, 222, 201, 1, 208, 5, 160,
	0, 238, 26, 14, 177, 252, 174, 24, 14, 74, 74, 74, 74, 9, 16, 141,
	10, 212, 141, 10, 212, 157, 1, 210, 177, 252, 9, 16, 141, 10, 212, 141,
	10, 212, 157, 1, 210, 200, 208, 206, 230, 253, 165, 253, 197, 248, 208, 198,
	140, 26, 14, 96, 144, 21, 169, 234, 141, 153, 12, 141, 154, 12, 141, 155,
	12, 141, 166, 12, 141, 167, 12, 141, 168, 12, 96, 169, 141, 141, 153, 12,
	141, 166, 12, 169, 10, 141, 154, 12, 141, 167, 12, 169, 212, 141, 155, 12,
	141, 168, 12, 96, 169, 0, 141, 26, 14, 173, 50, 7, 74, 32, 190, 12,
	169, 1, 141, 27, 14, 32, 128, 12, 173, 27, 14, 208, 248, 96, 169, 2,
	141, 27, 14, 141, 25, 14, 169, 24, 141, 7, 210, 169, 17, 133, 250, 169,
	13, 133, 251, 169, 173, 141, 97, 13, 141, 56, 13, 160, 0, 140, 121, 13,
	140, 127, 13, 174, 11, 212, 177, 252, 74, 74, 74, 74, 9, 16, 141, 7,
	210, 32, 117, 13, 236, 11, 212, 240, 251, 141, 5, 210, 174, 11, 212, 177,
	252, 230, 252, 208, 16, 230, 253, 198, 248, 208, 10, 169, 173, 141, 97, 13,
	141, 56, 13, 169, 8, 9, 16, 141, 7, 210, 32, 117, 13, 236, 11, 212,
	240, 251, 141, 5, 210, 173, 27, 14, 208, 185, 96, 24, 165, 249, 105, 0,
	133, 249, 165, 250, 105, 0, 133, 250, 144, 15, 230, 251, 165, 251, 201, 0,
	208, 7, 140, 121, 13, 140, 127, 13, 96, 177, 250, 36, 249, 48, 4, 74,
	74, 74, 74, 41, 15, 168, 185, 69, 10, 160, 0, 96, 160, 0, 140, 27,
	14, 140, 26, 14, 136, 140, 25, 14, 96 };
static const uint8_t CiResource_rmt4_obx[2007] = {
	255, 255, 144, 3, 96, 11, 128, 0, 128, 32, 128, 64, 0, 192, 128, 128,
	128, 160, 0, 192, 64, 192, 0, 1, 5, 11, 21, 0, 1, 255, 255, 1,
	1, 0, 255, 255, 0, 1, 1, 1, 0, 255, 255, 255, 255, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 242, 51, 150, 226, 56, 140, 0, 106, 232, 106,
	239, 128, 8, 174, 70, 230, 149, 65, 246, 176, 110, 48, 246, 187, 132, 82,
	34, 244, 200, 160, 122, 85, 52, 20, 245, 216, 189, 164, 141, 119, 96, 78,
	56, 39, 21, 6, 247, 232, 219, 207, 195, 184, 172, 162, 154, 144, 136, 127,
	120, 112, 106, 100, 94, 0, 191, 182, 170, 161, 152, 143, 137, 128, 242, 230,
	218, 206, 191, 182, 170, 161, 152, 143, 137, 128, 122, 113, 107, 101, 95, 92,
	86, 80, 77, 71, 68, 62, 60, 56, 53, 50, 47, 45, 42, 40, 37, 35,
	33, 31, 29, 28, 26, 24, 23, 22, 20, 19, 18, 17, 16, 15, 14, 13,
	12, 11, 10, 9, 8, 7, 255, 241, 228, 216, 202, 192, 181, 171, 162, 153,
	142, 135, 127, 121, 115, 112, 102, 97, 90, 85, 82, 75, 72, 67, 63, 60,
	57, 55, 51, 48, 45, 42, 40, 37, 36, 33, 31, 30, 28, 27, 25, 23,
	22, 21, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6,
	5, 4, 3, 2, 1, 0, 243, 230, 217, 204, 193, 181, 173, 162, 153, 144,
	136, 128, 121, 114, 108, 102, 96, 91, 85, 81, 76, 72, 68, 64, 60, 57,
	53, 50, 47, 45, 42, 40, 37, 35, 33, 31, 29, 28, 26, 24, 23, 22,
	20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5,
	4, 3, 2, 1, 0, 0, 13, 13, 12, 11, 11, 10, 10, 9, 8, 8,
	7, 7, 7, 6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3,
	3, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	1, 1, 2, 2, 2, 2, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2,
	2, 2, 2, 3, 3, 3, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
	3, 3, 3, 3, 4, 4, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3,
	3, 4, 4, 4, 5, 5, 0, 0, 1, 1, 2, 2, 2, 3, 3, 4,
	4, 4, 5, 5, 6, 6, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4,
	5, 5, 6, 6, 7, 7, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5,
	5, 6, 6, 7, 7, 8, 0, 1, 1, 2, 2, 3, 4, 4, 5, 5,
	6, 7, 7, 8, 8, 9, 0, 1, 1, 2, 3, 3, 4, 5, 5, 6,
	7, 7, 8, 9, 9, 10, 0, 1, 1, 2, 3, 4, 4, 5, 6, 7,
	7, 8, 9, 10, 10, 11, 0, 1, 2, 2, 3, 4, 5, 6, 6, 7,
	8, 9, 10, 10, 11, 12, 0, 1, 2, 3, 3, 4, 5, 6, 7, 8,
	9, 10, 10, 11, 12, 13, 0, 1, 2, 3, 4, 5, 6, 7, 7, 8,
	9, 10, 11, 12, 13, 14, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
	10, 11, 12, 13, 14, 15, 76, 15, 6, 76, 252, 7, 76, 33, 8, 76,
	88, 6, 76, 43, 11, 134, 211, 132, 212, 72, 160, 168, 169, 0, 153, 127,
	2, 136, 208, 250, 160, 4, 177, 211, 141, 21, 8, 200, 177, 211, 141, 36,
	3, 200, 177, 211, 141, 5, 8, 141, 38, 3, 160, 8, 177, 211, 153, 195,
	0, 200, 192, 16, 208, 246, 104, 72, 10, 10, 24, 101, 209, 133, 209, 104,
	8, 41, 192, 10, 42, 42, 40, 101, 210, 133, 210, 32, 110, 6, 169, 0,
	141, 8, 210, 160, 3, 140, 15, 210, 160, 8, 153, 0, 210, 136, 16, 250,
	173, 5, 8, 96, 162, 0, 142, 39, 3, 138, 168, 177, 209, 201, 254, 176,
	45, 168, 177, 205, 157, 128, 2, 177, 207, 157, 132, 2, 169, 0, 157, 136,
	2, 169, 1, 157, 140, 2, 169, 128, 157, 180, 2, 232, 224, 4, 208, 217,
	165, 209, 24, 105, 4, 133, 209, 144, 27, 230, 210, 76, 190, 6, 240, 4,
	169, 0, 240, 223, 160, 2, 177, 209, 170, 200, 177, 209, 133, 210, 134, 209,
	162, 0, 240, 181, 173, 36, 3, 141, 22, 7, 162, 255, 232, 222, 140, 2,
	208, 69, 189, 128, 2, 133, 211, 189, 132, 2, 133, 212, 188, 136, 2, 254,
	136, 2, 177, 211, 133, 217, 41, 63, 201, 61, 240, 17, 176, 56, 157, 144,
	2, 157, 16, 3, 200, 177, 211, 74, 41, 126, 157, 180, 2, 169, 1, 157,
	140, 2, 188, 136, 2, 254, 136, 2, 177, 211, 74, 102, 217, 74, 102, 217,
	165, 217, 41, 240, 157, 148, 2, 224, 3, 208, 177, 169, 255, 141, 36, 3,
	141, 37, 3, 76, 101, 7, 201, 63, 240, 27, 165, 217, 41, 192, 240, 9,
	10, 42, 42, 157, 140, 2, 76, 17, 7, 200, 177, 211, 157, 140, 2, 254,
	136, 2, 76, 17, 7, 165, 217, 48, 12, 200, 177, 211, 141, 22, 7, 254,
	136, 2, 76, 214, 6, 201, 255, 240, 9, 200, 177, 211, 157, 136, 2, 76,
	214, 6, 76, 110, 6, 76, 33, 8, 202, 48, 250, 188, 180, 2, 48, 248,
	177, 203, 157, 184, 2, 133, 215, 200, 177, 203, 157, 188, 2, 133, 216, 169,
	1, 157, 20, 3, 168, 177, 215, 157, 4, 3, 200, 177, 215, 157, 196, 2,
	200, 177, 215, 157, 200, 2, 200, 177, 215, 157, 240, 2, 41, 63, 157, 8,
	3, 177, 215, 41, 64, 157, 244, 2, 200, 177, 215, 157, 32, 3, 200, 177,
	215, 157, 208, 2, 200, 177, 215, 157, 216, 2, 200, 177, 215, 157, 220, 2,
	200, 177, 215, 168, 185, 160, 3, 157, 224, 2, 157, 228, 2, 185, 161, 3,
	157, 232, 2, 160, 10, 177, 215, 157, 236, 2, 169, 128, 157, 212, 2, 157,
	180, 2, 10, 157, 204, 2, 157, 156, 2, 168, 177, 215, 157, 0, 3, 105,
	0, 157, 192, 2, 169, 12, 157, 252, 2, 168, 177, 215, 157, 248, 2, 76,
	98, 7, 32, 43, 11, 206, 38, 3, 208, 29, 169, 255, 141, 38, 3, 206,
	37, 3, 208, 19, 238, 39, 3, 173, 39, 3, 201, 255, 240, 3, 76, 190,
	6, 76, 110, 6, 76, 95, 10, 169, 4, 133, 214, 162, 3, 189, 188, 2,
	240, 242, 133, 212, 189, 184, 2, 133, 211, 188, 192, 2, 177, 211, 133, 217,
	200, 177, 211, 133, 218, 200, 177, 211, 133, 219, 200, 152, 221, 196, 2, 144,
	10, 240, 8, 169, 128, 157, 204, 2, 189, 200, 2, 157, 192, 2, 165, 217,
	41, 15, 29, 148, 2, 168, 185, 0, 5, 133, 220, 165, 218, 41, 14, 168,
	185, 144, 3, 133, 213, 165, 220, 25, 145, 3, 157, 28, 3, 189, 220, 2,
	240, 40, 201, 1, 208, 33, 189, 156, 2, 24, 125, 236, 2, 24, 188, 224,
	2, 121, 165, 3, 157, 156, 2, 200, 152, 221, 232, 2, 208, 3, 189, 228,
	2, 157, 224, 2, 76, 164, 8, 222, 220, 2, 188, 0, 3, 192, 13, 144,
	60, 189, 8, 3, 16, 49, 152, 221, 252, 2, 208, 8, 189, 4, 3, 157,
	252, 2, 208, 3, 254, 252, 2, 189, 184, 2, 133, 215, 189, 188, 2, 133,
	216, 188, 252, 2, 177, 215, 188, 244, 2, 240, 4, 24, 125, 248, 2, 157,
	248, 2, 189, 240, 2, 41, 63, 56, 233, 1, 157, 8, 3, 189, 204, 2,
	16, 31, 189, 148, 2, 240, 26, 221, 216, 2, 240, 21, 144, 19, 168, 189,
	212, 2, 24, 125, 208, 2, 157, 212, 2, 144, 6, 152, 233, 16, 157, 148,
	2, 169, 0, 133, 221, 165, 218, 157, 12, 3, 41, 112, 74, 74, 141, 28,
	9, 144, 254, 76, 210, 9, 234, 76, 60, 9, 234, 76, 65, 9, 234, 76,
	75, 9, 234, 76, 87, 9, 234, 76, 102, 9, 234, 76, 169, 9, 234, 76,
	184, 9, 165, 219, 76, 21, 10, 165, 219, 133, 221, 189, 144, 2, 76, 216,
	9, 189, 144, 2, 24, 101, 219, 157, 144, 2, 76, 216, 9, 189, 156, 2,
	24, 101, 219, 157, 156, 2, 189, 144, 2, 76, 216, 9, 189, 240, 2, 16,
	12, 188, 144, 2, 177, 213, 24, 125, 248, 2, 76, 135, 9, 189, 144, 2,
	24, 125, 248, 2, 201, 61, 144, 2, 169, 63, 168, 177, 213, 157, 160, 2,
	164, 219, 208, 3, 157, 164, 2, 152, 74, 74, 74, 74, 157, 168, 2, 157,
	172, 2, 165, 219, 41, 15, 157, 176, 2, 189, 144, 2, 76, 216, 9, 165,
	219, 24, 125, 20, 3, 157, 20, 3, 189, 144, 2, 76, 216, 9, 165, 219,
	201, 128, 240, 6, 157, 144, 2, 76, 216, 9, 189, 28, 3, 9, 240, 157,
	28, 3, 189, 144, 2, 76, 216, 9, 189, 144, 2, 24, 101, 219, 188, 240,
	2, 48, 31, 24, 125, 248, 2, 201, 61, 144, 7, 169, 0, 157, 28, 3,
	169, 63, 157, 16, 3, 168, 177, 213, 24, 125, 156, 2, 24, 101, 221, 76,
	21, 10, 201, 61, 144, 7, 169, 0, 157, 28, 3, 169, 63, 168, 189, 156,
	2, 24, 125, 248, 2, 24, 113, 213, 24, 101, 221, 157, 24, 3, 189, 172,
	2, 240, 50, 222, 172, 2, 208, 45, 189, 168, 2, 157, 172, 2, 189, 164,
	2, 221, 160, 2, 240, 31, 176, 13, 125, 176, 2, 176, 18, 221, 160, 2,
	176, 13, 76, 76, 10, 253, 176, 2, 144, 5, 221, 160, 2, 176, 3, 189,
	160, 2, 157, 164, 2, 165, 218, 41, 1, 240, 10, 189, 164, 2, 24, 125,
	156, 2, 157, 24, 3, 202, 48, 3, 76, 39, 8, 173, 32, 3, 13, 33,
	3, 13, 34, 3, 13, 35, 3, 170, 142, 44, 11, 173, 12, 3, 16, 33,
	173, 28, 3, 41, 15, 240, 26, 173, 24, 3, 24, 109, 20, 3, 141, 26,
	3, 173, 30, 3, 41, 16, 208, 5, 169, 0, 141, 30, 3, 138, 9, 4,
	170, 173, 13, 3, 16, 33, 173, 29, 3, 41, 15, 240, 26, 173, 25, 3,
	24, 109, 21, 3, 141, 27, 3, 173, 31, 3, 41, 16, 208, 5, 169, 0,
	141, 31, 3, 138, 9, 2, 170, 236, 44, 11, 208, 94, 173, 13, 3, 41,
	14, 201, 6, 208, 38, 173, 29, 3, 41, 15, 240, 31, 172, 17, 3, 185,
	192, 3, 141, 24, 3, 185, 192, 4, 141, 25, 3, 173, 28, 3, 41, 16,
	208, 5, 169, 0, 141, 28, 3, 138, 9, 80, 170, 173, 15, 3, 41, 14,
	201, 6, 208, 38, 173, 31, 3, 41, 15, 240, 31, 172, 19, 3, 185, 192,
	3, 141, 26, 3, 185, 192, 4, 141, 27, 3, 173, 30, 3, 41, 16, 208,
	5, 169, 0, 141, 30, 3, 138, 9, 40, 170, 142, 44, 11, 173, 38, 3,
	96, 160, 255, 173, 24, 3, 174, 28, 3, 141, 0, 210, 142, 1, 210, 173,
	25, 3, 174, 29, 3, 141, 2, 210, 142, 3, 210, 173, 26, 3, 174, 30,
	3, 141, 4, 210, 142, 5, 210, 173, 27, 3, 174, 31, 3, 141, 6, 210,
	142, 7, 210, 140, 8, 210, 96 };
static const uint8_t CiResource_rmt8_obx[2275] = {
	255, 255, 144, 3, 108, 12, 128, 0, 128, 32, 128, 64, 0, 192, 128, 128,
	128, 160, 0, 192, 64, 192, 0, 1, 5, 11, 21, 0, 1, 255, 255, 1,
	1, 0, 255, 255, 0, 1, 1, 1, 0, 255, 255, 255, 255, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 242, 51, 150, 226, 56, 140, 0, 106, 232, 106,
	239, 128, 8, 174, 70, 230, 149, 65, 246, 176, 110, 48, 246, 187, 132, 82,
	34, 244, 200, 160, 122, 85, 52, 20, 245, 216, 189, 164, 141, 119, 96, 78,
	56, 39, 21, 6, 247, 232, 219, 207, 195, 184, 172, 162, 154, 144, 136, 127,
	120, 112, 106, 100, 94, 0, 191, 182, 170, 161, 152, 143, 137, 128, 242, 230,
	218, 206, 191, 182, 170, 161, 152, 143, 137, 128, 122, 113, 107, 101, 95, 92,
	86, 80, 77, 71, 68, 62, 60, 56, 53, 50, 47, 45, 42, 40, 37, 35,
	33, 31, 29, 28, 26, 24, 23, 22, 20, 19, 18, 17, 16, 15, 14, 13,
	12, 11, 10, 9, 8, 7, 255, 241, 228, 216, 202, 192, 181, 171, 162, 153,
	142, 135, 127, 121, 115, 112, 102, 97, 90, 85, 82, 75, 72, 67, 63, 60,
	57, 55, 51, 48, 45, 42, 40, 37, 36, 33, 31, 30, 28, 27, 25, 23,
	22, 21, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6,
	5, 4, 3, 2, 1, 0, 243, 230, 217, 204, 193, 181, 173, 162, 153, 144,
	136, 128, 121, 114, 108, 102, 96, 91, 85, 81, 76, 72, 68, 64, 60, 57,
	53, 50, 47, 45, 42, 40, 37, 35, 33, 31, 29, 28, 26, 24, 23, 22,
	20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5,
	4, 3, 2, 1, 0, 0, 13, 13, 12, 11, 11, 10, 10, 9, 8, 8,
	7, 7, 7, 6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3,
	3, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	1, 1, 2, 2, 2, 2, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2,
	2, 2, 2, 3, 3, 3, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
	3, 3, 3, 3, 4, 4, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3,
	3, 4, 4, 4, 5, 5, 0, 0, 1, 1, 2, 2, 2, 3, 3, 4,
	4, 4, 5, 5, 6, 6, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4,
	5, 5, 6, 6, 7, 7, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5,
	5, 6, 6, 7, 7, 8, 0, 1, 1, 2, 2, 3, 4, 4, 5, 5,
	6, 7, 7, 8, 8, 9, 0, 1, 1, 2, 3, 3, 4, 5, 5, 6,
	7, 7, 8, 9, 9, 10, 0, 1, 1, 2, 3, 4, 4, 5, 6, 7,
	7, 8, 9, 10, 10, 11, 0, 1, 2, 2, 3, 4, 5, 6, 6, 7,
	8, 9, 10, 10, 11, 12, 0, 1, 2, 3, 3, 4, 5, 6, 7, 8,
	9, 10, 10, 11, 12, 13, 0, 1, 2, 3, 4, 5, 6, 7, 7, 8,
	9, 10, 11, 12, 13, 14, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
	10, 11, 12, 13, 14, 15, 76, 15, 6, 76, 9, 8, 76, 46, 8, 76,
	92, 6, 76, 2, 12, 134, 211, 132, 212, 72, 160, 0, 152, 153, 0, 2,
	153, 76, 2, 200, 208, 247, 160, 4, 177, 211, 141, 34, 8, 200, 177, 211,
	141, 72, 3, 200, 177, 211, 141, 18, 8, 141, 74, 3, 160, 8, 177, 211,
	153, 195, 0, 200, 192, 16, 208, 246, 104, 72, 10, 10, 10, 24, 101, 209,
	133, 209, 104, 8, 41, 224, 10, 42, 42, 42, 40, 101, 210, 133, 210, 32,
	123, 6, 169, 0, 141, 8, 210, 141, 24, 210, 160, 3, 140, 15, 210, 140,
	31, 210, 160, 8, 153, 0, 210, 153, 16, 210, 136, 16, 247, 173, 18, 8,
	96, 162, 0, 142, 75, 3, 138, 168, 177, 209, 201, 254, 176, 45, 168, 177,
	205, 157, 0, 2, 177, 207, 157, 8, 2, 169, 0, 157, 16, 2, 169, 1,
	157, 24, 2, 169, 128, 157, 104, 2, 232, 224, 8, 208, 217, 165, 209, 24,
	105, 8, 133, 209, 144, 27, 230, 210, 76, 203, 6, 240, 4, 169, 0, 240,
	223, 160, 2, 177, 209, 170, 200, 177, 209, 133, 210, 134, 209, 162, 0, 240,
	181, 173, 72, 3, 141, 35, 7, 162, 255, 232, 222, 24, 2, 208, 69, 189,
	0, 2, 133, 211, 189, 8, 2, 133, 212, 188, 16, 2, 254, 16, 2, 177,
	211, 133, 217, 41, 63, 201, 61, 240, 17, 176, 56, 157, 32, 2, 157, 32,
	3, 200, 177, 211, 74, 41, 126, 157, 104, 2, 169, 1, 157, 24, 2, 188,
	16, 2, 254, 16, 2, 177, 211, 74, 102, 217, 74, 102, 217, 165, 217, 41,
	240, 157, 40, 2, 224, 7, 208, 177, 169, 255, 141, 72, 3, 141, 73, 3,
	76, 114, 7, 201, 63, 240, 27, 165, 217, 41, 192, 240, 9, 10, 42, 42,
	157, 24, 2, 76, 30, 7, 200, 177, 211, 157, 24, 2, 254, 16, 2, 76,
	30, 7, 165, 217, 48, 12, 200, 177, 211, 141, 35, 7, 254, 16, 2, 76,
	227, 6, 201, 255, 240, 9, 200, 177, 211, 157, 16, 2, 76, 227, 6, 76,
	123, 6, 76, 46, 8, 202, 48, 250, 188, 104, 2, 48, 248, 177, 203, 157,
	112, 2, 133, 215, 200, 177, 203, 157, 120, 2, 133, 216, 169, 1, 157, 40,
	3, 168, 177, 215, 157, 8, 3, 200, 177, 215, 157, 136, 2, 200, 177, 215,
	157, 144, 2, 200, 177, 215, 157, 224, 2, 41, 63, 157, 16, 3, 177, 215,
	41, 64, 157, 232, 2, 200, 177, 215, 157, 64, 3, 200, 177, 215, 157, 160,
	2, 200, 177, 215, 157, 176, 2, 200, 177, 215, 157, 184, 2, 200, 177, 215,
	168, 185, 160, 3, 157, 192, 2, 157, 200, 2, 185, 161, 3, 157, 208, 2,
	160, 10, 177, 215, 157, 216, 2, 169, 128, 157, 168, 2, 157, 104, 2, 10,
	157, 152, 2, 157, 56, 2, 168, 177, 215, 157, 0, 3, 105, 0, 157, 128,
	2, 169, 12, 157, 248, 2, 168, 177, 215, 157, 240, 2, 76, 111, 7, 32,
	2, 12, 206, 74, 3, 208, 29, 169, 255, 141, 74, 3, 206, 73, 3, 208,
	19, 238, 75, 3, 173, 75, 3, 201, 255, 240, 3, 76, 203, 6, 76, 123,
	6, 76, 116, 10, 169, 4, 133, 214, 162, 7, 189, 120, 2, 240, 242, 133,
	212, 189, 112, 2, 133, 211, 188, 128, 2, 177, 211, 133, 217, 200, 177, 211,
	133, 218, 200, 177, 211, 133, 219, 200, 152, 221, 136, 2, 144, 10, 240, 8,
	169, 128, 157, 152, 2, 189, 144, 2, 157, 128, 2, 165, 217, 224, 4, 144,
	4, 74, 74, 74, 74, 41, 15, 29, 40, 2, 168, 185, 0, 5, 133, 220,
	165, 218, 41, 14, 168, 185, 144, 3, 133, 213, 165, 220, 25, 145, 3, 157,
	56, 3, 189, 184, 2, 240, 40, 201, 1, 208, 33, 189, 56, 2, 24, 125,
	216, 2, 24, 188, 192, 2, 121, 165, 3, 157, 56, 2, 200, 152, 221, 208,
	2, 208, 3, 189, 200, 2, 157, 192, 2, 76, 185, 8, 222, 184, 2, 188,
	0, 3, 192, 13, 144, 60, 189, 16, 3, 16, 49, 152, 221, 248, 2, 208,
	8, 189, 8, 3, 157, 248, 2, 208, 3, 254, 248, 2, 189, 112, 2, 133,
	215, 189, 120, 2, 133, 216, 188, 248, 2, 177, 215, 188, 232, 2, 240, 4,
	24, 125, 240, 2, 157, 240, 2, 189, 224, 2, 41, 63, 56, 233, 1, 157,
	16, 3, 189, 152, 2, 16, 31, 189, 40, 2, 240, 26, 221, 176, 2, 240,
	21, 144, 19, 168, 189, 168, 2, 24, 125, 160, 2, 157, 168, 2, 144, 6,
	152, 233, 16, 157, 40, 2, 169, 0, 133, 221, 165, 218, 157, 24, 3, 41,
	112, 74, 74, 141, 49, 9, 144, 254, 76, 231, 9, 234, 76, 81, 9, 234,
	76, 86, 9, 234, 76, 96, 9, 234, 76, 108, 9, 234, 76, 123, 9, 234,
	76, 190, 9, 234, 76, 205, 9, 165, 219, 76, 42, 10, 165, 219, 133, 221,
	189, 32, 2, 76, 237, 9, 189, 32, 2, 24, 101, 219, 157, 32, 2, 76,
	237, 9, 189, 56, 2, 24, 101, 219, 157, 56, 2, 189, 32, 2, 76, 237,
	9, 189, 224, 2, 16, 12, 188, 32, 2, 177, 213, 24, 125, 240, 2, 76,
	156, 9, 189, 32, 2, 24, 125, 240, 2, 201, 61, 144, 2, 169, 63, 168,
	177, 213, 157, 64, 2, 164, 219, 208, 3, 157, 72, 2, 152, 74, 74, 74,
	74, 157, 80, 2, 157, 88, 2, 165, 219, 41, 15, 157, 96, 2, 189, 32,
	2, 76, 237, 9, 165, 219, 24, 125, 40, 3, 157, 40, 3, 189, 32, 2,
	76, 237, 9, 165, 219, 201, 128, 240, 6, 157, 32, 2, 76, 237, 9, 189,
	56, 3, 9, 240, 157, 56, 3, 189, 32, 2, 76, 237, 9, 189, 32, 2,
	24, 101, 219, 188, 224, 2, 48, 31, 24, 125, 240, 2, 201, 61, 144, 7,
	169, 0, 157, 56, 3, 169, 63, 157, 32, 3, 168, 177, 213, 24, 125, 56,
	2, 24, 101, 221, 76, 42, 10, 201, 61, 144, 7, 169, 0, 157, 56, 3,
	169, 63, 168, 189, 56, 2, 24, 125, 240, 2, 24, 113, 213, 24, 101, 221,
	157, 48, 3, 189, 88, 2, 240, 50, 222, 88, 2, 208, 45, 189, 80, 2,
	157, 88, 2, 189, 72, 2, 221, 64, 2, 240, 31, 176, 13, 125, 96, 2,
	176, 18, 221, 64, 2, 176, 13, 76, 97, 10, 253, 96, 2, 144, 5, 221,
	64, 2, 176, 3, 189, 64, 2, 157, 72, 2, 165, 218, 41, 1, 240, 10,
	189, 72, 2, 24, 125, 56, 2, 157, 48, 3, 202, 48, 3, 76, 52, 8,
	173, 64, 3, 13, 65, 3, 13, 66, 3, 13, 67, 3, 170, 142, 101, 12,
	173, 24, 3, 16, 33, 173, 56, 3, 41, 15, 240, 26, 173, 48, 3, 24,
	109, 40, 3, 141, 50, 3, 173, 58, 3, 41, 16, 208, 5, 169, 0, 141,
	58, 3, 138, 9, 4, 170, 173, 25, 3, 16, 33, 173, 57, 3, 41, 15,
	240, 26, 173, 49, 3, 24, 109, 41, 3, 141, 51, 3, 173, 59, 3, 41,
	16, 208, 5, 169, 0, 141, 59, 3, 138, 9, 2, 170, 236, 101, 12, 208,
	94, 173, 25, 3, 41, 14, 201, 6, 208, 38, 173, 57, 3, 41, 15, 240,
	31, 172, 33, 3, 185, 192, 3, 141, 48, 3, 185, 192, 4, 141, 49, 3,
	173, 56, 3, 41, 16, 208, 5, 169, 0, 141, 56, 3, 138, 9, 80, 170,
	173, 27, 3, 41, 14, 201, 6, 208, 38, 173, 59, 3, 41, 15, 240, 31,
	172, 35, 3, 185, 192, 3, 141, 50, 3, 185, 192, 4, 141, 51, 3, 173,
	58, 3, 41, 16, 208, 5, 169, 0, 141, 58, 3, 138, 9, 40, 170, 142,
	101, 12, 173, 68, 3, 13, 69, 3, 13, 70, 3, 13, 71, 3, 170, 142,
	3, 12, 173, 28, 3, 16, 33, 173, 60, 3, 41, 15, 240, 26, 173, 52,
	3, 24, 109, 44, 3, 141, 54, 3, 173, 62, 3, 41, 16, 208, 5, 169,
	0, 141, 62, 3, 138, 9, 4, 170, 173, 29, 3, 16, 33, 173, 61, 3,
	41, 15, 240, 26, 173, 53, 3, 24, 109, 45, 3, 141, 55, 3, 173, 63,
	3, 41, 16, 208, 5, 169, 0, 141, 63, 3, 138, 9, 2, 170, 236, 3,
	12, 208, 94, 173, 29, 3, 41, 14, 201, 6, 208, 38, 173, 61, 3, 41,
	15, 240, 31, 172, 37, 3, 185, 192, 3, 141, 52, 3, 185, 192, 4, 141,
	53, 3, 173, 60, 3, 41, 16, 208, 5, 169, 0, 141, 60, 3, 138, 9,
	80, 170, 173, 31, 3, 41, 14, 201, 6, 208, 38, 173, 63, 3, 41, 15,
	240, 31, 172, 39, 3, 185, 192, 3, 141, 54, 3, 185, 192, 4, 141, 55,
	3, 173, 62, 3, 41, 16, 208, 5, 169, 0, 141, 62, 3, 138, 9, 40,
	170, 142, 3, 12, 173, 74, 3, 96, 160, 255, 173, 52, 3, 174, 48, 3,
	141, 16, 210, 142, 0, 210, 173, 60, 3, 174, 56, 3, 141, 17, 210, 142,
	1, 210, 173, 53, 3, 174, 49, 3, 141, 18, 210, 142, 2, 210, 173, 61,
	3, 174, 57, 3, 141, 19, 210, 142, 3, 210, 173, 54, 3, 174, 50, 3,
	141, 20, 210, 142, 4, 210, 173, 62, 3, 174, 58, 3, 141, 21, 210, 142,
	5, 210, 173, 55, 3, 174, 51, 3, 141, 22, 210, 142, 6, 210, 173, 63,
	3, 174, 59, 3, 141, 23, 210, 142, 7, 210, 169, 255, 140, 24, 210, 141,
	8, 210, 96 };
static const uint8_t CiResource_tm2_obx[3698] = {
	255, 255, 0, 5, 107, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	1, 1, 2, 2, 2, 2, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2,
	2, 2, 2, 3, 3, 3, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
	3, 3, 3, 3, 4, 4, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3,
	3, 4, 4, 4, 5, 5, 0, 0, 1, 1, 2, 2, 2, 3, 3, 4,
	4, 4, 5, 5, 6, 6, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4,
	5, 5, 6, 6, 7, 7, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5,
	5, 6, 6, 7, 7, 8, 0, 1, 1, 2, 2, 3, 4, 4, 5, 5,
	6, 7, 7, 8, 8, 9, 0, 1, 1, 2, 3, 3, 4, 5, 5, 6,
	7, 7, 8, 9, 9, 10, 0, 1, 1, 2, 3, 4, 4, 5, 6, 7,
	7, 8, 9, 10, 10, 11, 0, 1, 2, 2, 3, 4, 5, 6, 6, 7,
	8, 9, 10, 10, 11, 12, 0, 1, 2, 3, 3, 4, 5, 6, 7, 8,
	9, 10, 10, 11, 12, 13, 0, 1, 2, 3, 4, 5, 6, 7, 7, 8,
	9, 10, 11, 12, 13, 14, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
	10, 11, 12, 13, 14, 15, 0, 241, 228, 215, 203, 192, 181, 170, 161, 152,
	143, 135, 127, 120, 114, 107, 101, 95, 90, 85, 80, 75, 71, 67, 63, 60,
	56, 53, 50, 47, 44, 42, 39, 37, 35, 33, 31, 29, 28, 26, 24, 23,
	22, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6,
	5, 4, 3, 2, 1, 0, 0, 242, 233, 218, 206, 191, 182, 170, 161, 152,
	143, 137, 128, 122, 113, 107, 101, 95, 92, 86, 80, 77, 71, 68, 62, 60,
	56, 53, 50, 47, 45, 42, 40, 37, 35, 33, 31, 29, 28, 26, 24, 23,
	22, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6,
	5, 4, 3, 2, 1, 0, 0, 255, 241, 228, 216, 202, 192, 181, 171, 162,
	153, 142, 135, 127, 121, 115, 112, 102, 97, 90, 85, 82, 75, 72, 67, 63,
	60, 57, 55, 51, 48, 45, 42, 40, 37, 36, 33, 31, 30, 28, 27, 25,
	23, 22, 21, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7,
	6, 5, 4, 3, 2, 1, 0, 243, 230, 217, 204, 193, 181, 173, 162, 153,
	144, 136, 128, 121, 114, 108, 102, 96, 91, 85, 81, 76, 72, 68, 64, 60,
	57, 53, 50, 47, 45, 42, 40, 37, 35, 33, 31, 29, 28, 26, 24, 23,
	22, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6,
	5, 4, 3, 2, 1, 0, 226, 56, 140, 0, 106, 232, 106, 239, 128, 8,
	174, 70, 230, 149, 65, 246, 176, 110, 48, 246, 187, 132, 82, 34, 244, 200,
	160, 122, 85, 52, 20, 245, 216, 189, 164, 141, 119, 96, 78, 56, 39, 21,
	6, 247, 232, 219, 207, 195, 184, 172, 162, 154, 144, 136, 127, 120, 112, 106,
	100, 94, 87, 82, 50, 10, 0, 242, 51, 150, 226, 56, 140, 0, 106, 232,
	106, 239, 128, 8, 174, 70, 230, 149, 65, 246, 176, 110, 48, 246, 187, 132,
	82, 34, 244, 200, 160, 122, 85, 52, 20, 245, 216, 189, 164, 141, 119, 96,
	78, 56, 39, 21, 6, 247, 232, 219, 207, 195, 184, 172, 162, 154, 144, 136,
	127, 120, 112, 106, 100, 94, 11, 11, 10, 10, 9, 8, 8, 7, 7, 7,
	6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 3, 2, 2,
	2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 13, 13, 12, 11, 11, 10, 10, 9, 8,
	8, 7, 7, 7, 6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3,
	3, 3, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 76, 228, 16, 76, 227, 9, 76, 159, 11, 1,
	16, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 5,
	6, 7, 0, 1, 2, 3, 4, 2, 0, 0, 4, 2, 0, 0, 0, 16,
	0, 8, 0, 16, 0, 8, 133, 211, 129, 169, 133, 211, 129, 169, 136, 177,
	250, 141, 23, 8, 162, 0, 134, 252, 10, 38, 252, 10, 38, 252, 10, 38,
	252, 10, 38, 252, 109, 23, 8, 144, 2, 230, 252, 24, 105, 0, 133, 250,
	165, 252, 105, 0, 133, 251, 76, 7, 10, 32, 181, 12, 173, 22, 8, 240,
	5, 206, 28, 8, 48, 3, 76, 162, 11, 206, 29, 8, 208, 82, 162, 0,
	238, 23, 8, 173, 25, 8, 133, 250, 173, 26, 8, 133, 251, 160, 16, 177,
	250, 48, 171, 208, 3, 76, 44, 18, 141, 29, 8, 136, 177, 250, 136, 132,
	252, 168, 185, 255, 255, 157, 80, 8, 185, 255, 255, 157, 88, 8, 169, 0,
	157, 112, 8, 157, 96, 8, 164, 252, 177, 250, 157, 104, 8, 232, 136, 16,
	219, 169, 17, 24, 101, 250, 141, 25, 8, 169, 0, 101, 251, 141, 26, 8,
	173, 27, 8, 141, 28, 8, 162, 7, 222, 112, 8, 48, 6, 202, 16, 248,
	76, 162, 11, 189, 80, 8, 133, 250, 189, 88, 8, 133, 251, 188, 96, 8,
	177, 250, 208, 28, 200, 177, 250, 157, 208, 8, 41, 240, 157, 216, 8, 177,
	250, 10, 10, 10, 10, 157, 224, 8, 200, 152, 157, 96, 8, 76, 87, 10,
	201, 64, 176, 79, 125, 104, 8, 157, 152, 8, 200, 177, 250, 16, 37, 41,
	127, 133, 252, 200, 177, 250, 157, 208, 8, 41, 240, 157, 216, 8, 177, 250,
	10, 10, 10, 10, 157, 224, 8, 200, 152, 157, 96, 8, 164, 252, 32, 156,
	18, 76, 87, 10, 168, 254, 96, 8, 254, 96, 8, 189, 208, 8, 41, 240,
	157, 216, 8, 189, 208, 8, 10, 10, 10, 10, 157, 224, 8, 32, 156, 18,
	76, 87, 10, 201, 128, 176, 37, 41, 63, 24, 125, 104, 8, 157, 152, 8,
	200, 177, 250, 157, 208, 8, 41, 240, 157, 216, 8, 177, 250, 10, 10, 10,
	10, 157, 224, 8, 200, 152, 157, 96, 8, 76, 87, 10, 208, 14, 200, 177,
	250, 157, 112, 8, 200, 152, 157, 96, 8, 76, 87, 10, 201, 192, 176, 15,
	41, 63, 24, 125, 104, 8, 157, 152, 8, 254, 96, 8, 76, 87, 10, 201,
	208, 176, 15, 200, 254, 96, 8, 41, 15, 141, 27, 8, 141, 28, 8, 76,
	106, 10, 201, 224, 176, 22, 177, 250, 133, 252, 200, 177, 250, 133, 253, 200,
	152, 157, 96, 8, 165, 252, 32, 14, 17, 76, 87, 10, 201, 240, 176, 46,
	177, 250, 133, 252, 200, 177, 250, 133, 253, 165, 252, 32, 14, 17, 188, 96,
	8, 200, 200, 177, 250, 157, 208, 8, 41, 240, 157, 216, 8, 177, 250, 10,
	10, 10, 10, 157, 224, 8, 200, 152, 157, 96, 8, 76, 87, 10, 201, 255,
	176, 11, 233, 239, 157, 112, 8, 254, 96, 8, 76, 87, 10, 169, 64, 157,
	112, 8, 76, 87, 10, 32, 181, 12, 162, 7, 189, 120, 8, 240, 115, 76,
	217, 13, 189, 14, 8, 240, 14, 169, 0, 157, 32, 8, 157, 40, 8, 202,
	16, 232, 76, 31, 12, 164, 253, 185, 0, 6, 24, 101, 252, 157, 56, 8,
	152, 157, 160, 8, 189, 176, 8, 61, 168, 9, 240, 40, 165, 253, 41, 127,
	168, 185, 0, 7, 24, 101, 252, 157, 55, 8, 185, 128, 7, 105, 0, 157,
	56, 8, 169, 0, 157, 31, 8, 188, 152, 9, 153, 39, 8, 202, 202, 16,
	169, 76, 31, 12, 189, 176, 8, 61, 160, 9, 240, 22, 189, 104, 9, 24,
	101, 253, 157, 162, 8, 168, 185, 0, 6, 24, 101, 252, 56, 101, 254, 157,
	58, 8, 202, 16, 133, 232, 134, 252, 162, 3, 173, 9, 8, 240, 6, 41,
	64, 208, 60, 162, 7, 138, 168, 185, 32, 8, 208, 12, 188, 152, 9, 185,
	40, 8, 208, 4, 138, 168, 169, 0, 25, 168, 8, 157, 48, 8, 185, 56,
	8, 157, 72, 8, 185, 160, 8, 157, 64, 8, 185, 176, 8, 5, 252, 133,
	252, 224, 4, 208, 3, 141, 31, 8, 202, 16, 202, 141, 30, 8, 96, 189,
	32, 8, 29, 168, 8, 157, 48, 8, 189, 44, 8, 29, 172, 8, 157, 52,
	8, 189, 56, 8, 157, 72, 8, 189, 60, 8, 157, 76, 8, 189, 160, 8,
	157, 64, 8, 189, 164, 8, 157, 68, 8, 202, 16, 211, 173, 176, 8, 13,
	177, 8, 13, 178, 8, 13, 179, 8, 141, 30, 8, 173, 180, 8, 13, 181,
	8, 13, 182, 8, 13, 183, 8, 141, 31, 8, 96, 173, 9, 8, 208, 3,
	76, 144, 13, 48, 3, 76, 72, 13, 173, 13, 8, 170, 74, 74, 41, 1,
	168, 185, 30, 8, 141, 56, 210, 138, 41, 4, 168, 185, 56, 8, 141, 48,
	210, 189, 32, 8, 141, 49, 210, 185, 57, 8, 141, 50, 210, 189, 33, 8,
	141, 51, 210, 185, 58, 8, 141, 52, 210, 189, 34, 8, 141, 53, 210, 185,
	59, 8, 141, 54, 210, 189, 35, 8, 141, 55, 210, 173, 12, 8, 170, 74,
	74, 41, 1, 168, 185, 30, 8, 141, 40, 210, 138, 41, 4, 168, 185, 56,
	8, 141, 32, 210, 189, 32, 8, 141, 33, 210, 185, 57, 8, 141, 34, 210,
	189, 33, 8, 141, 35, 210, 185, 58, 8, 141, 36, 210, 189, 34, 8, 141,
	37, 210, 185, 59, 8, 141, 38, 210, 189, 35, 8, 141, 39, 210, 173, 11,
	8, 170, 74, 74, 41, 1, 168, 185, 30, 8, 141, 24, 210, 138, 172, 9,
	8, 16, 2, 41, 4, 168, 185, 56, 8, 141, 16, 210, 189, 32, 8, 141,
	17, 210, 185, 57, 8, 141, 18, 210, 189, 33, 8, 141, 19, 210, 185, 58,
	8, 141, 20, 210, 189, 34, 8, 141, 21, 210, 185, 59, 8, 141, 22, 210,
	189, 35, 8, 141, 23, 210, 173, 10, 8, 170, 74, 74, 41, 1, 168, 185,
	30, 8, 141, 8, 210, 138, 172, 9, 8, 16, 2, 41, 4, 168, 185, 56,
	8, 141, 0, 210, 189, 32, 8, 141, 1, 210, 185, 57, 8, 141, 2, 210,
	189, 33, 8, 141, 3, 210, 185, 58, 8, 141, 4, 210, 189, 34, 8, 141,
	5, 210, 185, 59, 8, 141, 6, 210, 189, 35, 8, 141, 7, 210, 96, 189,
	128, 8, 133, 250, 189, 136, 8, 133, 251, 189, 128, 9, 133, 252, 189, 136,
	9, 133, 253, 189, 144, 9, 133, 254, 189, 184, 8, 221, 192, 8, 144, 12,
	157, 8, 9, 189, 200, 8, 157, 184, 8, 76, 11, 14, 189, 8, 9, 240,
	48, 189, 232, 8, 240, 19, 222, 248, 8, 208, 14, 157, 248, 8, 189, 216,
	8, 240, 6, 56, 233, 16, 157, 216, 8, 189, 240, 8, 240, 19, 222, 0,
	9, 208, 14, 157, 0, 9, 189, 224, 8, 240, 6, 56, 233, 16, 157, 224,
	8, 188, 72, 9, 177, 250, 24, 125, 152, 8, 24, 101, 253, 133, 253, 222,
	88, 9, 16, 57, 189, 80, 9, 157, 88, 9, 189, 96, 9, 240, 30, 24,
	125, 72, 9, 157, 72, 9, 240, 13, 221, 64, 9, 144, 32, 169, 255, 157,
	96, 9, 76, 135, 14, 169, 1, 157, 96, 9, 76, 135, 14, 254, 72, 9,
	189, 64, 9, 221, 72, 9, 176, 5, 169, 0, 157, 72, 9, 169, 19, 24,
	101, 250, 133, 250, 144, 2, 230, 251, 188, 184, 8, 177, 250, 41, 240, 157,
	168, 8, 177, 250, 41, 15, 29, 216, 8, 168, 185, 0, 5, 5, 255, 168,
	185, 0, 5, 157, 32, 8, 188, 184, 8, 200, 177, 250, 41, 15, 29, 224,
	8, 168, 185, 0, 5, 5, 255, 168, 185, 0, 5, 157, 40, 8, 189, 40,
	9, 208, 39, 189, 16, 9, 141, 212, 14, 16, 254, 76, 209, 15, 234, 76,
	108, 15, 234, 76, 167, 15, 234, 76, 212, 15, 234, 76, 1, 16, 234, 76,
	33, 16, 234, 76, 65, 16, 234, 76, 73, 16, 222, 40, 9, 188, 184, 8,
	200, 177, 250, 41, 112, 74, 74, 74, 141, 34, 15, 177, 250, 48, 6, 189,
	112, 9, 76, 18, 15, 189, 120, 9, 61, 176, 9, 157, 176, 8, 200, 200,
	152, 157, 184, 8, 136, 177, 250, 144, 254, 144, 22, 144, 12, 144, 34, 144,
	24, 144, 46, 144, 36, 144, 50, 144, 52, 125, 128, 9, 157, 128, 9, 177,
	250, 24, 101, 252, 133, 252, 76, 172, 11, 125, 136, 9, 157, 136, 9, 177,
	250, 24, 101, 253, 133, 253, 76, 172, 11, 125, 144, 9, 157, 144, 9, 177,
	250, 24, 101, 254, 133, 254, 76, 172, 11, 133, 252, 169, 0, 133, 253, 76,
	172, 11, 189, 32, 9, 41, 3, 74, 144, 10, 208, 25, 189, 24, 9, 24,
	101, 252, 133, 252, 222, 56, 9, 16, 78, 254, 32, 9, 189, 48, 9, 157,
	56, 9, 76, 247, 14, 165, 252, 253, 24, 9, 133, 252, 222, 56, 9, 16,
	54, 254, 32, 9, 189, 48, 9, 157, 56, 9, 76, 247, 14, 188, 32, 9,
	189, 24, 9, 48, 2, 200, 200, 136, 152, 24, 101, 252, 133, 252, 222, 56,
	9, 16, 20, 152, 157, 32, 9, 221, 24, 9, 208, 5, 73, 255, 157, 24,
	9, 189, 48, 9, 157, 56, 9, 76, 247, 14, 188, 32, 9, 189, 24, 9,
	48, 2, 200, 200, 136, 152, 24, 101, 253, 133, 253, 222, 56, 9, 16, 231,
	152, 157, 32, 9, 221, 24, 9, 208, 216, 73, 255, 157, 24, 9, 189, 48,
	9, 157, 56, 9, 76, 247, 14, 189, 32, 9, 24, 101, 252, 133, 252, 222,
	56, 9, 16, 195, 189, 24, 9, 24, 125, 32, 9, 157, 32, 9, 189, 48,
	9, 157, 56, 9, 76, 247, 14, 165, 253, 56, 253, 32, 9, 133, 253, 222,
	56, 9, 16, 163, 189, 24, 9, 24, 125, 32, 9, 157, 32, 9, 189, 48,
	9, 157, 56, 9, 76, 247, 14, 189, 24, 9, 24, 101, 252, 133, 252, 76,
	247, 14, 160, 16, 169, 0, 133, 250, 169, 0, 133, 251, 169, 0, 141, 23,
	8, 138, 240, 63, 177, 250, 240, 2, 16, 1, 202, 169, 17, 24, 101, 250,
	133, 250, 144, 2, 230, 251, 238, 23, 8, 208, 230, 162, 0, 169, 0, 133,
	252, 138, 141, 23, 8, 10, 38, 252, 10, 38, 252, 10, 38, 252, 10, 38,
	252, 109, 23, 8, 144, 2, 230, 252, 24, 105, 0, 133, 250, 165, 252, 105,
	0, 133, 251, 32, 44, 18, 165, 250, 141, 25, 8, 165, 251, 141, 26, 8,
	162, 7, 169, 255, 157, 208, 8, 169, 240, 157, 216, 8, 157, 224, 8, 202,
	16, 240, 169, 3, 141, 15, 210, 141, 31, 210, 141, 47, 210, 141, 63, 210,
	206, 23, 8, 232, 142, 28, 8, 232, 142, 29, 8, 142, 22, 8, 96, 138,
	41, 15, 141, 27, 8, 96, 142, 22, 8, 96, 201, 16, 176, 3, 76, 76,
	16, 201, 32, 144, 136, 201, 48, 176, 3, 76, 133, 18, 201, 64, 144, 223,
	201, 80, 176, 3, 76, 44, 18, 201, 96, 144, 219, 201, 112, 144, 3, 76,
	180, 17, 132, 253, 41, 15, 10, 141, 23, 17, 165, 253, 144, 254, 144, 30,
	144, 56, 144, 89, 144, 96, 144, 26, 144, 28, 144, 30, 144, 32, 144, 34,
	144, 36, 144, 13, 144, 11, 144, 9, 144, 7, 144, 5, 144, 3, 141, 24,
	8, 96, 157, 104, 9, 96, 157, 112, 9, 96, 157, 120, 9, 96, 157, 144,
	9, 96, 157, 128, 9, 96, 157, 136, 9, 96, 41, 112, 74, 74, 157, 16,
	9, 41, 48, 208, 3, 157, 32, 9, 165, 253, 48, 6, 41, 15, 157, 24,
	9, 96, 41, 15, 73, 255, 24, 105, 1, 157, 24, 9, 96, 41, 63, 157,
	48, 9, 157, 56, 9, 96, 41, 128, 10, 42, 157, 96, 9, 165, 253, 41,
	112, 74, 74, 74, 74, 157, 64, 9, 208, 3, 157, 96, 9, 165, 253, 41,
	15, 157, 80, 9, 157, 88, 9, 189, 72, 9, 221, 64, 9, 144, 143, 189,
	64, 9, 240, 2, 233, 1, 157, 72, 9, 96, 132, 250, 134, 251, 160, 25,
	177, 250, 200, 141, 9, 8, 177, 250, 200, 141, 10, 8, 177, 250, 200, 141,
	11, 8, 177, 250, 200, 141, 12, 8, 177, 250, 200, 141, 13, 8, 177, 250,
	141, 27, 8, 165, 250, 73, 128, 48, 1, 232, 141, 172, 18, 142, 173, 18,
	73, 128, 48, 1, 232, 141, 29, 10, 142, 30, 10, 232, 141, 35, 10, 142,
	36, 10, 232, 141, 162, 18, 142, 163, 18, 73, 128, 48, 1, 232, 141, 25,
	8, 141, 215, 9, 141, 79, 16, 141, 148, 16, 142, 26, 8, 142, 221, 9,
	142, 83, 16, 142, 154, 16, 169, 240, 133, 255, 169, 0, 141, 22, 8, 141,
	24, 8, 162, 7, 169, 0, 141, 22, 8, 157, 120, 8, 157, 176, 8, 157,
	32, 8, 157, 40, 8, 157, 48, 8, 157, 48, 210, 157, 32, 210, 157, 16,
	210, 157, 0, 210, 202, 16, 226, 141, 24, 210, 141, 8, 210, 141, 56, 210,
	141, 40, 210, 141, 30, 8, 141, 31, 8, 96, 157, 32, 8, 157, 40, 8,
	157, 48, 8, 157, 176, 8, 96, 152, 157, 208, 8, 41, 240, 157, 216, 8,
	189, 208, 8, 10, 10, 10, 10, 157, 224, 8, 96, 41, 7, 133, 250, 138,
	166, 250, 41, 63, 240, 225, 157, 152, 8, 152, 48, 238, 189, 208, 8, 32,
	117, 18, 169, 0, 157, 120, 8, 185, 255, 255, 240, 190, 157, 136, 8, 133,
	251, 185, 255, 255, 157, 128, 8, 133, 250, 152, 157, 144, 8, 160, 8, 177,
	250, 200, 157, 192, 8, 177, 250, 200, 157, 200, 8, 177, 250, 200, 157, 104,
	9, 177, 250, 200, 157, 112, 9, 177, 250, 200, 157, 120, 9, 177, 250, 200,
	157, 232, 8, 157, 248, 8, 177, 250, 200, 157, 240, 8, 157, 0, 9, 177,
	250, 41, 112, 74, 74, 157, 16, 9, 177, 250, 200, 48, 8, 41, 15, 157,
	24, 9, 76, 9, 19, 41, 15, 73, 255, 24, 105, 1, 157, 24, 9, 177,
	250, 200, 157, 40, 9, 177, 250, 200, 41, 63, 157, 48, 9, 157, 56, 9,
	177, 250, 41, 128, 10, 42, 157, 96, 9, 177, 250, 41, 112, 74, 74, 74,
	74, 157, 64, 9, 208, 3, 157, 96, 9, 177, 250, 136, 41, 15, 157, 80,
	9, 157, 88, 9, 177, 250, 41, 192, 29, 152, 8, 157, 152, 8, 168, 185,
	0, 6, 157, 56, 8, 169, 0, 157, 184, 8, 157, 32, 9, 157, 8, 9,
	157, 72, 9, 157, 128, 9, 157, 136, 9, 157, 144, 9, 169, 1, 157, 120,
	8, 96 };
static const uint8_t CiResource_tmc_obx[2671] = {
	255, 255, 0, 5, 104, 15, 76, 206, 13, 76, 208, 8, 76, 239, 9, 15,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
	2, 2, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3,
	3, 3, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
	4, 4, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4,
	5, 5, 0, 0, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 5, 5,
	6, 6, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
	7, 7, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7,
	7, 8, 0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6, 7, 7, 8,
	8, 9, 0, 1, 1, 2, 3, 3, 4, 5, 5, 6, 7, 7, 8, 9,
	9, 10, 0, 1, 1, 2, 3, 4, 4, 5, 6, 7, 7, 8, 9, 10,
	10, 11, 0, 1, 2, 2, 3, 4, 5, 6, 6, 7, 8, 9, 10, 10,
	11, 12, 0, 1, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10, 10, 11,
	12, 13, 0, 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 12,
	13, 14, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
	14, 15, 0, 241, 228, 215, 203, 192, 181, 170, 161, 152, 143, 135, 127, 120,
	114, 107, 101, 95, 90, 85, 80, 75, 71, 67, 63, 60, 56, 53, 50, 47,
	44, 42, 39, 37, 35, 33, 31, 29, 28, 26, 24, 23, 22, 20, 19, 18,
	17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
	1, 0, 0, 242, 230, 218, 206, 191, 182, 170, 161, 152, 143, 137, 128, 122,
	113, 107, 101, 95, 92, 86, 80, 77, 71, 68, 62, 60, 56, 53, 50, 47,
	45, 42, 40, 37, 35, 33, 31, 29, 28, 26, 24, 23, 22, 20, 19, 18,
	17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
	1, 0, 0, 255, 241, 228, 216, 202, 192, 181, 171, 162, 153, 142, 135, 127,
	121, 115, 112, 102, 97, 90, 85, 82, 75, 72, 67, 63, 60, 57, 55, 51,
	48, 45, 42, 40, 37, 36, 33, 31, 30, 28, 27, 25, 23, 22, 21, 19,
	18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3,
	2, 1, 0, 243, 230, 217, 204, 193, 181, 173, 162, 153, 144, 136, 128, 121,
	114, 108, 102, 96, 91, 85, 81, 76, 72, 68, 64, 60, 57, 53, 50, 47,
	45, 42, 40, 37, 35, 33, 31, 29, 28, 26, 24, 23, 22, 20, 19, 18,
	17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
	1, 0, 0, 242, 51, 150, 226, 56, 140, 0, 106, 232, 106, 239, 128, 8,
	174, 70, 230, 149, 65, 246, 176, 110, 48, 246, 187, 132, 82, 34, 244, 200,
	160, 122, 85, 52, 20, 245, 216, 189, 164, 141, 119, 96, 78, 56, 39, 21,
	6, 247, 232, 219, 207, 195, 184, 172, 162, 154, 144, 136, 127, 120, 112, 106,
	100, 94, 0, 13, 13, 12, 11, 11, 10, 10, 9, 8, 8, 7, 7, 7,
	6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 3, 2, 2,
	2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 5,
	6, 7, 0, 1, 2, 3, 4, 2, 0, 0, 4, 2, 0, 0, 0, 16,
	0, 8, 0, 16, 0, 8, 173, 183, 8, 240, 94, 173, 182, 8, 201, 64,
	144, 90, 206, 181, 8, 240, 3, 76, 239, 9, 162, 7, 169, 0, 157, 196,
	7, 157, 204, 7, 202, 16, 247, 141, 182, 8, 170, 160, 15, 177, 254, 16,
	32, 136, 177, 254, 16, 3, 76, 95, 14, 134, 252, 10, 10, 38, 252, 10,
	38, 252, 10, 38, 252, 105, 0, 133, 254, 165, 252, 105, 0, 133, 255, 144,
	218, 157, 212, 7, 136, 177, 254, 157, 220, 7, 232, 136, 16, 207, 24, 165,
	254, 105, 16, 133, 254, 144, 2, 230, 255, 76, 239, 9, 206, 181, 8, 16,
	248, 238, 182, 8, 173, 180, 8, 141, 181, 8, 162, 7, 222, 204, 7, 48,
	3, 76, 233, 9, 188, 212, 7, 185, 255, 255, 133, 252, 185, 255, 255, 133,
	253, 188, 196, 7, 177, 252, 208, 6, 32, 109, 13, 76, 230, 9, 201, 64,
	176, 18, 125, 220, 7, 157, 228, 7, 32, 109, 13, 188, 42, 5, 32, 188,
	14, 76, 230, 9, 208, 34, 200, 254, 196, 7, 177, 252, 16, 7, 133, 251,
	32, 109, 13, 165, 251, 41, 127, 208, 7, 169, 64, 141, 182, 8, 208, 76,
	141, 180, 8, 141, 181, 8, 208, 68, 201, 128, 176, 43, 41, 63, 125, 220,
	7, 157, 228, 7, 200, 254, 196, 7, 177, 252, 41, 127, 208, 7, 169, 64,
	141, 182, 8, 208, 6, 141, 180, 8, 141, 181, 8, 32, 109, 13, 188, 42,
	5, 32, 188, 14, 76, 230, 9, 201, 192, 176, 12, 41, 63, 157, 42, 5,
	200, 254, 196, 7, 76, 94, 9, 41, 63, 157, 204, 7, 254, 196, 7, 202,
	48, 3, 76, 70, 9, 162, 7, 189, 188, 7, 240, 33, 32, 46, 11, 189,
	50, 5, 61, 192, 8, 240, 22, 160, 71, 177, 252, 24, 125, 34, 5, 157,
	36, 5, 168, 185, 60, 6, 56, 125, 100, 8, 157, 246, 7, 202, 16, 215,
	14, 9, 5, 14, 9, 5, 14, 9, 5, 14, 9, 5, 232, 134, 252, 134,
	253, 162, 7, 138, 168, 185, 252, 7, 208, 12, 188, 184, 8, 185, 4, 8,
	208, 4, 138, 168, 169, 0, 133, 250, 152, 157, 26, 5, 185, 244, 7, 157,
	18, 5, 185, 50, 5, 133, 251, 5, 253, 133, 253, 165, 251, 61, 192, 8,
	240, 6, 185, 246, 7, 157, 20, 5, 165, 251, 61, 200, 8, 240, 18, 185,
	34, 5, 41, 63, 168, 200, 132, 252, 185, 123, 7, 157, 18, 5, 76, 137,
	10, 164, 252, 240, 10, 185, 59, 7, 157, 18, 5, 169, 0, 133, 252, 165,
	250, 13, 9, 5, 168, 185, 60, 5, 188, 26, 5, 25, 236, 7, 157, 10,
	5, 224, 4, 208, 9, 165, 253, 141, 59, 5, 169, 0, 133, 253, 202, 16,
	130, 78, 9, 5, 78, 9, 5, 78, 9, 5, 78, 9, 5, 165, 253, 162,
	3, 142, 31, 210, 142, 15, 210, 174, 22, 5, 172, 18, 5, 142, 16, 210,
	140, 0, 210, 174, 14, 5, 172, 10, 5, 142, 17, 210, 140, 1, 210, 174,
	23, 5, 172, 19, 5, 142, 18, 210, 140, 2, 210, 174, 15, 5, 172, 11,
	5, 142, 19, 210, 140, 3, 210, 174, 24, 5, 172, 20, 5, 142, 20, 210,
	140, 4, 210, 174, 16, 5, 172, 12, 5, 142, 21, 210, 140, 5, 210, 174,
	25, 5, 172, 21, 5, 142, 22, 210, 140, 6, 210, 174, 17, 5, 172, 13,
	5, 142, 23, 210, 140, 7, 210, 141, 58, 5, 174, 59, 5, 142, 24, 210,
	141, 8, 210, 96, 189, 28, 8, 133, 252, 189, 36, 8, 133, 253, 188, 44,
	8, 192, 63, 240, 123, 254, 44, 8, 254, 44, 8, 254, 44, 8, 177, 252,
	41, 240, 157, 236, 7, 177, 252, 41, 15, 56, 253, 12, 8, 16, 2, 169,
	0, 157, 252, 7, 200, 177, 252, 41, 15, 56, 253, 20, 8, 16, 2, 169,
	0, 157, 4, 8, 177, 252, 41, 240, 240, 116, 16, 11, 160, 73, 177, 252,
	188, 44, 8, 136, 136, 16, 2, 169, 0, 157, 50, 5, 177, 252, 41, 112,
	240, 99, 74, 74, 141, 154, 11, 169, 0, 157, 100, 8, 200, 177, 252, 144,
	254, 234, 234, 234, 234, 76, 56, 13, 234, 76, 53, 13, 234, 76, 60, 13,
	234, 76, 74, 13, 234, 76, 84, 13, 234, 76, 95, 13, 234, 76, 81, 13,
	189, 52, 8, 240, 18, 222, 68, 8, 208, 13, 157, 68, 8, 189, 252, 7,
	41, 15, 240, 3, 222, 252, 7, 189, 60, 8, 240, 18, 222, 76, 8, 208,
	13, 157, 76, 8, 189, 4, 8, 41, 15, 240, 3, 222, 4, 8, 160, 72,
	177, 252, 157, 50, 5, 189, 148, 8, 24, 105, 63, 168, 177, 252, 125, 228,
	7, 157, 34, 5, 168, 185, 60, 6, 157, 244, 7, 222, 164, 8, 16, 51,
	189, 156, 8, 157, 164, 8, 189, 172, 8, 240, 24, 24, 125, 148, 8, 157,
	148, 8, 240, 7, 221, 140, 8, 208, 26, 169, 254, 24, 105, 1, 157, 172,
	8, 208, 16, 254, 148, 8, 189, 140, 8, 221, 148, 8, 176, 5, 169, 0,
	157, 148, 8, 189, 116, 8, 240, 4, 222, 116, 8, 96, 189, 108, 8, 133,
	250, 189, 92, 8, 133, 251, 32, 105, 12, 222, 132, 8, 16, 16, 165, 250,
	157, 108, 8, 165, 251, 157, 92, 8, 189, 124, 8, 157, 132, 8, 96, 189,
	84, 8, 141, 112, 12, 16, 254, 76, 167, 12, 234, 76, 144, 12, 234, 76,
	174, 12, 234, 76, 180, 12, 234, 76, 190, 12, 234, 76, 210, 12, 234, 76,
	226, 12, 234, 76, 244, 12, 165, 250, 230, 250, 41, 3, 74, 144, 15, 208,
	71, 165, 251, 157, 100, 8, 24, 125, 244, 7, 157, 244, 7, 96, 169, 0,
	157, 100, 8, 96, 32, 29, 13, 76, 157, 12, 32, 29, 13, 24, 125, 34,
	5, 76, 84, 13, 165, 250, 157, 100, 8, 24, 125, 244, 7, 157, 244, 7,
	165, 250, 24, 101, 251, 133, 250, 96, 189, 34, 5, 56, 229, 250, 157, 34,
	5, 168, 185, 60, 6, 76, 199, 12, 189, 244, 7, 56, 229, 251, 157, 244,
	7, 56, 169, 0, 229, 251, 157, 100, 8, 96, 189, 132, 8, 208, 174, 165,
	251, 16, 16, 189, 4, 8, 240, 165, 189, 252, 7, 201, 15, 240, 158, 254,
	252, 7, 96, 189, 252, 7, 240, 149, 189, 4, 8, 201, 15, 240, 142, 254,
	4, 8, 96, 164, 250, 165, 251, 48, 2, 200, 200, 136, 152, 133, 250, 197,
	251, 208, 6, 165, 251, 73, 255, 133, 251, 152, 96, 125, 244, 7, 157, 244,
	7, 96, 188, 228, 7, 121, 60, 6, 157, 244, 7, 152, 157, 34, 5, 96,
	45, 10, 210, 157, 244, 7, 96, 125, 228, 7, 157, 34, 5, 168, 185, 60,
	6, 157, 244, 7, 96, 157, 34, 5, 168, 189, 244, 7, 121, 60, 6, 157,
	244, 7, 96, 200, 254, 196, 7, 177, 252, 74, 74, 74, 74, 157, 12, 8,
	177, 252, 41, 15, 157, 20, 8, 96, 32, 95, 14, 160, 15, 169, 0, 133,
	254, 169, 0, 133, 255, 138, 240, 46, 177, 254, 16, 1, 202, 24, 165, 254,
	105, 16, 133, 254, 144, 239, 230, 255, 176, 235, 32, 95, 14, 169, 0, 133,
	252, 138, 10, 10, 38, 252, 10, 38, 252, 10, 38, 252, 105, 0, 133, 254,
	165, 252, 105, 0, 133, 255, 169, 64, 141, 182, 8, 169, 1, 141, 181, 8,
	141, 183, 8, 96, 201, 16, 144, 176, 201, 32, 144, 206, 201, 48, 176, 3,
	76, 174, 14, 201, 64, 176, 9, 138, 41, 15, 240, 3, 141, 180, 8, 96,
	201, 80, 144, 113, 201, 96, 176, 6, 169, 0, 141, 183, 8, 96, 201, 112,
	144, 248, 169, 1, 141, 181, 8, 169, 64, 141, 182, 8, 132, 252, 134, 253,
	160, 30, 177, 252, 141, 180, 8, 165, 252, 24, 105, 32, 141, 194, 14, 144,
	1, 232, 142, 195, 14, 24, 105, 64, 141, 202, 14, 144, 1, 232, 142, 203,
	14, 24, 105, 64, 141, 82, 9, 144, 1, 232, 142, 83, 9, 24, 105, 128,
	141, 87, 9, 144, 1, 232, 142, 88, 9, 24, 105, 128, 133, 254, 141, 16,
	9, 141, 136, 13, 141, 183, 13, 144, 1, 232, 134, 255, 142, 22, 9, 142,
	140, 13, 142, 189, 13, 160, 7, 169, 0, 141, 183, 8, 153, 0, 210, 153,
	16, 210, 153, 10, 5, 153, 252, 7, 153, 4, 8, 153, 50, 5, 153, 188,
	7, 136, 16, 232, 141, 8, 210, 141, 24, 210, 141, 58, 5, 141, 59, 5,
	96, 157, 252, 7, 157, 4, 8, 157, 50, 5, 189, 228, 7, 157, 34, 5,
	96, 152, 73, 240, 74, 74, 74, 74, 157, 12, 8, 152, 41, 15, 73, 15,
	157, 20, 8, 96, 41, 7, 133, 252, 138, 166, 252, 41, 63, 240, 226, 157,
	228, 7, 169, 0, 157, 188, 7, 185, 255, 255, 157, 28, 8, 133, 252, 185,
	255, 255, 157, 36, 8, 133, 253, 5, 252, 240, 182, 160, 74, 177, 252, 157,
	52, 8, 157, 68, 8, 200, 177, 252, 157, 60, 8, 157, 76, 8, 200, 177,
	252, 41, 112, 74, 74, 157, 84, 8, 177, 252, 41, 15, 157, 92, 8, 177,
	252, 16, 11, 189, 92, 8, 73, 255, 24, 105, 1, 157, 92, 8, 200, 177,
	252, 157, 116, 8, 200, 177, 252, 41, 63, 157, 124, 8, 157, 132, 8, 200,
	177, 252, 41, 128, 240, 2, 169, 1, 157, 172, 8, 177, 252, 41, 112, 74,
	74, 74, 74, 157, 140, 8, 208, 3, 157, 172, 8, 177, 252, 41, 15, 157,
	156, 8, 157, 164, 8, 136, 177, 252, 41, 192, 24, 125, 228, 7, 157, 228,
	7, 157, 34, 5, 168, 185, 60, 6, 157, 244, 7, 169, 0, 157, 44, 8,
	157, 100, 8, 157, 108, 8, 157, 148, 8, 169, 1, 157, 188, 7, 96 };
static const uint8_t CiResource_xexb_obx[183] = {
	255, 255, 36, 1, 223, 1, 120, 160, 0, 140, 14, 212, 173, 11, 212, 208,
	251, 141, 0, 212, 162, 29, 157, 0, 208, 202, 16, 250, 162, 8, 157, 16,
	210, 157, 0, 210, 202, 16, 247, 169, 3, 141, 31, 210, 141, 0, 210, 169,
	130, 205, 11, 212, 208, 251, 141, 10, 212, 141, 10, 212, 141, 10, 212, 173,
	11, 212, 208, 3, 238, 145, 1, 173, 218, 1, 32, 212, 1, 169, 254, 141,
	1, 211, 169, 206, 174, 53, 1, 240, 2, 169, 194, 141, 250, 255, 169, 1,
	141, 251, 255, 169, 64, 141, 14, 212, 169, 125, 205, 11, 212, 208, 251, 32,
	215, 1, 162, 0, 173, 219, 1, 74, 144, 6, 169, 0, 238, 153, 1, 74,
	189, 220, 1, 105, 125, 176, 5, 221, 210, 1, 144, 4, 253, 210, 1, 56,
	141, 162, 1, 189, 222, 1, 105, 0, 141, 181, 1, 201, 0, 208, 252, 173,
	162, 1, 176, 198, 72, 138, 72, 174, 145, 1, 32, 152, 252, 104, 170, 104,
	238, 186, 1, 64, 156, 131, 76 };
static const uint8_t CiResource_xexd_obx[117] = {
	255, 255, 36, 1, 152, 1, 120, 160, 0, 140, 14, 212, 173, 11, 212, 208,
	251, 141, 0, 212, 162, 29, 157, 0, 208, 202, 16, 250, 141, 14, 210, 162,
	8, 157, 16, 210, 157, 0, 210, 202, 16, 247, 169, 3, 141, 31, 210, 141,
	0, 210, 169, 254, 141, 1, 211, 173, 149, 1, 201, 96, 240, 15, 169, 114,
	141, 250, 255, 169, 1, 141, 251, 255, 169, 64, 141, 14, 212, 173, 152, 1,
	88, 76, 146, 1, 40, 8, 72, 138, 72, 152, 72, 32, 149, 1, 174, 53,
	1, 240, 11, 174, 20, 208, 202, 240, 2, 162, 1, 32, 152, 252, 104, 168,
	104, 170, 104, 64, 76 };
static const uint8_t CiResource_xexinfo_obx[178] = {
	255, 255, 112, 252, 221, 252, 65, 80, 252, 173, 11, 212, 208, 251, 141, 5,
	212, 162, 38, 142, 22, 208, 162, 10, 142, 23, 208, 162, 33, 142, 0, 212,
	162, 80, 142, 2, 212, 162, 252, 142, 3, 212, 142, 9, 212, 96, 216, 189,
	218, 252, 24, 105, 0, 141, 158, 252, 189, 220, 252, 105, 0, 141, 166, 252,
	144, 18, 173, 75, 252, 13, 76, 252, 13, 78, 252, 13, 79, 252, 201, 49,
	162, 4, 176, 13, 96, 169, 57, 224, 3, 208, 2, 169, 53, 157, 75, 252,
	202, 189, 75, 252, 201, 49, 144, 237, 201, 58, 240, 244, 222, 75, 252, 96,
	34, 69, 5, 4, 0, 1, 57, 1, 120, 160, 0, 140, 14, 212, 173, 11,
	212, 208, 251, 141, 0, 212, 162, 29, 157, 0, 208, 202, 16, 250, 142, 1,
	211, 185, 0, 224, 72, 185, 0, 225, 72, 185, 0, 227, 202, 142, 1, 211,
	232, 153, 0, 255, 104, 153, 0, 254, 104, 153, 0, 253, 200, 208, 223, 32,
	115, 252 };

static void ASAP_Construct(ASAP *self)
{
	PokeyPair_Construct(&self->pokeys);
	ASAPInfo_Construct(&self->moduleInfo);
	self->silenceCycles = 0;
	self->cpu.asap = self;
}

static void ASAP_Destruct(ASAP *self)
{
	ASAPInfo_Destruct(&self->moduleInfo);
}

ASAP *ASAP_New(void)
{
	ASAP *self = (ASAP *) malloc(sizeof(ASAP));
	if (self != NULL)
		ASAP_Construct(self);
	return self;
}

void ASAP_Delete(ASAP *self)
{
	if (self == NULL)
		return;
	ASAP_Destruct(self);
	free(self);
}

void ASAP_DetectSilence(ASAP *self, int seconds)
{
	self->silenceCyclesCounter = self->silenceCycles = seconds * 1773447;
}

static int ASAP_PeekHardware(const ASAP *self, int addr)
{
	switch (addr & 65311) {
	case 53268:
		return ASAPInfo_IsNtsc(&self->moduleInfo) ? 15 : 1;
	case 53279:
		return ~self->consol & 15;
	case 53770:
	case 53786:
	case 53774:
	case 53790:
		return PokeyPair_Peek(&self->pokeys, addr, self->cpu.cycle);
	case 53772:
	case 53788:
	case 53775:
	case 53791:
		return 255;
	case 54283:
	case 54299:
		;
		int cycle = self->cpu.cycle;
		if (cycle > (ASAPInfo_IsNtsc(&self->moduleInfo) ? 29868 : 35568))
			return 0;
		return cycle / 228;
	case 54287:
	case 54303:
		switch (self->nmist) {
		case NmiStatus_RESET:
			return 31;
		case NmiStatus_WAS_V_BLANK:
			return 95;
		default:
			return self->cpu.cycle < 28291 ? 31 : 95;
		}
	default:
		return self->cpu.memory[addr];
	}
}

static void ASAP_PokeHardware(ASAP *self, int addr, int data)
{
	if (addr >> 8 == 210) {
		int t = PokeyPair_Poke(&self->pokeys, addr, data, self->cpu.cycle);
		if (self->nextEventCycle > t)
			self->nextEventCycle = t;
	}
	else if ((addr & 65295) == 54282) {
		int x = self->cpu.cycle % 114;
		self->cpu.cycle += (x <= 106 ? 106 : 220) - x;
	}
	else if ((addr & 65295) == 54287) {
		self->nmist = self->cpu.cycle < 28292 ? NmiStatus_ON_V_BLANK : NmiStatus_RESET;
	}
	else if ((addr & 65280) == ASAPInfo_GetCovoxAddress(&self->moduleInfo)) {
		Pokey *pokey;
		addr &= 3;
		if (addr == 0 || addr == 3)
			pokey = &self->pokeys.basePokey;
		else
			pokey = &self->pokeys.extraPokey;
		int delta = data - self->covox[addr];
		if (delta != 0) {
			Pokey_AddDelta(pokey, &self->pokeys, self->cpu.cycle, delta << 17);
			self->covox[addr] = (uint8_t) data;
			self->gtiaOrCovoxPlayedThisFrame = true;
		}
	}
	else if ((addr & 65311) == 53279) {
		int delta = ((self->consol & 8) - (data & 8)) << 20;
		if (delta != 0) {
			int cycle = self->cpu.cycle;
			Pokey_AddDelta(&self->pokeys.basePokey, &self->pokeys, cycle, delta);
			Pokey_AddDelta(&self->pokeys.extraPokey, &self->pokeys, cycle, delta);
			self->gtiaOrCovoxPlayedThisFrame = true;
		}
		self->consol = data;
	}
	else
		self->cpu.memory[addr] = (uint8_t) data;
}

static void ASAP_Call6502(ASAP *self, int addr)
{
	self->cpu.memory[53760] = 32;
	self->cpu.memory[53761] = (uint8_t) (addr & 255);
	self->cpu.memory[53762] = (uint8_t) (addr >> 8);
	self->cpu.memory[53763] = 210;
	self->cpu.pc = 53760;
}

static void ASAP_Call6502Player(ASAP *self)
{
	int player = self->moduleInfo.player;
	switch (self->moduleInfo.type) {
	case ASAPModuleType_SAP_B:
		ASAP_Call6502(self, player);
		break;
	case ASAPModuleType_SAP_C:
	case ASAPModuleType_CMC:
	case ASAPModuleType_CM3:
	case ASAPModuleType_CMR:
	case ASAPModuleType_CMS:
		ASAP_Call6502(self, player + 6);
		break;
	case ASAPModuleType_SAP_D:
		if (player >= 0) {
			Cpu6502_PushPc(&self->cpu);
			self->cpu.memory[53760] = 8;
			self->cpu.memory[53761] = 72;
			self->cpu.memory[53762] = 138;
			self->cpu.memory[53763] = 72;
			self->cpu.memory[53764] = 152;
			self->cpu.memory[53765] = 72;
			self->cpu.memory[53766] = 32;
			self->cpu.memory[53767] = (uint8_t) (player & 255);
			self->cpu.memory[53768] = (uint8_t) (player >> 8);
			self->cpu.memory[53769] = 104;
			self->cpu.memory[53770] = 168;
			self->cpu.memory[53771] = 104;
			self->cpu.memory[53772] = 170;
			self->cpu.memory[53773] = 104;
			self->cpu.memory[53774] = 64;
			self->cpu.pc = 53760;
		}
		break;
	case ASAPModuleType_SAP_S:
		;
		int i = self->cpu.memory[69] - 1;
		self->cpu.memory[69] = (uint8_t) (i & 255);
		if (i == 0)
			self->cpu.memory[45179] = (uint8_t) ((self->cpu.memory[45179] + 1) & 255);
		break;
	case ASAPModuleType_DLT:
		ASAP_Call6502(self, player + 259);
		break;
	case ASAPModuleType_MPT:
	case ASAPModuleType_RMT:
	case ASAPModuleType_TM2:
	case ASAPModuleType_FC:
		ASAP_Call6502(self, player + 3);
		break;
	case ASAPModuleType_TMC:
		if (--self->tmcPerFrameCounter <= 0) {
			self->tmcPerFrameCounter = self->cpu.memory[ASAPInfo_GetMusicAddress(&self->moduleInfo) + 31];
			ASAP_Call6502(self, player + 3);
		}
		else
			ASAP_Call6502(self, player + 6);
		break;
	}
}

static bool ASAP_IsIrq(const ASAP *self)
{
	return self->pokeys.basePokey.irqst != 255;
}

static void ASAP_HandleEvent(ASAP *self)
{
	int cycle = self->cpu.cycle;
	if (cycle >= self->nextScanlineCycle) {
		if (cycle - self->nextScanlineCycle < 50)
			self->cpu.cycle = cycle += 9;
		self->nextScanlineCycle += 114;
		if (cycle >= self->nextPlayerCycle) {
			ASAP_Call6502Player(self);
			self->nextPlayerCycle += 114 * ASAPInfo_GetPlayerRateScanlines(&self->moduleInfo);
		}
	}
	int nextEventCycle = self->nextScanlineCycle;
	nextEventCycle = Pokey_CheckIrq(&self->pokeys.basePokey, cycle, nextEventCycle);
	nextEventCycle = Pokey_CheckIrq(&self->pokeys.extraPokey, cycle, nextEventCycle);
	self->nextEventCycle = nextEventCycle;
}

static int ASAP_Do6502Frame(ASAP *self)
{
	self->nextEventCycle = 0;
	self->nextScanlineCycle = 0;
	self->nmist = self->nmist == NmiStatus_RESET ? NmiStatus_ON_V_BLANK : NmiStatus_WAS_V_BLANK;
	int cycles = ASAPInfo_IsNtsc(&self->moduleInfo) ? 29868 : 35568;
	Cpu6502_DoFrame(&self->cpu, cycles);
	self->cpu.cycle -= cycles;
	if (self->nextPlayerCycle != 8388608)
		self->nextPlayerCycle -= cycles;
	for (int i = 3;; i >>= 1) {
		PokeyChannel_EndFrame(&self->pokeys.basePokey.channels[i], cycles);
		PokeyChannel_EndFrame(&self->pokeys.extraPokey.channels[i], cycles);
		if (i == 0)
			break;
	}
	return cycles;
}

static int ASAP_DoFrame(ASAP *self)
{
	self->gtiaOrCovoxPlayedThisFrame = false;
	PokeyPair_StartFrame(&self->pokeys);
	int cycles = ASAP_Do6502Frame(self);
	PokeyPair_EndFrame(&self->pokeys, cycles);
	return cycles;
}

bool ASAP_Load(ASAP *self, const char *filename, uint8_t const *module, int moduleLen)
{
	if (!ASAPInfo_Load(&self->moduleInfo, filename, module, moduleLen))
		return false;
	uint8_t const *playerRoutine = ASAP6502_GetPlayerRoutine(&self->moduleInfo);
	if (playerRoutine != NULL) {
		int player = ASAPInfo_GetWord(playerRoutine, 2);
		int playerLastByte = ASAPInfo_GetWord(playerRoutine, 4);
		int music = ASAPInfo_GetMusicAddress(&self->moduleInfo);
		if (music <= playerLastByte)
			return false;
		self->cpu.memory[19456] = 0;
		if (self->moduleInfo.type == ASAPModuleType_FC)
			memcpy(self->cpu.memory + music, module, moduleLen);
		else
			memcpy(self->cpu.memory + music, module + 6, moduleLen - 6);
		memcpy(self->cpu.memory + player, playerRoutine + 6, playerLastByte + 1 - player);
		if (self->moduleInfo.player < 0)
			self->moduleInfo.player = player;
		return true;
	}
	memset(self->cpu.memory, 0, sizeof(self->cpu.memory));
	int moduleIndex = self->moduleInfo.headerLen + 2;
	while (moduleIndex + 5 <= moduleLen) {
		int startAddr = ASAPInfo_GetWord(module, moduleIndex);
		int blockLen = ASAPInfo_GetWord(module, moduleIndex + 2) + 1 - startAddr;
		if (blockLen <= 0 || moduleIndex + blockLen > moduleLen)
			return false;
		moduleIndex += 4;
		memcpy(self->cpu.memory + startAddr, module + moduleIndex, blockLen);
		moduleIndex += blockLen;
		if (moduleIndex == moduleLen)
			return true;
		if (moduleIndex + 7 <= moduleLen && module[moduleIndex] == 255 && module[moduleIndex + 1] == 255)
			moduleIndex += 2;
	}
	return false;
}

const ASAPInfo *ASAP_GetInfo(const ASAP *self)
{
	return &self->moduleInfo;
}

static bool ASAP_Do6502Init(ASAP *self, int pc, int a, int x, int y)
{
	self->cpu.pc = pc;
	self->cpu.a = a & 255;
	self->cpu.x = x & 255;
	self->cpu.y = y & 255;
	self->cpu.memory[53760] = 210;
	self->cpu.memory[510] = 255;
	self->cpu.memory[511] = 209;
	self->cpu.s = 253;
	for (int frame = 0; frame < 50; frame++) {
		ASAP_Do6502Frame(self);
		if (self->cpu.pc == 53760)
			return true;
	}
	return false;
}

void ASAP_MutePokeyChannels(ASAP *self, int mask)
{
	Pokey_Mute(&self->pokeys.basePokey, mask);
	Pokey_Mute(&self->pokeys.extraPokey, mask >> 4);
}

bool ASAP_PlaySong(ASAP *self, int song, int duration)
{
	if (song < 0 || song >= ASAPInfo_GetSongs(&self->moduleInfo))
		return false;
	self->currentSong = song;
	self->currentDuration = duration;
	self->nextPlayerCycle = 8388608;
	self->blocksPlayed = 0;
	self->silenceCyclesCounter = self->silenceCycles;
	Cpu6502_Reset(&self->cpu);
	self->nmist = NmiStatus_ON_V_BLANK;
	self->consol = 8;
	self->covox[0] = 128;
	self->covox[1] = 128;
	self->covox[2] = 128;
	self->covox[3] = 128;
	PokeyPair_Initialize(&self->pokeys, ASAPInfo_IsNtsc(&self->moduleInfo), ASAPInfo_GetChannels(&self->moduleInfo) > 1);
	ASAP_MutePokeyChannels(self, 255);
	int player = self->moduleInfo.player;
	int music = ASAPInfo_GetMusicAddress(&self->moduleInfo);
	switch (self->moduleInfo.type) {
	case ASAPModuleType_SAP_B:
		if (!ASAP_Do6502Init(self, ASAPInfo_GetInitAddress(&self->moduleInfo), song, 0, 0))
			return false;
		break;
	case ASAPModuleType_SAP_C:
	case ASAPModuleType_CMC:
	case ASAPModuleType_CM3:
	case ASAPModuleType_CMR:
	case ASAPModuleType_CMS:
		if (!ASAP_Do6502Init(self, player + 3, 112, music, music >> 8))
			return false;
		if (!ASAP_Do6502Init(self, player + 3, 0, song, 0))
			return false;
		break;
	case ASAPModuleType_SAP_D:
	case ASAPModuleType_SAP_S:
		self->cpu.pc = ASAPInfo_GetInitAddress(&self->moduleInfo);
		self->cpu.a = song;
		self->cpu.x = 0;
		self->cpu.y = 0;
		self->cpu.s = 255;
		break;
	case ASAPModuleType_DLT:
		if (!ASAP_Do6502Init(self, player + 256, 0, 0, self->moduleInfo.songPos[song]))
			return false;
		break;
	case ASAPModuleType_MPT:
		if (!ASAP_Do6502Init(self, player, 0, music >> 8, music))
			return false;
		if (!ASAP_Do6502Init(self, player, 2, self->moduleInfo.songPos[song], 0))
			return false;
		break;
	case ASAPModuleType_RMT:
		if (!ASAP_Do6502Init(self, player, self->moduleInfo.songPos[song], music, music >> 8))
			return false;
		break;
	case ASAPModuleType_TMC:
	case ASAPModuleType_TM2:
		if (!ASAP_Do6502Init(self, player, 112, music >> 8, music))
			return false;
		if (!ASAP_Do6502Init(self, player, 0, song, 0))
			return false;
		self->tmcPerFrameCounter = 1;
		break;
	case ASAPModuleType_FC:
		if (!ASAP_Do6502Init(self, player, song, 0, 0))
			return false;
		break;
	}
	ASAP_MutePokeyChannels(self, 0);
	self->nextPlayerCycle = 0;
	return true;
}

int ASAP_GetBlocksPlayed(const ASAP *self)
{
	return self->blocksPlayed;
}

int ASAP_GetPosition(const ASAP *self)
{
	return self->blocksPlayed * 10 / 441;
}

static int ASAP_MillisecondsToBlocks(int milliseconds)
{
	return milliseconds * 441 / 10;
}

bool ASAP_SeekSample(ASAP *self, int block)
{
	if (block < self->blocksPlayed) {
		if (!ASAP_PlaySong(self, self->currentSong, self->currentDuration))
			return false;
	}
	while (self->blocksPlayed + self->pokeys.readySamplesEnd < block) {
		self->blocksPlayed += self->pokeys.readySamplesEnd;
		ASAP_DoFrame(self);
	}
	self->pokeys.readySamplesStart = block - self->blocksPlayed;
	self->blocksPlayed = block;
	return true;
}

bool ASAP_Seek(ASAP *self, int position)
{
	return ASAP_SeekSample(self, ASAP_MillisecondsToBlocks(position));
}

static void ASAP_PutLittleEndian(uint8_t *buffer, int offset, int value)
{
	buffer[offset] = (uint8_t) (value & 255);
	buffer[offset + 1] = (uint8_t) (value >> 8 & 255);
	buffer[offset + 2] = (uint8_t) (value >> 16 & 255);
	buffer[offset + 3] = (uint8_t) (value >> 24 & 255);
}

static void ASAP_PutLittleEndians(uint8_t *buffer, int offset, int value1, int value2)
{
	ASAP_PutLittleEndian(buffer, offset, value1);
	ASAP_PutLittleEndian(buffer, offset + 4, value2);
}

static int ASAP_PutWavMetadata(uint8_t *buffer, int offset, int fourCC, const char *value)
{
	int len = (int) strlen(value);
	if (len > 0) {
		ASAP_PutLittleEndians(buffer, offset, fourCC, (len | 1) + 1);
		offset += 8;
		for (int i = 0; i < len; i++)
			buffer[offset++] = (uint8_t) value[i];
		buffer[offset++] = 0;
		if ((len & 1) == 0)
			buffer[offset++] = 0;
	}
	return offset;
}

int ASAP_GetWavHeader(const ASAP *self, uint8_t *buffer, ASAPSampleFormat format, bool metadata)
{
	int use16bit = format != ASAPSampleFormat_U8 ? 1 : 0;
	int blockSize = ASAPInfo_GetChannels(&self->moduleInfo) << use16bit;
	int bytesPerSecond = 44100 * blockSize;
	int totalBlocks = ASAP_MillisecondsToBlocks(self->currentDuration);
	int nBytes = (totalBlocks - self->blocksPlayed) * blockSize;
	ASAP_PutLittleEndian(buffer, 8, 1163280727);
	ASAP_PutLittleEndians(buffer, 12, 544501094, 16);
	buffer[20] = 1;
	buffer[21] = 0;
	buffer[22] = (uint8_t) ASAPInfo_GetChannels(&self->moduleInfo);
	buffer[23] = 0;
	ASAP_PutLittleEndians(buffer, 24, 44100, bytesPerSecond);
	buffer[32] = (uint8_t) blockSize;
	buffer[33] = 0;
	buffer[34] = (uint8_t) (8 << use16bit);
	buffer[35] = 0;
	int i = 36;
	if (metadata) {
		int year = ASAPInfo_GetYear(&self->moduleInfo);
		if (ASAPInfo_GetTitle(&self->moduleInfo)[0] != '\0' || ASAPInfo_GetAuthor(&self->moduleInfo)[0] != '\0' || year > 0) {
			ASAP_PutLittleEndian(buffer, 44, 1330007625);
			i = ASAP_PutWavMetadata(buffer, 48, 1296125513, ASAPInfo_GetTitle(&self->moduleInfo));
			i = ASAP_PutWavMetadata(buffer, i, 1414676809, ASAPInfo_GetAuthor(&self->moduleInfo));
			if (year > 0) {
				ASAP_PutLittleEndians(buffer, i, 1146241865, 6);
				for (int j = 3; j >= 0; j--) {
					buffer[i + 8 + j] = (uint8_t) (48 + year % 10);
					year /= 10;
				}
				buffer[i + 12] = 0;
				buffer[i + 13] = 0;
				i += 14;
			}
			ASAP_PutLittleEndians(buffer, 36, 1414744396, i - 44);
		}
	}
	ASAP_PutLittleEndians(buffer, 0, 1179011410, i + nBytes);
	ASAP_PutLittleEndians(buffer, i, 1635017060, nBytes);
	return i + 8;
}

static int ASAP_GenerateAt(ASAP *self, uint8_t *buffer, int bufferOffset, int bufferLen, ASAPSampleFormat format)
{
	if (self->silenceCycles > 0 && self->silenceCyclesCounter <= 0)
		return 0;
	int blockShift = ASAPInfo_GetChannels(&self->moduleInfo) - 1 + (format != ASAPSampleFormat_U8 ? 1 : 0);
	int bufferBlocks = bufferLen >> blockShift;
	if (self->currentDuration > 0) {
		int totalBlocks = ASAP_MillisecondsToBlocks(self->currentDuration);
		if (bufferBlocks > totalBlocks - self->blocksPlayed)
			bufferBlocks = totalBlocks - self->blocksPlayed;
	}
	int block = 0;
	for (;;) {
		int blocks = PokeyPair_Generate(&self->pokeys, buffer, bufferOffset + (block << blockShift), bufferBlocks - block, format);
		self->blocksPlayed += blocks;
		block += blocks;
		if (block >= bufferBlocks)
			break;
		int cycles = ASAP_DoFrame(self);
		if (self->silenceCycles > 0) {
			if (PokeyPair_IsSilent(&self->pokeys) && !self->gtiaOrCovoxPlayedThisFrame) {
				self->silenceCyclesCounter -= cycles;
				if (self->silenceCyclesCounter <= 0)
					break;
			}
			else
				self->silenceCyclesCounter = self->silenceCycles;
		}
	}
	return block << blockShift;
}

int ASAP_Generate(ASAP *self, uint8_t *buffer, int bufferLen, ASAPSampleFormat format)
{
	return ASAP_GenerateAt(self, buffer, 0, bufferLen, format);
}

int ASAP_GetPokeyChannelVolume(const ASAP *self, int channel)
{
	const Pokey *pokey = (channel & 4) == 0 ? &self->pokeys.basePokey : &self->pokeys.extraPokey;
	return pokey->channels[channel & 3].audc & 15;
}

static uint8_t const *ASAP6502_GetPlayerRoutine(const ASAPInfo *info)
{
	switch (info->type) {
	case ASAPModuleType_CMC:
		return CiResource_cmc_obx;
	case ASAPModuleType_CM3:
		return CiResource_cm3_obx;
	case ASAPModuleType_CMR:
		return CiResource_cmr_obx;
	case ASAPModuleType_CMS:
		return CiResource_cms_obx;
	case ASAPModuleType_DLT:
		return CiResource_dlt_obx;
	case ASAPModuleType_MPT:
		return CiResource_mpt_obx;
	case ASAPModuleType_RMT:
		return ASAPInfo_GetChannels(info) == 1 ? CiResource_rmt4_obx : CiResource_rmt8_obx;
	case ASAPModuleType_TMC:
		return CiResource_tmc_obx;
	case ASAPModuleType_TM2:
		return CiResource_tm2_obx;
	case ASAPModuleType_FC:
		return CiResource_fc_obx;
	default:
		return NULL;
	}
}

static int DurationParser_ParseDigit(DurationParser *self, int max)
{
	if (self->position >= self->length)
		return -1;
	int digit = self->source[self->position++] - 48;
	if (digit < 0 || digit > max)
		return -1;
	return digit;
}

static int DurationParser_Parse(DurationParser *self, const char *s)
{
	self->source = s;
	self->position = 0;
	self->length = (int) strlen(s);
	int result = DurationParser_ParseDigit(self, 9);
	if (result == -1)
		return -1;
	int digit;
	if (self->position < self->length) {
		digit = s[self->position] - 48;
		if (digit >= 0 && digit <= 9) {
			self->position++;
			result = result * 10 + digit;
		}
		if (self->position < self->length && s[self->position] == ':') {
			self->position++;
			if ((digit = DurationParser_ParseDigit(self, 5)) == -1)
				return -1;
			result = result * 60 + digit * 10;
			if ((digit = DurationParser_ParseDigit(self, 9)) == -1)
				return -1;
			result += digit;
		}
	}
	result *= 1000;
	if (self->position >= self->length)
		return result;
	if (s[self->position++] != '.')
		return -1;
	if ((digit = DurationParser_ParseDigit(self, 9)) == -1)
		return -1;
	result += digit * 100;
	if (self->position >= self->length)
		return result;
	if ((digit = DurationParser_ParseDigit(self, 9)) == -1)
		return -1;
	result += digit * 10;
	if (self->position >= self->length)
		return result;
	if ((digit = DurationParser_ParseDigit(self, 9)) == -1)
		return -1;
	result += digit;
	return result;
}

static void ASAPInfo_Construct(ASAPInfo *self)
{
	self->filename = NULL;
	self->author = NULL;
	self->title = NULL;
	self->date = NULL;
}

static void ASAPInfo_Destruct(ASAPInfo *self)
{
	free(self->date);
	free(self->title);
	free(self->author);
	free(self->filename);
}

ASAPInfo *ASAPInfo_New(void)
{
	ASAPInfo *self = (ASAPInfo *) malloc(sizeof(ASAPInfo));
	if (self != NULL)
		ASAPInfo_Construct(self);
	return self;
}

void ASAPInfo_Delete(ASAPInfo *self)
{
	if (self == NULL)
		return;
	ASAPInfo_Destruct(self);
	free(self);
}

static bool ASAPInfo_IsValidChar(int c)
{
	return c >= 32 && c <= 124 && c != 96 && c != 123;
}

static bool ASAPInfo_CheckValidChar(int c)
{
	if (!ASAPInfo_IsValidChar(c))
		return false;
	return true;
}

static bool ASAPInfo_CheckValidText(const char *s)
{
	int n = (int) strlen(s);
	if (n > 127)
		return false;
	for (int i = 0; i < n; i++) {
		if (!ASAPInfo_CheckValidChar(s[i]))
			return false;
	}
	return true;
}

const char *ASAPInfo_GetAuthor(const ASAPInfo *self)
{
	return self->author;
}

bool ASAPInfo_SetAuthor(ASAPInfo *self, const char *value)
{
	if (!ASAPInfo_CheckValidText(value))
		return false;
	CiString_Assign(&self->author, strdup(value));
	return true;
}

const char *ASAPInfo_GetTitle(const ASAPInfo *self)
{
	return self->title;
}

bool ASAPInfo_SetTitle(ASAPInfo *self, const char *value)
{
	if (!ASAPInfo_CheckValidText(value))
		return false;
	CiString_Assign(&self->title, strdup(value));
	return true;
}

const char *ASAPInfo_GetTitleOrFilename(const ASAPInfo *self)
{
	return self->title[0] != '\0' ? self->title : self->filename;
}

const char *ASAPInfo_GetDate(const ASAPInfo *self)
{
	return self->date;
}

bool ASAPInfo_SetDate(ASAPInfo *self, const char *value)
{
	if (!ASAPInfo_CheckValidText(value))
		return false;
	CiString_Assign(&self->date, strdup(value));
	return true;
}

static bool ASAPInfo_CheckTwoDateDigits(const ASAPInfo *self, int i)
{
	int d1 = self->date[i];
	int d2 = self->date[i + 1];
	return d1 >= 48 && d1 <= 57 && d2 >= 48 && d2 <= 57;
}

static int ASAPInfo_CheckDate(const ASAPInfo *self)
{
	int n = (int) strlen(self->date);
	switch (n) {
	case 10:
		if (!ASAPInfo_CheckTwoDateDigits(self, 0) || self->date[2] != '/')
			return -1;
	case 7:
		if (!ASAPInfo_CheckTwoDateDigits(self, n - 7) || self->date[n - 5] != '/')
			return -1;
	case 4:
		if (!ASAPInfo_CheckTwoDateDigits(self, n - 4) || !ASAPInfo_CheckTwoDateDigits(self, n - 2))
			return -1;
		return n;
	default:
		return -1;
	}
}

static int ASAPInfo_GetTwoDateDigits(const ASAPInfo *self, int i)
{
	return (self->date[i] - 48) * 10 + self->date[i + 1] - 48;
}

int ASAPInfo_GetYear(const ASAPInfo *self)
{
	int n = ASAPInfo_CheckDate(self);
	if (n < 0)
		return -1;
	return ASAPInfo_GetTwoDateDigits(self, n - 4) * 100 + ASAPInfo_GetTwoDateDigits(self, n - 2);
}

int ASAPInfo_GetMonth(const ASAPInfo *self)
{
	int n = ASAPInfo_CheckDate(self);
	if (n < 7)
		return -1;
	return ASAPInfo_GetTwoDateDigits(self, n - 7);
}

int ASAPInfo_GetDayOfMonth(const ASAPInfo *self)
{
	int n = ASAPInfo_CheckDate(self);
	if (n != 10)
		return -1;
	return ASAPInfo_GetTwoDateDigits(self, 0);
}

int ASAPInfo_GetChannels(const ASAPInfo *self)
{
	return self->channels;
}

int ASAPInfo_GetSongs(const ASAPInfo *self)
{
	return self->songs;
}

int ASAPInfo_GetDefaultSong(const ASAPInfo *self)
{
	return self->defaultSong;
}

bool ASAPInfo_SetDefaultSong(ASAPInfo *self, int song)
{
	if (song < 0 || song >= self->songs)
		return false;
	self->defaultSong = song;
	return true;
}

int ASAPInfo_GetDuration(const ASAPInfo *self, int song)
{
	return self->durations[song];
}

bool ASAPInfo_SetDuration(ASAPInfo *self, int song, int duration)
{
	if (song < 0 || song >= self->songs)
		return false;
	self->durations[song] = duration;
	return true;
}

bool ASAPInfo_GetLoop(const ASAPInfo *self, int song)
{
	return self->loops[song];
}

bool ASAPInfo_SetLoop(ASAPInfo *self, int song, bool loop)
{
	if (song < 0 || song >= self->songs)
		return false;
	self->loops[song] = loop;
	return true;
}

bool ASAPInfo_IsNtsc(const ASAPInfo *self)
{
	return self->ntsc;
}

int ASAPInfo_GetTypeLetter(const ASAPInfo *self)
{
	switch (self->type) {
	case ASAPModuleType_SAP_B:
		return 66;
	case ASAPModuleType_SAP_C:
		return 67;
	case ASAPModuleType_SAP_D:
		return 68;
	case ASAPModuleType_SAP_S:
		return 83;
	default:
		return 0;
	}
}

int ASAPInfo_GetPlayerRateScanlines(const ASAPInfo *self)
{
	return self->fastplay;
}

int ASAPInfo_GetPlayerRateHz(const ASAPInfo *self)
{
	int scanlineClock = self->ntsc ? 15699 : 15556;
	return (scanlineClock + (self->fastplay >> 1)) / self->fastplay;
}

int ASAPInfo_GetMusicAddress(const ASAPInfo *self)
{
	return self->music;
}

bool ASAPInfo_SetMusicAddress(ASAPInfo *self, int address)
{
	if (address < 0 || address >= 65535)
		return false;
	self->music = address;
	return true;
}

int ASAPInfo_GetInitAddress(const ASAPInfo *self)
{
	return self->init;
}

int ASAPInfo_GetPlayerAddress(const ASAPInfo *self)
{
	return self->player;
}

int ASAPInfo_GetCovoxAddress(const ASAPInfo *self)
{
	return self->covoxAddr;
}

int ASAPInfo_GetSapHeaderLength(const ASAPInfo *self)
{
	return self->headerLen;
}

int ASAPInfo_GetInstrumentNamesOffset(const ASAPInfo *self, uint8_t const *module, int moduleLen)
{
	if (self->type != ASAPModuleType_RMT)
		return -1;
	for (int offset = ASAPInfo_GetWord(module, 4) - ASAPInfo_GetWord(module, 2) + 12; offset < moduleLen; offset++) {
		if (module[offset - 1] == 0)
			return offset;
	}
	return -1;
}

static int ASAPInfo_GetWord(uint8_t const *array, int i)
{
	return array[i] + (array[i + 1] << 8);
}

static bool ASAPInfo_ParseModule(ASAPInfo *self, uint8_t const *module, int moduleLen)
{
	if ((module[0] != 255 || module[1] != 255) && (module[0] != 0 || module[1] != 0))
		return false;
	self->music = ASAPInfo_GetWord(module, 2);
	int musicLastByte = ASAPInfo_GetWord(module, 4);
	if (self->music <= 55295 && musicLastByte >= 53248)
		return false;
	int blockLen = musicLastByte + 1 - self->music;
	if (6 + blockLen != moduleLen) {
		if (self->type != ASAPModuleType_RMT || 11 + blockLen > moduleLen)
			return false;
		int infoAddr = ASAPInfo_GetWord(module, 6 + blockLen);
		if (infoAddr != self->music + blockLen)
			return false;
		int infoLen = ASAPInfo_GetWord(module, 8 + blockLen) + 1 - infoAddr;
		if (10 + blockLen + infoLen != moduleLen)
			return false;
	}
	return true;
}

static void ASAPInfo_AddSong(ASAPInfo *self, int playerCalls)
{
	int64_t scanlines = playerCalls * self->fastplay;
	self->durations[self->songs++] = (int) (scanlines * 38000 / 591149);
}

static void ASAPInfo_ParseCmcSong(ASAPInfo *self, uint8_t const *module, int pos)
{
	int tempo = module[25];
	int playerCalls = 0;
	int repStartPos = 0;
	int repEndPos = 0;
	int repTimes = 0;
	uint8_t seen[85] = { 0 };
	while (pos >= 0 && pos < 85) {
		if (pos == repEndPos && repTimes > 0) {
			for (int i = 0; i < 85; i++)
				if (seen[i] == 1 || seen[i] == 3)
					seen[i] = 0;
			repTimes--;
			pos = repStartPos;
		}
		if (seen[pos] != 0) {
			if (seen[pos] != 1)
				self->loops[self->songs] = true;
			break;
		}
		seen[pos] = 1;
		int p1 = module[518 + pos];
		int p2 = module[603 + pos];
		int p3 = module[688 + pos];
		if (p1 == 254 || p2 == 254 || p3 == 254) {
			pos++;
			continue;
		}
		p1 |= self->type == ASAPModuleType_CMS ? 7 : 15;
		switch (p1) {
		case 135:
		case 167:
			pos++;
			break;
		case 143:
			pos = -1;
			break;
		case 151:
			if (p2 < 128) {
				playerCalls += p2;
				if (p3 < 128)
					playerCalls += p3 * 50;
			}
			pos++;
			break;
		case 159:
			pos = p2;
			break;
		case 175:
			pos -= p2;
			break;
		case 191:
			pos += p2;
			break;
		case 207:
			if (p2 < 128) {
				tempo = p2;
				pos++;
			}
			else
				pos = -1;
			break;
		case 223:
			pos++;
			repStartPos = pos;
			repEndPos = pos + p2;
			repTimes = p3 - 1;
			break;
		case 239:
			self->loops[self->songs] = true;
			pos = -1;
			break;
		default:
			p2 = repTimes > 0 ? 3 : 2;
			for (p1 = 0; p1 < 85; p1++)
				if (seen[p1] == 1)
					seen[p1] = (uint8_t) p2;
			playerCalls += tempo * (self->type == ASAPModuleType_CM3 ? 48 : 64);
			pos++;
			break;
		}
	}
	ASAPInfo_AddSong(self, playerCalls);
}

static bool ASAPInfo_ParseCmc(ASAPInfo *self, uint8_t const *module, int moduleLen, ASAPModuleType type)
{
	if (moduleLen < 774)
		return false;
	self->type = type;
	if (!ASAPInfo_ParseModule(self, module, moduleLen))
		return false;
	int lastPos = 84;
	while (--lastPos >= 0) {
		if (module[518 + lastPos] < 176 || module[603 + lastPos] < 64 || module[688 + lastPos] < 64)
			break;
		if (self->channels == 2) {
			if (module[774 + lastPos] < 176 || module[859 + lastPos] < 64 || module[944 + lastPos] < 64)
				break;
		}
	}
	self->songs = 0;
	ASAPInfo_ParseCmcSong(self, module, 0);
	for (int pos = 0; pos < lastPos && self->songs < 32; pos++)
		if (module[518 + pos] == 143 || module[518 + pos] == 239)
			ASAPInfo_ParseCmcSong(self, module, pos + 1);
	return true;
}

static bool ASAPInfo_IsDltTrackEmpty(uint8_t const *module, int pos)
{
	return module[8198 + pos] >= 67 && module[8454 + pos] >= 64 && module[8710 + pos] >= 64 && module[8966 + pos] >= 64;
}

static bool ASAPInfo_IsDltPatternEnd(uint8_t const *module, int pos, int i)
{
	for (int ch = 0; ch < 4; ch++) {
		int pattern = module[8198 + (ch << 8) + pos];
		if (pattern < 64) {
			int offset = 6 + (pattern << 7) + (i << 1);
			if ((module[offset] & 128) == 0 && (module[offset + 1] & 128) != 0)
				return true;
		}
	}
	return false;
}

static void ASAPInfo_ParseDltSong(ASAPInfo *self, uint8_t const *module, bool *seen, int pos)
{
	while (pos < 128 && !seen[pos] && ASAPInfo_IsDltTrackEmpty(module, pos))
		seen[pos++] = true;
	self->songPos[self->songs] = (uint8_t) pos;
	int playerCalls = 0;
	bool loop = false;
	int tempo = 6;
	while (pos < 128) {
		if (seen[pos]) {
			loop = true;
			break;
		}
		seen[pos] = true;
		int p1 = module[8198 + pos];
		if (p1 == 64 || ASAPInfo_IsDltTrackEmpty(module, pos))
			break;
		if (p1 == 65)
			pos = module[8326 + pos];
		else if (p1 == 66)
			tempo = module[8326 + pos++];
		else {
			for (int i = 0; i < 64 && !ASAPInfo_IsDltPatternEnd(module, pos, i); i++)
				playerCalls += tempo;
			pos++;
		}
	}
	if (playerCalls > 0) {
		self->loops[self->songs] = loop;
		ASAPInfo_AddSong(self, playerCalls);
	}
}

static bool ASAPInfo_ParseDlt(ASAPInfo *self, uint8_t const *module, int moduleLen)
{
	if (moduleLen != 11270 && moduleLen != 11271)
		return false;
	self->type = ASAPModuleType_DLT;
	if (!ASAPInfo_ParseModule(self, module, moduleLen))
		return false;
	if (self->music != 8192)
		return false;
	bool seen[128] = { false };
	self->songs = 0;
	for (int pos = 0; pos < 128 && self->songs < 32; pos++) {
		if (!seen[pos])
			ASAPInfo_ParseDltSong(self, module, seen, pos);
	}
	if (self->songs == 0)
		return false;
	return true;
}

static void ASAPInfo_ParseMptSong(ASAPInfo *self, uint8_t const *module, bool *globalSeen, int songLen, int pos)
{
	int addrToOffset = ASAPInfo_GetWord(module, 2) - 6;
	int tempo = module[463];
	int playerCalls = 0;
	uint8_t seen[256] = { 0 };
	int patternOffset[4];
	int blankRows[4] = { 0 };
	int blankRowsCounter[4];
	while (pos < songLen) {
		if (seen[pos] != 0) {
			if (seen[pos] != 1)
				self->loops[self->songs] = true;
			break;
		}
		seen[pos] = 1;
		globalSeen[pos] = true;
		int i = module[464 + pos * 2];
		if (i == 255) {
			pos = module[465 + pos * 2];
			continue;
		}
		int ch;
		for (ch = 3; ch >= 0; ch--) {
			i = module[454 + ch] + (module[458 + ch] << 8) - addrToOffset;
			i = module[i + pos * 2];
			if (i >= 64)
				break;
			i <<= 1;
			i = ASAPInfo_GetWord(module, 70 + i);
			patternOffset[ch] = i == 0 ? 0 : i - addrToOffset;
			blankRowsCounter[ch] = 0;
		}
		if (ch >= 0)
			break;
		for (i = 0; i < songLen; i++)
			if (seen[i] == 1)
				seen[i] = 2;
		for (int patternRows = module[462]; --patternRows >= 0;) {
			for (ch = 3; ch >= 0; ch--) {
				if (patternOffset[ch] == 0 || --blankRowsCounter[ch] >= 0)
					continue;
				for (;;) {
					i = module[patternOffset[ch]++];
					if (i < 64 || i == 254)
						break;
					if (i < 128)
						continue;
					if (i < 192) {
						blankRows[ch] = i - 128;
						continue;
					}
					if (i < 208)
						continue;
					if (i < 224) {
						tempo = i - 207;
						continue;
					}
					patternRows = 0;
				}
				blankRowsCounter[ch] = blankRows[ch];
			}
			playerCalls += tempo;
		}
		pos++;
	}
	if (playerCalls > 0)
		ASAPInfo_AddSong(self, playerCalls);
}

static bool ASAPInfo_ParseMpt(ASAPInfo *self, uint8_t const *module, int moduleLen)
{
	if (moduleLen < 464)
		return false;
	self->type = ASAPModuleType_MPT;
	if (!ASAPInfo_ParseModule(self, module, moduleLen))
		return false;
	int track0Addr = ASAPInfo_GetWord(module, 2) + 458;
	if (module[454] + (module[458] << 8) != track0Addr)
		return false;
	int songLen = (module[455] + (module[459] << 8) - track0Addr) >> 1;
	if (songLen > 254)
		return false;
	bool globalSeen[256] = { false };
	self->songs = 0;
	for (int pos = 0; pos < songLen && self->songs < 32; pos++) {
		if (!globalSeen[pos]) {
			self->songPos[self->songs] = (uint8_t) pos;
			ASAPInfo_ParseMptSong(self, module, globalSeen, songLen, pos);
		}
	}
	if (self->songs == 0)
		return false;
	return true;
}

static int ASAPInfo_GetRmtInstrumentFrames(uint8_t const *module, int instrument, int volume, int volumeFrame, bool onExtraPokey)
{
	int addrToOffset = ASAPInfo_GetWord(module, 2) - 6;
	instrument = ASAPInfo_GetWord(module, 14) - addrToOffset + (instrument << 1);
	if (module[instrument + 1] == 0)
		return 0;
	instrument = ASAPInfo_GetWord(module, instrument) - addrToOffset;
	int perFrame = module[12];
	int playerCall = volumeFrame * perFrame;
	int playerCalls = playerCall;
	int index = module[instrument] + 1 + playerCall * 3;
	int indexEnd = module[instrument + 2] + 3;
	int indexLoop = module[instrument + 3];
	if (indexLoop >= indexEnd)
		return 0;
	int volumeSlideDepth = module[instrument + 6];
	int volumeMin = module[instrument + 7];
	static const uint8_t RMT_VOLUME_SILENT[16] = { 16, 8, 4, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1 };
	if (index >= indexEnd)
		index = (index - indexEnd) % (indexEnd - indexLoop) + indexLoop;
	else {
		do {
			int vol = module[instrument + index];
			if (onExtraPokey)
				vol >>= 4;
			if ((vol & 15) >= RMT_VOLUME_SILENT[volume])
				playerCalls = playerCall + 1;
			playerCall++;
			index += 3;
		}
		while (index < indexEnd);
	}
	if (volumeSlideDepth == 0)
		return playerCalls / perFrame;
	int volumeSlide = 128;
	bool silentLoop = false;
	for (;;) {
		if (index >= indexEnd) {
			if (silentLoop)
				break;
			silentLoop = true;
			index = indexLoop;
		}
		int vol = module[instrument + index];
		if (onExtraPokey)
			vol >>= 4;
		if ((vol & 15) >= RMT_VOLUME_SILENT[volume]) {
			playerCalls = playerCall + 1;
			silentLoop = false;
		}
		playerCall++;
		index += 3;
		volumeSlide -= volumeSlideDepth;
		if (volumeSlide < 0) {
			volumeSlide += 256;
			if (--volume <= volumeMin)
				break;
		}
	}
	return playerCalls / perFrame;
}

static void ASAPInfo_ParseRmtSong(ASAPInfo *self, uint8_t const *module, bool *globalSeen, int songLen, int posShift, int pos)
{
	int addrToOffset = ASAPInfo_GetWord(module, 2) - 6;
	int tempo = module[11];
	int frames = 0;
	int songOffset = ASAPInfo_GetWord(module, 20) - addrToOffset;
	int patternLoOffset = ASAPInfo_GetWord(module, 16) - addrToOffset;
	int patternHiOffset = ASAPInfo_GetWord(module, 18) - addrToOffset;
	uint8_t seen[256] = { 0 };
	int patternBegin[8];
	int patternOffset[8];
	int blankRows[8];
	int instrumentNo[8] = { 0 };
	int instrumentFrame[8] = { 0 };
	int volumeValue[8] = { 0 };
	int volumeFrame[8] = { 0 };
	while (pos < songLen) {
		if (seen[pos] != 0) {
			if (seen[pos] != 1)
				self->loops[self->songs] = true;
			break;
		}
		seen[pos] = 1;
		globalSeen[pos] = true;
		if (module[songOffset + (pos << posShift)] == 254) {
			pos = module[songOffset + (pos << posShift) + 1];
			continue;
		}
		for (int ch = 0; ch < 1 << posShift; ch++) {
			int p = module[songOffset + (pos << posShift) + ch];
			if (p == 255)
				blankRows[ch] = 256;
			else {
				patternOffset[ch] = patternBegin[ch] = module[patternLoOffset + p] + (module[patternHiOffset + p] << 8) - addrToOffset;
				if (patternOffset[ch] < 0)
					return;
				blankRows[ch] = 0;
			}
		}
		for (int i = 0; i < songLen; i++)
			if (seen[i] == 1)
				seen[i] = 2;
		for (int patternRows = module[10]; --patternRows >= 0;) {
			for (int ch = 0; ch < 1 << posShift; ch++) {
				if (--blankRows[ch] > 0)
					continue;
				for (;;) {
					int i = module[patternOffset[ch]++];
					if ((i & 63) < 62) {
						i += module[patternOffset[ch]++] << 8;
						if ((i & 63) != 61) {
							instrumentNo[ch] = i >> 10;
							instrumentFrame[ch] = frames;
						}
						volumeValue[ch] = i >> 6 & 15;
						volumeFrame[ch] = frames;
						break;
					}
					if (i == 62) {
						blankRows[ch] = module[patternOffset[ch]++];
						break;
					}
					if ((i & 63) == 62) {
						blankRows[ch] = i >> 6;
						break;
					}
					if ((i & 191) == 63) {
						tempo = module[patternOffset[ch]++];
						continue;
					}
					if (i == 191) {
						patternOffset[ch] = patternBegin[ch] + module[patternOffset[ch]];
						continue;
					}
					patternRows = -1;
					break;
				}
				if (patternRows < 0)
					break;
			}
			if (patternRows >= 0)
				frames += tempo;
		}
		pos++;
	}
	int instrumentFrames = 0;
	for (int ch = 0; ch < 1 << posShift; ch++) {
		int frame = instrumentFrame[ch];
		frame += ASAPInfo_GetRmtInstrumentFrames(module, instrumentNo[ch], volumeValue[ch], volumeFrame[ch] - frame, ch >= 4);
		if (instrumentFrames < frame)
			instrumentFrames = frame;
	}
	if (frames > instrumentFrames) {
		if (frames - instrumentFrames > 100)
			self->loops[self->songs] = false;
		frames = instrumentFrames;
	}
	if (frames > 0)
		ASAPInfo_AddSong(self, frames);
}

static bool ASAPInfo_ValidateRmt(uint8_t const *module, int moduleLen)
{
	if (moduleLen < 48)
		return false;
	if (module[6] != 82 || module[7] != 77 || module[8] != 84 || module[13] != 1)
		return false;
	return true;
}

static bool ASAPInfo_ParseRmt(ASAPInfo *self, uint8_t const *module, int moduleLen)
{
	if (!ASAPInfo_ValidateRmt(module, moduleLen))
		return false;
	int posShift;
	switch (module[9]) {
	case 52:
		posShift = 2;
		break;
	case 56:
		self->channels = 2;
		posShift = 3;
		break;
	default:
		return false;
	}
	int perFrame = module[12];
	if (perFrame < 1 || perFrame > 4)
		return false;
	self->type = ASAPModuleType_RMT;
	if (!ASAPInfo_ParseModule(self, module, moduleLen))
		return false;
	int blockLen = ASAPInfo_GetWord(module, 4) + 1 - self->music;
	int songLen = ASAPInfo_GetWord(module, 4) + 1 - ASAPInfo_GetWord(module, 20);
	if (posShift == 3 && (songLen & 4) != 0 && module[6 + blockLen - 4] == 254)
		songLen += 4;
	songLen >>= posShift;
	if (songLen >= 256)
		return false;
	bool globalSeen[256] = { false };
	self->songs = 0;
	for (int pos = 0; pos < songLen && self->songs < 32; pos++) {
		if (!globalSeen[pos]) {
			self->songPos[self->songs] = (uint8_t) pos;
			ASAPInfo_ParseRmtSong(self, module, globalSeen, songLen, posShift, pos);
		}
	}
	self->fastplay = 312 / perFrame;
	self->player = 1536;
	if (self->songs == 0)
		return false;
	uint8_t title[127];
	int titleLen;
	for (titleLen = 0; titleLen < 127 && 10 + blockLen + titleLen < moduleLen; titleLen++) {
		int c = module[10 + blockLen + titleLen];
		if (c == 0)
			break;
		title[titleLen] = (uint8_t) (ASAPInfo_IsValidChar(c) ? c : 32);
	}
	CiString_Assign(&self->title, CiString_Substring((const char *) title, titleLen));
	return true;
}

static void ASAPInfo_ParseTmcSong(ASAPInfo *self, uint8_t const *module, int pos)
{
	int addrToOffset = ASAPInfo_GetWord(module, 2) - 6;
	int tempo = module[36] + 1;
	int frames = 0;
	int patternOffset[8];
	int blankRows[8];
	while (module[437 + pos] < 128) {
		for (int ch = 7; ch >= 0; ch--) {
			int pat = module[437 + pos - 2 * ch];
			patternOffset[ch] = module[166 + pat] + (module[294 + pat] << 8) - addrToOffset;
			blankRows[ch] = 0;
		}
		for (int patternRows = 64; --patternRows >= 0;) {
			for (int ch = 7; ch >= 0; ch--) {
				if (--blankRows[ch] >= 0)
					continue;
				for (;;) {
					int i = module[patternOffset[ch]++];
					if (i < 64) {
						patternOffset[ch]++;
						break;
					}
					if (i == 64) {
						i = module[patternOffset[ch]++];
						if ((i & 127) == 0)
							patternRows = 0;
						else
							tempo = (i & 127) + 1;
						if (i >= 128)
							patternOffset[ch]++;
						break;
					}
					if (i < 128) {
						i = module[patternOffset[ch]++] & 127;
						if (i == 0)
							patternRows = 0;
						else
							tempo = i + 1;
						patternOffset[ch]++;
						break;
					}
					if (i < 192)
						continue;
					blankRows[ch] = i - 191;
					break;
				}
			}
			frames += tempo;
		}
		pos += 16;
	}
	if (module[436 + pos] < 128)
		self->loops[self->songs] = true;
	ASAPInfo_AddSong(self, frames);
}

static int ASAPInfo_ParseTmcTitle(uint8_t *title, int titleLen, uint8_t const *module, int moduleOffset)
{
	int lastOffset = moduleOffset + 29;
	while (module[lastOffset] == 32) {
		if (--lastOffset < moduleOffset)
			return titleLen;
	}
	if (titleLen > 0) {
		title[titleLen++] = 32;
		title[titleLen++] = 124;
		title[titleLen++] = 32;
	}
	while (moduleOffset <= lastOffset) {
		int c = module[moduleOffset++] & 127;
		switch (c) {
		case 20:
			c = 42;
			break;
		case 1:
		case 3:
		case 5:
		case 12:
		case 14:
		case 15:
		case 19:
			c += 96;
			break;
		case 24:
		case 26:
			c = 122;
			break;
		default:
			if (!ASAPInfo_IsValidChar(c))
				c = 32;
			break;
		}
		title[titleLen++] = (uint8_t) c;
	}
	return titleLen;
}

static bool ASAPInfo_ParseTmc(ASAPInfo *self, uint8_t const *module, int moduleLen)
{
	if (moduleLen < 464)
		return false;
	self->type = ASAPModuleType_TMC;
	if (!ASAPInfo_ParseModule(self, module, moduleLen))
		return false;
	self->channels = 2;
	int i = 0;
	while (module[102 + i] == 0) {
		if (++i >= 64)
			return false;
	}
	int lastPos = (module[102 + i] << 8) + module[38 + i] - ASAPInfo_GetWord(module, 2) - 432;
	if (437 + lastPos >= moduleLen)
		return false;
	do {
		if (lastPos <= 0)
			return false;
		lastPos -= 16;
	}
	while (module[437 + lastPos] >= 128);
	self->songs = 0;
	ASAPInfo_ParseTmcSong(self, module, 0);
	for (i = 0; i < lastPos && self->songs < 32; i += 16)
		if (module[437 + i] >= 128)
			ASAPInfo_ParseTmcSong(self, module, i + 16);
	i = module[37];
	if (i < 1 || i > 4)
		return false;
	self->fastplay = 312 / i;
	uint8_t title[127];
	int titleLen = ASAPInfo_ParseTmcTitle(title, 0, module, 6);
	CiString_Assign(&self->title, CiString_Substring((const char *) title, titleLen));
	return true;
}

static void ASAPInfo_ParseTm2Song(ASAPInfo *self, uint8_t const *module, int pos)
{
	int addrToOffset = ASAPInfo_GetWord(module, 2) - 6;
	int tempo = module[36] + 1;
	int playerCalls = 0;
	int patternOffset[8];
	int blankRows[8];
	for (;;) {
		int patternRows = module[918 + pos];
		if (patternRows == 0)
			break;
		if (patternRows >= 128) {
			self->loops[self->songs] = true;
			break;
		}
		for (int ch = 7; ch >= 0; ch--) {
			int pat = module[917 + pos - 2 * ch];
			patternOffset[ch] = module[262 + pat] + (module[518 + pat] << 8) - addrToOffset;
			blankRows[ch] = 0;
		}
		while (--patternRows >= 0) {
			for (int ch = 7; ch >= 0; ch--) {
				if (--blankRows[ch] >= 0)
					continue;
				for (;;) {
					int i = module[patternOffset[ch]++];
					if (i == 0) {
						patternOffset[ch]++;
						break;
					}
					if (i < 64) {
						if (module[patternOffset[ch]++] >= 128)
							patternOffset[ch]++;
						break;
					}
					if (i < 128) {
						patternOffset[ch]++;
						break;
					}
					if (i == 128) {
						blankRows[ch] = module[patternOffset[ch]++];
						break;
					}
					if (i < 192)
						break;
					if (i < 208) {
						tempo = i - 191;
						continue;
					}
					if (i < 224) {
						patternOffset[ch]++;
						break;
					}
					if (i < 240) {
						patternOffset[ch] += 2;
						break;
					}
					if (i < 255) {
						blankRows[ch] = i - 240;
						break;
					}
					blankRows[ch] = 64;
					break;
				}
			}
			playerCalls += tempo;
		}
		pos += 17;
	}
	ASAPInfo_AddSong(self, playerCalls);
}

static bool ASAPInfo_ParseTm2(ASAPInfo *self, uint8_t const *module, int moduleLen)
{
	if (moduleLen < 932)
		return false;
	self->type = ASAPModuleType_TM2;
	if (!ASAPInfo_ParseModule(self, module, moduleLen))
		return false;
	int i = module[37];
	if (i < 1 || i > 4)
		return false;
	self->fastplay = 312 / i;
	self->player = 2048;
	if (module[31] != 0)
		self->channels = 2;
	int lastPos = 65535;
	for (i = 0; i < 128; i++) {
		int instrAddr = module[134 + i] + (module[774 + i] << 8);
		if (instrAddr != 0 && instrAddr < lastPos)
			lastPos = instrAddr;
	}
	for (i = 0; i < 256; i++) {
		int patternAddr = module[262 + i] + (module[518 + i] << 8);
		if (patternAddr != 0 && patternAddr < lastPos)
			lastPos = patternAddr;
	}
	lastPos -= ASAPInfo_GetWord(module, 2) + 896;
	if (902 + lastPos >= moduleLen)
		return false;
	int c;
	do {
		if (lastPos <= 0)
			return false;
		lastPos -= 17;
		c = module[918 + lastPos];
	}
	while (c == 0 || c >= 128);
	self->songs = 0;
	ASAPInfo_ParseTm2Song(self, module, 0);
	for (i = 0; i < lastPos && self->songs < 32; i += 17) {
		c = module[918 + i];
		if (c == 0 || c >= 128)
			ASAPInfo_ParseTm2Song(self, module, i + 17);
	}
	uint8_t title[127];
	int titleLen = ASAPInfo_ParseTmcTitle(title, 0, module, 39);
	titleLen = ASAPInfo_ParseTmcTitle(title, titleLen, module, 71);
	titleLen = ASAPInfo_ParseTmcTitle(title, titleLen, module, 103);
	CiString_Assign(&self->title, CiString_Substring((const char *) title, titleLen));
	return true;
}

static int ASAPInfo_AfterFF(uint8_t const *module, int moduleLen, int currentOffset)
{
	while (currentOffset < moduleLen) {
		if (module[currentOffset++] == 255)
			return currentOffset;
	}
	return -1;
}

static int ASAPInfo_GetFcTrackCommand(uint8_t const *module, int const *trackPos, int n)
{
	return module[3 + (n << 8) + trackPos[n]];
}

static bool ASAPInfo_IsFcSongEnd(uint8_t const *module, int const *trackPos)
{
	bool allLoop = true;
	for (int n = 0; n < 3; n++) {
		if (trackPos[n] >= 256)
			return true;
		switch (ASAPInfo_GetFcTrackCommand(module, trackPos, n)) {
		case 254:
			return true;
		case 255:
			break;
		default:
			allLoop = false;
			break;
		}
	}
	return allLoop;
}

static bool ASAPInfo_ValidateFc(uint8_t const *module, int moduleLen)
{
	if (moduleLen < 899)
		return false;
	if (module[0] != 38 || module[1] != 35)
		return false;
	return true;
}

static bool ASAPInfo_ParseFc(ASAPInfo *self, uint8_t const *module, int moduleLen)
{
	if (!ASAPInfo_ValidateFc(module, moduleLen))
		return false;
	self->type = ASAPModuleType_FC;
	self->player = 1024;
	self->music = 2560;
	self->songs = 0;
	self->headerLen = -1;
	int patternOffsets[64];
	int currentOffset = 899;
	for (int i = 0; i < 64; i++) {
		patternOffsets[i] = currentOffset;
		if ((currentOffset = ASAPInfo_AfterFF(module, moduleLen, currentOffset)) == -1)
			return false;
	}
	for (int i = 0; i < 32; i++) {
		if ((currentOffset = ASAPInfo_AfterFF(module, moduleLen, currentOffset)) == -1)
			return false;
	}
	for (int pos = 0; pos < 256 && self->songs < 32;) {
		int trackPos[3];
		for (int n = 0; n < 3; n++)
			trackPos[n] = pos;
		int patternDelay[3] = { 0 };
		int noteDuration[3] = { 0 };
		int patternPos[3] = { 0 };
		int playerCalls = 0;
		self->loops[self->songs] = true;
		while (!ASAPInfo_IsFcSongEnd(module, trackPos)) {
			for (int n = 0; n < 3; n++) {
				int trackCmd = ASAPInfo_GetFcTrackCommand(module, trackPos, n);
				if (trackCmd != 255 && patternDelay[n]-- <= 0) {
					while (trackPos[n] < 256) {
						trackCmd = ASAPInfo_GetFcTrackCommand(module, trackPos, n);
						if (trackCmd < 64) {
							int patternCmd = module[patternOffsets[trackCmd] + patternPos[n]++];
							if (patternCmd < 64) {
								patternDelay[n] = noteDuration[n];
								break;
							}
							else if (patternCmd < 96)
								noteDuration[n] = patternCmd - 64;
							else if (patternCmd == 255) {
								patternDelay[n] = 0;
								noteDuration[n] = 0;
								patternPos[n] = 0;
								trackPos[n]++;
							}
						}
						else if (trackCmd == 64)
							trackPos[n] += 2;
						else if (trackCmd == 254) {
							self->loops[self->songs] = false;
							break;
						}
						else if (trackCmd == 255)
							break;
						else
							trackPos[n]++;
					}
				}
			}
			if (ASAPInfo_IsFcSongEnd(module, trackPos))
				break;
			playerCalls += module[2];
		}
		pos = -1;
		for (int n = 0; n < 3; n++) {
			int nxtrkpos = trackPos[n];
			if (patternPos[n] > 0)
				nxtrkpos++;
			if (pos < nxtrkpos)
				pos = nxtrkpos;
		}
		pos++;
		if (pos <= 256)
			ASAPInfo_AddSong(self, playerCalls);
	}
	return true;
}

static bool ASAPInfo_HasStringAt(uint8_t const *module, int moduleIndex, const char *s)
{
	int n = (int) strlen(s);
	for (int i = 0; i < n; i++)
		if (module[moduleIndex + i] != s[i])
			return false;
	return true;
}

static int ASAPInfo_ParseDec(const char *s, int minVal, int maxVal)
{
	int r = 0;
	int len = (int) strlen(s);
	for (int i = 0; i < len; i++) {
		int c = s[i];
		if (c < 48 || c > 57)
			return -1;
		r = r * 10 + c - 48;
		if (r > maxVal)
			return -1;
	}
	if (r < minVal)
		return -1;
	return r;
}

static int ASAPInfo_ParseHex(const char *s)
{
	int r = 0;
	int len = (int) strlen(s);
	for (int i = 0; i < len; i++) {
		if (r > 4095)
			return -1;
		int c = s[i];
		r <<= 4;
		if (c >= 48 && c <= 57)
			r += c - 48;
		else if (c >= 65 && c <= 70)
			r += c - 65 + 10;
		else if (c >= 97 && c <= 102)
			r += c - 97 + 10;
		else
			return -1;
	}
	return r;
}

int ASAPInfo_ParseDuration(const char *s)
{
	DurationParser parser;
	return DurationParser_Parse(&parser, s);
}

static bool ASAPInfo_ValidateSap(uint8_t const *module, int moduleLen)
{
	return moduleLen >= 30 && ASAPInfo_HasStringAt(module, 0, "SAP\r\n");
}

static bool ASAPInfo_ParseSap(ASAPInfo *self, uint8_t const *module, int moduleLen)
{
	if (!ASAPInfo_ValidateSap(module, moduleLen))
		return false;
	self->fastplay = -1;
	int type = 0;
	int moduleIndex = 5;
	int durationIndex = 0;
	while (module[moduleIndex] != 255) {
		int lineStart = moduleIndex;
		while (module[moduleIndex] > 32) {
			if (++moduleIndex >= moduleLen)
				return false;
		}
		int tagLen = moduleIndex - lineStart;
		int argStart = -1;
		int argLen = -1;
		for (;;) {
			int c = module[moduleIndex];
			if (c > 32) {
				if (!ASAPInfo_CheckValidChar(c))
					return false;
				if (argStart < 0)
					argStart = moduleIndex;
				argLen = -1;
			}
			else {
				if (argLen < 0)
					argLen = moduleIndex - argStart;
				if (c == 10)
					break;
			}
			if (++moduleIndex >= moduleLen)
				return false;
		}
		if (++moduleIndex + 6 >= moduleLen)
			return false;
		if (tagLen <= 8) {
			char *tag = CiString_Substring((const char *) module + lineStart, tagLen);
			if (argStart >= 0 && argLen <= 129) {
				char *arg = CiString_Substring((const char *) module + argStart, argLen);
				if (argLen >= 3 && arg[0] == '\"' && arg[argLen - 1] == '\"' && strcmp(arg, "\"<?>\"") != 0) {
					if (strcmp(tag, "AUTHOR") == 0)
						CiString_Assign(&self->author, CiString_Substring(arg + 1, argLen - 2));
					else if (strcmp(tag, "NAME") == 0)
						CiString_Assign(&self->title, CiString_Substring(arg + 1, argLen - 2));
					else if (strcmp(tag, "DATE") == 0)
						CiString_Assign(&self->date, CiString_Substring(arg + 1, argLen - 2));
				}
				else if (strcmp(tag, "SONGS") == 0) {
					if ((self->songs = ASAPInfo_ParseDec(arg, 1, 32)) == -1) {
						free(arg);
						free(tag);
						return false;
					}
				}
				else if (strcmp(tag, "DEFSONG") == 0) {
					if ((self->defaultSong = ASAPInfo_ParseDec(arg, 0, 31)) == -1) {
						free(arg);
						free(tag);
						return false;
					}
				}
				else if (strcmp(tag, "TIME") == 0) {
					if (durationIndex >= 32) {
						free(arg);
						free(tag);
						return false;
					}
					if (argLen > 5 && ASAPInfo_HasStringAt(module, argStart + argLen - 5, " LOOP")) {
						self->loops[durationIndex] = true;
						arg[argLen - 5] = '\0';
					}
					if ((self->durations[durationIndex++] = ASAPInfo_ParseDuration(arg)) == -1) {
						free(arg);
						free(tag);
						return false;
					}
				}
				else if (strcmp(tag, "TYPE") == 0)
					type = arg[0];
				else if (strcmp(tag, "FASTPLAY") == 0) {
					if ((self->fastplay = ASAPInfo_ParseDec(arg, 1, 32767)) == -1) {
						free(arg);
						free(tag);
						return false;
					}
				}
				else if (strcmp(tag, "MUSIC") == 0) {
					if ((self->music = ASAPInfo_ParseHex(arg)) == -1) {
						free(arg);
						free(tag);
						return false;
					}
				}
				else if (strcmp(tag, "INIT") == 0) {
					if ((self->init = ASAPInfo_ParseHex(arg)) == -1) {
						free(arg);
						free(tag);
						return false;
					}
				}
				else if (strcmp(tag, "PLAYER") == 0) {
					if ((self->player = ASAPInfo_ParseHex(arg)) == -1) {
						free(arg);
						free(tag);
						return false;
					}
				}
				else if (strcmp(tag, "COVOX") == 0) {
					if ((self->covoxAddr = ASAPInfo_ParseHex(arg)) == -1) {
						free(arg);
						free(tag);
						return false;
					}
					if (self->covoxAddr != 54784) {
						free(arg);
						free(tag);
						return false;
					}
					self->channels = 2;
				}
				free(arg);
			}
			else if (strcmp(tag, "STEREO") == 0)
				self->channels = 2;
			else if (strcmp(tag, "NTSC") == 0)
				self->ntsc = true;
			free(tag);
		}
	}
	if (self->defaultSong >= self->songs)
		return false;
	switch (type) {
	case 66:
		if (self->player < 0)
			return false;
		if (self->init < 0)
			return false;
		self->type = ASAPModuleType_SAP_B;
		break;
	case 67:
		if (self->player < 0)
			return false;
		if (self->music < 0)
			return false;
		self->type = ASAPModuleType_SAP_C;
		break;
	case 68:
		if (self->init < 0)
			return false;
		self->type = ASAPModuleType_SAP_D;
		break;
	case 83:
		if (self->init < 0)
			return false;
		self->type = ASAPModuleType_SAP_S;
		if (self->fastplay < 0)
			self->fastplay = 78;
		break;
	default:
		return false;
	}
	if (self->fastplay < 0)
		self->fastplay = self->ntsc ? 262 : 312;
	if (module[moduleIndex + 1] != 255)
		return false;
	self->headerLen = moduleIndex;
	return true;
}

static int ASAPInfo_PackExt(const char *ext)
{
	return (int) strlen(ext) == 2 && ext[0] <= 'z' && ext[1] <= 'z' ? ext[0] | ext[1] << 8 | 2105376 : (int) strlen(ext) == 3 && ext[0] <= 'z' && ext[1] <= 'z' && ext[2] <= 'z' ? ext[0] | ext[1] << 8 | ext[2] << 16 | 2105376 : 0;
}

static int ASAPInfo_GetPackedExt(const char *filename)
{
	int ext = 0;
	for (int i = (int) strlen(filename); --i > 0;) {
		int c = filename[i];
		if (c <= 32 || c > 122)
			return 0;
		if (c == 46)
			return ext | 2105376;
		ext = (ext << 8) + c;
	}
	return 0;
}

static bool ASAPInfo_IsOurPackedExt(int ext)
{
	switch (ext) {
	case 7364979:
	case 6516067:
	case 3370339:
	case 7499107:
	case 7564643:
	case 6516068:
	case 7629924:
	case 7630957:
	case 6582381:
	case 7630194:
	case 6516084:
	case 3698036:
	case 3304820:
	case 2122598:
		return true;
	default:
		return false;
	}
}

bool ASAPInfo_IsOurFile(const char *filename)
{
	return ASAPInfo_IsOurPackedExt(ASAPInfo_GetPackedExt(filename));
}

bool ASAPInfo_IsOurExt(const char *ext)
{
	return ASAPInfo_IsOurPackedExt(ASAPInfo_PackExt(ext));
}

static int ASAPInfo_GuessPackedExt(uint8_t const *module, int moduleLen)
{
	if (ASAPInfo_ValidateSap(module, moduleLen))
		return 7364979;
	if (ASAPInfo_ValidateFc(module, moduleLen))
		return 2122598;
	if (ASAPInfo_ValidateRmt(module, moduleLen))
		return 7630194;
	return -1;
}

bool ASAPInfo_Load(ASAPInfo *self, const char *filename, uint8_t const *module, int moduleLen)
{
	int ext;
	if (filename != NULL) {
		int len = (int) strlen(filename);
		int basename = 0;
		ext = -1;
		for (int i = len; --i >= 0;) {
			int c = filename[i];
			if (c == 47 || c == 92) {
				basename = i + 1;
				break;
			}
			if (c == 46)
				ext = i;
		}
		if (ext < 0)
			return false;
		ext -= basename;
		if (ext > 127)
			ext = 127;
		CiString_Assign(&self->filename, CiString_Substring(filename + basename, ext));
		ext = ASAPInfo_GetPackedExt(filename);
	}
	else {
		CiString_Assign(&self->filename, strdup(""));
		if ((ext = ASAPInfo_GuessPackedExt(module, moduleLen)) == -1)
			return false;
	}
	CiString_Assign(&self->author, strdup(""));
	CiString_Assign(&self->title, strdup(""));
	CiString_Assign(&self->date, strdup(""));
	self->channels = 1;
	self->songs = 1;
	self->defaultSong = 0;
	for (int i = 0; i < 32; i++) {
		self->durations[i] = -1;
		self->loops[i] = false;
	}
	self->ntsc = false;
	self->fastplay = 312;
	self->music = -1;
	self->init = -1;
	self->player = -1;
	self->covoxAddr = -1;
	self->headerLen = 0;
	switch (ext) {
	case 7364979:
		return ASAPInfo_ParseSap(self, module, moduleLen);
	case 6516067:
		return ASAPInfo_ParseCmc(self, module, moduleLen, ASAPModuleType_CMC);
	case 3370339:
		return ASAPInfo_ParseCmc(self, module, moduleLen, ASAPModuleType_CM3);
	case 7499107:
		return ASAPInfo_ParseCmc(self, module, moduleLen, ASAPModuleType_CMR);
	case 7564643:
		self->channels = 2;
		return ASAPInfo_ParseCmc(self, module, moduleLen, ASAPModuleType_CMS);
	case 6516068:
		self->fastplay = 156;
		return ASAPInfo_ParseCmc(self, module, moduleLen, ASAPModuleType_CMC);
	case 7629924:
		return ASAPInfo_ParseDlt(self, module, moduleLen);
	case 7630957:
		return ASAPInfo_ParseMpt(self, module, moduleLen);
	case 6582381:
		self->fastplay = 156;
		return ASAPInfo_ParseMpt(self, module, moduleLen);
	case 7630194:
		return ASAPInfo_ParseRmt(self, module, moduleLen);
	case 6516084:
	case 3698036:
		return ASAPInfo_ParseTmc(self, module, moduleLen);
	case 3304820:
		return ASAPInfo_ParseTm2(self, module, moduleLen);
	case 2122598:
		return ASAPInfo_ParseFc(self, module, moduleLen);
	default:
		return false;
	}
}

const char *ASAPInfo_GetExtDescription(const char *ext)
{
	switch (ASAPInfo_PackExt(ext)) {
	case 7364979:
		return "Slight Atari Player";
	case 6516067:
		return "Chaos Music Composer";
	case 3370339:
		return "CMC \"3/4\"";
	case 7499107:
		return "CMC \"Rzog\"";
	case 7564643:
		return "Stereo Double CMC";
	case 6516068:
		return "DoublePlay CMC";
	case 7629924:
		return "Delta Music Composer";
	case 7630957:
		return "Music ProTracker";
	case 6582381:
		return "MPT DoublePlay";
	case 7630194:
		return "Raster Music Tracker";
	case 6516084:
	case 3698036:
		return "Theta Music Composer 1.x";
	case 3304820:
		return "Theta Music Composer 2.x";
	case 2122598:
		return "Future Composer";
	case 7890296:
		return "Atari 8-bit executable";
	default:
		return NULL;
	}
}

static int ASAPInfo_GetRmtSapOffset(const ASAPInfo *self, uint8_t const *module, int moduleLen)
{
	if (self->player != 13315)
		return -1;
	int offset = self->headerLen + ASAPInfo_GetWord(module, self->headerLen + 4) - ASAPInfo_GetWord(module, self->headerLen + 2) + 7;
	if (offset + 6 >= moduleLen || module[offset + 4] != 82 || module[offset + 5] != 77 || module[offset + 6] != 84)
		return -1;
	return offset;
}

static ASAPModuleType ASAPInfo_GetOriginalModuleType(const ASAPInfo *self, uint8_t const *module, int moduleLen)
{
	switch (self->type) {
	case ASAPModuleType_SAP_B:
		if ((self->init == 1019 || self->init == 1017) && self->player == 1283)
			return ASAPModuleType_DLT;
		if (((self->init == 1267 || self->init == 1263) && self->player == 1283) || (self->init == 62707 && self->player == 62723))
			return ASAPModuleType_MPT;
		if (self->init == 3200 || ASAPInfo_GetRmtSapOffset(self, module, moduleLen) > 0)
			return ASAPModuleType_RMT;
		if (self->init == 1269 || self->init == 62709 || self->init == 1266 || ((self->init == 1255 || self->init == 62695 || self->init == 1252) && self->fastplay == 156) || ((self->init == 1253 || self->init == 62693 || self->init == 1250) && (self->fastplay == 104 || self->fastplay == 78)))
			return ASAPModuleType_TMC;
		if ((self->init == 4224 && self->player == 1283) || (self->init == 4992 && self->player == 2051))
			return ASAPModuleType_TM2;
		if (self->init == 1024 && self->player == 1027)
			return ASAPModuleType_FC;
		return self->type;
	case ASAPModuleType_SAP_C:
		if ((self->player == 1280 || self->player == 62720) && moduleLen >= 1024) {
			if (self->channels > 1)
				return ASAPModuleType_CMS;
			if (module[moduleLen - 170] == 30)
				return ASAPModuleType_CMR;
			if (module[moduleLen - 909] == 48)
				return ASAPModuleType_CM3;
			return ASAPModuleType_CMC;
		}
		return self->type;
	default:
		return self->type;
	}
}

const char *ASAPInfo_GetOriginalModuleExt(const ASAPInfo *self, uint8_t const *module, int moduleLen)
{
	switch (ASAPInfo_GetOriginalModuleType(self, module, moduleLen)) {
	case ASAPModuleType_CMC:
		return self->fastplay == 156 ? "dmc" : "cmc";
	case ASAPModuleType_CM3:
		return "cm3";
	case ASAPModuleType_CMR:
		return "cmr";
	case ASAPModuleType_CMS:
		return "cms";
	case ASAPModuleType_DLT:
		return "dlt";
	case ASAPModuleType_MPT:
		return self->fastplay == 156 ? "mpd" : "mpt";
	case ASAPModuleType_RMT:
		return "rmt";
	case ASAPModuleType_TMC:
		return "tmc";
	case ASAPModuleType_TM2:
		return "tm2";
	case ASAPModuleType_FC:
		return "fc";
	default:
		return NULL;
	}
}

static int ASAPNativeModuleWriter_GetByte(const ASAPNativeModuleWriter *self, int offset)
{
	return self->sourceModule[self->sourceOffset + offset];
}

static int ASAPNativeModuleWriter_GetWord(const ASAPNativeModuleWriter *self, int offset)
{
	return ASAPInfo_GetWord(self->sourceModule, self->sourceOffset + offset);
}

static bool ASAPNativeModuleWriter_Copy(const ASAPNativeModuleWriter *self, int endOffset)
{
	return ASAPWriter_WriteBytes(self->writer, self->sourceModule, self->sourceOffset + self->writer->outputOffset, self->sourceOffset + endOffset);
}

static bool ASAPNativeModuleWriter_RelocateBytes(const ASAPNativeModuleWriter *self, int lowOffset, int highOffset, int count, int shift)
{
	lowOffset += self->sourceOffset;
	highOffset += self->sourceOffset;
	for (int i = 0; i < count; i++) {
		int address = self->sourceModule[lowOffset + i] + (self->sourceModule[highOffset + i] << 8);
		if (address != 0 && address != 65535)
			address += self->addressDiff;
		if (!ASAPWriter_WriteByte(self->writer, address >> shift & 255))
			return false;
	}
	return true;
}

static bool ASAPNativeModuleWriter_RelocateLowHigh(const ASAPNativeModuleWriter *self, int count)
{
	int lowOffset = self->writer->outputOffset;
	if (!ASAPNativeModuleWriter_RelocateBytes(self, lowOffset, lowOffset + count, count, 0))
		return false;
	return ASAPNativeModuleWriter_RelocateBytes(self, lowOffset, lowOffset + count, count, 8);
}

static bool ASAPNativeModuleWriter_RelocateWords(const ASAPNativeModuleWriter *self, int count)
{
	while (--count >= 0) {
		int address = ASAPNativeModuleWriter_GetWord(self, self->writer->outputOffset);
		if (address != 0 && address != 65535)
			address += self->addressDiff;
		if (!ASAPWriter_WriteWord(self->writer, address))
			return false;
	}
	return true;
}

static bool ASAPNativeModuleWriter_Write(ASAPNativeModuleWriter *self, const ASAPInfo *info, ASAPModuleType type, int moduleLen)
{
	int startAddr = ASAPNativeModuleWriter_GetWord(self, 2);
	self->addressDiff = ASAPInfo_GetMusicAddress(info) < 0 ? 0 : ASAPInfo_GetMusicAddress(info) - startAddr;
	if (ASAPNativeModuleWriter_GetWord(self, 4) + self->addressDiff > 65535)
		return false;
	switch (type) {
	case ASAPModuleType_CMC:
	case ASAPModuleType_CM3:
	case ASAPModuleType_CMR:
	case ASAPModuleType_CMS:
		if (!ASAPNativeModuleWriter_RelocateWords(self, 3))
			return false;
		if (!ASAPNativeModuleWriter_Copy(self, 26))
			return false;
		if (!ASAPNativeModuleWriter_RelocateLowHigh(self, 64))
			return false;
		break;
	case ASAPModuleType_DLT:
		if (!ASAPNativeModuleWriter_RelocateWords(self, 3))
			return false;
		break;
	case ASAPModuleType_MPT:
		if (!ASAPNativeModuleWriter_RelocateWords(self, 99))
			return false;
		if (!ASAPNativeModuleWriter_Copy(self, 454))
			return false;
		if (!ASAPNativeModuleWriter_RelocateLowHigh(self, 4))
			return false;
		break;
	case ASAPModuleType_RMT:
		if (!ASAPWriter_WriteWord(self->writer, 65535))
			return false;
		if (!ASAPNativeModuleWriter_RelocateWords(self, 2))
			return false;
		if (!ASAPNativeModuleWriter_Copy(self, 14))
			return false;
		int patternLowAddress = ASAPNativeModuleWriter_GetWord(self, 16);
		if (!ASAPNativeModuleWriter_RelocateWords(self, (patternLowAddress - startAddr - 8) >> 1))
			return false;
		if (!ASAPNativeModuleWriter_RelocateLowHigh(self, ASAPNativeModuleWriter_GetWord(self, 18) - patternLowAddress))
			return false;
		int songOffset = 6 + ASAPNativeModuleWriter_GetWord(self, 20) - startAddr;
		if (!ASAPNativeModuleWriter_Copy(self, songOffset))
			return false;
		int songEnd = 7 + ASAPNativeModuleWriter_GetWord(self, 4) - startAddr;
		while (songOffset + 3 < songEnd) {
			int nextSongOffset = songOffset + ASAPNativeModuleWriter_GetByte(self, 9) - 48;
			if (ASAPNativeModuleWriter_GetByte(self, songOffset) == 254) {
				if (!ASAPNativeModuleWriter_Copy(self, songOffset + 2))
					return false;
				if (!ASAPNativeModuleWriter_RelocateWords(self, 1))
					return false;
			}
			if (nextSongOffset > songEnd)
				nextSongOffset = songEnd;
			if (!ASAPNativeModuleWriter_Copy(self, nextSongOffset))
				return false;
			songOffset = nextSongOffset;
		}
		if (!ASAPNativeModuleWriter_Copy(self, songEnd))
			return false;
		if (moduleLen >= songEnd + 5) {
			if (!ASAPNativeModuleWriter_RelocateWords(self, 2))
				return false;
		}
		break;
	case ASAPModuleType_TMC:
		if (!ASAPNativeModuleWriter_RelocateWords(self, 3))
			return false;
		if (!ASAPNativeModuleWriter_Copy(self, 38))
			return false;
		if (!ASAPNativeModuleWriter_RelocateLowHigh(self, 64))
			return false;
		if (!ASAPNativeModuleWriter_RelocateLowHigh(self, 128))
			return false;
		break;
	case ASAPModuleType_TM2:
		if (!ASAPNativeModuleWriter_RelocateWords(self, 3))
			return false;
		if (!ASAPNativeModuleWriter_Copy(self, 134))
			return false;
		if (!ASAPNativeModuleWriter_RelocateBytes(self, 134, 774, 128, 0))
			return false;
		if (!ASAPNativeModuleWriter_RelocateLowHigh(self, 256))
			return false;
		if (!ASAPNativeModuleWriter_RelocateBytes(self, 134, 774, 128, 8))
			return false;
		break;
	default:
		return false;
	}
	return ASAPNativeModuleWriter_Copy(self, moduleLen);
}

static void ASAPWriter_Construct(ASAPWriter *self)
{
}

ASAPWriter *ASAPWriter_New(void)
{
	ASAPWriter *self = (ASAPWriter *) malloc(sizeof(ASAPWriter));
	if (self != NULL)
		ASAPWriter_Construct(self);
	return self;
}

void ASAPWriter_Delete(ASAPWriter *self)
{
	free(self);
}

int ASAPWriter_GetSaveExts(const char **exts, const ASAPInfo *info, uint8_t const *module, int moduleLen)
{
	int i = 0;
	switch (info->type) {
	case ASAPModuleType_SAP_B:
	case ASAPModuleType_SAP_C:
		exts[i++] = "sap";
		const char *ext = ASAPInfo_GetOriginalModuleExt(info, module, moduleLen);
		if (ext != NULL)
			exts[i++] = ext;
		exts[i++] = "xex";
		break;
	case ASAPModuleType_SAP_D:
		exts[i++] = "sap";
		if (ASAPInfo_GetPlayerRateScanlines(info) == 312)
			exts[i++] = "xex";
		break;
	case ASAPModuleType_SAP_S:
		exts[i++] = "sap";
		break;
	default:
		exts[i++] = ASAPInfo_GetOriginalModuleExt(info, module, moduleLen);
		exts[i++] = "sap";
		exts[i++] = "xex";
		break;
	}
	return i;
}

static void ASAPWriter_TwoDigitsToString(uint8_t *result, int offset, int value)
{
	result[offset] = (uint8_t) (48 + value / 10);
	result[offset + 1] = (uint8_t) (48 + value % 10);
}

static bool ASAPWriter_SecondsToString(uint8_t *result, int offset, int value)
{
	if (value < 0 || value >= 6000000)
		return false;
	value /= 1000;
	ASAPWriter_TwoDigitsToString(result, offset, value / 60);
	result[offset + 2] = 58;
	ASAPWriter_TwoDigitsToString(result, offset + 3, value % 60);
	return true;
}

int ASAPWriter_DurationToString(uint8_t *result, int value)
{
	if (!ASAPWriter_SecondsToString(result, 0, value))
		return 0;
	value %= 1000;
	if (value == 0)
		return 5;
	result[5] = 46;
	ASAPWriter_TwoDigitsToString(result, 6, value / 10);
	value %= 10;
	if (value == 0)
		return 8;
	result[8] = (uint8_t) (48 + value);
	return 9;
}

void ASAPWriter_SetOutput(ASAPWriter *self, uint8_t *output, int startIndex, int endIndex)
{
	self->output = output;
	self->outputOffset = startIndex;
	self->outputEnd = endIndex;
}

static bool ASAPWriter_WriteByte(ASAPWriter *self, int value)
{
	if (self->outputOffset >= self->outputEnd)
		return false;
	self->output[self->outputOffset++] = (uint8_t) value;
	return true;
}

static bool ASAPWriter_WriteWord(ASAPWriter *self, int value)
{
	if (!ASAPWriter_WriteByte(self, value & 255))
		return false;
	return ASAPWriter_WriteByte(self, value >> 8);
}

static bool ASAPWriter_WriteBytes(ASAPWriter *self, uint8_t const *array, int startIndex, int endIndex)
{
	int length = endIndex - startIndex;
	if (self->outputOffset + length > self->outputEnd)
		return false;
	memcpy(self->output + self->outputOffset, array + startIndex, length);
	self->outputOffset += length;
	return true;
}

static bool ASAPWriter_WriteString(ASAPWriter *self, const char *s)
{
	int n = (int) strlen(s);
	for (int i = 0; i < n; i++) {
		if (!ASAPWriter_WriteByte(self, s[i]))
			return false;
	}
	return true;
}

static bool ASAPWriter_WriteDec(ASAPWriter *self, int value)
{
	if (value >= 10) {
		if (!ASAPWriter_WriteDec(self, value / 10))
			return false;
		value %= 10;
	}
	return ASAPWriter_WriteByte(self, 48 + value);
}

static bool ASAPWriter_WriteTextSapTag(ASAPWriter *self, const char *tag, const char *value)
{
	if (!ASAPWriter_WriteString(self, tag))
		return false;
	if (!ASAPWriter_WriteByte(self, 34))
		return false;
	if (value[0] == '\0')
		value = "<?>";
	if (!ASAPWriter_WriteString(self, value))
		return false;
	if (!ASAPWriter_WriteByte(self, 34))
		return false;
	if (!ASAPWriter_WriteByte(self, 13))
		return false;
	return ASAPWriter_WriteByte(self, 10);
}

static bool ASAPWriter_WriteDecSapTag(ASAPWriter *self, const char *tag, int value)
{
	if (!ASAPWriter_WriteString(self, tag))
		return false;
	if (!ASAPWriter_WriteDec(self, value))
		return false;
	if (!ASAPWriter_WriteByte(self, 13))
		return false;
	return ASAPWriter_WriteByte(self, 10);
}

static bool ASAPWriter_WriteHexSapTag(ASAPWriter *self, const char *tag, int value)
{
	if (value < 0)
		return true;
	if (!ASAPWriter_WriteString(self, tag))
		return false;
	for (int i = 12; i >= 0; i -= 4) {
		int digit = value >> i & 15;
		if (!ASAPWriter_WriteByte(self, digit + (digit < 10 ? 48 : 55)))
			return false;
	}
	if (!ASAPWriter_WriteByte(self, 13))
		return false;
	return ASAPWriter_WriteByte(self, 10);
}

static bool ASAPWriter_WriteSapHeader(ASAPWriter *self, const ASAPInfo *info, int type, int init, int player)
{
	if (!ASAPWriter_WriteString(self, "SAP\r\n"))
		return false;
	if (!ASAPWriter_WriteTextSapTag(self, "AUTHOR ", ASAPInfo_GetAuthor(info)))
		return false;
	if (!ASAPWriter_WriteTextSapTag(self, "NAME ", ASAPInfo_GetTitle(info)))
		return false;
	if (!ASAPWriter_WriteTextSapTag(self, "DATE ", ASAPInfo_GetDate(info)))
		return false;
	if (ASAPInfo_GetSongs(info) > 1) {
		if (!ASAPWriter_WriteDecSapTag(self, "SONGS ", ASAPInfo_GetSongs(info)))
			return false;
		if (ASAPInfo_GetDefaultSong(info) > 0) {
			if (!ASAPWriter_WriteDecSapTag(self, "DEFSONG ", ASAPInfo_GetDefaultSong(info)))
				return false;
		}
	}
	if (ASAPInfo_GetChannels(info) > 1) {
		if (!ASAPWriter_WriteString(self, "STEREO\r\n"))
			return false;
	}
	if (ASAPInfo_IsNtsc(info)) {
		if (!ASAPWriter_WriteString(self, "NTSC\r\n"))
			return false;
	}
	if (!ASAPWriter_WriteString(self, "TYPE "))
		return false;
	if (!ASAPWriter_WriteByte(self, type))
		return false;
	if (!ASAPWriter_WriteByte(self, 13))
		return false;
	if (!ASAPWriter_WriteByte(self, 10))
		return false;
	if (ASAPInfo_GetPlayerRateScanlines(info) != 312 || ASAPInfo_IsNtsc(info)) {
		if (!ASAPWriter_WriteDecSapTag(self, "FASTPLAY ", ASAPInfo_GetPlayerRateScanlines(info)))
			return false;
	}
	if (type == 67) {
		if (!ASAPWriter_WriteHexSapTag(self, "MUSIC ", ASAPInfo_GetMusicAddress(info)))
			return false;
	}
	if (!ASAPWriter_WriteHexSapTag(self, "INIT ", init))
		return false;
	if (!ASAPWriter_WriteHexSapTag(self, "PLAYER ", player))
		return false;
	if (!ASAPWriter_WriteHexSapTag(self, "COVOX ", ASAPInfo_GetCovoxAddress(info)))
		return false;
	for (int song = 0; song < ASAPInfo_GetSongs(info); song++) {
		if (ASAPInfo_GetDuration(info, song) < 0)
			break;
		if (!ASAPWriter_WriteString(self, "TIME "))
			return false;
		uint8_t s[9];
		if (!ASAPWriter_WriteBytes(self, s, 0, ASAPWriter_DurationToString(s, ASAPInfo_GetDuration(info, song))))
			return false;
		if (ASAPInfo_GetLoop(info, song)) {
			if (!ASAPWriter_WriteString(self, " LOOP"))
				return false;
		}
		if (!ASAPWriter_WriteByte(self, 13))
			return false;
		if (!ASAPWriter_WriteByte(self, 10))
			return false;
	}
	return true;
}

static bool ASAPWriter_WriteExecutableHeader(ASAPWriter *self, int *initAndPlayer, const ASAPInfo *info, int type, int init, int player)
{
	if (initAndPlayer == NULL) {
		if (!ASAPWriter_WriteSapHeader(self, info, type, init, player))
			return false;
	}
	else {
		initAndPlayer[0] = init;
		initAndPlayer[1] = player;
	}
	return true;
}

static bool ASAPWriter_WritePlaTaxLda0(ASAPWriter *self)
{
	if (!ASAPWriter_WriteByte(self, 104))
		return false;
	if (!ASAPWriter_WriteByte(self, 170))
		return false;
	if (!ASAPWriter_WriteByte(self, 169))
		return false;
	return ASAPWriter_WriteByte(self, 0);
}

static bool ASAPWriter_WriteCmcInit(ASAPWriter *self, int *initAndPlayer, const ASAPInfo *info)
{
	if (initAndPlayer == NULL)
		return true;
	if (!ASAPWriter_WriteWord(self, 4064))
		return false;
	if (!ASAPWriter_WriteWord(self, 4080))
		return false;
	if (!ASAPWriter_WriteByte(self, 72))
		return false;
	int music = ASAPInfo_GetMusicAddress(info);
	if (!ASAPWriter_WriteByte(self, 162))
		return false;
	if (!ASAPWriter_WriteByte(self, music & 255))
		return false;
	if (!ASAPWriter_WriteByte(self, 160))
		return false;
	if (!ASAPWriter_WriteByte(self, music >> 8))
		return false;
	if (!ASAPWriter_WriteByte(self, 169))
		return false;
	if (!ASAPWriter_WriteByte(self, 112))
		return false;
	if (!ASAPWriter_WriteByte(self, 32))
		return false;
	if (!ASAPWriter_WriteWord(self, initAndPlayer[1] + 3))
		return false;
	if (!ASAPWriter_WritePlaTaxLda0(self))
		return false;
	if (!ASAPWriter_WriteByte(self, 76))
		return false;
	if (!ASAPWriter_WriteWord(self, initAndPlayer[1] + 3))
		return false;
	initAndPlayer[0] = 4064;
	initAndPlayer[1] += 6;
	return true;
}

static bool ASAPWriter_WriteExecutableFromSap(ASAPWriter *self, int *initAndPlayer, const ASAPInfo *info, int type, uint8_t const *module, int moduleLen)
{
	if (!ASAPWriter_WriteExecutableHeader(self, initAndPlayer, info, type, ASAPInfo_GetInitAddress(info), info->player))
		return false;
	return ASAPWriter_WriteBytes(self, module, info->headerLen, moduleLen);
}

static int ASAPWriter_WriteExecutableHeaderForSongPos(ASAPWriter *self, int *initAndPlayer, const ASAPInfo *info, int player, int codeForOneSong, int codeForManySongs, int playerOffset)
{
	if (ASAPInfo_GetSongs(info) != 1) {
		return ASAPWriter_WriteExecutableHeader(self, initAndPlayer, info, 66, player - codeForManySongs, player + playerOffset) ? player - codeForManySongs - ASAPInfo_GetSongs(info) : -1;
	}
	return ASAPWriter_WriteExecutableHeader(self, initAndPlayer, info, 66, player - codeForOneSong, player + playerOffset) ? player - codeForOneSong : -1;
}

static bool ASAPWriter_WriteExecutable(ASAPWriter *self, int *initAndPlayer, const ASAPInfo *info, uint8_t const *module, int moduleLen)
{
	uint8_t const *playerRoutine = ASAP6502_GetPlayerRoutine(info);
	int player = -1;
	int playerLastByte = -1;
	int music = ASAPInfo_GetMusicAddress(info);
	if (playerRoutine != NULL) {
		player = ASAPInfo_GetWord(playerRoutine, 2);
		playerLastByte = ASAPInfo_GetWord(playerRoutine, 4);
		if (music <= playerLastByte)
			return false;
	}
	int startAddr;
	switch (info->type) {
	case ASAPModuleType_SAP_B:
		if (!ASAPWriter_WriteExecutableFromSap(self, initAndPlayer, info, 66, module, moduleLen))
			return false;
		break;
	case ASAPModuleType_SAP_C:
		if (!ASAPWriter_WriteExecutableFromSap(self, initAndPlayer, info, 67, module, moduleLen))
			return false;
		if (!ASAPWriter_WriteCmcInit(self, initAndPlayer, info))
			return false;
		break;
	case ASAPModuleType_SAP_D:
		if (!ASAPWriter_WriteExecutableFromSap(self, initAndPlayer, info, 68, module, moduleLen))
			return false;
		break;
	case ASAPModuleType_SAP_S:
		if (!ASAPWriter_WriteExecutableFromSap(self, initAndPlayer, info, 83, module, moduleLen))
			return false;
		break;
	case ASAPModuleType_CMC:
	case ASAPModuleType_CM3:
	case ASAPModuleType_CMR:
	case ASAPModuleType_CMS:
		if (!ASAPWriter_WriteExecutableHeader(self, initAndPlayer, info, 67, -1, player))
			return false;
		if (!ASAPWriter_WriteWord(self, 65535))
			return false;
		if (!ASAPWriter_WriteBytes(self, module, 2, moduleLen))
			return false;
		if (!ASAPWriter_WriteBytes(self, playerRoutine, 2, playerLastByte - player + 7))
			return false;
		if (!ASAPWriter_WriteCmcInit(self, initAndPlayer, info))
			return false;
		break;
	case ASAPModuleType_DLT:
		if ((startAddr = ASAPWriter_WriteExecutableHeaderForSongPos(self, initAndPlayer, info, player, 5, 7, 259)) == -1)
			return false;
		if (moduleLen == 11270) {
			if (!ASAPWriter_WriteBytes(self, module, 0, 4))
				return false;
			if (!ASAPWriter_WriteWord(self, 19456))
				return false;
			if (!ASAPWriter_WriteBytes(self, module, 6, moduleLen))
				return false;
			if (!ASAPWriter_WriteByte(self, 0))
				return false;
		}
		else {
			if (!ASAPWriter_WriteBytes(self, module, 0, moduleLen))
				return false;
		}
		if (!ASAPWriter_WriteWord(self, startAddr))
			return false;
		if (!ASAPWriter_WriteWord(self, playerLastByte))
			return false;
		if (ASAPInfo_GetSongs(info) != 1) {
			if (!ASAPWriter_WriteBytes(self, info->songPos, 0, ASAPInfo_GetSongs(info)))
				return false;
			if (!ASAPWriter_WriteByte(self, 170))
				return false;
			if (!ASAPWriter_WriteByte(self, 188))
				return false;
			if (!ASAPWriter_WriteWord(self, startAddr))
				return false;
		}
		else {
			if (!ASAPWriter_WriteByte(self, 160))
				return false;
			if (!ASAPWriter_WriteByte(self, 0))
				return false;
		}
		if (!ASAPWriter_WriteByte(self, 76))
			return false;
		if (!ASAPWriter_WriteWord(self, player + 256))
			return false;
		if (!ASAPWriter_WriteBytes(self, playerRoutine, 6, playerLastByte - player + 7))
			return false;
		break;
	case ASAPModuleType_MPT:
		if ((startAddr = ASAPWriter_WriteExecutableHeaderForSongPos(self, initAndPlayer, info, player, 13, 17, 3)) == -1)
			return false;
		if (!ASAPWriter_WriteBytes(self, module, 0, moduleLen))
			return false;
		if (!ASAPWriter_WriteWord(self, startAddr))
			return false;
		if (!ASAPWriter_WriteWord(self, playerLastByte))
			return false;
		if (ASAPInfo_GetSongs(info) != 1) {
			if (!ASAPWriter_WriteBytes(self, info->songPos, 0, ASAPInfo_GetSongs(info)))
				return false;
			if (!ASAPWriter_WriteByte(self, 72))
				return false;
		}
		if (!ASAPWriter_WriteByte(self, 160))
			return false;
		if (!ASAPWriter_WriteByte(self, music & 255))
			return false;
		if (!ASAPWriter_WriteByte(self, 162))
			return false;
		if (!ASAPWriter_WriteByte(self, music >> 8))
			return false;
		if (!ASAPWriter_WriteByte(self, 169))
			return false;
		if (!ASAPWriter_WriteByte(self, 0))
			return false;
		if (!ASAPWriter_WriteByte(self, 32))
			return false;
		if (!ASAPWriter_WriteWord(self, player))
			return false;
		if (ASAPInfo_GetSongs(info) != 1) {
			if (!ASAPWriter_WriteByte(self, 104))
				return false;
			if (!ASAPWriter_WriteByte(self, 168))
				return false;
			if (!ASAPWriter_WriteByte(self, 190))
				return false;
			if (!ASAPWriter_WriteWord(self, startAddr))
				return false;
		}
		else {
			if (!ASAPWriter_WriteByte(self, 162))
				return false;
			if (!ASAPWriter_WriteByte(self, 0))
				return false;
		}
		if (!ASAPWriter_WriteByte(self, 169))
			return false;
		if (!ASAPWriter_WriteByte(self, 2))
			return false;
		if (!ASAPWriter_WriteBytes(self, playerRoutine, 6, playerLastByte - player + 7))
			return false;
		break;
	case ASAPModuleType_RMT:
		if (!ASAPWriter_WriteExecutableHeader(self, initAndPlayer, info, 66, 3200, 1539))
			return false;
		if (!ASAPWriter_WriteBytes(self, module, 0, ASAPInfo_GetWord(module, 4) - music + 7))
			return false;
		if (!ASAPWriter_WriteWord(self, 3200))
			return false;
		if (ASAPInfo_GetSongs(info) != 1) {
			if (!ASAPWriter_WriteWord(self, 3210 + ASAPInfo_GetSongs(info)))
				return false;
			if (!ASAPWriter_WriteByte(self, 168))
				return false;
			if (!ASAPWriter_WriteByte(self, 185))
				return false;
			if (!ASAPWriter_WriteWord(self, 3211))
				return false;
		}
		else {
			if (!ASAPWriter_WriteWord(self, 3208))
				return false;
			if (!ASAPWriter_WriteByte(self, 169))
				return false;
			if (!ASAPWriter_WriteByte(self, 0))
				return false;
		}
		if (!ASAPWriter_WriteByte(self, 162))
			return false;
		if (!ASAPWriter_WriteByte(self, music & 255))
			return false;
		if (!ASAPWriter_WriteByte(self, 160))
			return false;
		if (!ASAPWriter_WriteByte(self, music >> 8))
			return false;
		if (!ASAPWriter_WriteByte(self, 76))
			return false;
		if (!ASAPWriter_WriteWord(self, 1536))
			return false;
		if (ASAPInfo_GetSongs(info) != 1) {
			if (!ASAPWriter_WriteBytes(self, info->songPos, 0, ASAPInfo_GetSongs(info)))
				return false;
		}
		if (!ASAPWriter_WriteBytes(self, playerRoutine, 2, playerLastByte - player + 7))
			return false;
		break;
	case ASAPModuleType_TMC:
		;
		static const int TMC_PLAYER_OFFSET[4] = { 3, -9, -10, -10 };
		int player2 = player + TMC_PLAYER_OFFSET[module[37] - 1];
		static const int TMC_INIT_OFFSET[4] = { -14, -16, -17, -17 };
		startAddr = player2 + TMC_INIT_OFFSET[module[37] - 1];
		if (ASAPInfo_GetSongs(info) != 1)
			startAddr -= 3;
		if (!ASAPWriter_WriteExecutableHeader(self, initAndPlayer, info, 66, startAddr, player2))
			return false;
		if (!ASAPWriter_WriteBytes(self, module, 0, moduleLen))
			return false;
		if (!ASAPWriter_WriteWord(self, startAddr))
			return false;
		if (!ASAPWriter_WriteWord(self, playerLastByte))
			return false;
		if (ASAPInfo_GetSongs(info) != 1) {
			if (!ASAPWriter_WriteByte(self, 72))
				return false;
		}
		if (!ASAPWriter_WriteByte(self, 160))
			return false;
		if (!ASAPWriter_WriteByte(self, music & 255))
			return false;
		if (!ASAPWriter_WriteByte(self, 162))
			return false;
		if (!ASAPWriter_WriteByte(self, music >> 8))
			return false;
		if (!ASAPWriter_WriteByte(self, 169))
			return false;
		if (!ASAPWriter_WriteByte(self, 112))
			return false;
		if (!ASAPWriter_WriteByte(self, 32))
			return false;
		if (!ASAPWriter_WriteWord(self, player))
			return false;
		if (ASAPInfo_GetSongs(info) != 1) {
			if (!ASAPWriter_WritePlaTaxLda0(self))
				return false;
		}
		else {
			if (!ASAPWriter_WriteByte(self, 169))
				return false;
			if (!ASAPWriter_WriteByte(self, 96))
				return false;
		}
		switch (module[37]) {
		case 2:
			if (!ASAPWriter_WriteByte(self, 6))
				return false;
			if (!ASAPWriter_WriteByte(self, 0))
				return false;
			if (!ASAPWriter_WriteByte(self, 76))
				return false;
			if (!ASAPWriter_WriteWord(self, player))
				return false;
			if (!ASAPWriter_WriteByte(self, 165))
				return false;
			if (!ASAPWriter_WriteByte(self, 0))
				return false;
			if (!ASAPWriter_WriteByte(self, 230))
				return false;
			if (!ASAPWriter_WriteByte(self, 0))
				return false;
			if (!ASAPWriter_WriteByte(self, 74))
				return false;
			if (!ASAPWriter_WriteByte(self, 144))
				return false;
			if (!ASAPWriter_WriteByte(self, 5))
				return false;
			if (!ASAPWriter_WriteByte(self, 176))
				return false;
			if (!ASAPWriter_WriteByte(self, 6))
				return false;
			break;
		case 3:
		case 4:
			if (!ASAPWriter_WriteByte(self, 160))
				return false;
			if (!ASAPWriter_WriteByte(self, 1))
				return false;
			if (!ASAPWriter_WriteByte(self, 132))
				return false;
			if (!ASAPWriter_WriteByte(self, 0))
				return false;
			if (!ASAPWriter_WriteByte(self, 208))
				return false;
			if (!ASAPWriter_WriteByte(self, 10))
				return false;
			if (!ASAPWriter_WriteByte(self, 198))
				return false;
			if (!ASAPWriter_WriteByte(self, 0))
				return false;
			if (!ASAPWriter_WriteByte(self, 208))
				return false;
			if (!ASAPWriter_WriteByte(self, 12))
				return false;
			if (!ASAPWriter_WriteByte(self, 160))
				return false;
			if (!ASAPWriter_WriteByte(self, module[37]))
				return false;
			if (!ASAPWriter_WriteByte(self, 132))
				return false;
			if (!ASAPWriter_WriteByte(self, 0))
				return false;
			if (!ASAPWriter_WriteByte(self, 208))
				return false;
			if (!ASAPWriter_WriteByte(self, 3))
				return false;
			break;
		}
		if (!ASAPWriter_WriteBytes(self, playerRoutine, 6, playerLastByte - player + 7))
			return false;
		break;
	case ASAPModuleType_TM2:
		if (!ASAPWriter_WriteExecutableHeader(self, initAndPlayer, info, 66, 4992, 2051))
			return false;
		if (!ASAPWriter_WriteBytes(self, module, 0, moduleLen))
			return false;
		if (!ASAPWriter_WriteWord(self, 4992))
			return false;
		if (ASAPInfo_GetSongs(info) != 1) {
			if (!ASAPWriter_WriteWord(self, 5008))
				return false;
			if (!ASAPWriter_WriteByte(self, 72))
				return false;
		}
		else {
			if (!ASAPWriter_WriteWord(self, 5006))
				return false;
		}
		if (!ASAPWriter_WriteByte(self, 160))
			return false;
		if (!ASAPWriter_WriteByte(self, music & 255))
			return false;
		if (!ASAPWriter_WriteByte(self, 162))
			return false;
		if (!ASAPWriter_WriteByte(self, music >> 8))
			return false;
		if (!ASAPWriter_WriteByte(self, 169))
			return false;
		if (!ASAPWriter_WriteByte(self, 112))
			return false;
		if (!ASAPWriter_WriteByte(self, 32))
			return false;
		if (!ASAPWriter_WriteWord(self, 2048))
			return false;
		if (ASAPInfo_GetSongs(info) != 1) {
			if (!ASAPWriter_WritePlaTaxLda0(self))
				return false;
		}
		else {
			if (!ASAPWriter_WriteByte(self, 169))
				return false;
			if (!ASAPWriter_WriteByte(self, 0))
				return false;
			if (!ASAPWriter_WriteByte(self, 170))
				return false;
		}
		if (!ASAPWriter_WriteByte(self, 76))
			return false;
		if (!ASAPWriter_WriteWord(self, 2048))
			return false;
		if (!ASAPWriter_WriteBytes(self, playerRoutine, 2, playerLastByte - player + 7))
			return false;
		break;
	case ASAPModuleType_FC:
		if (!ASAPWriter_WriteExecutableHeader(self, initAndPlayer, info, 66, player, player + 3))
			return false;
		if (!ASAPWriter_WriteWord(self, 65535))
			return false;
		if (!ASAPWriter_WriteWord(self, music))
			return false;
		if (!ASAPWriter_WriteWord(self, music + moduleLen - 1))
			return false;
		if (!ASAPWriter_WriteBytes(self, module, 0, moduleLen))
			return false;
		if (!ASAPWriter_WriteBytes(self, playerRoutine, 2, playerLastByte - player + 7))
			return false;
		break;
	}
	return true;
}

static int ASAPWriter_PadXexInfo(uint8_t *dest, int offset, int endColumn)
{
	while (offset % 32 != endColumn)
		dest[offset++] = 32;
	return offset;
}

static int ASAPWriter_FormatXexInfoText(uint8_t *dest, int destLen, int endColumn, const char *src, bool author)
{
	int srcLen = (int) strlen(src);
	for (int srcOffset = 0; srcOffset < srcLen;) {
		int c = src[srcOffset++];
		if (c == 32) {
			if (author && srcOffset < srcLen && src[srcOffset] == '&') {
				int authorLen;
				for (authorLen = 1; srcOffset + authorLen < srcLen; authorLen++) {
					if (src[srcOffset + authorLen] == ' ' && srcOffset + authorLen + 1 < srcLen && src[srcOffset + authorLen + 1] == '&')
						break;
				}
				if (authorLen <= 32 && destLen % 32 + 1 + authorLen > 32) {
					destLen = ASAPWriter_PadXexInfo(dest, destLen, 1);
					continue;
				}
			}
			int wordLen;
			for (wordLen = 0; srcOffset + wordLen < srcLen && src[srcOffset + wordLen] != ' '; wordLen++) {
			}
			if (wordLen <= 32 && destLen % 32 + 1 + wordLen > 32) {
				destLen = ASAPWriter_PadXexInfo(dest, destLen, 0);
				continue;
			}
		}
		dest[destLen++] = (uint8_t) c;
	}
	return ASAPWriter_PadXexInfo(dest, destLen, endColumn);
}

static bool ASAPWriter_WriteXexInfoTextDl(ASAPWriter *self, int address, int len, int verticalScrollAt)
{
	if (!ASAPWriter_WriteByte(self, verticalScrollAt == 0 ? 98 : 66))
		return false;
	if (!ASAPWriter_WriteWord(self, address))
		return false;
	for (int i = 32; i < len; i += 32) {
		if (!ASAPWriter_WriteByte(self, i == verticalScrollAt ? 34 : 2))
			return false;
	}
	return true;
}

static bool ASAPWriter_WriteXexInfo(ASAPWriter *self, const ASAPInfo *info)
{
	uint8_t title[256];
	int titleLen = ASAPWriter_FormatXexInfoText(title, 0, 0, ASAPInfo_GetTitle(info)[0] == '\0' ? "(untitled)" : ASAPInfo_GetTitle(info), false);
	uint8_t author[256];
	int authorLen;
	if (ASAPInfo_GetAuthor(info)[0] != '\0') {
		author[0] = 98;
		author[1] = 121;
		author[2] = 32;
		authorLen = ASAPWriter_FormatXexInfoText(author, 3, 0, ASAPInfo_GetAuthor(info), true);
	}
	else
		authorLen = 0;
	uint8_t other[256];
	int otherLen = ASAPWriter_FormatXexInfoText(other, 0, 19, ASAPInfo_GetDate(info), false);
	otherLen = ASAPWriter_FormatXexInfoText(other, otherLen, 27, ASAPInfo_GetChannels(info) > 1 ? " STEREO" : "   MONO", false);
	int duration = ASAPInfo_GetDuration(info, ASAPInfo_GetDefaultSong(info));
	if (duration > 0 && ASAPWriter_SecondsToString(other, otherLen, duration + 999))
		otherLen += 5;
	else
		otherLen = ASAPWriter_PadXexInfo(other, otherLen, 0);
	int totalCharacters = titleLen + authorLen + otherLen;
	int totalLines = totalCharacters / 32;
	int otherAddress = 64592 - otherLen;
	int titleAddress = otherAddress - authorLen - 8 - titleLen;
	if (!ASAPWriter_WriteWord(self, titleAddress))
		return false;
	if (!ASAPWriter_WriteBytes(self, CiResource_xexinfo_obx, 4, 6))
		return false;
	if (!ASAPWriter_WriteBytes(self, title, 0, titleLen))
		return false;
	for (int i = 0; i < 8; i++) {
		if (!ASAPWriter_WriteByte(self, 85))
			return false;
	}
	if (!ASAPWriter_WriteBytes(self, author, 0, authorLen))
		return false;
	if (!ASAPWriter_WriteBytes(self, other, 0, otherLen))
		return false;
	for (int i = totalLines; i < 26; i++) {
		if (!ASAPWriter_WriteByte(self, 112))
			return false;
	}
	if (!ASAPWriter_WriteByte(self, 48))
		return false;
	if (!ASAPWriter_WriteXexInfoTextDl(self, titleAddress, titleLen, titleLen - 32))
		return false;
	if (!ASAPWriter_WriteByte(self, 8))
		return false;
	if (!ASAPWriter_WriteByte(self, 0))
		return false;
	for (int i = 0; i < authorLen; i += 32) {
		if (!ASAPWriter_WriteByte(self, 2))
			return false;
	}
	if (!ASAPWriter_WriteByte(self, 16))
		return false;
	for (int i = 0; i < otherLen; i += 32) {
		if (!ASAPWriter_WriteByte(self, 2))
			return false;
	}
	return ASAPWriter_WriteBytes(self, CiResource_xexinfo_obx, 6, 178);
}

static bool ASAPWriter_WriteNative(ASAPWriter *self, const ASAPInfo *info, uint8_t const *module, int moduleLen)
{
	ASAPNativeModuleWriter nativeWriter;
	nativeWriter.writer = self;
	nativeWriter.sourceModule = module;
	ASAPModuleType type = info->type;
	switch (type) {
	case ASAPModuleType_SAP_B:
	case ASAPModuleType_SAP_C:
		;
		int offset = ASAPInfo_GetRmtSapOffset(info, module, moduleLen);
		if (offset > 0) {
			nativeWriter.sourceOffset = offset - 2;
			return ASAPNativeModuleWriter_Write(&nativeWriter, info, ASAPModuleType_RMT, moduleLen - offset + 2);
		}
		nativeWriter.sourceOffset = info->headerLen;
		int blockLen = ASAPNativeModuleWriter_GetWord(&nativeWriter, 4) - ASAPNativeModuleWriter_GetWord(&nativeWriter, 2) + 7;
		if (blockLen < 7 || info->headerLen + blockLen >= moduleLen)
			return false;
		type = ASAPInfo_GetOriginalModuleType(info, module, moduleLen);
		if (type == ASAPModuleType_FC) {
			if (!ASAPWriter_WriteBytes(self, module, info->headerLen + 6, info->headerLen + blockLen))
				return false;
		}
		else {
			if (!ASAPNativeModuleWriter_Write(&nativeWriter, info, type, blockLen))
				return false;
		}
		break;
	case ASAPModuleType_FC:
		return ASAPWriter_WriteBytes(self, module, 0, moduleLen);
	default:
		nativeWriter.sourceOffset = 0;
		if (!ASAPNativeModuleWriter_Write(&nativeWriter, info, type, moduleLen))
			return false;
		break;
	}
	return true;
}

int ASAPWriter_Write(ASAPWriter *self, const char *targetFilename, const ASAPInfo *info, uint8_t const *module, int moduleLen, bool tag)
{
	int destExt = ASAPInfo_GetPackedExt(targetFilename);
	switch (destExt) {
	case 7364979:
		return ASAPWriter_WriteExecutable(self, NULL, info, module, moduleLen) ? self->outputOffset : -1;
	case 7890296:
		{
			int initAndPlayer[2];
			if (!ASAPWriter_WriteExecutable(self, initAndPlayer, info, module, moduleLen))
				return -1;
			switch (info->type) {
			case ASAPModuleType_SAP_D:
				if (ASAPInfo_GetPlayerRateScanlines(info) != 312)
					return -1;
				if (!ASAPWriter_WriteBytes(self, CiResource_xexd_obx, 2, 117))
					return -1;
				if (!ASAPWriter_WriteWord(self, initAndPlayer[0]))
					return -1;
				if (initAndPlayer[1] < 0) {
					if (!ASAPWriter_WriteByte(self, 96))
						return -1;
					if (!ASAPWriter_WriteByte(self, 96))
						return -1;
					if (!ASAPWriter_WriteByte(self, 96))
						return -1;
				}
				else {
					if (!ASAPWriter_WriteByte(self, 76))
						return -1;
					if (!ASAPWriter_WriteWord(self, initAndPlayer[1]))
						return -1;
				}
				if (!ASAPWriter_WriteByte(self, ASAPInfo_GetDefaultSong(info)))
					return -1;
				break;
			case ASAPModuleType_SAP_S:
				return -1;
			default:
				if (!ASAPWriter_WriteBytes(self, CiResource_xexb_obx, 2, 183))
					return -1;
				if (!ASAPWriter_WriteWord(self, initAndPlayer[0]))
					return -1;
				if (!ASAPWriter_WriteByte(self, 76))
					return -1;
				if (!ASAPWriter_WriteWord(self, initAndPlayer[1]))
					return -1;
				if (!ASAPWriter_WriteByte(self, ASAPInfo_GetDefaultSong(info)))
					return -1;
				int fastplay = ASAPInfo_GetPlayerRateScanlines(info);
				if (!ASAPWriter_WriteByte(self, fastplay & 1))
					return -1;
				if (!ASAPWriter_WriteByte(self, (fastplay >> 1) % 156))
					return -1;
				if (!ASAPWriter_WriteByte(self, (fastplay >> 1) % 131))
					return -1;
				if (!ASAPWriter_WriteByte(self, fastplay / 312))
					return -1;
				if (!ASAPWriter_WriteByte(self, fastplay / 262))
					return -1;
				break;
			}
			if (tag) {
				if (!ASAPWriter_WriteXexInfo(self, info))
					return -1;
			}
			if (!ASAPWriter_WriteWord(self, 736))
				return -1;
			if (!ASAPWriter_WriteWord(self, 737))
				return -1;
			if (!ASAPWriter_WriteWord(self, tag ? 256 : 292))
				return -1;
			FlashPack flashPack;
			return FlashPack_Compress(&flashPack, self) ? self->outputOffset : -1;
		}
	default:
		;
		const char *possibleExt = ASAPInfo_GetOriginalModuleExt(info, module, moduleLen);
		if (possibleExt != NULL) {
			int packedPossibleExt = ASAPInfo_PackExt(possibleExt);
			if (destExt == packedPossibleExt || (destExt == 3698036 && packedPossibleExt == 6516084)) {
				return ASAPWriter_WriteNative(self, info, module, moduleLen) ? self->outputOffset : -1;
			}
		}
		return -1;
	}
}

static void Cpu6502_Reset(Cpu6502 *self)
{
	self->cycle = 0;
	self->nz = 0;
	self->c = 0;
	self->vdi = 0;
}

static int Cpu6502_Peek(const Cpu6502 *self, int addr)
{
	if ((addr & 63744) == 53248)
		return ASAP_PeekHardware(self->asap, addr);
	else
		return self->memory[addr];
}

static void Cpu6502_Poke(Cpu6502 *self, int addr, int data)
{
	if ((addr & 63744) == 53248)
		ASAP_PokeHardware(self->asap, addr, data);
	else
		self->memory[addr] = (uint8_t) data;
}

static int Cpu6502_PeekReadModifyWrite(Cpu6502 *self, int addr)
{
	if (addr >> 8 == 210) {
		self->cycle--;
		int data = ASAP_PeekHardware(self->asap, addr);
		ASAP_PokeHardware(self->asap, addr, data);
		self->cycle++;
		return data;
	}
	return self->memory[addr];
}

static int Cpu6502_Pull(Cpu6502 *self)
{
	int s = (self->s + 1) & 255;
	self->s = s;
	return self->memory[256 + s];
}

static void Cpu6502_PullFlags(Cpu6502 *self)
{
	int data = Cpu6502_Pull(self);
	self->nz = ((data & 128) << 1) + (~data & 2);
	self->c = data & 1;
	self->vdi = data & 76;
}

static void Cpu6502_Push(Cpu6502 *self, int data)
{
	int s = self->s;
	self->memory[256 + s] = (uint8_t) data;
	self->s = (s - 1) & 255;
}

static void Cpu6502_PushPc(Cpu6502 *self)
{
	Cpu6502_Push(self, self->pc >> 8);
	Cpu6502_Push(self, self->pc & 255);
}

static void Cpu6502_PushFlags(Cpu6502 *self, int b)
{
	int nz = self->nz;
	b += ((nz | nz >> 1) & 128) + self->vdi + self->c;
	if ((nz & 255) == 0)
		b += 2;
	Cpu6502_Push(self, b);
}

static void Cpu6502_AddWithCarry(Cpu6502 *self, int data)
{
	int a = self->a;
	int vdi = self->vdi;
	int tmp = a + data + self->c;
	self->nz = tmp & 255;
	if ((vdi & 8) == 0) {
		self->vdi = (vdi & 12) + ((~(data ^ a) & (a ^ tmp)) >> 1 & 64);
		self->c = tmp >> 8;
		self->a = self->nz;
	}
	else {
		int al = (a & 15) + (data & 15) + self->c;
		if (al >= 10) {
			tmp += al < 26 ? 6 : -10;
			if (self->nz != 0)
				self->nz = (tmp & 128) + 1;
		}
		self->vdi = (vdi & 12) + ((~(data ^ a) & (a ^ tmp)) >> 1 & 64);
		if (tmp >= 160) {
			self->c = 1;
			self->a = (tmp - 160) & 255;
		}
		else {
			self->c = 0;
			self->a = tmp;
		}
	}
}

static void Cpu6502_SubtractWithCarry(Cpu6502 *self, int data)
{
	int a = self->a;
	int vdi = self->vdi;
	int borrow = self->c - 1;
	int tmp = a - data + borrow;
	int al = (a & 15) - (data & 15) + borrow;
	self->vdi = (vdi & 12) + (((data ^ a) & (a ^ tmp)) >> 1 & 64);
	self->c = tmp >= 0 ? 1 : 0;
	self->nz = self->a = tmp & 255;
	if ((vdi & 8) != 0) {
		if (al < 0)
			self->a += al < -10 ? 10 : -6;
		if (self->c == 0)
			self->a = (self->a - 96) & 255;
	}
}

static int Cpu6502_ArithmeticShiftLeft(Cpu6502 *self, int addr)
{
	int data = Cpu6502_PeekReadModifyWrite(self, addr);
	self->c = data >> 7;
	data = data << 1 & 255;
	Cpu6502_Poke(self, addr, data);
	return data;
}

static int Cpu6502_RotateLeft(Cpu6502 *self, int addr)
{
	int data = (Cpu6502_PeekReadModifyWrite(self, addr) << 1) + self->c;
	self->c = data >> 8;
	data &= 255;
	Cpu6502_Poke(self, addr, data);
	return data;
}

static int Cpu6502_LogicalShiftRight(Cpu6502 *self, int addr)
{
	int data = Cpu6502_PeekReadModifyWrite(self, addr);
	self->c = data & 1;
	data >>= 1;
	Cpu6502_Poke(self, addr, data);
	return data;
}

static int Cpu6502_RotateRight(Cpu6502 *self, int addr)
{
	int data = (self->c << 8) + Cpu6502_PeekReadModifyWrite(self, addr);
	self->c = data & 1;
	data >>= 1;
	Cpu6502_Poke(self, addr, data);
	return data;
}

static int Cpu6502_Decrement(Cpu6502 *self, int addr)
{
	int data = (Cpu6502_PeekReadModifyWrite(self, addr) - 1) & 255;
	Cpu6502_Poke(self, addr, data);
	return data;
}

static int Cpu6502_Increment(Cpu6502 *self, int addr)
{
	int data = (Cpu6502_PeekReadModifyWrite(self, addr) + 1) & 255;
	Cpu6502_Poke(self, addr, data);
	return data;
}

static void Cpu6502_ExecuteIrq(Cpu6502 *self, int b)
{
	Cpu6502_PushPc(self);
	Cpu6502_PushFlags(self, b);
	self->vdi |= 4;
	self->pc = self->memory[65534] + (self->memory[65535] << 8);
}

static void Cpu6502_CheckIrq(Cpu6502 *self)
{
	if ((self->vdi & 4) == 0 && ASAP_IsIrq(self->asap)) {
		self->cycle += 7;
		Cpu6502_ExecuteIrq(self, 32);
	}
}

static void Cpu6502_Shx(Cpu6502 *self, int addr, int data)
{
	addr += self->memory[self->pc++];
	int hi = self->memory[self->pc++];
	data &= hi + 1;
	if (addr >= 256)
		hi = data - 1;
	addr += hi << 8;
	Cpu6502_Poke(self, addr, data);
}

static void Cpu6502_DoFrame(Cpu6502 *self, int cycleLimit)
{
	while (self->cycle < cycleLimit) {
		if (self->cycle >= self->asap->nextEventCycle) {
			ASAP_HandleEvent(self->asap);
			Cpu6502_CheckIrq(self);
		}
		int data = self->memory[self->pc++];
		static const uint8_t OPCODE_CYCLES[256] = { 7, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
			2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
			6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
			2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
			6, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6,
			2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
			6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6,
			2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
			2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
			2, 6, 2, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5,
			2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
			2, 5, 2, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
			2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
			2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
			2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
			2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7 };
		self->cycle += OPCODE_CYCLES[data];
		int addr = 0;
		switch (data) {
		case 0:
			self->pc++;
			Cpu6502_ExecuteIrq(self, 48);
			continue;
		case 1:
		case 3:
		case 33:
		case 35:
		case 65:
		case 67:
		case 97:
		case 99:
		case 129:
		case 131:
		case 161:
		case 163:
		case 193:
		case 195:
		case 225:
		case 227:
			addr = (self->memory[self->pc++] + self->x) & 255;
			addr = self->memory[addr] + (self->memory[(addr + 1) & 255] << 8);
			break;
		case 2:
		case 18:
		case 34:
		case 50:
		case 66:
		case 82:
		case 98:
		case 114:
		case 146:
		case 178:
		case 210:
		case 242:
			self->pc--;
			self->cycle = self->asap->nextEventCycle;
			continue;
		case 4:
		case 68:
		case 100:
		case 20:
		case 52:
		case 84:
		case 116:
		case 212:
		case 244:
		case 128:
		case 130:
		case 137:
		case 194:
		case 226:
			self->pc++;
			continue;
		case 5:
		case 6:
		case 7:
		case 36:
		case 37:
		case 38:
		case 39:
		case 69:
		case 70:
		case 71:
		case 101:
		case 102:
		case 103:
		case 132:
		case 133:
		case 134:
		case 135:
		case 164:
		case 165:
		case 166:
		case 167:
		case 196:
		case 197:
		case 198:
		case 199:
		case 228:
		case 229:
		case 230:
		case 231:
			addr = self->memory[self->pc++];
			break;
		case 8:
			Cpu6502_PushFlags(self, 48);
			continue;
		case 9:
		case 41:
		case 73:
		case 105:
		case 160:
		case 162:
		case 169:
		case 192:
		case 201:
		case 224:
		case 233:
		case 235:
			addr = self->pc++;
			break;
		case 10:
			self->c = self->a >> 7;
			self->nz = self->a = self->a << 1 & 255;
			continue;
		case 11:
		case 43:
			self->nz = self->a &= self->memory[self->pc++];
			self->c = self->nz >> 7;
			continue;
		case 12:
			self->pc += 2;
			continue;
		case 13:
		case 14:
		case 15:
		case 44:
		case 45:
		case 46:
		case 47:
		case 77:
		case 78:
		case 79:
		case 108:
		case 109:
		case 110:
		case 111:
		case 140:
		case 141:
		case 142:
		case 143:
		case 172:
		case 173:
		case 174:
		case 175:
		case 204:
		case 205:
		case 206:
		case 207:
		case 236:
		case 237:
		case 238:
		case 239:
			addr = self->memory[self->pc++];
			addr += self->memory[self->pc++] << 8;
			break;
		case 16:
			if (self->nz < 128)
				break;
			self->pc++;
			continue;
		case 17:
		case 49:
		case 81:
		case 113:
		case 177:
		case 179:
		case 209:
		case 241:
			;
			int zp = self->memory[self->pc++];
			addr = self->memory[zp] + self->y;
			if (addr >= 256)
				self->cycle++;
			addr = (addr + (self->memory[(zp + 1) & 255] << 8)) & 65535;
			break;
		case 19:
		case 51:
		case 83:
		case 115:
		case 145:
		case 211:
		case 243:
			addr = self->memory[self->pc++];
			addr = (self->memory[addr] + (self->memory[(addr + 1) & 255] << 8) + self->y) & 65535;
			break;
		case 21:
		case 22:
		case 23:
		case 53:
		case 54:
		case 55:
		case 85:
		case 86:
		case 87:
		case 117:
		case 118:
		case 119:
		case 148:
		case 149:
		case 180:
		case 181:
		case 213:
		case 214:
		case 215:
		case 245:
		case 246:
		case 247:
			addr = (self->memory[self->pc++] + self->x) & 255;
			break;
		case 24:
			self->c = 0;
			continue;
		case 25:
		case 57:
		case 89:
		case 121:
		case 185:
		case 187:
		case 190:
		case 191:
		case 217:
		case 249:
			addr = self->memory[self->pc++] + self->y;
			if (addr >= 256)
				self->cycle++;
			addr = (addr + (self->memory[self->pc++] << 8)) & 65535;
			break;
		case 27:
		case 59:
		case 91:
		case 123:
		case 153:
		case 219:
		case 251:
			addr = self->memory[self->pc++] + self->y;
			addr = (addr + (self->memory[self->pc++] << 8)) & 65535;
			break;
		case 28:
		case 60:
		case 92:
		case 124:
		case 220:
		case 252:
			if (self->memory[self->pc] + self->x >= 256)
				self->cycle++;
			self->pc += 2;
			continue;
		case 29:
		case 61:
		case 93:
		case 125:
		case 188:
		case 189:
		case 221:
		case 253:
			addr = self->memory[self->pc++] + self->x;
			if (addr >= 256)
				self->cycle++;
			addr = (addr + (self->memory[self->pc++] << 8)) & 65535;
			break;
		case 30:
		case 31:
		case 62:
		case 63:
		case 94:
		case 95:
		case 126:
		case 127:
		case 157:
		case 222:
		case 223:
		case 254:
		case 255:
			addr = self->memory[self->pc++] + self->x;
			addr = (addr + (self->memory[self->pc++] << 8)) & 65535;
			break;
		case 32:
			addr = self->memory[self->pc++];
			Cpu6502_PushPc(self);
			self->pc = addr + (self->memory[self->pc] << 8);
			continue;
		case 40:
			Cpu6502_PullFlags(self);
			Cpu6502_CheckIrq(self);
			continue;
		case 42:
			self->a = (self->a << 1) + self->c;
			self->c = self->a >> 8;
			self->nz = self->a &= 255;
			continue;
		case 48:
			if (self->nz >= 128)
				break;
			self->pc++;
			continue;
		case 56:
			self->c = 1;
			continue;
		case 64:
			Cpu6502_PullFlags(self);
			self->pc = Cpu6502_Pull(self);
			self->pc += Cpu6502_Pull(self) << 8;
			Cpu6502_CheckIrq(self);
			continue;
		case 72:
			Cpu6502_Push(self, self->a);
			continue;
		case 74:
			self->c = self->a & 1;
			self->nz = self->a >>= 1;
			continue;
		case 75:
			self->a &= self->memory[self->pc++];
			self->c = self->a & 1;
			self->nz = self->a >>= 1;
			continue;
		case 76:
			addr = self->memory[self->pc++];
			self->pc = addr + (self->memory[self->pc] << 8);
			continue;
		case 80:
			if ((self->vdi & 64) == 0)
				break;
			self->pc++;
			continue;
		case 88:
			self->vdi &= 72;
			Cpu6502_CheckIrq(self);
			continue;
		case 96:
			self->pc = Cpu6502_Pull(self);
			self->pc += (Cpu6502_Pull(self) << 8) + 1;
			continue;
		case 104:
			self->nz = self->a = Cpu6502_Pull(self);
			continue;
		case 106:
			self->nz = (self->c << 7) + (self->a >> 1);
			self->c = self->a & 1;
			self->a = self->nz;
			continue;
		case 107:
			data = self->a & self->memory[self->pc++];
			self->nz = self->a = (data >> 1) + (self->c << 7);
			self->vdi = (self->vdi & 12) + ((self->a ^ data) & 64);
			if ((self->vdi & 8) == 0)
				self->c = data >> 7;
			else {
				if ((data & 15) >= 5)
					self->a = (self->a & 240) + ((self->a + 6) & 15);
				if (data >= 80) {
					self->a = (self->a + 96) & 255;
					self->c = 1;
				}
				else
					self->c = 0;
			}
			continue;
		case 112:
			if ((self->vdi & 64) != 0)
				break;
			self->pc++;
			continue;
		case 120:
			self->vdi |= 4;
			continue;
		case 136:
			self->nz = self->y = (self->y - 1) & 255;
			continue;
		case 138:
			self->nz = self->a = self->x;
			continue;
		case 139:
			data = self->memory[self->pc++];
			self->a &= (data | 239) & self->x;
			self->nz = self->a & data;
			continue;
		case 144:
			if (self->c == 0)
				break;
			self->pc++;
			continue;
		case 147:
			{
				addr = self->memory[self->pc++];
				int hi = self->memory[(addr + 1) & 255];
				addr = self->memory[addr];
				data = (hi + 1) & self->a & self->x;
				addr += self->y;
				if (addr >= 256)
					hi = data - 1;
				addr += hi << 8;
				Cpu6502_Poke(self, addr, data);
			}
			continue;
		case 150:
		case 151:
		case 182:
		case 183:
			addr = (self->memory[self->pc++] + self->y) & 255;
			break;
		case 152:
			self->nz = self->a = self->y;
			continue;
		case 154:
			self->s = self->x;
			continue;
		case 155:
			self->s = self->a & self->x;
			Cpu6502_Shx(self, self->y, self->s);
			continue;
		case 156:
			Cpu6502_Shx(self, self->x, self->y);
			continue;
		case 158:
			Cpu6502_Shx(self, self->y, self->x);
			continue;
		case 159:
			Cpu6502_Shx(self, self->y, self->a & self->x);
			continue;
		case 168:
			self->nz = self->y = self->a;
			continue;
		case 170:
			self->nz = self->x = self->a;
			continue;
		case 171:
			self->nz = self->x = self->a &= self->memory[self->pc++];
			continue;
		case 176:
			if (self->c != 0)
				break;
			self->pc++;
			continue;
		case 184:
			self->vdi &= 12;
			continue;
		case 186:
			self->nz = self->x = self->s;
			continue;
		case 200:
			self->nz = self->y = (self->y + 1) & 255;
			continue;
		case 202:
			self->nz = self->x = (self->x - 1) & 255;
			continue;
		case 203:
			self->nz = self->memory[self->pc++];
			self->x &= self->a;
			self->c = self->x >= self->nz ? 1 : 0;
			self->nz = self->x = (self->x - self->nz) & 255;
			continue;
		case 208:
			if ((self->nz & 255) != 0)
				break;
			self->pc++;
			continue;
		case 216:
			self->vdi &= 68;
			continue;
		case 232:
			self->nz = self->x = (self->x + 1) & 255;
			continue;
		case 234:
		case 26:
		case 58:
		case 90:
		case 122:
		case 218:
		case 250:
			continue;
		case 240:
			if ((self->nz & 255) == 0)
				break;
			self->pc++;
			continue;
		case 248:
			self->vdi |= 8;
			continue;
		default:
			continue;
		}
		switch (data) {
		case 1:
		case 5:
		case 9:
		case 13:
		case 17:
		case 21:
		case 25:
		case 29:
			self->nz = self->a |= Cpu6502_Peek(self, addr);
			break;
		case 3:
		case 7:
		case 15:
		case 19:
		case 23:
		case 27:
		case 31:
			self->nz = self->a |= Cpu6502_ArithmeticShiftLeft(self, addr);
			break;
		case 6:
		case 14:
		case 22:
		case 30:
			self->nz = Cpu6502_ArithmeticShiftLeft(self, addr);
			break;
		case 16:
		case 48:
		case 80:
		case 112:
		case 144:
		case 176:
		case 208:
		case 240:
			addr = (self->memory[self->pc] ^ 128) - 128;
			self->pc++;
			addr += self->pc;
			self->cycle += (addr ^ self->pc) >> 8 != 0 ? 2 : 1;
			self->pc = addr;
			break;
		case 33:
		case 37:
		case 41:
		case 45:
		case 49:
		case 53:
		case 57:
		case 61:
			self->nz = self->a &= Cpu6502_Peek(self, addr);
			break;
		case 35:
		case 39:
		case 47:
		case 51:
		case 55:
		case 59:
		case 63:
			self->nz = self->a &= Cpu6502_RotateLeft(self, addr);
			break;
		case 36:
		case 44:
			self->nz = Cpu6502_Peek(self, addr);
			self->vdi = (self->vdi & 12) + (self->nz & 64);
			self->nz = ((self->nz & 128) << 1) + (self->nz & self->a);
			break;
		case 38:
		case 46:
		case 54:
		case 62:
			self->nz = Cpu6502_RotateLeft(self, addr);
			break;
		case 65:
		case 69:
		case 73:
		case 77:
		case 81:
		case 85:
		case 89:
		case 93:
			self->nz = self->a ^= Cpu6502_Peek(self, addr);
			break;
		case 67:
		case 71:
		case 79:
		case 83:
		case 87:
		case 91:
		case 95:
			self->nz = self->a ^= Cpu6502_LogicalShiftRight(self, addr);
			break;
		case 70:
		case 78:
		case 86:
		case 94:
			self->nz = Cpu6502_LogicalShiftRight(self, addr);
			break;
		case 97:
		case 101:
		case 105:
		case 109:
		case 113:
		case 117:
		case 121:
		case 125:
			Cpu6502_AddWithCarry(self, Cpu6502_Peek(self, addr));
			break;
		case 99:
		case 103:
		case 111:
		case 115:
		case 119:
		case 123:
		case 127:
			Cpu6502_AddWithCarry(self, Cpu6502_RotateRight(self, addr));
			break;
		case 102:
		case 110:
		case 118:
		case 126:
			self->nz = Cpu6502_RotateRight(self, addr);
			break;
		case 108:
			self->pc = self->memory[addr];
			if ((++addr & 255) == 0)
				addr -= 255;
			self->pc += self->memory[addr] << 8;
			break;
		case 129:
		case 133:
		case 141:
		case 145:
		case 149:
		case 153:
		case 157:
			Cpu6502_Poke(self, addr, self->a);
			break;
		case 131:
		case 135:
		case 143:
		case 151:
			Cpu6502_Poke(self, addr, self->a & self->x);
			break;
		case 132:
		case 140:
		case 148:
			Cpu6502_Poke(self, addr, self->y);
			break;
		case 134:
		case 142:
		case 150:
			Cpu6502_Poke(self, addr, self->x);
			break;
		case 160:
		case 164:
		case 172:
		case 180:
		case 188:
			self->nz = self->y = Cpu6502_Peek(self, addr);
			break;
		case 161:
		case 165:
		case 169:
		case 173:
		case 177:
		case 181:
		case 185:
		case 189:
			self->nz = self->a = Cpu6502_Peek(self, addr);
			break;
		case 162:
		case 166:
		case 174:
		case 182:
		case 190:
			self->nz = self->x = Cpu6502_Peek(self, addr);
			break;
		case 163:
		case 167:
		case 175:
		case 179:
		case 183:
		case 191:
			self->nz = self->x = self->a = Cpu6502_Peek(self, addr);
			break;
		case 187:
			self->nz = self->x = self->a = self->s &= Cpu6502_Peek(self, addr);
			break;
		case 192:
		case 196:
		case 204:
			self->nz = Cpu6502_Peek(self, addr);
			self->c = self->y >= self->nz ? 1 : 0;
			self->nz = (self->y - self->nz) & 255;
			break;
		case 193:
		case 197:
		case 201:
		case 205:
		case 209:
		case 213:
		case 217:
		case 221:
			self->nz = Cpu6502_Peek(self, addr);
			self->c = self->a >= self->nz ? 1 : 0;
			self->nz = (self->a - self->nz) & 255;
			break;
		case 195:
		case 199:
		case 207:
		case 211:
		case 215:
		case 219:
		case 223:
			data = Cpu6502_Decrement(self, addr);
			self->c = self->a >= data ? 1 : 0;
			self->nz = (self->a - data) & 255;
			break;
		case 198:
		case 206:
		case 214:
		case 222:
			self->nz = Cpu6502_Decrement(self, addr);
			break;
		case 224:
		case 228:
		case 236:
			self->nz = Cpu6502_Peek(self, addr);
			self->c = self->x >= self->nz ? 1 : 0;
			self->nz = (self->x - self->nz) & 255;
			break;
		case 225:
		case 229:
		case 233:
		case 235:
		case 237:
		case 241:
		case 245:
		case 249:
		case 253:
			Cpu6502_SubtractWithCarry(self, Cpu6502_Peek(self, addr));
			break;
		case 227:
		case 231:
		case 239:
		case 243:
		case 247:
		case 251:
		case 255:
			Cpu6502_SubtractWithCarry(self, Cpu6502_Increment(self, addr));
			break;
		case 230:
		case 238:
		case 246:
		case 254:
			self->nz = Cpu6502_Increment(self, addr);
			break;
		}
	}
}

static int FlashPackItem_WriteValueTo(const FlashPackItem *self, uint8_t *buffer, int index)
{
	switch (self->type) {
	case FlashPackItemType_LITERAL:
		buffer[index] = (uint8_t) self->value;
		return 1;
	case FlashPackItemType_COPY_TWO_BYTES:
		buffer[index] = (uint8_t) ((128 - self->value) << 1);
		return 1;
	case FlashPackItemType_COPY_THREE_BYTES:
		buffer[index] = (uint8_t) (((128 - self->value) << 1) + 1);
		return 1;
	case FlashPackItemType_COPY_MANY_BYTES:
		buffer[index] = 1;
		buffer[index + 1] = (uint8_t) self->value;
		return 2;
	case FlashPackItemType_SET_ADDRESS:
		;
		int value = self->value - 128;
		buffer[index] = 0;
		buffer[index + 1] = (uint8_t) (value & 255);
		buffer[index + 2] = (uint8_t) (value >> 8);
		return 3;
	default:
		buffer[index] = 1;
		buffer[index + 1] = 0;
		return 2;
	}
}

static int FlashPack_FindHole(const FlashPack *self)
{
	int end = 48159;
	for (;;) {
		while (self->memory[end] >= 0)
			if (--end < 9216)
				return -1;
		int start = end;
		while (self->memory[--start] < 0)
			if (end - start >= 1023)
				return end;
		end = start;
	}
}

static int FlashPack_GetInnerFlags(const FlashPack *self, int index)
{
	int flags = 1;
	do {
		flags <<= 1;
		if (index < self->itemsCount && self->items[index++].type != FlashPackItemType_LITERAL)
			flags++;
	}
	while (flags < 256);
	return flags & 255;
}

static void FlashPack_PutItems(FlashPack *self)
{
	int outerFlags = 0;
	for (int i = 0; i < self->itemsCount; i += 8) {
		if (FlashPack_GetInnerFlags(self, i) != 0)
			outerFlags |= 128 >> (i >> 3);
	}
	self->compressed[self->compressedLength++] = (uint8_t) outerFlags;
	for (int i = 0; i < self->itemsCount; i++) {
		if ((i & 7) == 0) {
			int flags = FlashPack_GetInnerFlags(self, i);
			if (flags != 0)
				self->compressed[self->compressedLength++] = (uint8_t) flags;
		}
		self->compressedLength += FlashPackItem_WriteValueTo(&self->items[i], self->compressed, self->compressedLength);
	}
}

static void FlashPack_PutItem(FlashPack *self, FlashPackItemType type, int value)
{
	if (self->itemsCount >= 64) {
		FlashPack_PutItems(self);
		self->itemsCount = 0;
	}
	self->items[self->itemsCount].type = type;
	self->items[self->itemsCount].value = value;
	self->itemsCount++;
}

static bool FlashPack_IsLiteralPreferred(const FlashPack *self)
{
	return (self->itemsCount & 7) == 7 && FlashPack_GetInnerFlags(self, self->itemsCount - 7) == 0;
}

static void FlashPack_CompressMemoryArea(FlashPack *self, int startAddress, int endAddress)
{
	int lastDistance = -1;
	for (int address = startAddress; address <= endAddress;) {
		while (self->memory[address] < 0)
			if (++address > endAddress)
				return;
		FlashPack_PutItem(self, FlashPackItemType_SET_ADDRESS, address);
		while (address <= endAddress && self->memory[address] >= 0) {
			int bestMatch = 0;
			int bestDistance = -1;
			for (int backAddress = address - 1; backAddress >= startAddress && address - backAddress < 128; backAddress--) {
				int match;
				for (match = 0; address + match <= endAddress; match++) {
					int data = self->memory[address + match];
					if (data < 0 || data != self->memory[backAddress + match])
						break;
				}
				if (bestMatch < match) {
					bestMatch = match;
					bestDistance = address - backAddress;
				}
				else if (bestMatch == match && address - backAddress == lastDistance)
					bestDistance = lastDistance;
			}
			switch (bestMatch) {
			case 0:
			case 1:
				FlashPack_PutItem(self, FlashPackItemType_LITERAL, self->memory[address++]);
				continue;
			case 2:
				FlashPack_PutItem(self, FlashPackItemType_COPY_TWO_BYTES, bestDistance);
				break;
			case 3:
				FlashPack_PutItem(self, FlashPackItemType_COPY_THREE_BYTES, bestDistance);
				break;
			case 4:
				if (bestDistance == lastDistance)
					FlashPack_PutItem(self, FlashPackItemType_COPY_MANY_BYTES, 4);
				else if (FlashPack_IsLiteralPreferred(self)) {
					FlashPack_PutItem(self, FlashPackItemType_LITERAL, self->memory[address]);
					FlashPack_PutItem(self, FlashPackItemType_COPY_THREE_BYTES, bestDistance);
				}
				else {
					FlashPack_PutItem(self, FlashPackItemType_COPY_THREE_BYTES, bestDistance);
					FlashPack_PutItem(self, FlashPackItemType_LITERAL, self->memory[address + 3]);
				}
				break;
			case 5:
				if (bestDistance == lastDistance)
					FlashPack_PutItem(self, FlashPackItemType_COPY_MANY_BYTES, 5);
				else {
					FlashPack_PutItem(self, FlashPackItemType_COPY_THREE_BYTES, bestDistance);
					FlashPack_PutItem(self, FlashPackItemType_COPY_TWO_BYTES, bestDistance);
				}
				break;
			case 6:
				if (bestDistance == lastDistance)
					FlashPack_PutItem(self, FlashPackItemType_COPY_MANY_BYTES, 6);
				else {
					FlashPack_PutItem(self, FlashPackItemType_COPY_THREE_BYTES, bestDistance);
					FlashPack_PutItem(self, FlashPackItemType_COPY_THREE_BYTES, bestDistance);
				}
				break;
			default:
				;
				int length = bestMatch;
				if (bestDistance != lastDistance) {
					if (FlashPack_IsLiteralPreferred(self) && length % 255 == 4) {
						FlashPack_PutItem(self, FlashPackItemType_LITERAL, self->memory[address]);
						length--;
					}
					FlashPack_PutItem(self, FlashPackItemType_COPY_THREE_BYTES, bestDistance);
					length -= 3;
				}
				else if (FlashPack_IsLiteralPreferred(self) && length % 255 == 1) {
					FlashPack_PutItem(self, FlashPackItemType_LITERAL, self->memory[address]);
					length--;
				}
				for (; length > 255; length -= 255)
					FlashPack_PutItem(self, FlashPackItemType_COPY_MANY_BYTES, 255);
				switch (length) {
				case 0:
					break;
				case 1:
					FlashPack_PutItem(self, FlashPackItemType_LITERAL, self->memory[address + bestMatch - 1]);
					break;
				case 2:
					FlashPack_PutItem(self, FlashPackItemType_COPY_TWO_BYTES, bestDistance);
					break;
				case 3:
					FlashPack_PutItem(self, FlashPackItemType_COPY_THREE_BYTES, bestDistance);
					break;
				default:
					FlashPack_PutItem(self, FlashPackItemType_COPY_MANY_BYTES, length);
					break;
				}
				break;
			}
			address += bestMatch;
			lastDistance = bestDistance;
		}
	}
}

static void FlashPack_PutPoke(FlashPack *self, int address, int value)
{
	FlashPack_PutItem(self, FlashPackItemType_SET_ADDRESS, address);
	FlashPack_PutItem(self, FlashPackItemType_LITERAL, value);
}

static bool FlashPack_Compress(FlashPack *self, ASAPWriter *w)
{
	for (int i = 0; i < 65536; i++)
		self->memory[i] = -1;
	for (int i = 0; i + 5 <= w->outputOffset;) {
		int startAddress = w->output[i] + (w->output[i + 1] << 8);
		if (startAddress == 65535) {
			i += 2;
			startAddress = w->output[i] + (w->output[i + 1] << 8);
		}
		int endAddress = w->output[i + 2] + (w->output[i + 3] << 8);
		if (startAddress > endAddress)
			return false;
		i += 4;
		if (i + endAddress - startAddress >= w->outputOffset)
			return false;
		while (startAddress <= endAddress)
			self->memory[startAddress++] = w->output[i++];
	}
	if (self->memory[736] < 0 || self->memory[737] < 0)
		return false;
	if (self->memory[252] >= 0 || self->memory[253] >= 0 || self->memory[254] >= 0 || self->memory[255] >= 0)
		return false;
	int runAddress = self->memory[736] + (self->memory[737] << 8);
	self->memory[736] = self->memory[737] = -1;
	int depackerEndAddress = FlashPack_FindHole(self);
	if (depackerEndAddress == -1)
		return false;
	self->compressedLength = 0;
	self->itemsCount = 0;
	FlashPack_PutPoke(self, 54286, 0);
	FlashPack_PutPoke(self, 53774, 0);
	FlashPack_PutPoke(self, 54272, 0);
	FlashPack_PutPoke(self, 54017, 254);
	FlashPack_PutPoke(self, 580, 255);
	FlashPack_CompressMemoryArea(self, depackerEndAddress, 65535);
	FlashPack_CompressMemoryArea(self, 0, depackerEndAddress);
	FlashPack_PutItem(self, FlashPackItemType_END_OF_STREAM, 0);
	FlashPack_PutItems(self);
	int depackerStartAddress = depackerEndAddress - 87;
	int compressedStartAddress = depackerStartAddress - self->compressedLength;
	if (compressedStartAddress < 8192)
		return false;
	w->outputOffset = 0;
	if (!ASAPWriter_WriteWord(w, 65535))
		return false;
	if (!ASAPWriter_WriteWord(w, 54017))
		return false;
	if (!ASAPWriter_WriteWord(w, 54017))
		return false;
	if (!ASAPWriter_WriteByte(w, 255))
		return false;
	if (!ASAPWriter_WriteWord(w, compressedStartAddress))
		return false;
	if (!ASAPWriter_WriteWord(w, depackerEndAddress))
		return false;
	if (!ASAPWriter_WriteBytes(w, self->compressed, 0, self->compressedLength))
		return false;
	if (!ASAPWriter_WriteByte(w, 173))
		return false;
	if (!ASAPWriter_WriteWord(w, compressedStartAddress))
		return false;
	if (!ASAPWriter_WriteByte(w, 238))
		return false;
	if (!ASAPWriter_WriteWord(w, depackerStartAddress + 1))
		return false;
	if (!ASAPWriter_WriteByte(w, 208))
		return false;
	if (!ASAPWriter_WriteByte(w, 3))
		return false;
	if (!ASAPWriter_WriteByte(w, 238))
		return false;
	if (!ASAPWriter_WriteWord(w, depackerStartAddress + 2))
		return false;
	if (!ASAPWriter_WriteByte(w, 96))
		return false;
	if (!ASAPWriter_WriteByte(w, 76))
		return false;
	if (!ASAPWriter_WriteWord(w, runAddress))
		return false;
	if (!ASAPWriter_WriteByte(w, 133))
		return false;
	if (!ASAPWriter_WriteByte(w, 254))
		return false;
	if (!ASAPWriter_WriteByte(w, 138))
		return false;
	if (!ASAPWriter_WriteByte(w, 42))
		return false;
	if (!ASAPWriter_WriteByte(w, 170))
		return false;
	if (!ASAPWriter_WriteByte(w, 240))
		return false;
	if (!ASAPWriter_WriteByte(w, 246))
		return false;
	if (!ASAPWriter_WriteByte(w, 177))
		return false;
	if (!ASAPWriter_WriteByte(w, 254))
		return false;
	if (!ASAPWriter_WriteByte(w, 153))
		return false;
	if (!ASAPWriter_WriteByte(w, 128))
		return false;
	if (!ASAPWriter_WriteByte(w, 128))
		return false;
	if (!ASAPWriter_WriteByte(w, 200))
		return false;
	if (!ASAPWriter_WriteByte(w, 208))
		return false;
	if (!ASAPWriter_WriteByte(w, 9))
		return false;
	if (!ASAPWriter_WriteByte(w, 152))
		return false;
	if (!ASAPWriter_WriteByte(w, 56))
		return false;
	if (!ASAPWriter_WriteByte(w, 101))
		return false;
	if (!ASAPWriter_WriteByte(w, 255))
		return false;
	if (!ASAPWriter_WriteByte(w, 133))
		return false;
	if (!ASAPWriter_WriteByte(w, 255))
		return false;
	if (!ASAPWriter_WriteByte(w, 141))
		return false;
	if (!ASAPWriter_WriteWord(w, depackerStartAddress + 26))
		return false;
	if (!ASAPWriter_WriteByte(w, 202))
		return false;
	if (!ASAPWriter_WriteByte(w, 208))
		return false;
	if (!ASAPWriter_WriteByte(w, 236))
		return false;
	if (!ASAPWriter_WriteByte(w, 6))
		return false;
	if (!ASAPWriter_WriteByte(w, 253))
		return false;
	if (!ASAPWriter_WriteByte(w, 208))
		return false;
	if (!ASAPWriter_WriteByte(w, 21))
		return false;
	if (!ASAPWriter_WriteByte(w, 6))
		return false;
	if (!ASAPWriter_WriteByte(w, 252))
		return false;
	if (!ASAPWriter_WriteByte(w, 208))
		return false;
	if (!ASAPWriter_WriteByte(w, 7))
		return false;
	if (!ASAPWriter_WriteByte(w, 56))
		return false;
	if (!ASAPWriter_WriteByte(w, 32))
		return false;
	if (!ASAPWriter_WriteWord(w, depackerStartAddress))
		return false;
	if (!ASAPWriter_WriteByte(w, 42))
		return false;
	if (!ASAPWriter_WriteByte(w, 133))
		return false;
	if (!ASAPWriter_WriteByte(w, 252))
		return false;
	if (!ASAPWriter_WriteByte(w, 169))
		return false;
	if (!ASAPWriter_WriteByte(w, 1))
		return false;
	if (!ASAPWriter_WriteByte(w, 144))
		return false;
	if (!ASAPWriter_WriteByte(w, 4))
		return false;
	if (!ASAPWriter_WriteByte(w, 32))
		return false;
	if (!ASAPWriter_WriteWord(w, depackerStartAddress))
		return false;
	if (!ASAPWriter_WriteByte(w, 42))
		return false;
	if (!ASAPWriter_WriteByte(w, 133))
		return false;
	if (!ASAPWriter_WriteByte(w, 253))
		return false;
	if (!ASAPWriter_WriteByte(w, 32))
		return false;
	if (!ASAPWriter_WriteWord(w, depackerStartAddress))
		return false;
	if (!ASAPWriter_WriteByte(w, 162))
		return false;
	if (!ASAPWriter_WriteByte(w, 1))
		return false;
	if (!ASAPWriter_WriteByte(w, 144))
		return false;
	if (!ASAPWriter_WriteByte(w, 206))
		return false;
	if (!ASAPWriter_WriteByte(w, 74))
		return false;
	if (!ASAPWriter_WriteByte(w, 208))
		return false;
	if (!ASAPWriter_WriteByte(w, 194))
		return false;
	if (!ASAPWriter_WriteByte(w, 32))
		return false;
	if (!ASAPWriter_WriteWord(w, depackerStartAddress))
		return false;
	if (!ASAPWriter_WriteByte(w, 176))
		return false;
	if (!ASAPWriter_WriteByte(w, 193))
		return false;
	if (!ASAPWriter_WriteByte(w, 168))
		return false;
	if (!ASAPWriter_WriteByte(w, 32))
		return false;
	if (!ASAPWriter_WriteWord(w, depackerStartAddress))
		return false;
	if (!ASAPWriter_WriteByte(w, 144))
		return false;
	if (!ASAPWriter_WriteByte(w, 202))
		return false;
	if (!ASAPWriter_WriteWord(w, 736))
		return false;
	if (!ASAPWriter_WriteWord(w, 737))
		return false;
	return ASAPWriter_WriteWord(w, depackerStartAddress + 50);
}

static void PokeyChannel_Initialize(PokeyChannel *self)
{
	self->audf = 0;
	self->audc = 0;
	self->periodCycles = 28;
	self->tickCycle = 8388608;
	self->timerCycle = 8388608;
	self->mute = 1;
	self->out = 0;
	self->delta = 0;
}

static void PokeyChannel_Slope(PokeyChannel *self, Pokey *pokey, const PokeyPair *pokeys, int cycle)
{
	self->delta = -self->delta;
	Pokey_AddDelta(pokey, pokeys, cycle, self->delta);
}

static void PokeyChannel_DoTick(PokeyChannel *self, Pokey *pokey, const PokeyPair *pokeys, int cycle, int ch)
{
	self->tickCycle += self->periodCycles;
	int audc = self->audc;
	if ((audc & 176) == 160)
		self->out ^= 1;
	else if ((audc & 16) != 0 || pokey->init)
		return;
	else {
		int poly = cycle + pokey->polyIndex - ch;
		if (audc < 128 && (1706902752 & 1 << poly % 31) == 0)
			return;
		if ((audc & 32) != 0)
			self->out ^= 1;
		else {
			int newOut;
			if ((audc & 64) != 0)
				newOut = 21360 >> poly % 15;
			else if (pokey->audctl < 128) {
				poly %= 131071;
				newOut = pokeys->poly17Lookup[poly >> 3] >> (poly & 7);
			}
			else
				newOut = pokeys->poly9Lookup[poly % 511];
			newOut &= 1;
			if (self->out == newOut)
				return;
			self->out = newOut;
		}
	}
	PokeyChannel_Slope(self, pokey, pokeys, cycle);
}

static void PokeyChannel_DoStimer(PokeyChannel *self, int cycle)
{
	if (self->tickCycle != 8388608)
		self->tickCycle = cycle + self->periodCycles;
}

static void PokeyChannel_SetMute(PokeyChannel *self, bool enable, int mask, int cycle)
{
	if (enable) {
		self->mute |= mask;
		self->tickCycle = 8388608;
	}
	else {
		self->mute &= ~mask;
		if (self->mute == 0 && self->tickCycle == 8388608)
			self->tickCycle = cycle;
	}
}

static void PokeyChannel_MuteUltrasound(PokeyChannel *self, int cycle)
{
	PokeyChannel_SetMute(self, self->periodCycles <= 112 && (self->audc & 176) == 160, 1, cycle);
}

static void PokeyChannel_SetAudc(PokeyChannel *self, Pokey *pokey, const PokeyPair *pokeys, int data, int cycle)
{
	if (self->audc == data)
		return;
	Pokey_GenerateUntilCycle(pokey, pokeys, cycle);
	self->audc = data;
	if ((data & 16) != 0) {
		data = (data & 15) << 20;
		if ((self->mute & 4) == 0)
			Pokey_AddDelta(pokey, pokeys, cycle, self->delta > 0 ? data - self->delta : data);
		self->delta = data;
	}
	else {
		data = (data & 15) << 20;
		PokeyChannel_MuteUltrasound(self, cycle);
		if (self->delta > 0) {
			if ((self->mute & 4) == 0)
				Pokey_AddDelta(pokey, pokeys, cycle, data - self->delta);
			self->delta = data;
		}
		else
			self->delta = -data;
	}
}

static void PokeyChannel_EndFrame(PokeyChannel *self, int cycle)
{
	if (self->timerCycle != 8388608)
		self->timerCycle -= cycle;
}

static void Pokey_StartFrame(Pokey *self)
{
	memset(self->deltaBuffer, 0, sizeof(self->deltaBuffer));
}

static void Pokey_Initialize(Pokey *self)
{
	for (int i = 0; i < 4; i++)
		PokeyChannel_Initialize(&self->channels[i]);
	self->audctl = 0;
	self->skctl = 3;
	self->irqst = 255;
	self->init = false;
	self->divCycles = 28;
	self->reloadCycles1 = 28;
	self->reloadCycles3 = 28;
	self->polyIndex = 60948015;
	self->iirAcc = 0;
	Pokey_StartFrame(self);
}

static void Pokey_AddDelta(Pokey *self, const PokeyPair *pokeys, int cycle, int delta)
{
	int i = cycle * pokeys->sampleFactor + pokeys->sampleOffset;
	int delta2 = (delta >> 16) * (i >> 4 & 65535);
	i >>= 20;
	self->deltaBuffer[i] += delta - delta2;
	self->deltaBuffer[i + 1] += delta2;
}

static void Pokey_GenerateUntilCycle(Pokey *self, const PokeyPair *pokeys, int cycleLimit)
{
	for (;;) {
		int cycle = cycleLimit;
		for (int i = 0; i < 4; i++) {
			int tickCycle = self->channels[i].tickCycle;
			if (cycle > tickCycle)
				cycle = tickCycle;
		}
		if (cycle == cycleLimit)
			break;
		if (cycle == self->channels[2].tickCycle) {
			if ((self->audctl & 4) != 0 && self->channels[0].delta > 0 && self->channels[0].mute == 0)
				PokeyChannel_Slope(&self->channels[0], self, pokeys, cycle);
			PokeyChannel_DoTick(&self->channels[2], self, pokeys, cycle, 2);
		}
		if (cycle == self->channels[3].tickCycle) {
			if ((self->audctl & 8) != 0)
				self->channels[2].tickCycle = cycle + self->reloadCycles3;
			if ((self->audctl & 2) != 0 && self->channels[1].delta > 0 && self->channels[1].mute == 0)
				PokeyChannel_Slope(&self->channels[1], self, pokeys, cycle);
			PokeyChannel_DoTick(&self->channels[3], self, pokeys, cycle, 3);
		}
		if (cycle == self->channels[0].tickCycle) {
			if ((self->skctl & 136) == 8)
				self->channels[1].tickCycle = cycle + self->channels[1].periodCycles;
			PokeyChannel_DoTick(&self->channels[0], self, pokeys, cycle, 0);
		}
		if (cycle == self->channels[1].tickCycle) {
			if ((self->audctl & 16) != 0)
				self->channels[0].tickCycle = cycle + self->reloadCycles1;
			else if ((self->skctl & 8) != 0)
				self->channels[0].tickCycle = cycle + self->channels[0].periodCycles;
			PokeyChannel_DoTick(&self->channels[1], self, pokeys, cycle, 1);
		}
	}
}

static void Pokey_EndFrame(Pokey *self, const PokeyPair *pokeys, int cycle)
{
	Pokey_GenerateUntilCycle(self, pokeys, cycle);
	self->polyIndex += cycle;
	int m = (self->audctl & 128) != 0 ? 237615 : 60948015;
	if (self->polyIndex >= 2 * m)
		self->polyIndex -= m;
	for (int i = 0; i < 4; i++) {
		int tickCycle = self->channels[i].tickCycle;
		if (tickCycle != 8388608)
			self->channels[i].tickCycle = tickCycle - cycle;
	}
}

static bool Pokey_IsSilent(const Pokey *self)
{
	for (int i = 0; i < 4; i++)
		if ((self->channels[i].audc & 15) != 0)
			return false;
	return true;
}

static void Pokey_Mute(Pokey *self, int mask)
{
	for (int i = 0; i < 4; i++)
		PokeyChannel_SetMute(&self->channels[i], (mask & 1 << i) != 0, 4, 0);
}

static void Pokey_InitMute(Pokey *self, int cycle)
{
	bool init = self->init;
	int audctl = self->audctl;
	PokeyChannel_SetMute(&self->channels[0], init && (audctl & 64) == 0, 2, cycle);
	PokeyChannel_SetMute(&self->channels[1], init && (audctl & 80) != 80, 2, cycle);
	PokeyChannel_SetMute(&self->channels[2], init && (audctl & 32) == 0, 2, cycle);
	PokeyChannel_SetMute(&self->channels[3], init && (audctl & 40) != 40, 2, cycle);
}

static int Pokey_Poke(Pokey *self, const PokeyPair *pokeys, int addr, int data, int cycle)
{
	int nextEventCycle = 8388608;
	switch (addr & 15) {
	case 0:
		if (data == self->channels[0].audf)
			break;
		Pokey_GenerateUntilCycle(self, pokeys, cycle);
		self->channels[0].audf = data;
		switch (self->audctl & 80) {
		case 0:
			self->channels[0].periodCycles = self->divCycles * (data + 1);
			break;
		case 16:
			self->channels[1].periodCycles = self->divCycles * (data + (self->channels[1].audf << 8) + 1);
			self->reloadCycles1 = self->divCycles * (data + 1);
			PokeyChannel_MuteUltrasound(&self->channels[1], cycle);
			break;
		case 64:
			self->channels[0].periodCycles = data + 4;
			break;
		case 80:
			self->channels[1].periodCycles = data + (self->channels[1].audf << 8) + 7;
			self->reloadCycles1 = data + 4;
			PokeyChannel_MuteUltrasound(&self->channels[1], cycle);
			break;
		}
		PokeyChannel_MuteUltrasound(&self->channels[0], cycle);
		break;
	case 1:
		PokeyChannel_SetAudc(&self->channels[0], self, pokeys, data, cycle);
		break;
	case 2:
		if (data == self->channels[1].audf)
			break;
		Pokey_GenerateUntilCycle(self, pokeys, cycle);
		self->channels[1].audf = data;
		switch (self->audctl & 80) {
		case 0:
		case 64:
			self->channels[1].periodCycles = self->divCycles * (data + 1);
			break;
		case 16:
			self->channels[1].periodCycles = self->divCycles * (self->channels[0].audf + (data << 8) + 1);
			break;
		case 80:
			self->channels[1].periodCycles = self->channels[0].audf + (data << 8) + 7;
			break;
		}
		PokeyChannel_MuteUltrasound(&self->channels[1], cycle);
		break;
	case 3:
		PokeyChannel_SetAudc(&self->channels[1], self, pokeys, data, cycle);
		break;
	case 4:
		if (data == self->channels[2].audf)
			break;
		Pokey_GenerateUntilCycle(self, pokeys, cycle);
		self->channels[2].audf = data;
		switch (self->audctl & 40) {
		case 0:
			self->channels[2].periodCycles = self->divCycles * (data + 1);
			break;
		case 8:
			self->channels[3].periodCycles = self->divCycles * (data + (self->channels[3].audf << 8) + 1);
			self->reloadCycles3 = self->divCycles * (data + 1);
			PokeyChannel_MuteUltrasound(&self->channels[3], cycle);
			break;
		case 32:
			self->channels[2].periodCycles = data + 4;
			break;
		case 40:
			self->channels[3].periodCycles = data + (self->channels[3].audf << 8) + 7;
			self->reloadCycles3 = data + 4;
			PokeyChannel_MuteUltrasound(&self->channels[3], cycle);
			break;
		}
		PokeyChannel_MuteUltrasound(&self->channels[2], cycle);
		break;
	case 5:
		PokeyChannel_SetAudc(&self->channels[2], self, pokeys, data, cycle);
		break;
	case 6:
		if (data == self->channels[3].audf)
			break;
		Pokey_GenerateUntilCycle(self, pokeys, cycle);
		self->channels[3].audf = data;
		switch (self->audctl & 40) {
		case 0:
		case 32:
			self->channels[3].periodCycles = self->divCycles * (data + 1);
			break;
		case 8:
			self->channels[3].periodCycles = self->divCycles * (self->channels[2].audf + (data << 8) + 1);
			break;
		case 40:
			self->channels[3].periodCycles = self->channels[2].audf + (data << 8) + 7;
			break;
		}
		PokeyChannel_MuteUltrasound(&self->channels[3], cycle);
		break;
	case 7:
		PokeyChannel_SetAudc(&self->channels[3], self, pokeys, data, cycle);
		break;
	case 8:
		if (data == self->audctl)
			break;
		Pokey_GenerateUntilCycle(self, pokeys, cycle);
		self->audctl = data;
		self->divCycles = (data & 1) != 0 ? 114 : 28;
		switch (data & 80) {
		case 0:
			self->channels[0].periodCycles = self->divCycles * (self->channels[0].audf + 1);
			self->channels[1].periodCycles = self->divCycles * (self->channels[1].audf + 1);
			break;
		case 16:
			self->channels[0].periodCycles = self->divCycles << 8;
			self->channels[1].periodCycles = self->divCycles * (self->channels[0].audf + (self->channels[1].audf << 8) + 1);
			self->reloadCycles1 = self->divCycles * (self->channels[0].audf + 1);
			break;
		case 64:
			self->channels[0].periodCycles = self->channels[0].audf + 4;
			self->channels[1].periodCycles = self->divCycles * (self->channels[1].audf + 1);
			break;
		case 80:
			self->channels[0].periodCycles = 256;
			self->channels[1].periodCycles = self->channels[0].audf + (self->channels[1].audf << 8) + 7;
			self->reloadCycles1 = self->channels[0].audf + 4;
			break;
		}
		PokeyChannel_MuteUltrasound(&self->channels[0], cycle);
		PokeyChannel_MuteUltrasound(&self->channels[1], cycle);
		switch (data & 40) {
		case 0:
			self->channels[2].periodCycles = self->divCycles * (self->channels[2].audf + 1);
			self->channels[3].periodCycles = self->divCycles * (self->channels[3].audf + 1);
			break;
		case 8:
			self->channels[2].periodCycles = self->divCycles << 8;
			self->channels[3].periodCycles = self->divCycles * (self->channels[2].audf + (self->channels[3].audf << 8) + 1);
			self->reloadCycles3 = self->divCycles * (self->channels[2].audf + 1);
			break;
		case 32:
			self->channels[2].periodCycles = self->channels[2].audf + 4;
			self->channels[3].periodCycles = self->divCycles * (self->channels[3].audf + 1);
			break;
		case 40:
			self->channels[2].periodCycles = 256;
			self->channels[3].periodCycles = self->channels[2].audf + (self->channels[3].audf << 8) + 7;
			self->reloadCycles3 = self->channels[2].audf + 4;
			break;
		}
		PokeyChannel_MuteUltrasound(&self->channels[2], cycle);
		PokeyChannel_MuteUltrasound(&self->channels[3], cycle);
		Pokey_InitMute(self, cycle);
		break;
	case 9:
		for (int i = 0; i < 4; i++)
			PokeyChannel_DoStimer(&self->channels[i], cycle);
		break;
	case 14:
		self->irqst |= data ^ 255;
		for (int i = 3;; i >>= 1) {
			if ((data & self->irqst & (i + 1)) != 0) {
				if (self->channels[i].timerCycle == 8388608) {
					int t = self->channels[i].tickCycle;
					while (t < cycle)
						t += self->channels[i].periodCycles;
					self->channels[i].timerCycle = t;
					if (nextEventCycle > t)
						nextEventCycle = t;
				}
			}
			else
				self->channels[i].timerCycle = 8388608;
			if (i == 0)
				break;
		}
		break;
	case 15:
		if (data == self->skctl)
			break;
		Pokey_GenerateUntilCycle(self, pokeys, cycle);
		self->skctl = data;
		bool init = (data & 3) == 0;
		if (self->init && !init)
			self->polyIndex = ((self->audctl & 128) != 0 ? 237614 : 60948014) - cycle;
		self->init = init;
		Pokey_InitMute(self, cycle);
		PokeyChannel_SetMute(&self->channels[2], (data & 16) != 0, 8, cycle);
		PokeyChannel_SetMute(&self->channels[3], (data & 16) != 0, 8, cycle);
		break;
	default:
		break;
	}
	return nextEventCycle;
}

static int Pokey_CheckIrq(Pokey *self, int cycle, int nextEventCycle)
{
	for (int i = 3;; i >>= 1) {
		int timerCycle = self->channels[i].timerCycle;
		if (cycle >= timerCycle) {
			self->irqst &= ~(i + 1);
			self->channels[i].timerCycle = 8388608;
		}
		else if (nextEventCycle > timerCycle)
			nextEventCycle = timerCycle;
		if (i == 0)
			break;
	}
	return nextEventCycle;
}

static int Pokey_StoreSample(Pokey *self, uint8_t *buffer, int bufferOffset, int i, ASAPSampleFormat format)
{
	self->iirAcc += self->deltaBuffer[i] - (self->iirAcc * 3 >> 10);
	int sample = self->iirAcc >> 11;
	if (sample < -32767)
		sample = -32767;
	else if (sample > 32767)
		sample = 32767;
	switch (format) {
	case ASAPSampleFormat_U8:
		buffer[bufferOffset++] = (uint8_t) ((sample >> 8) + 128);
		break;
	case ASAPSampleFormat_S16_L_E:
		buffer[bufferOffset++] = (uint8_t) (sample & 255);
		buffer[bufferOffset++] = (uint8_t) (sample >> 8 & 255);
		break;
	case ASAPSampleFormat_S16_B_E:
		buffer[bufferOffset++] = (uint8_t) (sample >> 8 & 255);
		buffer[bufferOffset++] = (uint8_t) (sample & 255);
		break;
	}
	return bufferOffset;
}

static void Pokey_AccumulateTrailing(Pokey *self, int i)
{
	self->iirAcc += self->deltaBuffer[i] + self->deltaBuffer[i + 1];
}

static void PokeyPair_Construct(PokeyPair *self)
{
	int reg = 511;
	for (int i = 0; i < 511; i++) {
		reg = (((reg >> 5 ^ reg) & 1) << 8) + (reg >> 1);
		self->poly9Lookup[i] = (uint8_t) (reg & 255);
	}
	reg = 131071;
	for (int i = 0; i < 16385; i++) {
		reg = (((reg >> 5 ^ reg) & 255) << 9) + (reg >> 8);
		self->poly17Lookup[i] = (uint8_t) (reg >> 1 & 255);
	}
}

static void PokeyPair_Initialize(PokeyPair *self, bool ntsc, bool stereo)
{
	self->extraPokeyMask = stereo ? 16 : 0;
	Pokey_Initialize(&self->basePokey);
	Pokey_Initialize(&self->extraPokey);
	self->sampleFactor = ntsc ? 25837 : 26075;
	self->sampleOffset = 0;
	self->readySamplesStart = 0;
	self->readySamplesEnd = 0;
}

static int PokeyPair_Poke(PokeyPair *self, int addr, int data, int cycle)
{
	Pokey *pokey = (addr & self->extraPokeyMask) != 0 ? &self->extraPokey : &self->basePokey;
	return Pokey_Poke(pokey, self, addr, data, cycle);
}

static int PokeyPair_Peek(const PokeyPair *self, int addr, int cycle)
{
	const Pokey *pokey = (addr & self->extraPokeyMask) != 0 ? &self->extraPokey : &self->basePokey;
	switch (addr & 15) {
	case 10:
		if (pokey->init)
			return 255;
		int i = cycle + pokey->polyIndex;
		if ((pokey->audctl & 128) != 0)
			return self->poly9Lookup[i % 511];
		i %= 131071;
		int j = i >> 3;
		i &= 7;
		return ((self->poly17Lookup[j] >> i) + (self->poly17Lookup[j + 1] << (8 - i))) & 255;
	case 14:
		return pokey->irqst;
	default:
		return 255;
	}
}

static void PokeyPair_StartFrame(PokeyPair *self)
{
	Pokey_StartFrame(&self->basePokey);
	if (self->extraPokeyMask != 0)
		Pokey_StartFrame(&self->extraPokey);
}

static int PokeyPair_EndFrame(PokeyPair *self, int cycle)
{
	Pokey_EndFrame(&self->basePokey, self, cycle);
	if (self->extraPokeyMask != 0)
		Pokey_EndFrame(&self->extraPokey, self, cycle);
	self->sampleOffset += cycle * self->sampleFactor;
	self->readySamplesStart = 0;
	self->readySamplesEnd = self->sampleOffset >> 20;
	self->sampleOffset &= 1048575;
	return self->readySamplesEnd;
}

static int PokeyPair_Generate(PokeyPair *self, uint8_t *buffer, int bufferOffset, int blocks, ASAPSampleFormat format)
{
	int i = self->readySamplesStart;
	int samplesEnd = self->readySamplesEnd;
	if (blocks < samplesEnd - i)
		samplesEnd = i + blocks;
	else
		blocks = samplesEnd - i;
	for (; i < samplesEnd; i++) {
		bufferOffset = Pokey_StoreSample(&self->basePokey, buffer, bufferOffset, i, format);
		if (self->extraPokeyMask != 0)
			bufferOffset = Pokey_StoreSample(&self->extraPokey, buffer, bufferOffset, i, format);
	}
	if (i == self->readySamplesEnd) {
		Pokey_AccumulateTrailing(&self->basePokey, i);
		Pokey_AccumulateTrailing(&self->extraPokey, i);
	}
	self->readySamplesStart = i;
	return blocks;
}

static bool PokeyPair_IsSilent(const PokeyPair *self)
{
	return Pokey_IsSilent(&self->basePokey) && Pokey_IsSilent(&self->extraPokey);
}
