#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "umr.h"

namespace umr {

#include "urf.h"


/*
upkg_hdr *hdr;			// read the urf.h for these 4...
upkg_exports *exports;
upkg_imports *imports;
upkg_names *names;

FILE *file;			// we store the file pointer globally around here

int data_size,			// a way to standardize some freaky parts of the format
 pkg_opened = 0;		// sanity check
int indent_level;

char header[4096],		// we load the header into this buffer
 buf[256];			// temp buf for get_string()
*/

// this function decodes the encoded indices in the upkg files
signed long upkg::get_fci(char *in)
{
	signed long a;
	int size;

	a = 0;
	size = 1;

	a = in[0] & 0x3f;

	if (in[0] & 0x40) {
		size++;
		a |= (in[1] & 0x7f) << 6;

		if (in[1] & 0x80) {
			size++;
			a |= (in[2] & 0x7f) << 13;

			if (in[2] & 0x80) {
				size++;
				a |= (in[3] & 0x7f) << 20;

				if (in[3] & 0x80) {
					size++;
					a |= (in[4] & 0x3f) << 27;
				}
			}
		}
	}

	if (in[0] & 0x80)
		a = -a;

	data_size = size;

	return a;
}
    
unsigned long upkg::get_u32(void *addr)
{
    const uint8_t * addr8 = (const uint8_t *) addr;
    uint32_t rval = addr8[0] | (addr8[1] << 8) | (addr8[2] << 16) | (addr8[3] << 24);
	data_size = sizeof(uint32_t);
	return rval;
}

signed long upkg::get_s32(void *addr)
{
    const uint8_t * addr8 = (const uint8_t *) addr;
    int32_t rval = addr8[0] | (addr8[1] << 8) | (addr8[2] << 16) | (addr8[3] << 24);
	data_size = sizeof(int32_t);
	return rval;
}

signed long upkg::get_s16(void *addr)
{
    const uint8_t * addr8 = (const uint8_t *) addr;
    int16_t rval = addr8[0] | (addr8[1] << 8);
	data_size = sizeof(int16_t);
	return rval;
}

signed long upkg::get_s8(void *addr)
{
	data_size = sizeof(int8_t);
	return *(int8_t *) addr;
}

char * upkg::get_string(char *addr, int count)
{
	if (count > UPKG_MAX_NAME_SIZE || count == UPKG_NAME_NOCOUNT)
		count = UPKG_MAX_NAME_SIZE;

	strncpy(buf, addr, count);	// the string stops at count chars, or is ASCIIZ

	data_size = strlen(buf) + 1;

	return buf;
}

signed int export_index(signed int i)
{
	if (i > 0) {
		return i - 1;
	}

	return -1;
}

signed int import_index(signed int i)
{
	if (i < 0) {
		return -i - 1;
	}

	return -1;
}

// idx == exports[idx], c_idx == index to the next element from idx
int upkg::set_classname(int idx, int c_idx) {
    int i, next;
    
    i = c_idx;
    
    do {
	if (i < 0) {
	    i = import_index(i);
	    if (!strcmp(names[imports[i].class_name].name, "Class")) {
		exports[idx].class_name = imports[i].object_name;
		return imports[i].package_index;
	    }
	    
	    next = imports[i].package_index;
	}
	
	if (i > 0) {
	    i = export_index(i);
	    next = exports[i].class_index;
	} else {
	    break;
	}
	
	i = next;
    } while (i >= -hdr->import_count && i < hdr->export_count);
    
    exports[idx].class_name = hdr->name_count;
    return c_idx;
}

int upkg::set_pkgname(int idx, int c_idx) {
    int i, next;
    
    i = c_idx;
    
    do {
	if (i < 0) {
	    i = import_index(i);
	    if (!strcmp(names[imports[i].class_name].name, "Package")) {
		exports[idx].package_name = imports[i].object_name;
		return imports[i].package_index;
	    }
	    
	    next = imports[i].package_index;
	}
	
	if (i > 0) {
	    i = export_index(i);

	    next = exports[i].class_index;
	} else {
	    break;
	}
	
	i = next;
    } while (i >= -hdr->import_count && i < hdr->export_count);
    
    exports[idx].package_name = hdr->name_count;
    return c_idx;
}

// load in the header, AWA allocating the needed memory for the tables
int upkg::load_upkg(void)
{
	hdr = (upkg_hdr *) header;

	if (get_u32(&hdr->tag) != UPKG_HDR_TAG)
		return -1;

    int i;
	for (i = 0; export_desc[i].version; i++) {
		if (get_u32(&hdr->file_version) == export_desc[i].version) {
			break;
		}
	}

	if (export_desc[i].version == 0)
		return -1;

	names =
	    (upkg_names *) malloc(sizeof(upkg_names) * (hdr->name_count + 1));
	if (names == NULL)
		return -1;

	exports =
	    (upkg_exports *) malloc(sizeof(upkg_exports) *
				    hdr->export_count);
	if (exports == NULL) {
		free(names);
		return -1;
	}

	imports =
	    (upkg_imports *) malloc(sizeof(upkg_imports) *
				    hdr->import_count);
	if (imports == NULL) {
		free(exports);
		free(names);
		return -1;
	}
	
	return 0;
}

// load the name table
void upkg::get_names(void)
{
	int i, j, index;

	index = get_u32(&hdr->name_offset);

	for (i = 0, j = get_u32(&hdr->name_count); i < j; i++) {
		if (get_u32(&hdr->file_version) >= 64) {
			get_string(&header[index + 1],
				   get_s8(&header[index]));
			index++;
		} else {
			get_string(&header[index], UPKG_NAME_NOCOUNT);
		}
		index += data_size;

		strncpy(names[i].name, buf, UPKG_MAX_NAME_SIZE);

		names[i].flags = get_s32(&header[index]);
		index += data_size;
	}
	
// hdr->name_count + 1 names total, this one's last
	strncpy(names[i].name, "(NULL)", UPKG_MAX_NAME_SIZE);
	names[i].flags = 0;
}

// load the export table (which is at the end of the file... go figure)
void upkg::get_exports_cpnames(int idx) {
    int x;
    
    if (idx < 0 || idx >= get_u32(&hdr->export_count))
	return;
    
    x = get_u32(&exports[idx].class_index);
    
    x = set_classname(idx, x);
    
    set_pkgname(idx, x);
}

void upkg::get_exports(void)
{
	int i, j, index;
	char readbuf[1024];

	reader->seek(hdr->export_offset);
	reader->read(readbuf, 1024);

	index = 0;

	for (i = 0, j = get_u32(&hdr->export_count); i < j; i++) {
		exports[i].class_index = get_fci(&readbuf[index]);
		index += data_size;

		exports[i].package_index = get_s32(&readbuf[index]);
		index += data_size;

		exports[i].super_index = get_fci(&readbuf[index]);
		index += data_size;

		exports[i].object_name = get_fci(&readbuf[index]);
		index += data_size;

		exports[i].object_flags = get_s32(&readbuf[index]);
		index += data_size;

		exports[i].serial_size = get_fci(&readbuf[index]);
		index += data_size;

		if (exports[i].serial_size > 0) {
			exports[i].serial_offset =
			    get_fci(&readbuf[index]);
			index += data_size;
		} else {
			exports[i].serial_offset = -1;
		}

		get_exports_cpnames(i);	// go grab the class & package names
	}
}

// load the import table (notice a trend?).  same story as get_exports()
void upkg::get_imports(void)
{
	int i, j, index;
	char readbuf[1024];

	reader->seek(hdr->import_offset);
	reader->read(readbuf, 1024);

	index = 0;

	for (i = 0, j = get_u32(&hdr->import_count); i < j; i++) {
		imports[i].class_package = get_fci(&readbuf[index]);
		index += data_size;

		imports[i].class_name = get_fci(&readbuf[index]);
		index += data_size;

		imports[i].package_index = get_s32(&readbuf[index]);
		index += data_size;

		imports[i].object_name = get_fci(&readbuf[index]);
		index += data_size;
	}
}	

// load the type_names
void upkg::get_type(char *buf, int e, int d)
{
	int i, j, index;
	signed long tmp;
	char *chtmp;

	index = 0;

	for (i = 0, j = strlen(export_desc[d].order); i < j; i++) {
		switch (export_desc[d].order[i]) {
		case UPKG_DATA_FCI:
			tmp = get_fci(&buf[index]);
			index += data_size;
			break;
		case UPKG_DATA_32:
			tmp = get_s32(&buf[index]);
			index += data_size;
			break;
		case UPKG_DATA_16:
			tmp = get_s16(&buf[index]);
			index += data_size;
			break;
		case UPKG_DATA_8:
			tmp = get_s8(&buf[index]);
			index += data_size;
			break;
		case UPKG_DATA_ASCIC:
			chtmp =
			    get_string(&buf[index + 1],
				       get_s8(&buf[index]));
			index += data_size + 1;
			break;
		case UPKG_DATA_ASCIZ:
			chtmp = get_string(&buf[index], UPKG_NAME_NOCOUNT);
			index += data_size;
			break;
		case UPKG_OBJ_JUNK:	// do nothing!!!
			break;
		case UPKG_OBJ_NAME:
			exports[e].type_name = tmp;
			break;
		case UPKG_EXP_SIZE:	// maybe we'll do something later on
			break;
		case UPKG_OBJ_SIZE:
			exports[e].object_size = tmp;
			break;
		default:
			exports[e].type_name = -1;
			return;
		}
	}

	exports[e].object_offset = exports[e].serial_offset + index;
}

int upkg::get_types_isgood(int idx)
{
	int i;
	for (i = 0; export_desc[i].version; i++) {
		if (export_desc[i].version == get_u32(&hdr->file_version)) {
			if (strcmp(export_desc[i].class_name,
				    names[exports[idx].class_name].name
			    ) == 0) {
				return i;
			}
		}
	}

	return -1;
}

void upkg::check_type(int e, int d)
{
	int i;
	char readbuf[101], s, l;

	reader->seek(exports[e].object_offset);
	reader->read(readbuf, 100);

	for (i = 0; object_desc[i].sig_offset != -1; i++) {
		s = object_desc[i].sig_offset;
		l = strlen(object_desc[i].object_sig);
		readbuf[100] = readbuf[s + l];

		readbuf[s + l] = 0;

		if (!strcmp(&readbuf[s], object_desc[i].object_sig)) {
			return;
		}

		readbuf[s + l] = readbuf[100];
	}

	exports[e].type_name = -1;
}


void upkg::get_types(void)
{
	int i, j, k;
	char readbuf[UPKG_MAX_ORDERS * 4];

	for (i = 0, k = get_u32(&hdr->export_count); i < k; i++) {
		if ((j = get_types_isgood(i)) != -1) {
			reader->seek(exports[i].serial_offset);
			reader->read(readbuf, 4 * UPKG_MAX_ORDERS);

			get_type(readbuf, i, j);

			check_type(i, j);
		} else {
			exports[i].type_name = -1;
		}
	}
}


//**************  GLOBALS


// open that puppy!!!  gets the file opened and the data structs read for use
bool upkg::open(file_reader * p_reader)
{
	if (pkg_opened)		// is there a pkg opened already?
		return false;	// if so, don't try to open another one!

	if (p_reader == NULL)
		return false;

	reader = p_reader;

	if (reader->read(header, 4096) < 4096) {
		return false;
	}

	if (load_upkg() != 0) {
		return false;
	}

	pkg_opened = 1;

	get_names();		// this order is important.
	get_imports();
	get_exports();
	get_types();

	return true;
}

// close everything out
void upkg::close(void)
{
	if (pkg_opened == 0)
		return;

	free(imports);
	free(exports);
	free(names);
	hdr = (upkg_hdr *) 0;

	pkg_opened = 0;
}

// API stuff...  should be self-explainatory (upkg_o* == unreal package object *)
signed int upkg::ocount(void)
{
	if (pkg_opened == 0)
		return -1;

	return hdr->export_count;
}


char * upkg::oname(signed int idx)
{
	idx = export_index(idx);
	if (idx == -1 || pkg_opened == 0)
		return NULL;

	return names[exports[idx].object_name].name;
}

char * upkg::oclassname(signed int idx)
{
	idx = export_index(idx);
	if (idx == -1 || pkg_opened == 0)
		return NULL;

	return names[exports[idx].class_name].name;
}

char * upkg::opackagename(signed int idx)
{
	idx = export_index(idx);
	if (idx == -1 || pkg_opened == 0)
		return NULL;

	return names[exports[idx].package_name].name;
}

char * upkg::otype(signed int idx)
{
	idx = export_index(idx);
	if (idx == -1 || pkg_opened == 0)
		return NULL;

	if (exports[idx].type_name == -1)
		return NULL;

	return names[exports[idx].type_name].name;
}

signed int upkg::export_size(signed int idx)
{
	idx = export_index(idx);
	if (idx == -1 || pkg_opened == 0)
		return 0;

	return exports[idx].serial_size;
}

signed int upkg::object_size(signed int idx)
{
	idx = export_index(idx);
	if (idx == -1 || pkg_opened == 0)
		return 0;

	return exports[idx].object_size;
}

signed int upkg::export_offset(signed int idx)
{
	idx = export_index(idx);
	if (idx == -1 || pkg_opened == 0)
		return 0;

	return exports[idx].serial_offset;
}

signed int upkg::object_offset(signed int idx)
{
	idx = export_index(idx);
	if (idx == -1 || pkg_opened == 0)
		return 0;

	return exports[idx].object_offset;
}


int upkg::read(void *readbuf, signed int bytes, signed int offset)
{
	if (pkg_opened == 0)
		return -1;

	reader->seek(offset);

	return reader->read(readbuf, bytes);
}

int upkg::export_dump(file_writer * writer, signed int idx)
{
	int count, diff;
	void *buffer;

	idx = export_index(idx);
	if (idx == -1 || pkg_opened == 0)
		return -1;

	buffer = malloc(4096);
	if (buffer == NULL)
		return -1;

	reader->seek(exports[idx].serial_offset);

	count = exports[idx].serial_size;

	do {
		diff =
			reader->read(buffer, ((count > 4096) ? 4096 : count));
		writer->write(buffer, diff);
		count -= diff;
	} while (count > 0);

	free(buffer);

	return 0;
}

int upkg::object_dump(file_writer * writer, signed int idx)
{
	int count, diff;
	void *buffer;

	idx = export_index(idx);
	if (idx == -1 || pkg_opened == 0)
		return -1;

	buffer = malloc(4096);
	if (buffer == NULL)
		return -1;

	reader->seek(exports[idx].object_offset);

	count = exports[idx].object_size;

	do {
		diff =
			reader->read(buffer, ((count > 4096) ? 4096 : count));
		writer->write(buffer, diff);
		count -= diff;
	} while (count > 0);

	free(buffer);

	return 0;
}

}
