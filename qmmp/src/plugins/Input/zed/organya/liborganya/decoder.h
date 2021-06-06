/*
 *  decoder.h
 *  liborganya
 *
 *  Created by Vincent Spader on 6/20/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "file.h"
#include "blip_buf.h"

typedef struct {
  uint16_t current_note; // Index into org_instrument_t.notes
  uint8_t playing;       // Whether or not the track is currently playing

  org_note_t note;       // Current note properties.

  int16_t last_amp[2];
  int32_t last_clock;
} org_track_t;

typedef struct {
  int16_t* wave;       // PCM data. samples are unsigned 16-bit BIG ENDIAN at 22050hz on disk. Converted to host endian when loaded.
  uint32_t length;     // number of samples
} org_sample_data_t;

typedef struct {
  uint8_t primed;          // Simple flag to determine if the decoder has been used or not
  uint32_t current_beat;   // The current beat number
  uint32_t current_sample; // The current sample number

  uint32_t current_loop;   // The current loop number
  uint32_t loop_count;     // The total number of times we will loop. This is passed in org_decoder_create.

  uint32_t sample_rate;

  blip_t* blip_buffer[2];

  org_track_t tracks[16];       // 16 tracks for each org file
} org_decoder_state_t;

struct org_decoder_t {
  org_file_t* file;
  org_decoder_state_t state;
  org_sample_data_t samples[16];       // The sample data for each track
};

// Loads instrument sample data for the decoder
uint8_t _org_decoder_load_samples(org_decoder_t* decoder);
// Advances the decoder by 1 beat
void _org_advance_beat(org_decoder_t* decoder);
// Clean up the decoder
void org_decoder_destroy(org_decoder_t* decoder);
