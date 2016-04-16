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

#pragma once

#include <stdlib.h>             
#include <stdio.h>
#include <string.h>
#include <stdint.h>		// adds uintXX_t support
#include "xtrapbits.h"	// bit macros

//==================================
// Definitions
//==================================

typedef struct rb_node {
	uint32_t id;	// 64-bit ID, this is our key
	int color;		// 0 is black, 1 is red

	uint8_t* q_mask;
	uint8_t* b_mask;
	
	struct rb_node* left;
	struct rb_node* right;
	struct rb_node* parent;
} rb_node;

typedef struct rb_node {
	struct rb_node* head;
} rb_tree;

//==================================
// Function Prototypes
//==================================

// creates new tree with ID and rule as head node
rb_tree* CreateTree(uint32_t id, char* rule, uint64_t width);
void FreeTree(rb_tree* tree);
void FreeSubTree(rb_node* node);
void DeleteNode(rb_tree* tree, rb_node* node);
void InsertNode(rb_tree* tree, uint32_t id, char* rule);
rb_node* FindByKey(rb_tree* tree, uint32_t key);
uint8_t* ParseQMask(char* rule, uint64_t width);
uint8_t* ParseBMask(char* rule, uint64_t width);

//==================================
// Inline Functions
//==================================

/* Return index in packing order (msb in byte first) */
#define PackingIndex(bit) ((((bit)/BitsInByte)*BitsInByte)  \
                          + (BitsInByte - 1 - (bit)%BitsInByte))

