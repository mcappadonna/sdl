#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <wchar.h>
#define SDL_MAIN_USE_CALLBACKS  1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_Texture *texture = NULL;
static int texture_width = 0;
static int texture_height = 0;

#define TITLE "Color mods"
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
  SDL_SetAppMetadata(TITLE, "1.0", "com.example.color-mods");
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize sdl: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  if (!SDL_CreateWindowAndRenderer(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("Couldn't initialize window/rendere: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  SDL_Surface *surface = NULL;
  char *png_path;
  SDL_asprintf(&png_path, "%s/../../data/sample.png", SDL_GetBasePath());
  surface = SDL_LoadPNG(png_path);
  if (!surface) {
    SDL_Log("Couldn't load png: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_free(png_path);

  texture_width = surface->w;
  texture_height = surface->h;

  texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture) {
    SDL_Log("Couldn't initialize texture: %s", SDL_GetError());
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
  SDL_FRect dst_rect;
  const double now = ((double) SDL_GetTicks()) / 1000.0;

  const float red = (float)(0.5 + 0.5 * SDL_sin(now));
  const float green = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
  const float blue = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));

  // Clean the renderer
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  // kill green and red from texture
  dst_rect.x = 0.0f;
  dst_rect.y = 0.0f;
  dst_rect.w = (float)texture_width;
  dst_rect.h = (float)texture_height;
  SDL_SetTextureColorModFloat(texture, 0.0f, 0.0f, 1.0f);
  SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

  // center this one and have it cycle through different modulation
  dst_rect.x = ((float) (WINDOW_WIDTH - texture_width)) / 2.0f;
  dst_rect.y = ((float) (WINDOW_HEIGHT - texture_height)) / 2.0f;
  SDL_SetTextureColorModFloat(texture, red, green, blue);
  SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

  // bottom right and red
  dst_rect.x = (float)(WINDOW_WIDTH - texture_width);
  dst_rect.y = (float)(WINDOW_HEIGHT - texture_height);
  SDL_SetTextureColorModFloat(texture, 1.0f, 0.0f, 0.0f);
  SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

  // Present everything
  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  SDL_DestroyTexture(texture);
}
