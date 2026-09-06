// SPDX-FileCopyrightText: 2019-2026 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: CC-BY-NC-ND-4.0

#pragma once

#include <png.h>

class Error;

#define DYN_LIBPNG_FUNCTIONS(X)                                                                                        \
  X(png_create_read_struct)         /* ordinal 4 */                                                                    \
  X(png_create_write_struct)        /* ordinal 5 */                                                                    \
  X(png_set_longjmp_fn)             /* ordinal 8 */                                                                    \
  X(png_longjmp)                    /* ordinal 9 */                                                                    \
  X(png_create_info_struct)         /* ordinal 18 */                                                                   \
  X(png_write_info)                 /* ordinal 21 */                                                                   \
  X(png_read_info)                  /* ordinal 22 */                                                                   \
  X(png_set_expand_gray_1_2_4_to_8) /* ordinal 27 */                                                                   \
  X(png_set_palette_to_rgb)         /* ordinal 28 */                                                                   \
  X(png_set_tRNS_to_alpha)          /* ordinal 29 */                                                                   \
  X(png_set_gray_to_rgb)            /* ordinal 31 */                                                                   \
  X(png_set_filler)                 /* ordinal 39 */                                                                   \
  X(png_set_interlace_handling)     /* ordinal 45 */                                                                   \
  X(png_set_strip_16)               /* ordinal 48 */                                                                   \
  X(png_read_update_info)           /* ordinal 54 */                                                                   \
  X(png_read_row)                   /* ordinal 56 */                                                                   \
  X(png_write_row)                  /* ordinal 58 */                                                                   \
  X(png_write_end)                  /* ordinal 61 */                                                                   \
  X(png_destroy_read_struct)        /* ordinal 64 */                                                                   \
  X(png_destroy_write_struct)       /* ordinal 65 */                                                                   \
  X(png_set_compression_level)      /* ordinal 69 */                                                                   \
  X(png_set_error_fn)               /* ordinal 75 */                                                                   \
  X(png_get_error_ptr)              /* ordinal 76 */                                                                   \
  X(png_set_write_fn)               /* ordinal 77 */                                                                   \
  X(png_set_read_fn)                /* ordinal 78 */                                                                   \
  X(png_get_io_ptr)                 /* ordinal 79 */                                                                   \
  X(png_error)                      /* ordinal 102 */                                                                  \
  X(png_get_valid)                  /* ordinal 110 */                                                                  \
  X(png_set_IHDR)                   /* ordinal 144 */                                                                  \
  X(png_get_image_width)            /* ordinal 115 */                                                                  \
  X(png_get_image_height)           /* ordinal 116 */                                                                  \
  X(png_get_bit_depth)              /* ordinal 117 */                                                                  \
  X(png_get_color_type)             /* ordinal 118 */                                                                  \
  X(png_set_acTL)                   /* ordinal 261 */                                                                  \
  X(png_get_num_frames)             /* ordinal 262 */                                                                  \
  X(png_get_next_frame_width)       /* ordinal 266 */                                                                  \
  X(png_get_next_frame_height)      /* ordinal 267 */                                                                  \
  X(png_get_next_frame_delay_num)   /* ordinal 270 */                                                                  \
  X(png_get_next_frame_delay_den)   /* ordinal 271 */                                                                  \
  X(png_read_frame_head)            /* ordinal 276 */                                                                  \
  X(png_write_frame_head)           /* ordinal 278 */                                                                  \
  X(png_write_frame_tail)           /* ordinal 279 */

struct DynLibPNG
{
#define ADD_FUNC(F) decltype(&::F) F;
  DYN_LIBPNG_FUNCTIONS(ADD_FUNC)
#undef ADD_FUNC

  bool Open(Error* error);
};

extern DynLibPNG g_dyn_libpng;
