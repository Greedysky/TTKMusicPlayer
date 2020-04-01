#ifndef _UMR_H
#define _UMR_H

#include <stdint.h>

namespace umr
{

class file_reader
{
public:
	virtual long read(void * ptr, long howmany) =0;
	virtual void seek(long where) =0;
};

class file_writer
{
public:
	virtual void write(void * ptr, long howmany) =0;
};

#include "urf.h"

class upkg
{
	upkg_hdr *hdr;			// read the urf.h for these 4...
	upkg_exports *exports;
	upkg_imports *imports;
	upkg_names *names;

	file_reader * reader;

	int data_size,			// a way to standardize some freaky parts of the format
	 pkg_opened;		// sanity check

	char header[4096],		// we load the header into this buffer
	 buf[256];			// temp buf for get_string()

public:
	upkg() { pkg_opened = 0; }
	~upkg() { close(); }

	bool open(file_reader * reader);		// open a upkg format file.
	void close(void);

	signed int ocount(void);	// returns the number of exports
	char *oname(signed int);	// returns the name of the export
	char *oclassname(signed int);	// returns the name of the export's class
	char *opackagename(signed int);	// returns the name of the export's package
	char *otype(signed int);	// returns the name of the type of object
	signed int export_size(signed int);	// return the size of the export described
	signed int object_size(signed int);	// return the size of the object described
	signed int export_offset(signed int);	// return the offset to said export
	signed int object_offset(signed int);	// same

	int read(void *, signed int, signed int);	// read data from the upkg file

	int export_dump(file_writer *, signed int);	// dump an export
	int object_dump(file_writer *, signed int);	// dump an object

private:
	signed long get_fci(char *in);
    unsigned long get_u32(void *addr);
	signed long get_s32(void *addr);
	signed long get_s16(void *addr);
	signed long get_s8(void *addr);
	char *get_string(char *addr, int count);
	
	int set_classname(int idx, int c_idx);
	int set_pkgname(int idx, int c_idx);

	int load_upkg(void);

	void get_names(void);

	void get_exports_cpnames(int idx);
	void get_exports(void);

	void get_imports(void);

	void get_type(char *buf, int e, int d);
	int get_types_isgood(int idx);
	void check_type(int e, int d);
	void get_types(void);
};

}

#endif				// _UMR_H
