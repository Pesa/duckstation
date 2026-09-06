// SPDX-FileCopyrightText: 2019-2026 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: CC-BY-NC-ND-4.0

#include "dyn_freetype.h"
#include "dyn_plutosvg.h"

#include "common/dynamic_library.h"
#include "common/error.h"
#include "common/log.h"

LOG_CHANNEL(DynamicLibrary);

static_assert(std::is_trivially_copyable_v<DynFreeType> && std::is_standard_layout_v<DynFreeType>);
static_assert(std::is_trivially_copyable_v<DynPlutoSvg> && std::is_standard_layout_v<DynPlutoSvg>);

// Because of course friggin linux is different...
#ifdef _WIN32
static constexpr int FREETYPE_MAJOR_VERSION = -1;
static constexpr int PLUTOSVG_MAJOR_VERSION = -1;
#else
static constexpr int FREETYPE_MAJOR_VERSION = 6;
static constexpr int PLUTOSVG_MAJOR_VERSION = 0;
#endif

namespace {
struct Locals
{
  // Dynamic libraries
  DynamicLibrary freetype_library;
  std::once_flag freetype_init_flag;
  DynamicLibrary plutosvg_library;
  std::once_flag plutosvg_init_flag;
};
} // namespace

DynFreeType g_dyn_freetype;
DynPlutoSvg g_dyn_plutosvg;
static Locals s_locals;

static bool LoadDynLib(const char* libname, int major_version, DynamicLibrary& dynlib, std::once_flag& once_flag,
                       std::span<const DynamicLibrary::SymbolTable> symbols, Error* const error)
{
  std::call_once(once_flag, [&libname, &major_version, &dynlib, &symbols, error]() {
    Error lerror;
    DynamicLibrary lib;
    if (!lib.Open(DynamicLibrary::GetVersionedFilename(libname, major_version).c_str(), &lerror) ||
        !lib.ResolveSymbols(symbols.data(), symbols.size(), &lerror))
    {
      ERROR_LOG("Failed to load {}: {}", libname, lerror.GetDescription());
      Error::SetStringFmt(error, "Failed to load {}: {}", libname, lerror.GetDescription());
      return;
    }

    dynlib = std::move(lib);
  });

  return dynlib.IsOpen();
}

static const DynamicLibrary::SymbolTable s_freetype_symbols[] = {
#define RESOLVE_SYMBOL(F) {#F, (void**)&g_dyn_freetype.F},
  DYN_FREETYPE_FUNCTIONS(RESOLVE_SYMBOL)
#undef RESOLVE_SYMBOL
};

bool DynFreeType::Open(Error* const error)
{
  if (s_locals.freetype_library.IsOpen()) [[likely]]
    return true;

  return LoadDynLib("freetype", FREETYPE_MAJOR_VERSION, s_locals.freetype_library, s_locals.freetype_init_flag,
                    s_freetype_symbols, error);
}

static const DynamicLibrary::SymbolTable s_plutosvg_symbols[] = {
#define RESOLVE_SYMBOL(F) {#F, (void**)&g_dyn_plutosvg.F},
  DYN_PLUTOSVG_FUNCTIONS(RESOLVE_SYMBOL)
#undef RESOLVE_SYMBOL
};

bool DynPlutoSvg::Open(Error* const error)
{
  if (s_locals.plutosvg_library.IsOpen()) [[likely]]
    return true;

  return LoadDynLib("plutosvg", PLUTOSVG_MAJOR_VERSION, s_locals.plutosvg_library, s_locals.plutosvg_init_flag,
                    s_plutosvg_symbols, error);
}
