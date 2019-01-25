#ifndef RPI_H
#define RPI_H

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BCM2835_PERI_BASE		0x3f000000
#define GPIO_BASE				(BCM2835_PERI_BASE + 0x200000)	// GPIO controller
#define BLOCK_SIZE				(4*1024)
// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x)
#define INP_GPIO(g)				*(gpio.addr + ((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g)				*(gpio.addr + ((g)/10)) |=  (1<<(((g)%10)	*3))
//#define SET_GPIO_ALT(g,a) *(gpio.addr + (((g)/10))) |= (((a)<=3?(a) + 4:(a)==4?3:2)<<(((g)%10)*3))
#define GPIO_SET				*(gpio.addr + 7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR				*(gpio.addr + 10) // clears bits which are 1 ignores bits which are 0
//#define GPIO_READ(g)  *(gpio.addr + 13) &= (1<<(g))

// IO Access
struct bcm2835_peripheral {
    unsigned long addr_p;
    int mem_fd;
    void *map;
    volatile unsigned int *addr;
};

typedef struct line_info {
	int line_length;
	int words_count;
	int *chars_per_word_count;
} LINE_INFO;

typedef struct cpu_stats  {
	int *cpu_stats;
	int length;
} CPU_STATS;

extern struct bcm2835_peripheral gpio;
int map_peripheral(struct bcm2835_peripheral *);
void unmap_peripheral(struct bcm2835_peripheral *);
void enable_gpio_pins(unsigned int *, unsigned int);
int get_cpu_usage(int);
CPU_STATS *get_cpu_stats();
char **get_cpu_times(char *, LINE_INFO *);
int *cpu_times_to_int(char **, LINE_INFO *);
LINE_INFO *count_line_length(FILE *);
int summ_members_of_array(int *, int);


#endif
