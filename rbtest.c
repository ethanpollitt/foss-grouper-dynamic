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

	printf("Creating tree...\n");
	fflush(stdout);

	rb_tree* tree = CreateTree(id, rule, width);

	printf("Tree created, exiting...\n");
	fflush(stdout);

	return 0;
}
