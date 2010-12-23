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
/* Returns a Newick string representing the tree rooted at 'node'.*/

struct rnode;

/* Returns a Newick representation of the tree. Memory is allocated, don't
 * forget to free() it. Returns NULL in case of failure (which will be a memory
 * allocation problem). WARNING: the current implementation is recursive, and
 * hence is slow for big trees.  */

char *to_newick(struct rnode* root);

/* Like to_newick, but returns a list of strings. Concatenating the strings in
 * list order results in the Newick representation of the tree. The function
 * does not do this, because it would involve repeated calls to concat(), which
 * are costly and not necessarily needed (e.g., if you just want to print the
 * Newick, which is usually the case, you can just print the strings in list
 * order -- and dump_newick() does this for you, using buffered output) Also,
 * this function is iterative rather than recursive. */

struct llist *to_newick_i(struct rnode *root);

/* Dumps the newick rooted at 'root' to stdout, using to_newick_i() */

void dump_newick(struct rnode* root);
