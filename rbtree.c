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

/*
 * Creates a new tree with a head node. This function should be called before
 * inserting any new nodes into the tree.
 */
rb_tree* CreateTree(uint32_t id, char* rule, uint64_t width) {
	rb_tree* newTree;
	rb_node* head;

	// create q mask
	printf("Creating q_mask\n");
	uint8_t* q_mask = ParseQMask(rule, width);

	// create b mask
	printf("Creating b_mask\n");
	uint8_t* b_mask = ParseBMask(rule, width);

	printf("Allocating memory for tree\n");
	newTree = (rb_tree*) malloc(sizeof(rb_tree));
	if(newTree == NULL) {
		// Something went wrong!
		printf("Could not malloc memory needed for tree structure!\n");
		fflush(stdout);
		return NULL;
	}

	printf("Allocating memory for head node\n");
	head = (rb_node*) malloc(sizeof(rb_node));
	if(head == NULL) {
		printf("Could not malloc memory needed for head node!\n");
		fflush(stdout);
		return NULL;
	}

	printf("Setting node members\n");
	head->color = 0;	// root node is black
	head->id = id;
	head->q_mask = q_mask;
	head->b_mask = b_mask;
	head->left = NULL;
	head->right = NULL;
	head->parent = NULL;

	printf("Setting tree head node\n");
	newTree->head = head;

	printf("Done creating tree, returning\n");
	return newTree;
}

void FreeTree(rb_tree* tree) {
	rb_node* head = tree->head;
	FreeSubTree(head->left);
	FreeSubTree(head->right);
	free(head);
	free(tree);
}

void FreeSubTree(rb_node* node) {
	if(node->left)
		FreeSubTree(node->left);
	if(node->right)
		FreeSubTree(node->right);
	free(node);
}

int DeleteNode(rb_tree* tree, rb_node* node) {

}

/*
 * Inserts a new node into the tree. Call CreateTree before invoking this
 * function.
 */
int InsertNode(rb_tree* tree, uint32_t id, char* rule, uint64_t width) {
	if(tree == NULL) {
		printf("ERROR Please call CreateTree before trying to insert a node!");
		fflush(stdout);
		return 0;
	}

	rb_node* new_node = (rb_node*) malloc(sizeof(rb_node));
	new_node->id = id;
	new_node->color = 1;	// Set every new node to red
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->parent = NULL;
	new_node->q_mask = ParseQMask(rule, width);
	new_node->b_mask = ParseBMask(rule, width);

	// do BST insert before fixing up tree
	int success = _BSTInsert(tree, new_node);
	if(!success || new_node->parent == NULL) {	// If new_node->parent = null, raise error
		// Something went wrong with BST insert!
		printf("ERROR returned during BST insert!");
		fflush(stdout);
		return 0;
	}

	// Case 2: check if new_node->parent is red
	if(new_node->parent->color == 1) {
		rb_node* parent = new_node->parent;

		if(parent->parent == NULL) {	// No grandparent, therefore no uncle
			// This is an error case, the base node should be black!
			printf("ERROR no grandparent on red node!");
			fflush(stdout);
			return 0;
		}

		rb_node* grand_parent = parent->parent;
		rb_node* uncle;
		if(grand_parent->left == parent)
			uncle = grand_parent->right;
		else
			uncle = grand_parent->left;
		
		if(uncle != NULL && uncle->color == 1) {
			// Case 3: Recolor parent, uncle, grandparent, then fix up
			parent->color = 0;
			uncle->color = 0;
			grand_parent->color = 1;
			
			// TODO: Need to recolor the entire tree from bottom up?

		} else { 
			// Case 4: rotate and fix up using C5
			if(parent->left = new_node) {
				if(!RotateLeft(parent)) {
					// TODO: error!
					return 0;
				}
				new_node = new_node->left;
			} else {
				if(!RotateRight(parent)) {
					// TODO: error!
					return 0;
				}
				new_node = new_node->right;
			}
		}

		// Case 5: 
	}
}

/* !!LOCAL FUNCITON ONLY!!
 * do a Binary Search Tree insert for new_node on tree
 */
static int _BSTInsert(rb_tree* tree, rb_node* new_node) {
	rb_node* temp = tree->head;
	int flag = 1;
	while(flag) {
		if(temp == NULL) {	// check for error cond
			printf("ERROR while BST insert! (temp is NULL)");
			fflush(stdout);
			return 0;
		}
		if(new_node->id < temp->id) {
			// left tree
			if(temp->left == NULL) {
				flag = 0;
				temp->left = new_node;
				new_node->parent = temp;
			} else
				temp = temp->left;
		} else if (new_node->id > temp->id) {
			// right tree
			if(temp->right == NULL) {
				flag = 0;
				temp->right = new_node;
				new_node->parent = temp;
			} else
				temp = temp->right;
		} else {	// ID's are equal !ERROR!
			printf("ERROR while BST insert! (same ID inserted twice)");
			fflush(stdout);
			return 0;
		}
	}
	return 1;
}

rb_node* FindByKey(rb_tree* tree, uint32_t key) {

}

uint8_t* ParseQMask(char* rule, uint64_t width) {
	uint8_t* q_mask = (uint8_t*) calloc(1, sizeof(uint8_t) * width);
	for(int j = 0; j < width; ++j) {	// ignore compiler warning, max size will be 12k
		if (rule[j] == '?') {
			q_mask[PackingIndex(j)] = 0;
		} else {
			q_mask[PackingIndex(j)] = 1;
		}
	}
	return q_mask;
}

uint8_t* ParseBMask(char* rule, uint64_t width) {
	uint8_t* b_mask = (uint8_t*) calloc(1, sizeof(uint8_t) * width);
	for(int j = 0; j < strlen(rule); ++j) { // ignore compiler warning, max size will be 12k
		if (rule[j] == '0' || rule[j]=='?') {
			b_mask[PackingIndex(j)] = 0;
		}
		if (rule[j] == '1') {
			b_mask[PackingIndex(j)] = 1;
		}
	}
	return b_mask;
}
