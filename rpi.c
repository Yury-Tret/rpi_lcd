#include "rpi.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

struct bcm2835_peripheral gpio = {GPIO_BASE};
// Exposes the physical address defined in the passed structure using mmap on /dev/mem
int map_peripheral(struct bcm2835_peripheral *p)
{
	// Open /dev/mem
	if ((p->mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
	{
		printf("Failed to open /dev/mem, try checking permissions.\n");
		return -1;
	}
	p->map = mmap(
	NULL,
	BLOCK_SIZE,
	PROT_READ|PROT_WRITE,
	MAP_SHARED,
	p->mem_fd,      // File descriptor to physical memory virtual file '/dev/mem'
	p->addr_p       // Address in physical map that we want this memory block to expose
	);
	if (p->map == MAP_FAILED)
	{
		perror("mmap");
		return -1;
	}
	p->addr = (volatile unsigned int *)p->map;
	return 0;
}

void unmap_peripheral(struct bcm2835_peripheral *p)
{
	munmap(p->map, BLOCK_SIZE);
	close(p->mem_fd);
}

void enable_gpio_pins(unsigned int *pins, unsigned int length)
{
	for (int i=0; i<length; i++)
	{
		INP_GPIO(pins[i]);
		OUT_GPIO(pins[i]);
		GPIO_CLR = 1 << pins[i];
	}
}

char **get_cpu_times(char *buffer, LINE_INFO *lip)
{
	char **cpu_times_array = (char **)calloc(lip->words_count, sizeof(double));
//	if (cpu_times_array == NULL)
//	{
//		printf("Calloc error\n");
//		return 0;
//	}
	int line_counter = 0;
	int char_counter = 0;

	*(cpu_times_array) = (char *)calloc(lip->chars_per_word_count[0], sizeof(char));
	for (int i=0; i <= strlen(buffer); i++ )
	{
		if (isdigit(*(buffer + i)) > 0)
		{
			memcpy((*(cpu_times_array + line_counter) + char_counter), buffer + i, 1);  /////////////////////
			char_counter++;
		}
		else
		{
			line_counter++;
			char_counter = 0;
			*(cpu_times_array + line_counter) = (char *)calloc(lip->chars_per_word_count[line_counter], sizeof(char));
		}
	}
	return cpu_times_array;
}

LINE_INFO *count_line_length(FILE *fp)
{
    LINE_INFO *lip = (LINE_INFO *)calloc(1, sizeof(LINE_INFO));

    int current_char = 0;
    int chars_counter = 0;
    int words_counter = 0;
    int chars_counter_per_word = 0;
    char flag = 0;

    while(1)
    {
    	current_char = fgetc(fp);
        if (current_char == EOF || current_char == '\n')
        {
            break;
        }
        chars_counter++;

        if (isdigit(current_char) > 0 && !flag)
        {
        	flag = 1;
        	words_counter++;
        }
        else if (isdigit(current_char) == 0)
        {
        	flag = 0;
        }
    }
    lip->line_length = chars_counter;
    lip->words_count = words_counter;
    lip->chars_per_word_count = (int *)calloc(words_counter, sizeof(int));

    fseek(fp, 0, SEEK_SET);							// rewind file pointer to begin of the file

    words_counter = 0;
    flag = 1;

    while(1)
    {
    	current_char = fgetc(fp);

		if (isdigit(current_char) > 0)
		{
			chars_counter_per_word++;
			flag = 0;
		}
		else if (!flag)
		{
			lip->chars_per_word_count[words_counter] = chars_counter_per_word;
			chars_counter_per_word = 0;
			words_counter++;
			flag = 1;
		}

		if (current_char == EOF || current_char == '\n')
		{
			break;
		}
    }

    return lip;
}

int *cpu_times_to_int(char **cpu_times_array, LINE_INFO *lip)
{
	int *cpu_times_int = (int *)calloc(lip->words_count, sizeof(int));
	for (int i=0; i<lip->words_count; i++)
	{
		cpu_times_int[i] = (int)strtol(cpu_times_array[i], NULL, 10);
	}
	return cpu_times_int;
}

CPU_STATS *get_cpu_stats()
{
	FILE *fp = fopen("/proc/stat", "r");
	if (fp == NULL)
	{
		printf("Error while opening file");
	}

	LINE_INFO *lip = count_line_length(fp);

	char buffer[lip->line_length + 1];

	fseek(fp, 0, SEEK_SET);							// rewind file pointer to begin of the file

	char **cpu_times_array;
	int *cpu_times_array_int;

	if (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		int shift = 0;
		while(shift < sizeof(buffer))
		{
			if (isdigit(buffer[shift]) > 0)
			{
				break;
			}
			shift++;
		}

		cpu_times_array = get_cpu_times(buffer + shift, lip);
		cpu_times_array_int = cpu_times_to_int(cpu_times_array, lip);

	}

	fclose(fp);

	CPU_STATS *cpu_stats = (CPU_STATS *)calloc(1, sizeof(CPU_STATS));
	cpu_stats->cpu_stats = cpu_times_array_int;
	cpu_stats->length = lip->words_count;

	for (int i=0; i<lip->words_count; i++)
	{
		free(cpu_times_array[i]);
	}
	free(cpu_times_array);
	free(lip->chars_per_word_count);
	free(lip);

	return cpu_stats;
}

int summ_members_of_array(int *array, int length)
{
	int result = 0;
	for (int i=0; i<length; i++)
	{
		result += array[i];
	}
	return result;
}

int get_cpu_usage(int period)
{
	CPU_STATS *first_cpu_stats = get_cpu_stats();
	sleep(period);
	CPU_STATS *second_cpu_stats = get_cpu_stats();

	int first_total = summ_members_of_array(first_cpu_stats->cpu_stats, first_cpu_stats->length);
	int second_total = summ_members_of_array(second_cpu_stats->cpu_stats, second_cpu_stats->length);
	float delta_total = second_total - first_total;
	float delta_idle = second_cpu_stats->cpu_stats[3] - first_cpu_stats->cpu_stats[3];
	float idle_fraction = delta_idle/delta_total;
	int cpu_usage = (int)((1 - idle_fraction)*100);

	free(first_cpu_stats->cpu_stats);
	free(second_cpu_stats->cpu_stats);
	free(first_cpu_stats);
	free(second_cpu_stats);

	return cpu_usage;
}
