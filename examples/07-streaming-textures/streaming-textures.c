#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <wchar.h>
#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

#define TEXTURE_SIZE  150

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
  // Initialize SDL
  SDL_SetAppMetadata("Streaming Textures", "1.0", "com.examples.streaming-textures");
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  if (!SDL_CreateWindowAndRenderer("Streaming Textures", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("Couldn't initialize window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  // Create the texture
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TEXTURE_SIZE, TEXTURE_SIZE);
  if (!texture) {
    SDL_Log("Couldn't initialize texture: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
  SDL_FRect dst_rect;
  const Uint64 now = SDL_GetTicks();
  SDL_Surface *surface = NULL;

  const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
  const float scale = ((float) (((int) (now % 1000)) - 500) / 500.0f) * direction;

  // SDL_LockTexture retrieve an array of raw pixels; we use SDL_LockTextureToSurface
  // to be able to use Surface method instead of dealing with single pixels
  if (SDL_LockTextureToSurface(texture, NULL, &surface)) {
    SDL_Rect r;
    SDL_FillSurfaceRect(surface, NULL, SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), NULL, 0, 0, 0));
    r.w = TEXTURE_SIZE;
    r.h = TEXTURE_SIZE / 10;
    r.x = 0;
    r.y = (int)(((float) (TEXTURE_SIZE - r.h)) * ((scale + 1.0f) / 2.0f));
    SDL_FillSurfaceRect(surface, &r, SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), NULL, 0, 255, 0));
    SDL_UnlockTexture(texture);
  }

  // start from black renderer
  SDL_SetRenderDrawColor(renderer, 66, 66, 66, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  // center this one, it'll draw the latest version of the texture
  dst_rect.x = ((float) (WINDOW_WIDTH - TEXTURE_SIZE)) / 2.0f;
  dst_rect.y = ((float) (WINDOW_HEIGHT - TEXTURE_SIZE)) / 2.0f;
  dst_rect.w = dst_rect.h = TEXTURE_SIZE;
  SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  SDL_DestroyTexture(texture);
}
