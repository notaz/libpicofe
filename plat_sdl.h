#include <SDL.h>

extern SDL_Surface *plat_sdl_screen;
extern SDL_Overlay *plat_sdl_overlay;
extern int plat_sdl_gl_active;

int plat_sdl_init(void);
int plat_sdl_change_video_mode(int w, int h, int force);
void plat_sdl_overlay_clear(void);
void plat_sdl_event_handler(void *event_);
void plat_sdl_finish(void);
