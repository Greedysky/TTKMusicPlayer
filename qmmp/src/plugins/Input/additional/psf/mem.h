//
// Audio Overload
// Emulated music player
//
// (C) 2000-2008 Richard F. Bannister
//

// mem.h

// defined in eng_qsf.c
uint8 memory_read(void *userdata, uint16 addr);
uint8 memory_readop(void *userdata, uint16 addr);
uint8 memory_readport(void *userdata, uint16 addr);
void memory_write(void *userdata, uint16 addr, uint8 byte);
void memory_writeport(void *userdata, uint16 addr, uint8 byte);

uint8 dc_read8(uint32 addr);
uint16 dc_read16(uint32 addr);
uint32 dc_read32(uint32 addr);
void dc_write8(uint32 addr, uint8 byte);
void dc_write16(uint32 addr, uint16 word);
void dc_write32(uint32 addr, uint32 dword);

