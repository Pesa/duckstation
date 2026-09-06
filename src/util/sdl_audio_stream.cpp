// SPDX-FileCopyrightText: 2019-2026 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: CC-BY-NC-ND-4.0

#include "audio_stream.h"
#include "dyn_sdl.h"

#include "common/assert.h"
#include "common/error.h"
#include "common/log.h"

LOG_CHANNEL(AudioStream);

namespace {

class SDLAudioStream final : public AudioStream
{
public:
  SDLAudioStream(AudioStreamSource* source, u32 channels);
  ~SDLAudioStream() override;

  bool Initialize(u32 sample_rate, u32 channels, u32 output_latency_frames, bool output_latency_minimal,
                  bool auto_start, Error* error);

  bool Start(Error* error) override;
  bool Stop(Error* error) override;

protected:
  static void AudioCallback(void* userdata, SDL_AudioStream* stream, int additional_amount, int total_amount);

  AudioStreamSource* m_source;
  SDL_AudioStream* m_sdl_stream = nullptr;
  u32 m_channels;
};
} // namespace

SDLAudioStream::SDLAudioStream(AudioStreamSource* source, u32 channels) : m_source(source), m_channels(channels)
{
}

SDLAudioStream::~SDLAudioStream()
{
  if (m_sdl_stream)
  {
    g_dyn_sdl.SDL_DestroyAudioStream(m_sdl_stream);
    m_sdl_stream = nullptr;
  }
}

bool SDLAudioStream::Initialize(u32 sample_rate, u32 channels, u32 output_latency_frames, bool output_latency_minimal,
                                bool auto_start, Error* error)
{
  const SDL_AudioSpec spec = {
    .format = SDL_AUDIO_S16LE, .channels = static_cast<int>(channels), .freq = static_cast<int>(sample_rate)};

  m_sdl_stream = g_dyn_sdl.SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, AudioCallback, this);
  if (!m_sdl_stream)
  {
    Error::SetStringFmt(error, "SDL_OpenAudioDeviceStream() failed: {}", g_dyn_sdl.SDL_GetError());
    return false;
  }

  if (auto_start)
    g_dyn_sdl.SDL_ResumeAudioStreamDevice(m_sdl_stream);

  return true;
}

bool SDLAudioStream::Start(Error* error)
{
  if (!g_dyn_sdl.SDL_ResumeAudioStreamDevice(m_sdl_stream))
  {
    Error::SetStringFmt(error, "SDL_ResumeAudioStreamDevice() failed: {}", g_dyn_sdl.SDL_GetError());
    return false;
  }

  return true;
}

bool SDLAudioStream::Stop(Error* error)
{
  if (!g_dyn_sdl.SDL_PauseAudioStreamDevice(m_sdl_stream))
  {
    Error::SetStringFmt(error, "SDL_PauseAudioStreamDevice() failed: {}", g_dyn_sdl.SDL_GetError());
    return false;
  }

  return true;
}

void SDLAudioStream::AudioCallback(void* userdata, SDL_AudioStream* stream, int additional_amount, int total_amount)
{
  if (additional_amount == 0)
    return;

  u8* data = SDL_stack_alloc(u8, additional_amount);
  if (data)
  {
    SDLAudioStream* const this_ptr = static_cast<SDLAudioStream*>(userdata);
    const u32 num_frames = static_cast<u32>(additional_amount) / (sizeof(SampleType) * this_ptr->m_channels);
    this_ptr->m_source->ReadFrames(reinterpret_cast<SampleType*>(data), num_frames);
    g_dyn_sdl.SDL_PutAudioStreamData(stream, data, additional_amount);
    SDL_stack_free(data);
  }
}

std::unique_ptr<AudioStream> AudioStream::CreateSDLAudioStream(u32 sample_rate, u32 channels, u32 output_latency_frames,
                                                               bool output_latency_minimal, AudioStreamSource* source,
                                                               bool auto_start, Error* error)
{
  // May as well keep it alive until the process exits.
  if (!g_dyn_sdl.Open(error) || !g_dyn_sdl.InitSubSystem(SDL_INIT_AUDIO, error))
    return {};

  std::unique_ptr<SDLAudioStream> stream = std::make_unique<SDLAudioStream>(source, channels);
  if (!stream->Initialize(sample_rate, channels, output_latency_frames, output_latency_minimal, auto_start, error))
    stream.reset();

  return stream;
}
