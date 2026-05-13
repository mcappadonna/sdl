#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <wchar.h>
#define SDL_MAIN_USE_CALLBACKS  1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static SDL_Texture *texture = NULL;
static int texture_width = 0;
static int texture_height = 0;

#define TITLE "Geometry"
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
  SDL_SetAppMetadata(TITLE, "1.0", "com.examples.geometry");
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize sdl: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  if (!SDL_CreateWindowAndRenderer(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("Couldn't initialize window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  SDL_Surface *surface;
  char *png_path;
  SDL_asprintf(&png_path, "%s../../data/sample.png", SDL_GetBasePath());
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
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  const Uint64 now = SDL_GetTicks();

  const float direction = (((float) (now % 2000)) >= 1000) ? 1.0f : -1.0f;
  const float scale = ((float) (((int) (now % 1000)) - 500) / 500.0f) * direction;
  const float size = 200.0f + (200.0f * scale);

  SDL_Vertex vertices[4];

  // draw a single triangle with a different color on each vertex
  SDL_zeroa(vertices);
  vertices[0].position.x = ((float) WINDOW_WIDTH) / 2.0f;
  vertices[0].position.y = ((float) WINDOW_HEIGHT - size) / 2.0f;
  vertices[0].color.a = 1.0f;
  vertices[0].color.r = 1.0f;
  vertices[1].position.x = (((float) WINDOW_WIDTH) + size) / 2.0f;
  vertices[1].position.y = (((float) WINDOW_HEIGHT) + size) / 2.0f;
  vertices[1].color.a = 1.0f;
  vertices[1].color.g = 1.0f;
  vertices[2].position.x = (((float) WINDOW_WIDTH) - size) / 2.0f;
  vertices[2].position.y = (((float) WINDOW_HEIGHT) + size) / 2.0f;
  vertices[2].color.a = 1.0f;
  vertices[2].color.b = 1.0f;
  SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);

  // map a texture to the geometry
  SDL_zeroa(vertices);
  vertices[0].position.x = 10.0f;
  vertices[0].position.y = 10.0f;
  vertices[0].color.a = vertices[0].color.r = vertices[0].color.g = vertices[0].color.b = 1.0f;
  vertices[0].tex_coord.x = 0.0f;
  vertices[0].tex_coord.y = 0.0f;
  vertices[1].position.x = 150.0f;
  vertices[1].position.y = 10.0f;
  vertices[1].color.a = vertices[1].color.r = vertices[1].color.g = vertices[1].color.b = 1.0f;
  vertices[1].tex_coord.x = 1.0f;
  vertices[1].tex_coord.y = 0.0f;
  vertices[2].position.x = 10.0f;
  vertices[2].position.y = 150.0f;
  vertices[2].color.a = vertices[2].color.r = vertices[2].color.g = vertices[2].color.b = 1.0f;
  vertices[2].tex_coord.x = 0.0f;
  vertices[2].tex_coord.y = 1.0f;
  SDL_RenderGeometry(renderer, texture, vertices, 3, NULL, 0);
  // ^ this is only half of the texture, let's draw another one but full

  // Let's shift to the right
  for (int i=0; i<3; i++) {
    vertices[i].position.x += 450;
  }
  vertices[3].position.x = 600.0f;
  vertices[3].position.y = 150.0f;
  vertices[3].color.a = vertices[3].color.r = vertices[3].color.g = vertices[3].color.b = 1.0f;
  vertices[3].tex_coord.x = 1.0f;
  vertices[3].tex_coord.y = 1.0f;
  // and add an index to reuse some of the vertex (faster)
  const int indices[] = { 0, 1 , 2, 1, 2, 3 };
  SDL_RenderGeometry(renderer, texture, vertices, 4, indices, SDL_arraysize(indices));

  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  SDL_DestroyTexture(texture);
}
