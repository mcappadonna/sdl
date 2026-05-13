#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <wchar.h>
#define SDL_MAIN_USE_CALLBACKS  1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_Texture *texture = NULL;
static int texture_width = 0;
static int texture_height = 0;

#define TITLE "Scaling Textures"
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
  // Init sdl
  SDL_SetAppMetadata(TITLE, "1.0", "com.example.scaling-textures");
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize sdl: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  if (!SDL_CreateWindowAndRenderer(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("Coudln't initialize window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  SDL_Surface *surface = NULL;
  char *dst_png = NULL;
  SDL_asprintf(&dst_png, "%s/../../data/sample.png", SDL_GetBasePath());
  surface = SDL_LoadPNG(dst_png);
  if (!surface) {
    SDL_Log("Couldn't load png: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_free(dst_png);

  texture_width = surface->w;
  texture_height = surface->h;

  texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture) {
    SDL_Log("Couldn't create texture: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_DestroySurface(surface);

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
  Uint64 now = SDL_GetTicks();
  float direction = (((float) (now % 2000)) >= 1000) ? 1.0f : -1.0f;
  float scale = ((float) (((int) (now % 1000)) - 500) / 500.0f) * direction;
  SDL_FRect dst_rect;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  dst_rect.w = (float)texture_width + (texture_width * scale);
  dst_rect.h = (float)texture_height + (texture_height * scale);
  dst_rect.x = ((float) (WINDOW_WIDTH - texture_width)) / 2.0f;
  dst_rect.y = ((float) (WINDOW_HEIGHT - texture_height)) / 2.0f;
  SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  SDL_DestroyTexture(texture);
}
