/* Modem for MIPS   AJF	  January 1995
   Sine generator */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "private.h"
#include "complex.h"
#include "sinegen.h"

#define SLENK 11  // ??? was 15
#define SINELEN (1 << SLENK)
#define TWO32 4.294967296e9 /* 2^32 */
#define TWOPI (2.0 * M_PI)

static struct Init_Sines { Init_Sines(); } init_sines;

static float* sinetab;

Init_Sines::Init_Sines() { /* once-only initialization, done before main() is
                              called */
  sinetab = new float[SINELEN];
  if (sinetab == NULL) {
    fprintf(stderr, "OutOfMemoryException Init_Sines\n");
    exit(-1);
  }
  for (int k = 0; k < SINELEN; k++) {
    float th = TWOPI * (float)k / (float)SINELEN;
    sinetab[k] = sin(th);
  }
}

SineGen::SineGen(float f) {
  setfreq(f);
  resetphase();
}

void SineGen::setfreq(float f) {
  phinc = (int)(f * TWO32 / (float)SAMPLERATE);
}

float SineGen::fnext() {
  float x = sinetab[ptr >> (32 - SLENK)]; /* keep top SLENK bits */
  ptr += phinc;
  return x;
}

complex SineGen::cnext() {
  complex z = complex(sinetab[ptr >> (32 - SLENK)],
                      sinetab[(ptr + (1 << 30)) >> (32 - SLENK)]);
  ptr += phinc;
  return z;
}
