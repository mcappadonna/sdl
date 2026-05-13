#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_FPoint points[500];

// Initialize the app
SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
  int i;

  SDL_SetAppMetadata("Example Renderer Primitives", "1.0", "com.renderer.primitives");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("examples/renderer/primitives", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("Couldn't initialize window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  // set up some random points
  for (int i = 0; i < SDL_arraysize(points); i++) {
    points[i].x = (SDL_randf() * 440.0f) + 100.0f;
    points[i].y = (SDL_randf() * 280.0f) + 100.0f;
  }

  return SDL_APP_CONTINUE;
}

// Catch the events
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

// Every frame
SDL_AppResult SDL_AppIterate(void *appstate)
{
  SDL_FRect rect;

  // renderer draws over what is drawed before
  SDL_SetRenderDrawColor(renderer, 33, 33, 33, SDL_ALPHA_OPAQUE); // dark gray, full alpha
  SDL_RenderClear(renderer); // start with with canvas

  // draw a filled rectangle in the middle of the canvas
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE); // blue, full alpha
  rect.x = rect.y = 100;
  rect.w = 440;
  rect.h = 280;
  SDL_RenderFillRect(renderer, &rect);

  // draw some points across the canvas
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE); // red, full alpha
  SDL_RenderPoints(renderer, points, SDL_arraysize(points));

  // draw an unfilled rectangle in-set a little bit
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE); // green, full alpha
  rect.x += 30;
  rect.y += 30;
  rect.w -= 60;
  rect.h -= 60;
  SDL_RenderRect(renderer, &rect);

  // draw two lines in X across the whole canvas
  SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE); // yellow, full alpha
  SDL_RenderLine(renderer, 0, 0, 640, 480);
  SDL_RenderLine(renderer, 0, 480, 640, 0);

  // put all on screen
  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

// shutdown app
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}

