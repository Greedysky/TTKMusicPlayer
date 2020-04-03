//
// Audio Overload
// Emulated music player
//
// (C) 2000-2007 Richard F. Bannister
//

//
// eng_protos.h
//

void *psf_start(const char *path, uint8 *, uint32 length);
int32 psf_gen(void *, int16 *, uint32);
int32 psf_stop(void *);
int32 psf_command(void *, int32, int32);
int32 psf_fill_info(void *, ao_display_info *);

void *psf2_start(const char *path, uint8 *, uint32 length);
int32 psf2_gen(void *, int16 *, uint32);
int32 psf2_stop(void *);
int32 psf2_command(void *, int32, int32);
int32 psf2_fill_info(void *, ao_display_info *);

void *qsf_start(const char *path, uint8 *, uint32 length);
int32 qsf_gen(void *, int16 *, uint32);
int32 qsf_stop(void *);
int32 qsf_command(void *, int32, int32);
int32 qsf_fill_info(void*, ao_display_info *);

void *ssf_start(const char *path, uint8 *, uint32 length);
int32 ssf_gen(void *, int16 *, uint32);
int32 ssf_stop(void *);
int32 ssf_command(void *, int32, int32);
int32 ssf_fill_info(void *, ao_display_info *);

void *spu_start(const char *path, uint8 *, uint32 length);
int32 spu_gen(void *, int16 *, uint32);
int32 spu_stop(void *);
int32 spu_command(void *, int32, int32);
int32 spu_fill_info(void *, ao_display_info *);

uint8 qsf_memory_read(uint16 addr);
uint8 qsf_memory_readop(uint16 addr);
uint8 qsf_memory_readport(uint16 addr);
void qsf_memory_write(uint16 addr, uint8 byte);
void qsf_memory_writeport(uint16 addr, uint8 byte);

void *dsf_start(const char *path, uint8 *, uint32 length);
int32 dsf_gen(void *, int16 *, uint32);
int32 dsf_stop(void *);
int32 dsf_command(void *, int32, int32);
int32 dsf_fill_info(void *, ao_display_info *);

