/*
 *  file.c
 *  liborganya
 *
 *  Created by Vincent Spader on 6/20/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "swap.h"

// File reading helpers
uint8_t _org_read_8(void* fin)
{
  uint8_t i = 0;
  fread(&i, 1, 1, static_cast<FILE *>(fin));
  return i;
}

uint16_t _org_read_16(void* fin)
{
  uint16_t i = 0;
  fread(&i, 1, 2, static_cast<FILE *>(fin));
  return org_ltoh_16(i);
}

uint32_t _org_read_32(void* fin)
{
  uint32_t i = 0;
  fread(&i, 1, 4, static_cast<FILE *>(fin));
  return org_ltoh_32(i);
}

// Read the usual org header
void _org_read_header(org_header_t* header, void* fin)
{
  // Read the magic. All orgyana files start with Org-02.
  int8_t buf[6];
  fread(buf, 1, 6, static_cast<FILE *>(fin));

  int version = -1;
  if(0 != memcmp(buf, "Org-01", 6))
    version = 1;
  else if(0 != memcmp(buf, "Org-02", 6))
    version = 2;
  else if(0 != memcmp(buf, "Org-03", 6))
    version = 3;
  else
    throw NULL;

  header->tempo          = _org_read_16(fin);
  header->steps_per_bar  = _org_read_8(fin);
  header->beats_per_step = _org_read_8(fin);
  header->loop_start     = _org_read_32(fin);
  header->loop_end       = _org_read_32(fin);
}

// Read properties for the instrument
void _org_read_instrument(org_instrument_t* instrument, void* fin)
{
  instrument->pitch           = _org_read_16(fin);
  instrument->instrument      = _org_read_8(fin);
  instrument->disable_sustain = _org_read_8(fin);
  instrument->note_count      = _org_read_16(fin);
}

// Read properties for each note
void _org_read_notes(org_note_t notes[], void* fin, uint16_t note_count)
{
  for (uint16_t i = 0; i < note_count; i++)
  {
    notes[i].start = _org_read_32(fin);
  }
  for (uint16_t i = 0; i < note_count; i++)
  {
    notes[i].key = _org_read_8(fin);
  }
  for (uint16_t i = 0; i < note_count; i++)
  {
    notes[i].length = _org_read_8(fin);
  }
  for (uint16_t i = 0; i < note_count; i++)
  {
    notes[i].volume = _org_read_8(fin);
  }
  for (uint16_t i = 0; i < note_count; i++)
  {
    notes[i].pan = _org_read_8(fin);
  }
}

// Rather straightforward just follows the file format.
org_file_t * _org_file_create(const char* org_file)
{
  org_file_t* org = ( org_file_t * ) calloc(1, sizeof(org_file_t));
  if ( !org )
    return NULL;

  FILE* file = fopen(org_file, "rb");
  if(!file)
    return NULL;

  try
  {
    _org_read_header(&org->header, file);
    // Read instrument properties
    for (uint8_t i = 0; i < 16; i++)
    {
      _org_read_instrument(&org->instruments[i], file);

      // Allocate space for notes
      if (org->instruments[i].note_count)
      {
        org->instruments[i].notes = ( org_note_t * ) malloc(sizeof(org_note_t) * org->instruments[i].note_count);
        if ( !org->instruments[i].notes )
          return NULL;
      }
      else
      {
        org->instruments[i].notes = NULL;
      }
    }

    // Read notes for each instrument
    for (uint8_t i = 0; i < 16; i++)
    {
      _org_read_notes(org->instruments[i].notes, file, org->instruments[i].note_count);
    }

    fclose(file);
    return org;
  }
  catch (...)
  {
    _org_file_destroy(org);
    fclose(file);
    throw;
  }
}

void _org_file_destroy(org_file_t* org)
{
  // Free up memory
  for (uint8_t i = 0; i < 16; i++)
  {
    if (org->instruments[i].notes)
      free(org->instruments[i].notes);
  }

  free(org);
}
