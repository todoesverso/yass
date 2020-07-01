/*
 * YASS (Yet Another Simple Synthesizer)
 * Copyright (C) 2020 Victor Rosales <todoesverso@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lib/osc.h"
#include "lib/utils.h"

static OSC* osc_malloc();
static void osc_init(OSC* p_osc, uint32_t srate);
static double calc_new_phase(OSC* p_osc);

static OSC* osc_malloc() {
  OSC* p_osc;
  p_osc = (OSC*) malloc(sizeof(OSC));  // NOLINT(readability/casting)
  return p_osc;
}

static void osc_init(OSC* p_osc, uint32_t srate) {
  p_osc->freq = 0.0f;
  p_osc->phase = 0.0f;
  p_osc->srate = srate;
}

static double calc_new_phase(OSC* p_osc) {
  return (TWO_PI / p_osc->srate) * p_osc->freq;
}

OSC* new_osc(uint32_t srate) {
  OSC* p_osc = osc_malloc();
  if (p_osc == NULL)
    return NULL;
  osc_init(p_osc, srate);
  return p_osc;
}

void update_freq(OSC* p_osc, double newfreq) {
  p_osc->freq = newfreq;
  p_osc->phase = calc_new_phase(p_osc);
}
void update_freq_from_midi_note(OSC* p_osc, uint8_t note) {
  p_osc->freq = midi_to_freq(note);
  p_osc->phase = calc_new_phase(p_osc);
}
void update_phase(OSC *p_osc, double phase) {
  p_osc->phase = phase;
}
void update_srate(OSC *p_osc, uint32_t srate) {
  p_osc->srate = srate;
}

double get_output(OSC* p_osc) {
  double val;

  val = sin(p_osc->phase);
  p_osc->phase += calc_new_phase(p_osc);
  if (p_osc->phase >= TWO_PI)
    p_osc->phase -= TWO_PI;
  if (p_osc->phase < 0.0)
    p_osc->phase += TWO_PI;

  return val;
}