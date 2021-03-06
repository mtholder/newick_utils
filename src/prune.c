/* 

Copyright (c) 2009 Thomas Junier and Evgeny Zdobnov, University of Geneva
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
* Neither the name of the University of Geneva nor the names of its
    contributors may be used to endorse or promote products derived from this
    software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
/* prune.c: remove nodes from tree */

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#include "tree.h"
#include "parser.h"
#include "to_newick.h"
#include "rnode.h"
#include "link.h"
#include "set.h"
#include "list.h"

enum prune_mode { PRUNE_DIRECT, PRUNE_REVERSE };

struct prune_data {
	bool kept_descendant;
};

struct parameters {
	set_t 	*cl_labels;
	enum prune_mode mode;
};

void help(char *argv[])
{
	printf (
"Removes nodes by label\n"
"\n"
"Synopsis\n"
"--------\n"
"\n"
"%s [-hi:v] <newick trees filename|-> <label> [label+]\n"
"\n"
"Input\n"
"-----\n"
"\n"
"Argument is the name of a file that contains Newick trees, or '-' (in which\n"
"case trees are read from standard input).\n"
"\n"
"Output\n"
"------\n"
"\n"
"Removes all nodes whose labels are passed on the command line, and prints\n"
"out the modified tree. If removing a node causes its parent to have only\n"
"one child (as is always the case in strictly binary trees), the parent is\n"
"spliced out and the remaining child is attached to its grandparent,\n"
"preserving length.\n"
"\n"
"Only labeled nodes are considered for pruning.\n"
"\n"
"Options\n"
"-------\n"
"\n"
"    -h: print this message and exit\n"
"    -v: reverse: prune nodes whose labels are NOT passed on the command\n"
"        line. Inner nodes are not pruned, unless -i is also set (see\n"
"        above). This allows pruning of trees with support values, which\n"
"        syntactically are node labels, without inner nodes disappearing\n"
"        because their 'label' was not passed on the command line.\n"
"\n"
"Assumptions and Limitations\n"
"---------------------------\n"
"\n"
"Labels are assumed to be unique. \n"
"\n"
"Examples\n"
"--------\n"
"\n"
"# Remove humans and gorilla\n"
"$ %s data/catarrhini Homo Gorilla\n"
"\n"
"# Remove humans, chimp, and gorilla\n"
"$ %s data/catarrhini Homo Gorilla Pan\n"
"\n"
"# the same, but using the clade's label\n"
"$ %s data/catarrhini Homininae\n"
"\n"
"# keep great apes and Colobines:\n"
"$ %s -v data/catarrhini Gorilla Pan Homo Pongo Simias Colobus\n"
"\n"
"# same, using clade labels:\n"
"$ %s -v data/catarrhini Hominidae Colobinae\n",
	argv[0],
	argv[0],
	argv[0],
	argv[0],
	argv[0],
	argv[0]
	);
}

struct parameters get_params(int argc, char *argv[])
{
	struct parameters params;
	params.mode = PRUNE_DIRECT;

	int opt_char;
	while ((opt_char = getopt(argc, argv, "hv")) != -1) {
		switch (opt_char) {
		case 'h':
			help(argv);
			exit (EXIT_SUCCESS);
		case 'v':
			params.mode = PRUNE_REVERSE;
			break;
		default:
			fprintf (stderr, "Unknown option '-%c'\n", opt_char);
			exit (EXIT_FAILURE);
		}
	}

	/* check arguments */
	if ((argc - optind) >= 2)	{
		if (0 != strcmp("-", argv[optind])) {
			FILE *fin = fopen(argv[optind], "r");
			extern FILE *nwsin;
			if (NULL == fin) {
				perror(NULL);
				exit(EXIT_FAILURE);
			}
			nwsin = fin;
		}
		set_t *cl_labels = create_set();
		if (NULL == cl_labels) { perror(NULL); exit(EXIT_FAILURE); }
		optind++;	/* optind is now index of 1st label */
		for (; optind < argc; optind++) {
			if (set_add(cl_labels, argv[optind]) < 0) {
				perror(NULL);
				exit(EXIT_FAILURE);
			}
		}
		params.cl_labels = cl_labels;
	} else {
		fprintf(stderr, "Usage: %s [-h] <filename|-> <label> [label+]\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}

	return params;
}

/* We buld a hash of the passed labels, and visit the tree, unlinking nodes as
 * needed. In Direct mode, we traverse in reverse order, unlinking nodes to
 * prune. In Reverse mode, things are a bit more hairy because we need to keep
 * not just the nodes passed as argument, but also their ancestors. So we pass
 * through th etree first in direct order, marking nodes to keep and their
 * ancestors. Then we go back in reverse order, pruning. */

static struct rooted_tree * process_tree_direct(
		struct rooted_tree *tree, set_t *cl_labels)
{
	struct llist *rev_nodes = llist_reverse(tree->nodes_in_order);
	struct list_elem *el;
	struct rnode *current;
	char *label;

	for (el = rev_nodes->head; NULL != el; el = el->next) {
		current = el->data;
		label = current->label;
		/* skip this node iff parent is marked ("seen") */
		if (!is_root(current) && current->parent->seen) {
			current->seen = true;	/* inherit mark */
			continue;
		}
		if (set_has_element(cl_labels, label)) {
			unlink_rnode(current);
			current->seen = true;
		}
	}

	destroy_llist(rev_nodes);
	reset_seen(tree);
	return tree;
}

/* Prune predicate: retains the nodes passed on CL, but discards their
 * children. */

// TODO: this might be a useful prune predicate, add option to use it.

bool prune_predicate_trim_kids(struct rnode *node, void *param)
{
	if (node->seen) {
		/* ancestor of a passed node */
		return true;
	}
}

/* Prune predicate: retains the nodes passed on CL and their children. */

bool prune_predicate_keep_clade(struct rnode *node, void *param)
{
	set_t *cl_labels = (set_t *) param;

	if (node->seen) {
		return true;
	}
	/* Node isn't an ancestor of a passed node. Node can be a _descendant_
	 * of a passed node, though, in which case we must keep it as well. */
	if (!is_root(node)) {
		if (NULL != node->parent->data) {
			struct prune_data *pdata = node->parent->data;
			if (pdata->kept_descendant) {
				struct prune_data *pdata =
					malloc(sizeof(struct prune_data));
				if (NULL == pdata) {
					perror(NULL); exit(EXIT_FAILURE);
				}
				pdata->kept_descendant = true;
				node->data = pdata;
				return true;
			}
		}
	}
	
	/* neither an ancestor nor a descendant of a passed node - drop. */
	return false;
}

static struct rooted_tree * process_tree_reverse(
		struct rooted_tree *tree, set_t *cl_labels)
{
	struct list_elem *el = tree->nodes_in_order->head;
	struct rnode *current;
	char *label;

	for (; NULL != el; el = el->next) {
		current = el->data;
		if (is_root(current)) break;
		label = current->label;
		/* mark this node (to keep it) if its label is on the CL */
		if (set_has_element(cl_labels, label)) {
			current->seen = true;
			struct prune_data *pdata =
				malloc(sizeof(struct prune_data));
			if (NULL == pdata) {perror(NULL); exit(EXIT_FAILURE); }
			pdata->kept_descendant = true;
			current->data = pdata;
		}
		/* and propagate 'seen' to parent (kept_descendant is
		 * propagated to children (not parents), see
		 * prune_predicate_keep_clade() */
		if (current->seen) {
			current->parent->seen = true;	/* inherit mark */
		}
	}

	struct rooted_tree *pruned = clone_tree_cond(tree,
			prune_predicate_keep_clade, cl_labels);	
	destroy_tree(tree);
	return pruned;
}

int main(int argc, char *argv[])
{
	struct rooted_tree *tree;	
	struct parameters params;
	static struct rooted_tree * (*process_tree)(struct rooted_tree *, set_t *);
	
	params = get_params(argc, argv);

	switch (params.mode) {
	case PRUNE_DIRECT:
		process_tree = process_tree_direct;
		break;
	case PRUNE_REVERSE:
		process_tree = process_tree_reverse;
		break;
	default:
		assert (0);
	}

	while (NULL != (tree = parse_tree())) {
		tree = process_tree(tree, params.cl_labels);
		dump_newick(tree->root);
		destroy_all_rnodes(NULL);
		destroy_tree(tree);
	}

	destroy_set(params.cl_labels);

	return 0;
}
