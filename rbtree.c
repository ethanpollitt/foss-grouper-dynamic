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
rb_node* InsertNode(rb_tree* tree, uint32_t id, char* rule, uint64_t width) {
	if(tree == NULL) {
		printf("ERROR Please call CreateTree before trying to insert a node!\n");
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
	if(!success) {
		// Something went wrong with BST insert!
		printf("ERROR returned during BST insert!\n");
		fflush(stdout);
		return NULL;
	}

	/*
	 * Followed logical case breakdown as shown on the Red Black tree Wiki,
	 * along with examples from many other sites.
	 */

	// Do recursive calls to fix up tree
	if(!_InsertCase1(tree, new_node)) {
		printf("ERROR returned by RB insert\n");
		fflush(stdout);
		return NULL;
	}

	return new_node;
}

/* !!LOCAL FUNCITON ONLY!!
 * do a Binary Search Tree insert for new_node on tree
 */
static int _BSTInsert(rb_tree* tree, rb_node* new_node) {
	rb_node* temp = tree->head;
	int flag = 1;
	while(flag) {
		if(temp == NULL) {	// check for error cond
			printf("ERROR while BST insert! (temp is NULL)\n");
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
			printf("ERROR while BST insert! (same ID inserted twice)\n");
			fflush(stdout);
			return 0;
		}
	}
	return 1;
}

static int _InsertCase1(rb_tree* tree, rb_node* node) {
	if(node->parent == NULL) {
		node->color = 0;
		tree->head = node;
	} else
		return _InsertCase2(tree, node);
	PrintDebug("Node parent is null, making head of tree! (Case 1 complete)", node);
	return 1;
}

static int _InsertCase2(rb_tree* tree, rb_node* node) {
	if(node->parent->color == 1)
		return _InsertCase3(tree, node);
	PrintDebug("Node parent is black, no violations (Case 2 complete)", node);
	return 1;
}

static int _InsertCase3(rb_tree* tree, rb_node* node) {
	rb_node* parent = node->parent;
	rb_node* grand_parent = parent->parent;
	rb_node* uncle = NULL;

	// Get family members
	if(grand_parent != NULL) {
		if(grand_parent->left == parent) {
			PrintDebug("Uncle is right tree", node);
			uncle = grand_parent->right;
		} else {
			PrintDebug("Uncle is left tree", node);
			uncle = grand_parent->left;
		}

		if(uncle == NULL || uncle->color == 0)
			PrintDebug("Uncle is null or black (execute Case 4)", node);
		else if(uncle->color == 1) {
			// Case 3: Recolor parent, uncle, grandparent, then fix up
			parent->color = 0;
			uncle->color = 0;
			grand_parent->color = 1;
			PrintDebug("Uncle is red (execute Case 1 on grandparent)", node);
			return _InsertCase1(tree, grand_parent);
		}
	}
	return _InsertCase4(tree, node);
}

static int _InsertCase4(rb_tree* tree, rb_node* node) {
	rb_node* parent = node->parent;
	rb_node* grand_parent = parent->parent;

	// Case 4: black uncle, rotate child & parent
	if(parent->right == node && parent == grand_parent->left) {
		PrintDebug("Node is right of parent, left of grandparent", node);
		_RotateLeft(tree, parent);
		node = node->left;
	} else if(parent->left == node && parent == grand_parent->right) {
		PrintDebug("Node is left of parent, right of grandparent", node);
		_RotateRight(tree, parent);
		node = node->right;
	}
	PrintDebug("Execute Case 5", node);
	return _InsertCase5(tree, node);
}

static int _InsertCase5(rb_tree* tree, rb_node* node) {
	rb_node* parent = node->parent;
	rb_node* grand_parent = parent->parent;

	// Case 5: Recolor and rotate parent & grandparent
	parent->color = 0;
	grand_parent->color = 1;
	if (parent->left == node)
		return _RotateRight(tree, grand_parent);
	else
		return _RotateLeft(tree, grand_parent);
}

static int _RotateRight(rb_tree* tree, rb_node* node) {
	PrintDebug("Rotating right", node);

	rb_node* parent=node->parent;
	if(parent == NULL) {
		// Case if node is head of tree
		rb_node* old_left = node->left;
		rb_node* old_left_right = node->left->right;
		tree->head = old_left;
		node->parent = old_left;
		old_left->parent = NULL;
		old_left->right = node;
		if(old_left_right != NULL)
			old_left_right->parent = node;
		node->left = old_left_right;
	} else {
		rb_node* grand_parent = parent->parent;
		if(grand_parent == NULL) {
			// Case if parent of node head of tree
			tree->head = node;
			node->parent = NULL;
		} else {
			// Normal case
			grand_parent->right=node;
			node->parent = grand_parent;
		}
		rb_node* old_right = node->right;
		node->right=parent;
		parent->left=old_right;
		parent->parent = node;
		if(old_right != NULL)
			old_right->parent = parent;
	}
	return 1;
}

static int _RotateLeft(rb_tree* tree, rb_node* node) {
	PrintDebug("Rotating left", node);

	rb_node* parent = node->parent;
	if(parent == NULL) {
		// Case if node is head of tree
		rb_node* old_right = node->right;
		rb_node* old_right_left = node->right->left;
		tree->head = old_right;
		node->parent = old_right;
		old_right->parent = NULL;
		old_right->left = node;
		if(old_right_left != NULL)
				old_right_left->parent = node;
		node->right = old_right_left;
	} else {
		rb_node* grand_parent = parent->parent;
		if(grand_parent == NULL) {
			// parent is tree head
			tree->head = node;
			node->parent = NULL;
		} else {
			grand_parent->left=node;
			node->parent = grand_parent;
		}
		rb_node* old_left = node->left;
		node->left=parent;
		parent->right=old_left;
		parent->parent = node;
		if(old_left != NULL)
			old_left->parent = parent;
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

void PrintDebug(char* message, rb_node* node) {
	uint32_t parent_id = 0;
	if(node->parent != NULL)
		parent_id = node->parent->id;

	uint32_t left_id = 0;
	if(node->left != NULL)
		left_id = node->left->id;

	uint32_t right_id = 0;
	if(node->right != NULL)
		right_id = node->right->id;

	printf("DEBUG (id: %i c: %i l: %i r: %i pid: %i) >> %s\n", node->id, node->color, left_id, right_id, parent_id, message);
}
