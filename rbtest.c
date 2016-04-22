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

#include <assert.h>
#include "rbtree.h"

int print_masks = 0;

void PrintMasks(rb_node* node, uint64_t width);

int main(int argc, char* argv[]) {
	char rule[] = "10?001?0";
	uint64_t width = 8;
	uint32_t id = 1;

	printf("Creating tree...\n");
	rb_tree* tree = CreateTree(id, rule, width);
	printf("Tree created\n");

	// Save first node for printing
	rb_node* first_node = tree->head;

	// Add first new node
	rb_node* new_node = InsertNode(tree, ++id, "101?101?", width);
	if(new_node == NULL) {
		printf("Error! 0 :)\n");
		fflush(stdout);
		return EXIT_FAILURE;
	}

	assert(new_node->parent == tree->head);

	/*!!! Add a few more new nodes !!!*/

	rb_node* new_node1 = InsertNode(tree, ++id, "1010101?", width);
	if(new_node1 == NULL) {
		printf("Error! 1 :)\n");
		fflush(stdout);
		return EXIT_FAILURE;
	}

	rb_node* new_node2 = InsertNode(tree, ++id, "1011101?", width);
	if(new_node2 == NULL) {
		printf("Error! 2 :)\n");
		fflush(stdout);
		return EXIT_FAILURE;
	}

	rb_node* new_node3 = InsertNode(tree, ++id, "10?1101?", width);
	if(new_node3 == NULL) {
		printf("Error! 3 :)\n");
		fflush(stdout);
		return EXIT_FAILURE;
	}

	rb_node* new_node4 = InsertNode(tree, ++id, "0001101?", width);
	if(new_node4 == NULL) {
		printf("Error! 4 :)\n");
		fflush(stdout);
		return EXIT_FAILURE;
	}

	rb_node* new_node5 = InsertNode(tree, ++id, "00?1101?", width);
	if(new_node5 == NULL) {
		printf("Error! 5 :)\n");
		fflush(stdout);
		return EXIT_FAILURE;
	}

	/* Print the results of the new tree */

	printf("\n====================================\n");
	printf("RESULTS\n");
	printf("====================================\n");
	PrintMasks(first_node, width);
	PrintMasks(new_node, width);
	PrintMasks(new_node1, width);
	PrintMasks(new_node2, width);
	PrintMasks(new_node3, width);
	PrintMasks(new_node4, width);
	PrintMasks(new_node5, width);

	fflush(stdout);
	fflush(stderr);

	return EXIT_SUCCESS;
}

void PrintMasks(rb_node* node, uint64_t width) {
	// Print q_mask as test
	char* q_mask_char = (char*) malloc(sizeof(char) * (width + 1));
	for(int i = 0; i < width; i++) {
		q_mask_char[i] = node->q_mask[i] + '0';
	}
	q_mask_char[width] = '\0';

	// Print b_mask as test
	char* b_mask_char = (char*) malloc(sizeof(char) * (width + 1));
	for(int i = 0; i < width; i++) {
		b_mask_char[i] = node->b_mask[i] + '0';
	}
	b_mask_char[width] = '\0';

	uint32_t parent_id = 0;
	if(node->parent != NULL)
		parent_id = node->parent->id;

	uint32_t left_id = 0;
	if(node->left != NULL)
		left_id = node->left->id;

	uint32_t right_id = 0;
	if(node->right != NULL)
		right_id = node->right->id;

	printf("((id: %i | c: %i | l: %i | r: %i | pid: %i))\n", node->id, node->color, left_id, right_id, parent_id);
	if(print_masks)
		printf(">> q: %s <> b: %s\n", q_mask_char, b_mask_char);
	fflush(stdout);
}
