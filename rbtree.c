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

rb_tree* CreateTree(uint32_t id, char* rule) {
	rb_tree* newTree;
	rb_node* head;

	printf("Creating q_mask");
	fflush(stdout);

	// create q mask
	uint8_t q_mask = ParseQMask(rule);

	printf("Creating b_mask");
	fflush(stdout);

	// create b mask
	uint8_t b_mask = ParseBMask(rule);

	printf("Allocating memory for tree");
	fflush(stdout);

	newTree = (rb_tree*) malloc(sizeof(rb_tree));
	if(!newTree) {
		// Something went wrong!
		printf("Could not malloc memory needed for tree structure!");
		fflush(stdout);
		return NULL;
	}

	printf("Allocating memory for head node");
	fflush(stdout);

	head = (rb_node*) malloc(sizeof(rb_node));
	if(!head) {
		printf("Could not malloc memory needed for head node!");
		fflush(stdout);
		return NULL;
	}

	printf("Setting node members");
	fflush(stdout);

	head->color = 0;	// root node is black
	head->id = id;
	head->q_mask = q_mask;
	head->b_mask = b_mask;
	head->left = NULL;
	head->right = NULL;
	head->parent = NULL;

	printf("Setting tree head node");
	fflush(stdout);

	newTree->head = head;

	printf("Done creating tree, returning");
	fflush(stdout);

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

void DeleteNode(rb_tree* tree, rb_node* node) {

}

void InsertNode(rb_tree* tree, uint32_t id, char* rule) {

}

rb_node* FindByKey(rb_tree* tree, uint32_t key) {

}

uint8_t* ParseQMask(char* rule) {
	uint8_t q_mask = 0;
	for(int j = 0; j < strlen(rule); ++j) {	// ignore compiler warning, max size will be 12k
		if (rule[j] == '?')
			BitFalse(q_mask, PackingIndex(j));
		else
			BitTrue(q_mask, PackingIndex(j));
	}
	return q_mask;
}

uint8_t* ParseBMask(char* rule) {
	uint8_t b_mask = 0;
	for(int j = 0; j < strlen(rule); ++j) { // ignore compiler warning, max size will be 12k
		if (rule[j] == '0' || rule[j]=='?')
			BitFalse(b_mask,PackingIndex(j));
		if (rule[j] == '1')
			BitTrue(b_mask, PackingIndex(j));
	}
	return b_mask;
}
