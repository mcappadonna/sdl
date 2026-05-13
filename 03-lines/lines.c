#define SDL_MAIN_USE_CALLBACKS  1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// Window and renderer
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

// Startup
SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
  SDL_SetAppMetadata("Example Renderer Lines", "1.0", "com.renderer.lines");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("examples/renderer/lines", 640, 489, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("Couldn't initialize window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);
  return SDL_APP_CONTINUE;
}

// Input
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
  int i;

  // Lines are sets of points, (x,y) for each end of the line
  static const SDL_FPoint line_points[] = {
    { 100, 354 }, { 220, 230 }, { 140, 230 }, { 320, 100 }, { 500, 230 },
    { 420, 230 }, { 540, 354 }, { 100, 354 }
  };

  // Renderer draws on top of the current screen
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE); // gray
  SDL_RenderClear(renderer); // start with a black canvas

  // Draw lines one at time, like these brown ones
  SDL_SetRenderDrawColor(renderer, 127, 49, 32, SDL_ALPHA_OPAQUE); // brown
  SDL_RenderLine(renderer, 240, 450, 400, 450);
  SDL_RenderLine(renderer, 240, 356, 400, 356);
  SDL_RenderLine(renderer, 240, 356, 240, 450);
  SDL_RenderLine(renderer, 400, 356, 400, 450);

  // Draw a series of connected lines
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE); // green
  SDL_RenderLines(renderer, line_points, SDL_arraysize(line_points));

  // Bunch of lines drawn out from a center point in a circle, with color
  // randomization
  for (i = 0; i <360; i++) {
    const float size = 30.0f;
    const float x = 320.0f;
    const float y = 95.0f - (size / 2.0f);
    const float r = (float)i * (SDL_PI_F / 180.0f);

    SDL_SetRenderDrawColor(renderer, SDL_rand(256), SDL_rand(256), SDL_rand(256), SDL_ALPHA_OPAQUE);
    SDL_RenderLine(renderer, x, y, x + SDL_cosf(r) * size, y + SDL_sinf(r) * size);
  }

  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}
