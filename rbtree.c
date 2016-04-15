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

#include "rbtree.h"

rb_tree* CreateTree(uint32_t id, char *rule) {
	rb_tree* newTree;
	rb_node* head;

	// TODO: Need to calculate q and b masks

	newTree = (rb_tree*) malloc(sizeof(rb_tree));
	if(!newTree) {
		// Something went wrong!
	}

	head = (rb_node*) malloc(sizeof(rb_node));
	head->id = id;
	head->
}

void FreeTree(rb_tree *tree) {

}

void DeleteNode(rb_tree *tree, rb_node *node) {

}

void InsertNode(rb_tree *tree, uint32_t id, char *rule) {

}

rb_node* FindByKey(rb_tree *tree, uint32_t key) {

}
