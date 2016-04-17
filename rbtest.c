/*
 * Copyright (c) 2016 University of South Florida
 * Created by Ethan Pollitt and Sakwannueng Trakoolshoke-Satian
 * All rights reserved.
 *
 * This file is part of Grouper.
 *
 *  Grouper is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Grouper is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Grouper.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>		// adds uintXX_t support
#include "rbtree.h"

int main(int argc, char* argv[]) {
	char rule[] = "10?001?0";
	uint64_t width = 8;
	uint32_t id = 1;

	Trace("Creating tree...\n");

	rb_tree* tree = CreateTree(id, rule, width);

	Trace("Tree created\n");

	// Print q_mask as test
	char* q_mask_char = (char*) malloc(sizeof(char) * (width + 1));
	for(int i = 0; i < width; i++) {
		q_mask_char[i] = tree->head->q_mask[i] + '0';
	}
	q_mask_char[width] = '\0';
	Trace("\nq_mask: %s\n", q_mask_char);

	// Print b_mask as test
	char* b_mask_char = (char*) malloc(sizeof(char) * (width + 1));
	for(int i = 0; i < width; i++) {
		b_mask_char[i] = tree->head->b_mask[i] + '0';
	}
	b_mask_char[width] = '\0';
	Trace("\nb_mask: %s\n", b_mask_char);

	return 0;
}
