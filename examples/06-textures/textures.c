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
#include <stdint.h>
#define SDL_MAIN_USE_CALLBACKS  1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

static SDL_Texture *texture = NULL;
static int texture_width = 0;
static int texture_height = 0;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
  // Initialize SDL
  SDL_SetAppMetadata("Textures", "1.0", "com.examples.textures");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("Textures", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("Couldn't initialize window/rendere: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  // Initialization
  SDL_Surface *surface = NULL;
  char *png_path = NULL;

  // SDL_Surface is pixel data that CPU can access.
  // SDL_Texture is pixel data that GPU can access.
  // Load a .png into a surface, move it to a texture there
  SDL_asprintf(&png_path, "%s/../../data/sample.png", SDL_GetBasePath());
  surface = SDL_LoadPNG(png_path);
  if (!surface) {
    SDL_Log("Couldn't load png: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  // done with this, the surface is loaded
  SDL_free(png_path);

  texture_width = surface->w;
  texture_height = surface->h;

  texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture) {
    SDL_Log("Couldn't create texture: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  // done with the surface, we have the texture
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
  // Start from black canvas
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  // We have some texture, let's move around that.
  SDL_FRect dst_rect;
  const Uint64 now = SDL_GetTicks();

  const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
  const float scale = ((float) (((int) (now % 1000)) - 500) / 500.0f) * direction;

  // top left
  dst_rect.x = (100.0f * scale);
  dst_rect.y = 0;
  dst_rect.w = (float)texture_width;
  dst_rect.h = (float)texture_height;
  SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

  // center
  dst_rect.x = ((float) (WINDOW_WIDTH - texture_width)) / 2.0f;
  dst_rect.y = ((float) (WINDOW_HEIGHT - texture_height)) / 2.0f;
  dst_rect.w = (float)texture_width;
  dst_rect.h = (float)texture_height;
  SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

  // bottom right
  dst_rect.x = ((float) (WINDOW_WIDTH - texture_width)) - (100.0f * scale);
  dst_rect.y = ((float) (WINDOW_HEIGHT - texture_height));
  dst_rect.w = (float)texture_width;
  dst_rect.h = (float)texture_height;
  SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

  // Present the renderer
  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}
