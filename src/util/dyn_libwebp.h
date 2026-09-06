// SPDX-FileCopyrightText: 2019-2026 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: CC-BY-NC-ND-4.0

#pragma once

#include <webp/decode.h>
#include <webp/encode.h>

class Error;

#define DYN_LIBWEBP_FUNCTIONS(X)                                                                                       \
  X(WebPGetInfo)                                                                                                       \
  X(WebPDecodeRGBAInto)                                                                                                \
  X(WebPEncodeRGBA)                                                                                                    \
  X(WebPFree)

struct DynLibWebP
{
#define ADD_FUNC(F) decltype(&::F) F;
  DYN_LIBWEBP_FUNCTIONS(ADD_FUNC)
#undef ADD_FUNC

  bool Open(Error* error);
};

extern DynLibWebP g_dyn_libwebp;
