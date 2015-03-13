/*
 * Copyright (c) 2014, Marek Koza (qyx@krtko.org)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "asciigraph.h"

static const char *histogram_chars_vertical[] = {
	" ", "▁", "▂", "▃", "▄", "▅", "▆", "▇"
};

static const char *histogram_chars_horizontal[] = {
	" ", "▏", "▎", "▍", "▌", "▋", "▊", "▉"
};

static const char *box_chars[] = {
	" ",
	"╴", /* LINE_LEFT */
	"╶", /* LINE_RIGHT */
	"─", /* LINE_LEFT | LINE_RIGHT */
	"╵", /* LINE_UP */
	"┘", /* LINE_LEFT | LINE_UP */
	"└", /* LINE_RIGHT | LINE_UP */
	"┴", /* LINE_LEFT | LINE_RIGHT | LINE_UP */
	"╷", /* LINE_DOWN */
	"┐", /* LINE_LEFT | LINE_DOWN */
	"┌", /* LINE_RIGHT | LINE_DOWN */
	"┬", /* LINE_LEFT | LINE_RIGHT | LINE_DOWN */
	"│", /* LINE_UP | LINE_DOWN */
	"┤", /* LINE_LEFT | LINE_UP | LINE_DOWN */
	"├", /* LINE_RIGHT | LINE_UP | LINE_DOWN */
	"┼", /* LINE_LEFT | LINE_RIGHT | LINE_UP | LINE_DOWN */
};


int32_t asciigraph_print(struct asciigraph_dataset *data, struct asciigraph_style *style) {

	int32_t nw = ((style->borders & ASCIIGRAPH_LEFT) ? 1 : 0) + ((style->tics & ASCIIGRAPH_LEFT) ? style->border_width : 0);
	int32_t nh = ((style->borders & ASCIIGRAPH_BOTTOM) ? 1 : 0) + ((style->tics & ASCIIGRAPH_BOTTOM) ? style->border_height : 0);

	int32_t w = style->width + ((style->borders & ASCIIGRAPH_RIGHT) ? 1 : 0) + ((style->tics & ASCIIGRAPH_RIGHT) ? 1 + style->border_width : 0);
	int32_t h = style->height + ((style->borders & ASCIIGRAPH_TOP) ? 1 : 0) + ((style->tics & ASCIIGRAPH_TOP) ? 1 + style->border_height : 0);

	const char *last_clr = 0;
	const char *clr = 0;

	for (int32_t y = h - 1; y >= 0 - nh; y--) {
		for (int32_t x = 0 - nw; x < w; x++) {

			const char *c = FILL_EMPTY;

			/* By default draw lines in no directions. */
			uint8_t directions = 0;

			/* Add directions for vertical and horizontal lines. */
			if (x == -1 || x == style->width) {
				directions |= LINE_UP | LINE_DOWN;
				clr = style->color_box;
			}
			if (y == -1 || y == style->height) {
				directions |= LINE_LEFT | LINE_RIGHT;
				clr = style->color_box;
			}

			/* Y-tics */
			if (style->tics & ASCIIGRAPH_LEFT && y >= 0 && y < style->height) {
				if ((y % ASCIIGRAPH_YTICS_INT) == 0) {
					if (x < -1) {
						c = "x";
						clr = style->color_tics;
					}
					if (x == -1 && y != 0) {
						directions |= LINE_LEFT;
						clr = style->color_box;
					}
				}
			}

			/* X-tics */
			if (style->tics & ASCIIGRAPH_LEFT && x >= 0 && x < style->width) {
				if ((x % ASCIIGRAPH_XTICS_INT) == 0) {
					if (y < -1) {
						c = "x";
						clr = style->color_tics;
					}
					if (y == -1 && x != 0) {
						directions |= LINE_DOWN;
						clr = style->color_box;
					}
				}
			}

			if ((style->grid & ASCIIGRAPH_HORIZONTAL) && x >= -1 && x <= style->width && y > 0) {
				if ((y % ASCIIGRAPH_YTICS_INT) == 0) {
					directions |= LINE_LEFT | LINE_RIGHT;
				}
			}

			if ((style->grid & ASCIIGRAPH_VERTICAL) && y >= -1 && y <= style->height && x > 0) {
				if ((x % ASCIIGRAPH_XTICS_INT) == 0) {
					directions |= LINE_UP | LINE_DOWN;
				}
			}

			/* Remove directions if no ticks are displayed (the space
			 * behind the border is zero. */
			if (x == -1 && !(style->tics & ASCIIGRAPH_LEFT)) {
				directions &= ~LINE_LEFT;
			}
			if (x == style->width && !(style->tics & ASCIIGRAPH_RIGHT)) {
				directions &= ~LINE_RIGHT;
			}
			if (y == -1 && !(style->tics & ASCIIGRAPH_BOTTOM)) {
				directions &= ~LINE_DOWN;
			}
			if (y == style->height && !(style->tics & ASCIIGRAPH_TOP)) {
				directions &= ~LINE_UP;
			}


			/* This is the valid area where the graph is drawn. */
			if ((x >= 0 && x < style->width) && (y >= 0 && y < style->height)) {
				c = FILL_SHADE;
				clr = style->color_graph;

				if (style->type == ASCIIGRAPH_HISTOGRAM_VERTICAL) {
					int32_t value = data->data[x * data->data_len / style->width];

					if (value < data->min) {
						value = data->min;
					}
					if (value > data->max) {
						value = data->max;
					}
					value -= data->min;
					value = value * style->height * 8 / (data->max - data->min);

					if (value >= ((y + 1) * 8)) {
						directions = 0;
						c = FILL_FULL;
					} else if (value <= (y * 8)) {
						c = FILL_EMPTY;
						clr = style->color_box;
					} else {
						directions = 0;
						c = histogram_chars_vertical[value % 8];
					}
				}

				if (style->type == ASCIIGRAPH_HISTOGRAM_HORIZONTAL) {
					int32_t value = data->data[y * data->data_len / style->height];

					if (value < data->min) {
						value = data->min;
					}
					if (value > data->max) {
						value = data->max;
					}
					value -= data->min;
					value = value * style->width * 8 / (data->max - data->min);

					if (value >= ((x + 1) * 8)) {
						c = FILL_FULL;
					} else if (value <= (x * 8)) {
						c = FILL_EMPTY;
						clr = style->color_box;
					} else {
						c = histogram_chars_horizontal[value % 8];
					}
				}
			}

			/* Set a box drawing character according to the computed
			 * line directions (if line drawing is requested, box
			 * character is drawn instead of the one set to c. */
			if (directions != 0) {
				c = box_chars[directions];
			}

			if (clr != last_clr) {
				printf("%s", clr);
				last_clr = clr;
			}

			printf(c);
		}
		printf(ASCIIGRAPH_NEWLINE);
	}
	printf("\x1b[0m");

	return 0;
}


/* For temporary testing only. */
struct asciigraph_dataset data1 = {
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
	.max = 1200,
};


struct asciigraph_style style1 = {
	.type = ASCIIGRAPH_HISTOGRAM_VERTICAL,
	.width = 80,
	.height = 10,
	.borders = ASCIIGRAPH_BOTTOM | ASCIIGRAPH_LEFT | ASCIIGRAPH_TOP | ASCIIGRAPH_RIGHT,
	//~ .borders = ASCIIGRAPH_LEFT | ASCIIGRAPH_BOTTOM | ASCIIGRAPH_RIGHT | ASCIIGRAPH_TOP,
	//~ .ticks = ASCIIGRAPH_LEFT | ASCIIGRAPH_BOTTOM | ASCIIGRAPH_RIGHT | ASCIIGRAPH_TOP,
	.tics = ASCIIGRAPH_LEFT | ASCIIGRAPH_BOTTOM,
	.grid = ASCIIGRAPH_HORIZONTAL | ASCIIGRAPH_VERTICAL,
	.border_width = 5,
	.border_height = 1,
	.color_graph = "\x1b[34m",
	.color_box = "\x1b[33m",
	.color_tics = "\x1b[31m",
};


int main() {
	asciigraph_print(&data1, &style1);


}
