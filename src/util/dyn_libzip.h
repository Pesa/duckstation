// SPDX-FileCopyrightText: 2019-2026 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: CC-BY-NC-ND-4.0

#pragma once

#include <zip.h>

class Error;

#define DYN_LIBZIP_FUNCTIONS(X)                                                                                        \
  X(zip_get_error)                                                                                                     \
  X(zip_error_set)                                                                                                     \
  X(zip_error_strerror)                                                                                                \
  X(zip_error_fini)                                                                                                    \
  X(zip_open_from_source)                                                                                              \
  X(zip_source_file_create)                                                                                            \
  X(zip_source_function_create)                                                                                        \
  X(zip_source_buffer_create)                                                                                          \
  X(zip_source_free)                                                                                                   \
  X(zip_close)                                                                                                         \
  X(zip_discard)                                                                                                       \
  X(zip_get_num_entries)                                                                                               \
  X(zip_get_name)                                                                                                      \
  X(zip_name_locate)                                                                                                   \
  X(zip_stat)                                                                                                          \
  X(zip_stat_index)                                                                                                    \
  X(zip_fopen)                                                                                                         \
  X(zip_fopen_index)                                                                                                   \
  X(zip_fread)                                                                                                         \
  X(zip_fclose)

struct DynLibZip
{
#define ADD_FUNC(F) decltype(&::F) F;
  DYN_LIBZIP_FUNCTIONS(ADD_FUNC)
#undef ADD_FUNC

  bool Open(Error* error);
};

extern DynLibZip g_dyn_libzip;
