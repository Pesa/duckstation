// SPDX-FileCopyrightText: 2019-2026 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: CC-BY-NC-ND-4.0

#pragma once

#include <jpeglib.h>

class Error;

#define DYN_LIBJPEG_FUNCTIONS(X)                                                                                       \
  X(jpeg_CreateDecompress)                                                                                             \
  X(jpeg_CreateCompress)                                                                                               \
  X(jpeg_destroy_decompress)                                                                                           \
  X(jpeg_destroy_compress)                                                                                             \
  X(jpeg_std_error)                                                                                                    \
  X(jpeg_read_header)                                                                                                  \
  X(jpeg_start_decompress)                                                                                             \
  X(jpeg_read_scanlines)                                                                                               \
  X(jpeg_finish_decompress)                                                                                            \
  X(jpeg_mem_src)                                                                                                      \
  X(jpeg_resync_to_restart)                                                                                            \
  X(jpeg_set_defaults)                                                                                                 \
  X(jpeg_set_quality)                                                                                                  \
  X(jpeg_start_compress)                                                                                               \
  X(jpeg_write_scanlines)                                                                                              \
  X(jpeg_finish_compress)

struct DynLibJPEG
{
#define ADD_FUNC(F) decltype(&::F) F;
  DYN_LIBJPEG_FUNCTIONS(ADD_FUNC)
#undef ADD_FUNC

  bool Open(Error* error);
};

extern DynLibJPEG g_dyn_libjpeg;
