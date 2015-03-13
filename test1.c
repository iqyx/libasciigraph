#include <stdio.h>
#include <stdint.h>

#include "asciigraph.h"


const struct asciigraph_dataset data1 = {
	.data = (int32_t[]) {
		234, 120, 0, 66, 110, 150, 160, 160, 170, 140,
		230, 480, 520, 630, 800, 965, 730, 650, 420, 580,
		234, 120, 0, 66, 110, 150, 160, 160, 170, 140,
		230, 480, 520, 630, 800, 965, 730, 650, 420, 580,
		234, 120, 0, 66, 110, 150, 160, 160, 170, 140,
		230, 480, 520, 630, 800, 965, 730, 650, 420, 580,
		234, 120, 0, 66, 110, 150, 160, 160, 170, 140,
		230, 480, 520, 630, 800, 965, 730, 650, 420, 580,
		234, 120, 0, 66, 110, 150, 160, 160, 170, 140,
		230, 480, 520, 630, 800, 965, 730, 650, 420, 580,
		234, 120, 0, 66, 110, 150, 160, 160, 170, 140,
		230, 480, 520, 630, 800, 965, 730, 650, 420, 580,
		234, 120, 0, 66, 110, 150, 160, 160, 170, 140,
		230, 480, 520, 630, 800, 965, 730, 650, 420, 580,
		234, 120, 0, 66, 110, 150, 160, 160, 170, 140,
		230, 480, 520, 630, 800, 965, 730, 650, 420, 580,
	},
	.data_len = 80,
	.min = 0,
	.max = 1000,
};


const struct asciigraph_style style1 = {
	.type = ASCIIGRAPH_HISTOGRAM_VERTICAL,
	.width = 80,
	.height = 1,
	.borders = ASCIIGRAPH_BOTTOM | ASCIIGRAPH_LEFT | ASCIIGRAPH_RIGHT | ASCIIGRAPH_TOP,
	//~ .borders = ASCIIGRAPH_LEFT | ASCIIGRAPH_BOTTOM | ASCIIGRAPH_RIGHT | ASCIIGRAPH_TOP,
	//~ .ticks = ASCIIGRAPH_LEFT | ASCIIGRAPH_BOTTOM | ASCIIGRAPH_RIGHT | ASCIIGRAPH_TOP,
	.tics = ASCIIGRAPH_LEFT | ASCIIGRAPH_BOTTOM,
	.grid = ASCIIGRAPH_HORIZONTAL,
	.border_width = 0,
	.border_height = 0	,
	.color_graph = "\x1b[34m",
	.color_box = "\x1b[33m",
	.color_tics = "\x1b[33m",
	.xtic_interval = 10,
	.ytic_interval = 2,
};


static void print_callback(const char *s) {
	printf("%s", s);
}

int main() {
	asciigraph_print(&data1, &style1, print_callback);
}

