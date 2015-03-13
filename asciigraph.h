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

#ifndef _ASCIIGRAPH_H_
#define _ASCIIGRAPH_H_

#include <stdint.h>
#include <stdbool.h>

#define ASCIIGRAPH_XTICS_INT 10
#define ASCIIGRAPH_YTICS_INT 3

#define ASCIIGRAPH_NEWLINE "\n"

#define FILL_SHADE "░"
#define FILL_EMPTY " "
/* The full block fill is implemented as an inverted space. There is an UTF8
 * character for full block but it renders incorrectly with most fonts - there
 * is a gap between lines. */
//~ #define FILL_FULL "█"
#define FILL_FULL "\x1b[7m \x1b[27m"


#define LINE_LEFT 1
#define LINE_RIGHT 2
#define LINE_UP 4
#define LINE_DOWN 8

struct asciigraph_dataset {
	int32_t *data;
	size_t data_len;

	int32_t min;
	int32_t max;

};

#define ASCIIGRAPH_LEFT 1
#define ASCIIGRAPH_RIGHT 2
#define ASCIIGRAPH_TOP 4
#define ASCIIGRAPH_BOTTOM 8

#define ASCIIGRAPH_HORIZONTAL 16
#define ASCIIGRAPH_VERTICAL 32

enum asciigraph_type {
	ASCIIGRAPH_HISTOGRAM_VERTICAL,
	ASCIIGRAPH_HISTOGRAM_HORIZONTAL,
};

struct asciigraph_style {
	enum asciigraph_type type;

	uint16_t width;
	uint16_t height;

	uint8_t borders;
	uint8_t tics;
	uint8_t grid;
	uint16_t border_width;
	uint16_t border_height;

	const char *color_graph;
	const char *color_box;
	const char *color_tics;
};

int32_t asciigraph_print(struct asciigraph_dataset *data, struct asciigraph_style *style);





#endif

