/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2018 Sam Lantinga <slouken@libsdl.org>

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

#include "../../SDL_internal.h"
#include "SDL_hints.h"
#include "SDL_log.h"

#if SDL_VIDEO_DRIVER_SWITCH

#include "SDL_video.h"
#include "SDL_switchopengles.h"
#include "SDL_switchvideo.h"

/* EGL implementation of SDL OpenGL support */

void
SWITCH_GLES_DefaultProfileConfig(_THIS, int *mask, int *major, int *minor)
{
    /* lime-switch: Mesa on Switch exposes desktop OpenGL 4.3 through EGL, but
       OpenGL ES is still the safe default because most GLES-era shaders only
       compile against an ES context.

       A project opts into desktop GL with the Haxe define
       LIME_SWITCH_DESKTOP_GL. lime-switch turns that into a #define for the
       app's Main.cpp, which sets this hint before SDL starts; SDL itself is
       already compiled into liblime.a, so this is the only way to choose at
       build time without rebuilding the native library.

       The compatibility profile (not core) is deliberate: Lime and OpenFL
       draw with GLSL 1.10/1.20 code (attribute, varying, gl_FragColor), which
       a core profile rejects. */
    const char *profile = SDL_GetHint ("LIME_SWITCH_GL");
    SDL_bool desktop;

    if (profile != NULL) {
        desktop = (SDL_strcasecmp (profile, "desktop") == 0 || SDL_strcasecmp (profile, "gl") == 0) ? SDL_TRUE : SDL_FALSE;
    } else {
#ifdef LIME_SWITCH_DESKTOP_GL
        /* Global default, for a liblime.a rebuilt with this define. */
        desktop = SDL_TRUE;
#else
        desktop = SDL_FALSE;
#endif
    }

    if (desktop) {
        *mask = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;
        *major = 4;
        *minor = 3;
    } else {
        *mask = SDL_GL_CONTEXT_PROFILE_ES;
        *major = 2;
        *minor = 0;
    }
}

int
SWITCH_GLES_LoadLibrary(_THIS, const char *path)
{
    return SDL_EGL_LoadLibrary(_this, path, EGL_DEFAULT_DISPLAY, 0);
}

SDL_EGL_CreateContext_impl(SWITCH)
SDL_EGL_MakeCurrent_impl(SWITCH)
SDL_EGL_SwapWindow_impl(SWITCH)

#endif /* SDL_VIDEO_DRIVER_SWITCH */

/* vi: set ts=4 sw=4 expandtab: */
