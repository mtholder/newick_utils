#include <stdio.h>
#include <string.h>

#include "rnode.h"
#include "tree.h"
#include "hash.h"
#include "nodemap.h"
#include "tree_stubs.h"

int test_create_rnode()
{
	const char *test_name = "test_create_rnode";
	struct rnode *node;
	char *label = "test";
	char *length = "2.0456";
	node = create_rnode(label,length);
	if (0 != strcmp(node->label, label)) {
		printf("%s: expected label '%s' (got '%s')\n",
				test_name, label, node->label);
		return 1;
	}
	if (0 != strcmp(node->edge_length_as_string, length)) {
		printf("%s: expected length '%s', got '%s'\n",
			test_name, length, node->edge_length_as_string);
		return 1;
	}
	if (NULL != node->parent) {
		printf ("%s: parent should be NULL.\n", test_name);
		return 1;
	}
	if (NULL != node->next_sibling) {
		printf ("%s: next_sibling should be NULL.\n", test_name);
		return 1;
	}
	if (NULL != node->first_child) {
		printf ("%s: first_child should be NULL.\n", test_name);
		return 1;
	}
	if (NULL != node->last_child) {
		printf ("%s: last_child should be NULL.\n", test_name);
		return 1;
	}
	if (0 != node->child_count) {
		printf("%s: 'child_count' should be 0 (got %d)\n", test_name,
				node->child_count);
		return 1;
	}
	if (0 != node->seen) {
		printf("%s: 'seen' should be 0 (got %d)\n", test_name,
				node->seen);
		return 1;
	}
	if (NULL != node->current_child) {
		printf("%s: 'current_child_elem' should be NULL (got %p)\n", test_name,
				node->current_child);
		return 1;
	}
	printf("%s ok.\n", test_name);
	return 0;
}

int test_create_rnode_nulllabel()
{
	const char *test_name = "test_create_rnode_nulllabel";
	struct rnode *node;
	node = create_rnode(NULL,"");
	if (0 != strcmp(node->label, "")) {
		printf("%s: expected empty label (""), got '%s'\n",
				test_name, node->label);
		return 1;
	}
	if (0 != strcmp(node->edge_length_as_string, "")) {
		printf("%s: expected empty length, got '%s'\n",
			test_name, node->edge_length_as_string);
		return 1;
	}
	if (NULL != node->parent) {
		printf ("%s: parent node should be NULL.\n", test_name);
		return 1;
	}
	if (NULL != node->next_sibling) {
		printf ("%s: next_sibling should be NULL.\n", test_name);
		return 1;
	}
	if (NULL != node->first_child) {
		printf ("%s: first_child should be NULL.\n", test_name);
		return 1;
	}
	if (NULL != node->last_child) {
		printf ("%s: last_child should be NULL.\n", test_name);
		return 1;
	}
	if (0 != node->seen) {
		printf("%s: 'seen' should be 0 (got %d)\n", test_name,
				node->seen);
		return 1;
	}
	if (NULL != node->current_child) {
		printf("%s: 'current_child_elem' should be NULL (got %p)\n", test_name, node->current_child);
		return 1;
	}
	printf("%s ok.\n", test_name);
	return 0;
}

int test_create_rnode_emptylabel()
{
	const char *test_name = "test_create_rnode_emptylabel";
	struct rnode *node;
	node = create_rnode("","");
	if (0 != strcmp(node->label, "")) {
		printf("%s: expected empty label (""), got '%s'\n",
				test_name, node->label);
		return 1;
	}
	if (0 != strcmp(node->edge_length_as_string, "")) {
		printf("%s: expected empty length, got '%s'\n",
			test_name, node->edge_length_as_string);
		return 1;
	}
	if (NULL != node->parent) {
		printf ("%s: parent node should be NULL.\n", test_name);
		return 1;
	}
	if (NULL != node->next_sibling) {
		printf ("%s: next_sibling should be NULL.\n", test_name);
		return 1;
	}
	if (NULL != node->first_child) {
		printf ("%s: first_child should be NULL.\n", test_name);
		return 1;
	}
	if (NULL != node->last_child) {
		printf ("%s: last_child should be NULL.\n", test_name);
		return 1;
	}
	if (0 != node->seen) {
		printf("%s: 'seen' should be 0 (got %d)\n", test_name,
				node->seen);
		return 1;
	}
	if (NULL != node->current_child) {
		printf("%s: 'current_child_elem' should be NULL (got %p)\n", test_name,
				node->current_child);
		return 1;
	}
	printf("%s ok.\n", test_name);
	return 0;
}

int test_create_rnode_nulllength()
{
	const char *test_name = "test_create_rnode_nulllength";
	struct rnode *node;
	char *label = "test";
	node = create_rnode(label,NULL);
	if (0 != strcmp(node->label, label)) {
		printf("%s: expected label '%s' (got '%s')\n",
				test_name, label, node->label);
		return 1;
	}
	if (0 != strcmp(node->edge_length_as_string, "")) {
		printf("%s: expected empty length, got '%s'\n",
			test_name, node->edge_length_as_string);
		return 1;
	}
	if (NULL != node->parent) {
		printf ("%s: parent should be NULL.\n", test_name);
		return 1;
	}
	if (NULL != node->next_sibling) {
		printf ("%s: next_sibling should be NULL.\n", test_name);
		return 1;
	}
	if (NULL != node->first_child) {
		printf ("%s: first_child should be NULL.\n", test_name);
		return 1;
	}
	if (NULL != node->last_child) {
		printf ("%s: last_child should be NULL.\n", test_name);
		return 1;
	}
	if (0 != node->seen) {
		printf("%s: 'seen' should be 0 (got %d)\n", test_name,
				node->seen);
		return 1;
	}
	if (NULL != node->current_child) {
		printf("%s: 'current_child_elem' should be NULL (got %p)\n", test_name,
				node->current_child);
		return 1;
	}
	printf("%s ok.\n", test_name);
	return 0;
}

int test_create_rnode_emptylength()
{
	const char *test_name = "test_create_rnode_emptylength";
	struct rnode *node;
	char *label = "test";
	node = create_rnode(label,"");
	if (0 != strcmp(node->label, label)) {
		printf("%s: expected label '%s' (got '%s')\n",
				test_name, label, node->label);
		return 1;
	}
	if (0 != strcmp(node->edge_length_as_string, "")) {
		printf("%s: expected empty length, got '%s'\n",
			test_name, node->edge_length_as_string);
		return 1;
	}
	if (NULL != node->parent) {
		printf ("%s: parent should be NULL.\n", test_name);
		return 1;
	}
	if (NULL != node->next_sibling) {
		printf ("%s: next_sibling should be NULL.\n", test_name);
		return 1;
	}
	if (NULL != node->first_child) {
		printf ("%s: first_child should be NULL.\n", test_name);
		return 1;
	}
	if (NULL != node->last_child) {
		printf ("%s: last_child should be NULL.\n", test_name);
		return 1;
	}
	if (0 != node->seen) {
		printf("%s: 'seen' should be 0 (got %d)\n", test_name,
				node->seen);
		return 1;
	}
	if (NULL != node->current_child) {
		printf("%s: 'current_child_elem' should be NULL (got %p)\n", test_name,
				node->current_child);
		return 1;
	}
	printf("%s ok.\n", test_name);
	return 0;
}

int test_create_many()
{
	int i;
	int num = 100000;
	for (i = 0; i < num; i++) {
		struct rnode *node;
		node = create_rnode("test", "2.34");
	}
	printf ("test_create_many Ok (created %d).\n", num);
	return 0; 	/* crashes on failure */
}

int test_all_children_are_leaves()
{
	const char *test_name = __func__;
	/* ((A,B)f,(C,(D,E)g)h)i; */
	struct rooted_tree tree = tree_2();
	struct hash *map = create_label2node_map(tree.nodes_in_order);
	struct rnode *node_A = hash_get(map, "A");
	struct rnode *node_f = hash_get(map, "f");
	struct rnode *node_g = hash_get(map, "g");
	struct rnode *node_h = hash_get(map, "h");
	struct rnode *node_i = hash_get(map, "i");

	if (all_children_are_leaves(node_A)) {
		printf ("%s: A _is_ a leaf: f() should be false.\n", test_name);
		return 1;
	}
	if (! all_children_are_leaves(node_f)) {
		printf ("%s: all f's children are leaves.\n", test_name);
		return 1;
	}
	if (! all_children_are_leaves(node_g)) {
		printf ("%s: all g's children are leaves.\n", test_name);
		return 1;
	}
	if (all_children_are_leaves(node_h)) {
		printf ("%s: only one of h's chldren is a leaf\n", test_name);
		return 1;
	}
	if (all_children_are_leaves(node_i)) {
		printf ("%s: none of i's chldren is a leaf\n", test_name);
		return 1;
	}

	printf ("%s: ok.\n", test_name);
	return 0;
}

int test_all_children_have_same_label()
{
	const char *test_name = __func__;
	/* ((A:1,B:1.0)f:2.0,(C:1,(C:1,C:1)g:2)h:3)i;  - one clade made of
	 * three 'C's */
	struct rooted_tree tree = tree_4();
	struct hash *map = create_label2node_map(tree.nodes_in_order);
	struct rnode *node_A = hash_get(map, "A");
	struct rnode *node_f = hash_get(map, "f");
	struct rnode *node_g = hash_get(map, "g");
	struct rnode *node_i = hash_get(map, "i");

	struct rnode *mum = create_rnode("mum", "");
	struct rnode *kid1 = create_rnode("", "");
	struct rnode *kid2 = create_rnode("", "");
	add_child(mum, kid1);
	add_child(mum, kid2);

	char *label;

	if (all_children_have_same_label(node_A, &label)) {
		printf ("%s: A is a leaf: f() should be false.\n", test_name);
		return 1;
	}
	if (all_children_have_same_label(node_f, &label)) {
		printf ("%s: f's children have different labels.\n", test_name);
		return 1;
	}
	if (! all_children_have_same_label(node_g, &label)) {
		printf ("%s: all g's children have the same label.\n", test_name);
		return 1;
	}
	if (strcmp(label, "C") != 0) {
		printf("%s: shared label should be 'C'\n", test_name);
		return 1;
	}
	if (all_children_have_same_label(node_i, &label)) {
		printf ("%s: h's children have different labels.\n", test_name);
		return 1;
	}
	if (! all_children_have_same_label(mum, &label)) {
		printf ("%s: all mum's children have the same label.\n", test_name);
		return 1;
	}
	if (strcmp(label, "") != 0) {
		printf("%s: shared label should be ''\n", test_name);
		return 1;
	}

	printf ("%s: ok.\n", test_name);
	return 0;
}

int main()
{
	int failures = 0;
	printf("Starting rooted node test...\n");
	failures += test_create_rnode();
	failures += test_create_rnode_nulllabel();
	failures += test_create_rnode_emptylabel();
	failures += test_create_rnode_nulllength();
	failures += test_create_rnode_emptylength();
	failures += test_all_children_are_leaves();
	failures += test_all_children_have_same_label();
	failures += test_create_many();
	if (0 == failures) {
		printf("All tests ok.\n");
	} else {
		printf("%d test(s) FAILED.\n", failures);
		return 1;
	}

	return 0;
}
