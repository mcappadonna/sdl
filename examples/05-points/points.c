#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static Uint64 last_time = 0;

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

#define NUM_POINTS  500
#define MIN_PIXELS_PER_SECOND 30
#define MAX_PIXELS_PER_SECOND 60

// track everything as a parallel array so we can pass the coordinates to a
// single function call.
static SDL_FPoint points[NUM_POINTS];
static float point_speeds[NUM_POINTS];

// Initialize
SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
  SDL_SetAppMetadata("Points", "1.0", "com.examples.point");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  if (!SDL_CreateWindowAndRenderer("com/examples/point", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("Cloudn't initialize window/rendere: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  // set up the data for a bunch of points
  for (int i = 0; i < SDL_arraysize(points); i++) {
    points[i].x = SDL_randf() * WINDOW_WIDTH;
    points[i].y = SDL_randf() * WINDOW_HEIGHT;
    point_speeds[i] = MIN_PIXELS_PER_SECOND + (SDL_randf() * (MAX_PIXELS_PER_SECOND - MIN_PIXELS_PER_SECOND));
  }

  last_time = SDL_GetTicks();

  return SDL_APP_CONTINUE;
}

// Events
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
  const Uint64 now = SDL_GetTicks();
  const float elapsed = ((float) (now - last_time)) / 1000.0f; // seconds since last iteration
  
  for (int i = 0; i < SDL_arraysize(points); i++) {
    const float distance = elapsed * point_speeds[i];
    points[i].x += distance;
    points[i].y += distance;

    // off the screen, restart elsewhere
    if ((points[i].x >= WINDOW_WIDTH) || (points[i].y >= WINDOW_HEIGHT)) {
      if (SDL_rand(2)) {
        points[i].x = SDL_randf() * ((float) WINDOW_WIDTH);
        points[i].y = 0;
      }
      else {
        points[i].x = 0;
        points[i].y = SDL_randf() * ((float) WINDOW_HEIGHT);
      }
      point_speeds[i] = MIN_PIXELS_PER_SECOND + (SDL_randf() * (MAX_PIXELS_PER_SECOND - MIN_PIXELS_PER_SECOND));
    }
  }

  last_time = now;

  // Clear the screen
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  // Draw white points
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderPoints(renderer, points, SDL_arraysize(points));

  // Present the renderer
  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}
