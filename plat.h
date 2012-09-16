#ifndef LIBPICOFE_PLAT_H
#define LIBPICOFE_PLAT_H

#ifdef __cplusplus
extern "C" {
#endif

/* target device, everything is optional */
struct plat_target {
	int (*cpu_clock_get)(void);
	int (*cpu_clock_set)(int clock);
	int (*get_bat_capacity)(void);
	void (*set_filter)(int which);
	char **filters;
	void (*set_lcdrate)(int is_pal);
	void (*step_volume)(int is_up);
};

extern struct plat_target plat_target;
int  plat_target_init(void);
void plat_target_finish(void);
void plat_target_setup_input(void);

static __inline void plat_target_set_filter(int which)
{
	if (plat_target.set_filter)
		plat_target.set_filter(which);
}

static __inline void plat_target_set_lcdrate(int is_pal)
{
	if (plat_target.set_lcdrate)
		plat_target.set_lcdrate(is_pal);
}

/* menu: enter (switch bpp, etc), begin/end drawing */
void plat_video_menu_enter(int is_rom_loaded);
void plat_video_menu_begin(void);
void plat_video_menu_end(void);
void plat_video_menu_leave(void);

void plat_video_flip(void);
void plat_video_wait_vsync(void);

/* return the dir/ where configs, saves, bios, etc. are found */
int  plat_get_root_dir(char *dst, int len);

int  plat_is_dir(const char *path);
int  plat_wait_event(int *fds_hnds, int count, int timeout_ms);
void plat_sleep_ms(int ms);

void *plat_mmap(unsigned long addr, size_t size, int need_exec, int is_fixed);
void *plat_mremap(void *ptr, size_t oldsize, size_t newsize);
void  plat_munmap(void *ptr, size_t size);

/* timers, to be used for time diff and must refer to the same clock */
unsigned int plat_get_ticks_ms(void);
unsigned int plat_get_ticks_us(void);
void plat_wait_till_us(unsigned int us);

void plat_debug_cat(char *str);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LIBPICOFE_PLAT_H */
