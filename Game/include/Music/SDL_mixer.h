/*
  SDL_mixer:  An audio mixer library based on the SDL library
  Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/**
 *  \file SDL_mixer.h
 *
 *  Header file for SDL_mixer library
 *
 * A simple library to play and mix sounds and musics
 */
#ifndef SDL_MIXER_H_
#define SDL_MIXER_H_

#include <SDL3/SDL.h>
#include <SDL3/SDL_begin_code.h>

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Printable format: "%d.%d.%d", MAJOR, MINOR, MICRO
 */
#define SDL_MIXER_MAJOR_VERSION 3
#define SDL_MIXER_MINOR_VERSION 0
#define SDL_MIXER_MICRO_VERSION 0

/**
 * This is the version number macro for the current SDL_mixer version.
 */
#define SDL_MIXER_VERSION \
    SDL_VERSIONNUM(SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_MICRO_VERSION)

/**
 * This macro will evaluate to true if compiled with SDL_mixer at least X.Y.Z.
 */
#define SDL_MIXER_VERSION_ATLEAST(X, Y, Z) \
    ((SDL_MIXER_MAJOR_VERSION >= X) && \
     (SDL_MIXER_MAJOR_VERSION > X || SDL_MIXER_MINOR_VERSION >= Y) && \
     (SDL_MIXER_MAJOR_VERSION > X || SDL_MIXER_MINOR_VERSION > Y || SDL_MIXER_MICRO_VERSION >= Z))

/**
 * This function gets the version of the dynamically linked SDL_mixer library.
 *
 * \returns SDL_mixer version.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */
extern SDL_DECLSPEC int SDLCALL Mix_Version(void);

/**
 * Initialization flags
 */
typedef Uint32 MIX_InitFlags;

#define MIX_INIT_FLAC       0x00000001
#define MIX_INIT_MOD        0x00000002
#define MIX_INIT_MP3        0x00000008
#define MIX_INIT_OGG        0x00000010
#define MIX_INIT_MID        0x00000020
#define MIX_INIT_OPUS       0x00000040
#define MIX_INIT_WAVPACK    0x00000080

/**
 * Initialize SDL_mixer.
 *
 * This function loads dynamic libraries that SDL_mixer needs, and prepares
 * them for use.
 *
 * Note that, unlike other SDL libraries, this call is optional! If you load a
 * music file, SDL_mixer will handle initialization on the fly. This function
 * will let you know, up front, whether a specific format will be available
 * for use.
 *
 * Flags should be one or more flags from MIX_InitFlags OR'd together. It
 * returns the flags successfully initialized, or 0 on failure.
 *
 * Currently, these flags are:
 *
 * - `MIX_INIT_FLAC`
 * - `MIX_INIT_MOD`
 * - `MIX_INIT_MP3`
 * - `MIX_INIT_OGG`
 * - `MIX_INIT_MID`
 * - `MIX_INIT_OPUS`
 * - `MIX_INIT_WAVPACK`
 *
 * More flags may be added in a future SDL_mixer release.
 *
 * This function may need to load external shared libraries to support various
 * codecs, which means this function can fail to initialize that support on an
 * otherwise-reasonable system if the library isn't available; this is not
 * just a question of exceptional circumstances like running out of memory at
 * startup!
 *
 * Note that you may call this function more than once to initialize with
 * additional flags. The return value will reflect both new flags that
 * successfully initialized, and also include flags that had previously been
 * initialized as well.
 *
 * As this will return previously-initialized flags, it's legal to call this
 * with zero (no flags set). This is a safe no-op that can be used to query
 * the current initialization state without changing it at all.
 *
 * Since this returns previously-initialized flags as well as new ones, and
 * you can call this with zero, you should not check for a zero return value
 * to determine an error condition. Instead, you should check to make sure all
 * the flags you require are set in the return value. If you have a game with
 * data in a specific format, this might be a fatal error. If you're a generic
 * media player, perhaps you are fine with only having WAV and MP3 support and
 * can live without Opus playback, even if you request support for everything.
 *
 * Unlike other SDL satellite libraries, calls to Mix_Init do not stack; a
 * single call to Mix_Quit() will deinitialize everything and does not have to
 * be paired with a matching Mix_Init call. For that reason, it's considered
 * best practices to have a single Mix_Init and Mix_Quit call in your program.
 * While this isn't required, be aware of the risks of deviating from that
 * behavior.
 *
 * After initializing SDL_mixer, the next step is to open an audio device to
 * prepare to play sound (with Mix_OpenAudio()), and load audio data to play
 * with that device.
 *
 * \param flags initialization flags, OR'd together.
 * \returns all currently initialized flags.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_Quit
 */
extern SDL_DECLSPEC MIX_InitFlags SDLCALL Mix_Init(MIX_InitFlags flags);

/**
 * Deinitialize SDL_mixer.
 *
 * This should be the last function you call in SDL_mixer, after freeing all
 * other resources and closing all audio devices. This will unload any shared
 * libraries it is using for various codecs.
 *
 * After this call, a call to Mix_Init(0) will return 0 (no codecs loaded).
 *
 * You can safely call Mix_Init() to reload various codec support after this
 * call.
 *
 * Unlike other SDL satellite libraries, calls to Mix_Init do not stack; a
 * single call to Mix_Quit() will deinitialize everything and does not have to
 * be paired with a matching Mix_Init call. For that reason, it's considered
 * best practices to have a single Mix_Init and Mix_Quit call in your program.
 * While this isn't required, be aware of the risks of deviating from that
 * behavior.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_Init
 */
extern SDL_DECLSPEC void SDLCALL Mix_Quit(void);


/**
 * The default mixer has 8 simultaneous mixing channels
 */
#ifndef MIX_CHANNELS
#define MIX_CHANNELS    8
#endif

/* Good default values for a PC soundcard */
#define MIX_DEFAULT_FREQUENCY   44100
#define MIX_DEFAULT_FORMAT      SDL_AUDIO_S16
#define MIX_DEFAULT_CHANNELS    2
#define MIX_MAX_VOLUME          128 /* Volume of a chunk */

/**
 * The internal format for an audio chunk
 */
typedef struct Mix_Chunk {
    int allocated;
    Uint8 *abuf;
    Uint32 alen;
    Uint8 volume;       /* Per-sample volume, 0-128 */
} Mix_Chunk;

/**
 * The different fading types supported
 */
typedef enum Mix_Fading {
    MIX_NO_FADING,
    MIX_FADING_OUT,
    MIX_FADING_IN
} Mix_Fading;

/**
 * These are types of music files (not libraries used to load them)
 */
typedef enum Mix_MusicType {
    MUS_NONE,
    MUS_WAV,
    MUS_MOD,
    MUS_MID,
    MUS_OGG,
    MUS_MP3,
    MUS_MP3_MAD_UNUSED,
    MUS_FLAC,
    MUS_MODPLUG_UNUSED,
    MUS_OPUS,
    MUS_WAVPACK,
    MUS_GME
} Mix_MusicType;

/**
 * The internal format for a music chunk interpreted via codecs
 */
typedef struct Mix_Music Mix_Music;

/**
 * Open an audio device for playback.
 *
 * An audio device is what generates sound, so the app must open one to make
 * noise.
 *
 * This function will check if SDL's audio system is initialized, and if not,
 * it will initialize it by calling `SDL_Init(SDL_INIT_AUDIO)` on your behalf.
 * You are free to (and encouraged to!) initialize it yourself before calling
 * this function, as this gives your program more control over the process.
 *
 * If you aren't particularly concerned with the specifics of the audio
 * device, and your data isn't in a specific format, you can pass a NULL for
 * the `spec` parameter and SDL_mixer will choose a reasonable default.
 * SDL_mixer will convert audio data you feed it to the hardware's format
 * behind the scenes.
 *
 * That being said, if you have control of your audio data and you know its
 * format ahead of time, you may save CPU time by opening the audio device in
 * that exact format so SDL_mixer does not have to spend time converting
 * anything behind the scenes, and can just pass the data straight through to
 * the hardware.
 *
 * The other reason to care about specific formats: if you plan to touch the
 * mix buffer directly (with Mix_SetPostMix, a registered effect, or
 * Mix_HookMusic), you might have code that expects it to be in a specific
 * format, and you should specify that here.
 *
 * This function allows you to select specific audio hardware on the system
 * with the `devid` parameter. If you specify 0, SDL_mixer will choose the
 * best default it can on your behalf (which, in many cases, is exactly what
 * you want anyhow). This is equivalent to specifying
 * `SDL_AUDIO_DEVICE_DEFAULT_OUTPUT`, but is less wordy. SDL_mixer does not
 * offer a mechanism to determine device IDs to open, but you can use
 * SDL_GetAudioOutputDevices() to get a list of available devices. If you do
 * this, be sure to call `SDL_Init(SDL_INIT_AUDIO)` first to initialize SDL's
 * audio system!
 *
 * If this function reports success, you are ready to start making noise! Load
 * some audio data and start playing!
 *
 * When done with an audio device, probably at the end of the program, the app
 * should close the audio with Mix_CloseAudio().
 *
 * \param devid the device name to open, or 0 for a reasonable default.
 * \param spec the audio format you'd like SDL_mixer to work in.
 * \returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_CloseAudio
 * \sa Mix_QuerySpec
 */
extern SDL_DECLSPEC bool SDLCALL Mix_OpenAudio(SDL_AudioDeviceID devid, const SDL_AudioSpec *spec);

/**
 * Suspend or resume the whole audio output.
 *
 * \param pause_on 1 to pause audio output, or 0 to resume.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */
extern SDL_DECLSPEC void SDLCALL Mix_PauseAudio(int pause_on);

/**
 * Find out what the actual audio device parameters are.
 *
 * Note this is only important if the app intends to touch the audio buffers
 * being sent to the hardware directly. If an app just wants to play audio
 * files and let SDL_mixer handle the low-level details, this function can
 * probably be ignored.
 *
 * If the audio device is not opened, this function will return 0.
 *
 * \param frequency On return, will be filled with the audio device's
 *                  frequency in Hz.
 * \param format On return, will be filled with the audio device's format.
 * \param channels On return, will be filled with the audio device's channel
 *                 count.
 * \returns true if the audio device has been opened, true otherwise.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_OpenAudio
 */
extern SDL_DECLSPEC bool SDLCALL Mix_QuerySpec(int *frequency, SDL_AudioFormat *format, int *channels);

/**
 * Dynamically change the number of channels managed by the mixer.
 *
 * SDL_mixer deals with "channels," which is not the same thing as the
 * mono/stereo channels; they might be better described as "tracks," as each
 * one corresponds to a separate source of audio data. Three different WAV
 * files playing at the same time would be three separate SDL_mixer channels,
 * for example.
 *
 * An app needs as many channels as it has audio data it wants to play
 * simultaneously, mixing them into a single stream to send to the audio
 * device.
 *
 * SDL_mixer allocates `MIX_CHANNELS` (currently 8) channels when you open an
 * audio device, which may be more than an app needs, but if the app needs
 * more or wants less, this function can change it.
 *
 * If decreasing the number of channels, any upper channels currently playing
 * are stopped. This will deregister all effects on those channels and call
 * any callback specified by Mix_ChannelFinished() for each removed channel.
 *
 * If `numchans` is less than zero, this will return the current number of
 * channels without changing anything.
 *
 * \param numchans the new number of channels, or < 0 to query current channel
 *                 count.
 * \returns the new number of allocated channels.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */
extern SDL_DECLSPEC int SDLCALL Mix_AllocateChannels(int numchans);

/**
 * Load a supported audio format into a chunk.
 *
 * SDL_mixer has two separate data structures for audio data. One it calls a
 * "chunk," which is meant to be a file completely decoded into memory up
 * front, and the other it calls "music" which is a file intended to be
 * decoded on demand. Originally, simple formats like uncompressed WAV files
 * were meant to be chunks and compressed things, like MP3s, were meant to be
 * music, and you would stream one thing for a game's music and make repeating
 * sound effects with the chunks.
 *
 * In modern times, this isn't split by format anymore, and most are
 * interchangeable, so the question is what the app thinks is worth
 * predecoding or not. Chunks might take more memory, but once they are loaded
 * won't need to decode again, whereas music always needs to be decoded on the
 * fly. Also, crucially, there are as many channels for chunks as the app can
 * allocate, but SDL_mixer only offers a single "music" channel.
 *
 * If `closeio` is true, the IOStream will be closed before returning, whether
 * this function succeeds or not. SDL_mixer reads everything it needs from the
 * IOStream during this call in any case.
 *
 * There is a separate function (a macro, before SDL_mixer 3.0.0) to read
 * files from disk without having to deal with SDL_IOStream:
 * `Mix_LoadWAV("filename.wav")` will call this function and manage those
 * details for you.
 *
 * When done with a chunk, the app should dispose of it with a call to
 * Mix_FreeChunk().
 *
 * \param src an SDL_IOStream that data will be read from.
 * \param closeio true to close the SDL_IOStream before returning, false to
 *                leave it open.
 * \returns a new chunk, or NULL on error.
 *
 * \since This function is available since SDL_mixer 3.0.0
 *
 * \sa Mix_LoadWAV
 * \sa Mix_FreeChunk
 */
extern SDL_DECLSPEC Mix_Chunk * SDLCALL Mix_LoadWAV_IO(SDL_IOStream *src, bool closeio);

/**
 * Load a supported audio format into a chunk.
 *
 * SDL_mixer has two separate data structures for audio data. One it calls a
 * "chunk," which is meant to be a file completely decoded into memory up
 * front, and the other it calls "music" which is a file intended to be
 * decoded on demand. Originally, simple formats like uncompressed WAV files
 * were meant to be chunks and compressed things, like MP3s, were meant to be
 * music, and you would stream one thing for a game's music and make repeating
 * sound effects with the chunks.
 *
 * In modern times, this isn't split by format anymore, and most are
 * interchangeable, so the question is what the app thinks is worth
 * predecoding or not. Chunks might take more memory, but once they are loaded
 * won't need to decode again, whereas music always needs to be decoded on the
 * fly. Also, crucially, there are as many channels for chunks as the app can
 * allocate, but SDL_mixer only offers a single "music" channel.
 *
 * If you would rather use the abstract SDL_IOStream interface to load data
 * from somewhere other than the filesystem, you can use Mix_LoadWAV_IO()
 * instead.
 *
 * When done with a chunk, the app should dispose of it with a call to
 * Mix_FreeChunk().
 *
 * Note that before SDL_mixer 3.0.0, this function was a macro that called
 * Mix_LoadWAV_IO(), creating a IOStream and setting `closeio` to true. This
 * macro has since been promoted to a proper API function. Older binaries
 * linked against a newer SDL_mixer will still call Mix_LoadWAV_IO directly,
 * as they are using the macro, which was available since the dawn of time.
 *
 * \param file the filesystem path to load data from.
 * \returns a new chunk, or NULL on error.
 *
 * \since This function is available since SDL_mixer 3.0.0
 *
 * \sa Mix_LoadWAV_IO
 * \sa Mix_FreeChunk
 */
extern SDL_DECLSPEC Mix_Chunk * SDLCALL Mix_LoadWAV(const char *file);


/**
 * Load a supported audio format into a music object.
 *
 * SDL_mixer has two separate data structures for audio data. One it calls a
 * "chunk," which is meant to be a file completely decoded into memory up
 * front, and the other it calls "music" which is a file intended to be
 * decoded on demand. Originally, simple formats like uncompressed WAV files
 * were meant to be chunks and compressed things, like MP3s, were meant to be
 * music, and you would stream one thing for a game's music and make repeating
 * sound effects with the chunks.
 *
 * In modern times, this isn't split by format anymore, and most are
 * interchangeable, so the question is what the app thinks is worth
 * predecoding or not. Chunks might take more memory, but once they are loaded
 * won't need to decode again, whereas music always needs to be decoded on the
 * fly. Also, crucially, there are as many channels for chunks as the app can
 * allocate, but SDL_mixer only offers a single "music" channel.
 *
 * When done with this music, the app should dispose of it with a call to
 * Mix_FreeMusic().
 *
 * \param file a file path from where to load music data.
 * \returns a new music object, or NULL on error.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_FreeMusic
 */
extern SDL_DECLSPEC Mix_Music * SDLCALL Mix_LoadMUS(const char *file);

/**
 * Load a supported audio format into a music object.
 *
 * SDL_mixer has two separate data structures for audio data. One it calls a
 * "chunk," which is meant to be a file completely decoded into memory up
 * front, and the other it calls "music" which is a file intended to be
 * decoded on demand. Originally, simple formats like uncompressed WAV files
 * were meant to be chunks and compressed things, like MP3s, were meant to be
 * music, and you would stream one thing for a game's music and make repeating
 * sound effects with the chunks.
 *
 * In modern times, this isn't split by format anymore, and most are
 * interchangeable, so the question is what the app thinks is worth
 * predecoding or not. Chunks might take more memory, but once they are loaded
 * won't need to decode again, whereas music always needs to be decoded on the
 * fly. Also, crucially, there are as many channels for chunks as the app can
 * allocate, but SDL_mixer only offers a single "music" channel.
 *
 * If `closeio` is true, the IOStream will be closed before returning, whether
 * this function succeeds or not. SDL_mixer reads everything it needs from the
 * IOStream during this call in any case.
 *
 * As a convenience, there is a function to read files from disk without
 * having to deal with SDL_IOStream: `Mix_LoadMUS("filename.mp3")` will manage
 * those details for you.
 *
 * This function attempts to guess the file format from incoming data. If the
 * caller knows the format, or wants to force it, it should use
 * Mix_LoadMUSType_IO() instead.
 *
 * When done with this music, the app should dispose of it with a call to
 * Mix_FreeMusic().
 *
 * \param src an SDL_IOStream that data will be read from.
 * \param closeio true to close the SDL_IOStream before returning, false to
 *                leave it open.
 * \returns a new music object, or NULL on error.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_FreeMusic
 */
extern SDL_DECLSPEC Mix_Music * SDLCALL Mix_LoadMUS_IO(SDL_IOStream *src, bool closeio);

/**
 * Load an audio format into a music object, assuming a specific format.
 *
 * SDL_mixer has two separate data structures for audio data. One it calls a
 * "chunk," which is meant to be a file completely decoded into memory up
 * front, and the other it calls "music" which is a file intended to be
 * decoded on demand. Originally, simple formats like uncompressed WAV files
 * were meant to be chunks and compressed things, like MP3s, were meant to be
 * music, and you would stream one thing for a game's music and make repeating
 * sound effects with the chunks.
 *
 * In modern times, this isn't split by format anymore, and most are
 * interchangeable, so the question is what the app thinks is worth
 * predecoding or not. Chunks might take more memory, but once they are loaded
 * won't need to decode again, whereas music always needs to be decoded on the
 * fly. Also, crucially, there are as many channels for chunks as the app can
 * allocate, but SDL_mixer only offers a single "music" channel.
 *
 * This function loads music data, and lets the application specify the type
 * of music being loaded, which might be useful if SDL_mixer cannot figure it
 * out from the data stream itself.
 *
 * Currently, the following types are supported:
 *
 * - `MUS_NONE` (SDL_mixer should guess, based on the data)
 * - `MUS_WAV` (Microsoft WAV files)
 * - `MUS_MOD` (Various tracker formats)
 * - `MUS_MID` (MIDI files)
 * - `MUS_OGG` (Ogg Vorbis files)
 * - `MUS_MP3` (MP3 files)
 * - `MUS_FLAC` (FLAC files)
 * - `MUS_OPUS` (Opus files)
 * - `MUS_WAVPACK` (WavPack files)
 *
 * If `closeio` is true, the IOStream will be closed before returning, whether
 * this function succeeds or not. SDL_mixer reads everything it needs from the
 * IOStream during this call in any case.
 *
 * As a convenience, there is a function to read files from disk without
 * having to deal with SDL_IOStream: `Mix_LoadMUS("filename.mp3")` will manage
 * those details for you (but not let you specify the music type explicitly)..
 *
 * When done with this music, the app should dispose of it with a call to
 * Mix_FreeMusic().
 *
 * \param src an SDL_IOStream that data will be read from.
 * \param type the type of audio data provided by `src`.
 * \param closeio true to close the SDL_IOStream before returning, false to
 *                leave it open.
 * \returns a new music object, or NULL on error.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_FreeMusic
 */
extern SDL_DECLSPEC Mix_Music * SDLCALL Mix_LoadMUSType_IO(SDL_IOStream *src, Mix_MusicType type, bool closeio);

/**
 * Load a WAV file from memory as quickly as possible.
 *
 * Unlike Mix_LoadWAV_IO, this function has several requirements, and unless
 * you control all your audio data and know what you're doing, you should
 * consider this function unsafe and not use it.
 *
 * - The provided audio data MUST be in Microsoft WAV format.
 * - The provided audio data shouldn't use any strange WAV extensions.
 * - The audio data MUST be in the exact same format as the audio device. This
 *   function will not attempt to convert it, or even verify it's in the right
 *   format.
 * - The audio data must be valid; this function does not know the size of the
 *   memory buffer, so if the WAV data is corrupted, it can read past the end
 *   of the buffer, causing a crash.
 * - The audio data must live at least as long as the returned Mix_Chunk,
 *   because SDL_mixer will use that data directly and not make a copy of it.
 *
 * This function will do NO error checking! Be extremely careful here!
 *
 * (Seriously, use Mix_LoadWAV_IO instead.)
 *
 * If this function is successful, the provided memory buffer must remain
 * available until Mix_FreeChunk() is called on the returned chunk.
 *
 * \param mem memory buffer containing of a WAV file.
 * \returns a new chunk, or NULL on error.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_LoadWAV_IO
 * \sa Mix_FreeChunk
 */
extern SDL_DECLSPEC Mix_Chunk * SDLCALL Mix_QuickLoad_WAV(Uint8 *mem);

/**
 * Load a raw audio data from memory as quickly as possible.
 *
 * The audio data MUST be in the exact same format as the audio device. This
 * function will not attempt to convert it, or even verify it's in the right
 * format.
 *
 * If this function is successful, the provided memory buffer must remain
 * available until Mix_FreeChunk() is called on the returned chunk.
 *
 * \param mem memory buffer containing raw PCM data.
 * \param len length of buffer pointed to by `mem`, in bytes.
 * \returns a new chunk, or NULL on error.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_FreeChunk
 */
extern SDL_DECLSPEC Mix_Chunk * SDLCALL Mix_QuickLoad_RAW(Uint8 *mem, Uint32 len);

/**
 * Free an audio chunk.
 *
 * An app should call this function when it is done with a Mix_Chunk and wants
 * to dispose of its resources.
 *
 * SDL_mixer will stop any channels this chunk is currently playing on. This
 * will deregister all effects on those channels and call any callback
 * specified by Mix_ChannelFinished() for each removed channel.
 *
 * \param chunk the chunk to free.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_LoadWAV
 * \sa Mix_LoadWAV_IO
 * \sa Mix_QuickLoad_WAV
 * \sa Mix_QuickLoad_RAW
 */
extern SDL_DECLSPEC void SDLCALL Mix_FreeChunk(Mix_Chunk *chunk);

/**
 * Free a music object.
 *
 * If this music is currently playing, it will be stopped.
 *
 * If this music is in the process of fading out (via Mix_FadeOutMusic()),
 * this function will *block* until the fade completes. If you need to avoid
 * this, be sure to call Mix_HaltMusic() before freeing the music.
 *
 * \param music the music object to free.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_LoadMUS
 * \sa Mix_LoadMUS_IO
 * \sa Mix_LoadMUSType_IO
 */
extern SDL_DECLSPEC void SDLCALL Mix_FreeMusic(Mix_Music *music);

/**
 * Get a list of chunk decoders that this build of SDL_mixer provides.
 *
 * This list can change between builds AND runs of the program, if external
 * libraries that add functionality become available. You must successfully
 * call Mix_OpenAudio() before calling this function, as decoders are
 * activated at device open time.
 *
 * Appearing in this list doesn't promise your specific audio file will
 * decode...but it's handy to know if you have, say, a functioning Ogg Vorbis
 * install.
 *
 * These return values are static, read-only data; do not modify or free it.
 * The pointers remain valid until you call Mix_CloseAudio().
 *
 * \returns number of chunk decoders available.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_GetChunkDecoder
 * \sa Mix_HasChunkDecoder
 */
extern SDL_DECLSPEC int SDLCALL Mix_GetNumChunkDecoders(void);

/**
 * Get a chunk decoder's name.
 *
 * The requested decoder's index must be between zero and
 * Mix_GetNumChunkDecoders()-1. It's safe to call this with an invalid index;
 * this function will return NULL in that case.
 *
 * This list can change between builds AND runs of the program, if external
 * libraries that add functionality become available. You must successfully
 * call Mix_OpenAudio() before calling this function, as decoders are
 * activated at device open time.
 *
 * \param index index of the chunk decoder.
 * \returns the chunk decoder's name.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_GetNumChunkDecoders
 */
extern SDL_DECLSPEC const char * SDLCALL Mix_GetChunkDecoder(int index);

/**
 * Check if a chunk decoder is available by name.
 *
 * This result can change between builds AND runs of the program, if external
 * libraries that add functionality become available. You must successfully
 * call Mix_OpenAudio() before calling this function, as decoders are
 * activated at device open time.
 *
 * Decoder names are arbitrary but also obvious, so you have to know what
 * you're looking for ahead of time, but usually it's the file extension in
 * capital letters (some example names are "AIFF", "VOC", "WAV").
 *
 * \param name the decoder name to query.
 * \returns true if a decoder by that name is available, false otherwise.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_GetNumChunkDecoders
 * \sa Mix_GetChunkDecoder
 */
extern SDL_DECLSPEC bool SDLCALL Mix_HasChunkDecoder(const char *name);

/**
 * Get a list of music decoders that this build of SDL_mixer provides.
 *
 * This list can change between builds AND runs of the program, if external
 * libraries that add functionality become available. You must successfully
 * call Mix_OpenAudio() before calling this function, as decoders are
 * activated at device open time.
 *
 * Appearing in this list doesn't promise your specific audio file will
 * decode...but it's handy to know if you have, say, a functioning Ogg Vorbis
 * install.
 *
 * These return values are static, read-only data; do not modify or free it.
 * The pointers remain valid until you call Mix_CloseAudio().
 *
 * \returns number of music decoders available.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_GetMusicDecoder
 * \sa Mix_HasMusicDecoder
 */
extern SDL_DECLSPEC int SDLCALL Mix_GetNumMusicDecoders(void);

/**
 * Get a music decoder's name.
 *
 * The requested decoder's index must be between zero and
 * Mix_GetNumMusicDecoders()-1. It's safe to call this with an invalid index;
 * this function will return NULL in that case.
 *
 * This list can change between builds AND runs of the program, if external
 * libraries that add functionality become available. You must successfully
 * call Mix_OpenAudio() before calling this function, as decoders are
 * activated at device open time.
 *
 * \param index index of the music decoder.
 * \returns the music decoder's name.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_GetNumMusicDecoders
 */
extern SDL_DECLSPEC const char * SDLCALL Mix_GetMusicDecoder(int index);

/**
 * Check if a music decoder is available by name.
 *
 * This result can change between builds AND runs of the program, if external
 * libraries that add functionality become available. You must successfully
 * call Mix_OpenAudio() before calling this function, as decoders are
 * activated at device open time.
 *
 * Decoder names are arbitrary but also obvious, so you have to know what
 * you're looking for ahead of time, but usually it's the file extension in
 * capital letters (some example names are "MOD", "MP3", "FLAC").
 *
 * \param name the decoder name to query.
 * \returns true if a decoder by that name is available, false otherwise.
 *
 * \since This function is available since SDL_mixer 3.0.0
 *
 * \sa Mix_GetNumMusicDecoders
 * \sa Mix_GetMusicDecoder
 */
extern SDL_DECLSPEC bool SDLCALL Mix_HasMusicDecoder(const char *name);

/**
 * Find out the format of a mixer music.
 *
 * If `music` is NULL, this will query the currently playing music (and return
 * MUS_NONE if nothing is currently playing).
 *
 * \param music the music object to query, or NULL for the currently-playing
 *              music.
 * \returns the Mix_MusicType for the music object.
 *
 * \since This function is available since SDL_mixer 3.0.0
 */
extern SDL_DECLSPEC Mix_MusicType SDLCALL Mix_GetMusicType(const Mix_Music *music);

/**
 * Get the title for a music object, or its filename.
 *
 * This returns format-specific metadata. Not all file formats supply this!
 *
 * If `music` is NULL, this will query the currently-playing music.
 *
 * If music's title tag is missing or empty, the filename will be returned. If
 * you'd rather have the actual metadata or nothing, use
 * Mix_GetMusicTitleTag() instead.
 *
 * Please note that if the music was loaded from an SDL_IOStream instead of a
 * filename, the filename returned will be an empty string ("").
 *
 * This function never returns NULL! If no data is available, it will return
 * an empty string ("").
 *
 * \param music the music object to query, or NULL for the currently-playing
 *              music.
 * \returns the music's title if available, or the filename if not, or "".
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_GetMusicTitleTag
 * \sa Mix_GetMusicArtistTag
 * \sa Mix_GetMusicAlbumTag
 * \sa Mix_GetMusicCopyrightTag
 */
extern SDL_DECLSPEC const char *SDLCALL Mix_GetMusicTitle(const Mix_Music *music);

/**
 * Get the title for a music object.
 *
 * This returns format-specific metadata. Not all file formats supply this!
 *
 * If `music` is NULL, this will query the currently-playing music.
 *
 * Unlike this function, Mix_GetMusicTitle() produce a string with the music's
 * filename if a title isn't available, which might be preferable for some
 * applications.
 *
 * This function never returns NULL! If no data is available, it will return
 * an empty string ("").
 *
 * \param music the music object to query, or NULL for the currently-playing
 *              music.
 * \returns the music's title if available, or "".
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_GetMusicTitle
 * \sa Mix_GetMusicArtistTag
 * \sa Mix_GetMusicAlbumTag
 * \sa Mix_GetMusicCopyrightTag
 */
extern SDL_DECLSPEC const char *SDLCALL Mix_GetMusicTitleTag(const Mix_Music *music);

/**
 * Get the artist name for a music object.
 *
 * This returns format-specific metadata. Not all file formats supply this!
 *
 * If `music` is NULL, this will query the currently-playing music.
 *
 * This function never returns NULL! If no data is available, it will return
 * an empty string ("").
 *
 * \param music the music object to query, or NULL for the currently-playing
 *              music.
 * \returns the music's artist name if available, or "".
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_GetMusicTitleTag
 * \sa Mix_GetMusicAlbumTag
 * \sa Mix_GetMusicCopyrightTag
 */
extern SDL_DECLSPEC const char *SDLCALL Mix_GetMusicArtistTag(const Mix_Music *music);

/**
 * Get the album name for a music object.
 *
 * This returns format-specific metadata. Not all file formats supply this!
 *
 * If `music` is NULL, this will query the currently-playing music.
 *
 * This function never returns NULL! If no data is available, it will return
 * an empty string ("").
 *
 * \param music the music object to query, or NULL for the currently-playing
 *              music.
 * \returns the music's album name if available, or "".
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_GetMusicTitleTag
 * \sa Mix_GetMusicArtistTag
 * \sa Mix_GetMusicCopyrightTag
 */
extern SDL_DECLSPEC const char *SDLCALL Mix_GetMusicAlbumTag(const Mix_Music *music);

/**
 * Get the copyright text for a music object.
 *
 * This returns format-specific metadata. Not all file formats supply this!
 *
 * If `music` is NULL, this will query the currently-playing music.
 *
 * This function never returns NULL! If no data is available, it will return
 * an empty string ("").
 *
 * \param music the music object to query, or NULL for the currently-playing
 *              music.
 * \returns the music's copyright text if available, or "".
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_GetMusicTitleTag
 * \sa Mix_GetMusicArtistTag
 * \sa Mix_GetMusicAlbumTag
 */
extern SDL_DECLSPEC const char *SDLCALL Mix_GetMusicCopyrightTag(const Mix_Music *music);

typedef void (SDLCALL *Mix_MixCallback)(void *udata, Uint8 *stream, int len);

/**
 * Set a function that is called after all mixing is performed.
 *
 * This can be used to provide real-time visual display of the audio stream or
 * add a custom mixer filter for the stream data.
 *
 * The callback will fire every time SDL_mixer is ready to supply more data to
 * the audio device, after it has finished all its mixing work. This runs
 * inside an SDL audio callback, so it's important that the callback return
 * quickly, or there could be problems in the audio playback.
 *
 * The data provided to the callback is in the format that the audio device
 * was opened in, and it represents the exact waveform SDL_mixer has mixed
 * from all playing chunks and music for playback. You are allowed to modify
 * the data, but it cannot be resized (so you can't add a reverb effect that
 * goes past the end of the buffer without saving some state between runs to
 * add it into the next callback, or resample the buffer to a smaller size to
 * speed it up, etc).
 *
 * The `arg` pointer supplied here is passed to the callback as-is, for
 * whatever the callback might want to do with it (keep track of some ongoing
 * state, settings, etc).
 *
 * Passing a NULL callback disables the post-mix callback until such a time as
 * a new one callback is set.
 *
 * There is only one callback available. If you need to mix multiple inputs,
 * be prepared to handle them from a single function.
 *
 * \param mix_func the callback function to become the new post-mix callback.
 * \param arg a pointer that is passed, untouched, to the callback.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_HookMusic
 */
extern SDL_DECLSPEC void SDLCALL Mix_SetPostMix(Mix_MixCallback mix_func, void *arg);

/**
 * Add your own music player or additional mixer function.
 *
 * This works something like Mix_SetPostMix(), but it has some crucial
 * differences. Note that an app can use this _and_ Mix_SetPostMix() at the
 * same time. This allows an app to replace the built-in music playback,
 * either with it's own music decoder or with some sort of
 * procedurally-generated audio output.
 *
 * The supplied callback will fire every time SDL_mixer is preparing to supply
 * more data to the audio device. This runs inside an SDL audio callback, so
 * it's important that the callback return quickly, or there could be problems
 * in the audio playback.
 *
 * Running this callback is the first thing SDL_mixer will do when starting to
 * mix more audio. The buffer will contain silence upon entry, so the callback
 * does not need to mix into existing data or initialize the buffer.
 *
 * Note that while a callback is set through this function, SDL_mixer will not
 * mix any playing music; this callback is used instead. To disable this
 * callback (and thus reenable built-in music playback) call this function
 * with a NULL callback.
 *
 * The data written to by the callback is in the format that the audio device
 * was opened in, and upon return from the callback, SDL_mixer will mix any
 * playing chunks (but not music!) into the buffer. The callback cannot resize
 * the buffer (so you must be prepared to provide exactly the amount of data
 * demanded or leave it as silence).
 *
 * The `arg` pointer supplied here is passed to the callback as-is, for
 * whatever the callback might want to do with it (keep track of some ongoing
 * state, settings, etc).
 *
 * As there is only one music "channel" mixed, there is only one callback
 * available. If you need to mix multiple inputs, be prepared to handle them
 * from a single function.
 *
 * \param mix_func the callback function to become the new post-mix callback.
 * \param arg a pointer that is passed, untouched, to the callback.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_SetPostMix
 */
extern SDL_DECLSPEC void SDLCALL Mix_HookMusic(Mix_MixCallback mix_func, void *arg);

typedef void (SDLCALL *Mix_MusicFinishedCallback)(void);

/**
 * Set a callback that runs when a music object has stopped playing.
 *
 * This callback will fire when the currently-playing music has completed, or
 * when it has been explicitly stopped from a call to Mix_HaltMusic. As such,
 * this callback might fire from an arbitrary background thread at almost any
 * time; try to limit what you do here.
 *
 * It is legal to start a new music object playing in this callback (or
 * restart the one that just stopped). If the music finished normally, this
 * can be used to loop the music without a gap in the audio playback.
 *
 * Do not call SDL_LockAudio() from this callback; you will either be inside
 * the audio callback, or SDL_mixer will explicitly lock the audio before
 * calling your callback.
 *
 * A NULL pointer will disable the callback.
 *
 * \param music_finished the callback function to become the new notification
 *                       mechanism.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */
extern SDL_DECLSPEC void SDLCALL Mix_HookMusicFinished(Mix_MusicFinishedCallback music_finished);

/**
 * Get a pointer to the user data for the current music hook.
 *
 * This returns the `arg` pointer last passed to Mix_HookMusic(), or NULL if
 * that function has never been called.
 *
 * \returns pointer to the user data previously passed to Mix_HookMusic.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */
extern SDL_DECLSPEC void * SDLCALL Mix_GetMusicHookData(void);

typedef void (SDLCALL *Mix_ChannelFinishedCallback)(int channel);

/**
 * Set a callback that runs when a channel has finished playing.
 *
 * The callback may be called from the mixer's audio callback or it could be
 * called as a result of Mix_HaltChannel(), etc.
 *
 * The callback has a single parameter, `channel`, which says what mixer
 * channel has just stopped.
 *
 * Do not call SDL_LockAudio() from this callback; you will either be inside
 * the audio callback, or SDL_mixer will explicitly lock the audio before
 * calling your callback.
 *
 * A NULL pointer will disable the callback.
 *
 * \param channel_finished the callback function to become the new
 *                         notification mechanism.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */
extern SDL_DECLSPEC void SDLCALL Mix_ChannelFinished(Mix_ChannelFinishedCallback channel_finished);

/**
 * Magic number for effects to operate on the postmix instead of a channel.
 */
#define MIX_CHANNEL_POST  (-2)

/**
 * This is the format of a special effect callback:
 *
 * myeffect(int chan, void *stream, int len, void *udata);
 *
 * (chan) is the channel number that your effect is affecting. (stream) is the
 * buffer of data to work upon. (len) is the size of (stream), and (udata) is
 * a user-defined bit of data, which you pass as the last arg of
 * Mix_RegisterEffect(), and is passed back unmolested to your callback. Your
 * effect changes the contents of (stream) based on whatever parameters are
 * significant, or just leaves it be, if you prefer. You can do whatever you
 * like to the buffer, though, and it will continue in its changed state down
 * the mixing pipeline, through any other effect functions, then finally to be
 * mixed with the rest of the channels and music for the final output stream.
 *
 * DO NOT EVER call SDL_LockAudio() from your callback function!
 */
typedef void (SDLCALL *Mix_EffectFunc_t)(int chan, void *stream, int len, void *udata);

/**
 * This is a callback that signifies that a channel has finished all its loops
 * and has completed playback.
 *
 * This gets called if the buffer plays out normally, or if you call
 * Mix_HaltChannel(), implicitly stop a channel via Mix_AllocateChannels(), or
 * unregister a callback while it's still playing.
 *
 * DO NOT EVER call SDL_LockAudio() from your callback function!
 */
typedef void (SDLCALL *Mix_EffectDone_t)(int chan, void *udata);

/**
 * Register a special effect function.
 *
 * At mixing time, the channel data is copied into a buffer and passed through
 * each registered effect function. After it passes through all the functions,
 * it is mixed into the final output stream. The copy to buffer is performed
 * once, then each effect function performs on the output of the previous
 * effect. Understand that this extra copy to a buffer is not performed if
 * there are no effects registered for a given chunk, which saves CPU cycles,
 * and any given effect will be extra cycles, too, so it is crucial that your
 * code run fast. Also note that the data that your function is given is in
 * the format of the sound device, and not the format you gave to
 * Mix_OpenAudio(), although they may in reality be the same. This is an
 * unfortunate but necessary speed concern. Use Mix_QuerySpec() to determine
 * if you can handle the data before you register your effect, and take
 * appropriate actions.
 *
 * You may also specify a callback (Mix_EffectDone_t) that is called when the
 * channel finishes playing. This gives you a more fine-grained control than
 * Mix_ChannelFinished(), in case you need to free effect-specific resources,
 * etc. If you don't need this, you can specify NULL.
 *
 * You may set the callbacks before or after calling Mix_PlayChannel().
 *
 * Things like Mix_SetPanning() are just internal special effect functions, so
 * if you are using that, you've already incurred the overhead of a copy to a
 * separate buffer, and that these effects will be in the queue with any
 * functions you've registered. The list of registered effects for a channel
 * is reset when a chunk finishes playing, so you need to explicitly set them
 * with each call to Mix_PlayChannel*().
 *
 * You may also register a special effect function that is to be run after
 * final mixing occurs. The rules for these callbacks are identical to those
 * in Mix_RegisterEffect, but they are run after all the channels and the
 * music have been mixed into a single stream, whereas channel-specific
 * effects run on a given channel before any other mixing occurs. These global
 * effect callbacks are call "posteffects". Posteffects only have their
 * Mix_EffectDone_t function called when they are unregistered (since the main
 * output stream is never "done" in the same sense as a channel). You must
 * unregister them manually when you've had enough. Your callback will be told
 * that the channel being mixed is `MIX_CHANNEL_POST` if the processing is
 * considered a posteffect.
 *
 * After all these effects have finished processing, the callback registered
 * through Mix_SetPostMix() runs, and then the stream goes to the audio
 * device.
 *
 * DO NOT EVER call SDL_LockAudio() from your callback function! You are
 * already running in the audio thread and the lock is already held!
 *
 * Note that unlike most SDL and SDL_mixer functions, this function returns
 * zero if there's an error, not on success. We apologize for the API design
 * inconsistency here.
 *
 * \param chan the channel to register an effect to, or MIX_CHANNEL_POST.
 * \param f effect the callback to run when more of this channel is to be
 *          mixed.
 * \param d effect done callback.
 * \param arg argument.
 * \returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */
extern SDL_DECLSPEC bool SDLCALL Mix_RegisterEffect(int chan, Mix_EffectFunc_t f, Mix_EffectDone_t d, void *arg);


/**
 * Explicitly unregister a special effect function.
 *
 * You may not need to call this at all, unless you need to stop an effect
 * from processing in the middle of a chunk's playback.
 *
 * Posteffects are never implicitly unregistered as they are for channels (as
 * the output stream does not have an end), but they may be explicitly
 * unregistered through this function by specifying MIX_CHANNEL_POST for a
 * channel.
 *
 * Note that unlike most SDL and SDL_mixer functions, this function returns
 * zero if there's an error, not on success. We apologize for the API design
 * inconsistency here.
 *
 * \param channel the channel to unregister an effect on, or MIX_CHANNEL_POST.
 * \param f effect the callback stop calling in future mixing iterations.
 * \returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */
extern SDL_DECLSPEC bool SDLCALL Mix_UnregisterEffect(int channel, Mix_EffectFunc_t f);

/**
 * Explicitly unregister all special effect functions.
 *
 * You may not need to call this at all, unless you need to stop all effects
 * from processing in the middle of a chunk's playback.
 *
 * Note that this will also shut off some internal effect processing, since
 * Mix_SetPanning() and others may use this API under the hood. This is called
 * internally when a channel completes playback. Posteffects are never
 * implicitly unregistered as they are for channels, but they may be
 * explicitly unregistered through this function by specifying
 * MIX_CHANNEL_POST for a channel.
 *
 * Note that unlike most SDL and SDL_mixer functions, this function returns
 * zero if there's an error, not on success. We apologize for the API design
 * inconsistency here.
 *
 * \param channel the channel to unregister all effects on, or
 *                MIX_CHANNEL_POST.
 * \returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */
extern SDL_DECLSPEC bool SDLCALL Mix_UnregisterAllEffects(int channel);

/**
 * Environment variable that makes some mixing effects favor speed over
 * quality.
 */
#define MIX_EFFECTSMAXSPEED  "MIX_EFFECTSMAXSPEED"

/*
 * These are the internally-defined mixing effects. They use the same API that
 *  effects defined in the application use, but are provided here as a
 *  convenience. Some effects can reduce their quality or use more memory in
 *  the name of speed; to enable this, make sure the environment variable
 *  MIX_EFFECTSMAXSPEED (see above) is defined before you call
 *  Mix_OpenAudio().
 */


/**
 * Set the panning of a channel.
 *
 * The left and right channels are specified as integers between 0 and 255,
 * quietest to loudest, respectively.
 *
 * Technically, this is just individual volume control for a sample with two
 * (stereo) channels, so it can be used for more than just panning. If you
 * want real panning, call it like this:
 *
 * ```c
 * Mix_SetPanning(channel, left, 255 - left);
 * ```
 *
 * Setting `channel` to MIX_CHANNEL_POST registers this as a posteffect, and
 * the panning will be done to the final mixed stream before passing it on to
 * the audio device.
 *
 * This uses the Mix_RegisterEffect() API internally, and returns without
 * registering the effect function if the audio device is not configured for
 * stereo output. Setting both `left` and `right` to 255 causes this effect to
 * be unregistered, since that is the data's normal state.
 *
 * Note that an audio device in mono mode is a no-op, but this call will
 * return successful in that case. Error messages can be retrieved from
 * Mix_GetError().
 *
 * Note that unlike most SDL and SDL_mixer functions, this function returns
 * zero if there's an error, not on success. We apologize for the API design
 * inconsistency here.
 *
 * \param channel The mixer channel to pan or MIX_CHANNEL_POST.
 * \param left Volume of stereo left channel, 0 is silence, 255 is full
 *             volume.
 * \param right Volume of stereo right channel, 0 is silence, 255 is full
 *              volume.
 * \returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 *
 * \sa Mix_SetPosition
 * \sa Mix_SetDistance
 */
extern SDL_DECLSPEC bool SDLCALL Mix_SetPanning(int channel, Uint8 left, Uint8 right);


/**
 * Set the position of a channel.
 *
 * `angle` is an integer from 0 to 360, that specifies the location of the
 * sound in relation to the listener. `angle` will be reduced as necessary
 * (540 becomes 180 degrees, -100 becomes 260). Angle 0 is due north, and
 * rotates clockwise as the value increases. For efficiency, the precision of
 * this effect may be limited (angles 1 through 7 might all produce the same
 * effect, 8 through 15 are equal, etc). `distance` is an integer between 0
 * and 255 that specifies the space between the sound and the listener. The
 * larger the number, the further away the sound is. Using 255 does not
 * guarantee that the channel will be removed from the mixing process or be
 * completely silent. For efficiency, the precision of this effect may be
 * limited (distance 0 through 5 might all produce the same effect, 6 through
 * 10 are equal, etc). Setting `angle` and `distance` to 0 unregisters this
 * effect, since the data would be unchanged.
 *
 * If you need more precise positional audio, consider using OpenAL for
 * spatialized effects instead of SDL_mixer. This is only meant to be a basic
 * effect for simple "3D" games.
 *
 * If the audio device is configured for mono output, then you won't get any
 * effectiveness from the angle; however, distance attenuation on the channel
 * will still occur. While this effect will function with stereo voices, it
 * makes more sense to use voices with only one channel of sound, so when they
 * are mixed through this effect, the positioning will sound correct. You can
 * convert them to mono through SDL before giving them to the mixer in the
 * first place if you like.
 *
 * Setting the channel to MIX_CHANNEL_POST registers this as a posteffect, and
 * the positioning will be done to the final mixed stream before passing it on
 * to the audio device.
 *
 * This is a convenience wrapper over Mix_SetDistance() and Mix_SetPanning().
 *
 * Note that unlike most SDL and SDL_mixer functions, this function returns
 * zero if there's an error, not on success. We apologize for the API design
 * inconsistency here.
 *
 * \param channel The mixer channel to position, or MIX_CHANNEL_POST.
 * \param angle angle, in degrees. North is 0, and goes clockwise.
 * \param distance distance; 0 is the listener, 255 is maxiumum distance away.
 * \returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */
extern SDL_DECLSPEC bool SDLCALL Mix_SetPosition(int channel, Sint16 angle, Uint8 distance);



/**
 * Play an audio chunk on a specific channel, fading in the audio.
 *
 * This will start the new sound playing, much like Mix_PlayChannel() will,
 * but will start the sound playing at silence and fade in to its normal
 * volume over the specified number of milliseconds.
 *
 * If the specified channel is -1, play on the first free channel (and return
 * -1 without playing anything new if no free channel was available).
 *
 * If a specific channel was requested, and there is a chunk already playing
 * there, that chunk will be halted and the new chunk will take its place.
 *
 * If `loops` is greater than zero, loop the sound that many times. If `loops`
 * is -1, loop "infinitely" (~65000 times).
 *
 * A fading channel will change it's volume progressively, as if Mix_Volume()
 * was called on it (which is to say: you probably shouldn't call Mix_Volume()
 * on a fading channel).
 *
 * Note that before SDL_mixer 3.0.0, this function was a macro that called
 * Mix_FadeInChannelTimed() with a fourth parameter ("ticks") of -1. This
 * function still does the same thing, but promotes it to a proper API
 * function. Older binaries linked against a newer SDL_mixer will still call
 * Mix_FadeInChannelTimed directly, as they are using the macro, which was
 * available since the dawn of time.
 *
 * \param channel the channel on which to play the new chunk, or -1 to find
 *                any available.
 * \param chunk the new chunk to play.
 * \param loops the number of times the chunk should loop, -1 to loop (not
 *              actually) infinitely.
 * \param ms the number of milliseconds to spend fading in.
 * \returns which channel was used to play the sound, or -1 if sound could not
 *          be played.
 *
 * \since This function is available since SDL_mixer 3.0.0
 */
extern SDL_DECLSPEC int SDLCALL Mix_FadeInChannel(int channel, Mix_Chunk *chunk, int loops, int ms);

/**
 * Play an audio chunk on a specific channel, fading in the audio, for a
 * maximum time.
 *
 * This will start the new sound playing, much like Mix_PlayChannel() will,
 * but will start the sound playing at silence and fade in to its normal
 * volume over the specified number of milliseconds.
 *
 * If the specified channel is -1, play on the first free channel (and return
 * -1 without playing anything new if no free channel was available).
 *
 * If a specific channel was requested, and there is a chunk already playing
 * there, that chunk will be halted and the new chunk will take its place.
 *
 * If `loops` is greater than zero, loop the sound that many times. If `loops`
 * is -1, loop "infinitely" (~65000 times).
 *
 * `ticks` specifies the maximum number of milliseconds to play this chunk
 * before halting it. If you want the chunk to play until all data has been
 * mixed, specify -1.
 *
 * Note that this function does not block for the number of ticks requested;
 * it just schedules the chunk to play and notes the maximum for the mixer to
 * manage later, and returns immediately.
 *
 * A fading channel will change it's volume progressively, as if Mix_Volume()
 * was called on it (which is to say: you probably shouldn't call Mix_Volume()
 * on a fading channel).
 *
 * \param channel the channel on which to play the new chunk, or -1 to find
 *                any available.
 * \param chunk the new chunk to play.
 * \param loops the number of times the chunk should loop, -1 to loop (not
 *              actually) infinitely.
 * \param ms the number of milliseconds to spend fading in.
 * \param ticks the maximum number of milliseconds of this chunk to mix for
 *              playback.
 * \returns which channel was used to play the sound, or -1 if sound could not
 *          be played.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */

/**
 * Resume a particular channel.
 *
 * It is legal to resume an unpaused or invalid channel; it causes no effect
 * and reports no error.
 *
 * If the paused channel has an expiration, its expiration countdown resumes
 * now, as well.
 *
 * Specifying a channel of -1 will resume _all_ paused channels. Any music is
 * unaffected.
 *
 * \param channel the channel to resume, or -1 to resume all paused channels.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */
extern SDL_DECLSPEC void SDLCALL Mix_Resume(int channel);

/**
 * Resume playing of a group of channels by arbitrary tag.
 *
 * It is legal to resume an unpaused or invalid channel; it causes no effect
 * and reports no error.
 *
 * If the paused channel has an expiration, its expiration countdown resumes
 * now, as well.
 *
 * A tag is an arbitrary number that can be assigned to several mixer
 * channels, to form groups of channels.
 *
 * The default tag for a channel is -1.
 *
 * \param tag an arbitrary value, assigned to channels, to search for.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */
extern SDL_DECLSPEC void SDLCALL Mix_ResumeGroup(int tag);

/**
 * Query whether a particular channel is paused.
 *
 * If an invalid channel is specified, this function returns zero.
 *
 * \param channel the channel to query, or -1 to query all channels.
 * \return 1 if channel paused, 0 otherwise. If `channel` is -1, returns the
 *         number of paused channels.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */
extern SDL_DECLSPEC int SDLCALL Mix_Paused(int channel);

/**
 * Pause the music stream.
 *
 * Pausing the music stream will prevent further playback of the assigned
 * music object, but will maintain the object's current mixing position. When
 * resumed, this channel will continue to mix the music where it left off.
 *
 * Paused music can be resumed by calling Mix_ResumeMusic().
 *
 * It is legal to halt paused music. Playing a new music object when music is
 * paused will replace the current music and unpause the music stream.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */
extern SDL_DECLSPEC void SDLCALL Mix_PauseMusic(void);

/**
 * Resume the music stream.
 *
 * It is legal to resume an unpaused music stream; it causes no effect and
 * reports no error.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */
extern SDL_DECLSPEC void SDLCALL Mix_ResumeMusic(void);

/**
 * Rewind the music stream.
 *
 * This causes the currently-playing music to start mixing from the beginning
 * of the music, as if it were just started.
 *
 * It's a legal no-op to rewind the music stream when not playing.
 *
 * \since This function is available since SDL_mixer 3.0.0.
 */
extern SDL_DECLSPEC void SDLCALL Mix_RewindMusic(void);

/**
 * Query whether the music stream is paused.
 *
 * \return true if music is paused, false otherwise.
 *
 * \since This function is available since SDL_mixer 3.0.0.

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#include <SDL3/SDL_close_code.h>

#endif /* SDL_MIXER_H_ */