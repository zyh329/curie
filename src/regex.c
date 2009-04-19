/*
 * This file is part of the kyuba.org Curie project.
 * See the appropriate repository at http://git.kyuba.org/ for exact file
 * modification records.
*/

/*
 * Copyright (c) 2008, 2009, Kyuba Project Members
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/

#include <curie/regex.h>
#include <curie/memory.h>
#include <curie/utf-8.h>

static void rx_compile_add_nodes (struct graph *g, const unsigned char *s)
{
    unsigned int p = 0, np;
    char quote = 0, cclass = 0;
    int_32 ra;

    while ((np = utf8_get_character (s, p, &ra)), (ra != 0))
    {
        if (quote)
        {
            quote = 0;
            goto add;
        }

        if (cclass)
        {
            if (ra == ']')
            {
                cclass = 0;
            }
            p = np;
            continue;
        }

        switch (ra)
        {
            case '\\':
                quote = 1;
                break;
            case '?':
            case '*':
            case '+':
            case '(':
            case ')':
            case '|':
                break;
            case '[':
                cclass = 1;
            default:
                add:
                graph_add_node (g, make_integer(p));
                break;
        }

        p = np;
    }
}

static struct graph_node *rx_compile_recurse
    (struct graph *g, struct graph_node *n, struct graph_node *e,
     const unsigned char *s, unsigned int *pp)
{
    unsigned int p = *pp, np;
    char quote = 0;
    struct graph_node *c = n, *last = (struct graph_node *)0;
    sexpr el = truep(e->label) ? make_integer(0) : sx_false;
    int_32 ra;

    while ((np = utf8_get_character (s, p, &ra)), (ra != 0))
    {
        if (quote)
        {
            quote = 0;
            goto add;
        }

        switch (ra)
        {
            case '\\':
                quote = 1;
                break;
            case '[':
                {
                    struct graph_node *t =
                            graph_search_node (g, make_integer(p));
                    int lastchar = 0, range = 0;

                    last = c;
                    c = t;

                    p = np;

                    while (((p = utf8_get_character (s, p, &ra)), (ra != 0)))
                    {
                        if (ra == ']') break;

                        if (range == 1)
                        {
                            for (int n = lastchar; n <= ra; n++)
                            {
                                graph_node_add_edge (last, t, make_integer(n));
                            }

                            lastchar = ra;
                            range = 0;
                            continue;
                        } else if (ra == '-') {
                            lastchar++;
                            range = 1;
                            continue;
                        }

                        graph_node_add_edge (last, t, make_integer(ra));

                        lastchar = ra;
                    }

                    if (ra == 0)
                    {
                        goto quit;
                    }

                    goto next;
                }
                break;
            case '?':
                if (last != (struct graph_node *)0);
                {
                    struct graph_node *ne = graph_add_node (g, sx_false);

                    graph_node_add_edge (c, ne, sx_false);
                    graph_node_add_edge (last, ne, sx_false);

                    last = c;
                    c = ne;
                }
                break;
            case '+':
                if (last != (struct graph_node *)0);
                {
                    struct graph_node *ne = graph_add_node (g, sx_false);

                    graph_node_add_edge (c, last, sx_false);
                    graph_node_add_edge (c, ne, sx_false);

                    last = c;
                    c = ne;
                }
                break;
            case '*':
                if (last != (struct graph_node *)0);
                {
                    struct graph_node *ne = graph_add_node (g, sx_false);

                    graph_node_add_edge (c, ne, sx_false);
                    graph_node_add_edge (last, ne, sx_false);
                    graph_node_add_edge (c, last, sx_false);

                    last = c;
                    c = ne;
                }
                break;
            case '(':
                {
                    struct graph_node *ne = graph_add_node (g, sx_false);

                    p = np;

                    last = c;
                    c = rx_compile_recurse (g, c, ne, s, &p);
                }
                goto next;
            case ')':
                p = np;
                el = sx_false;
                goto quit;
            case '|':
                {
                    struct graph_node *ne = graph_add_node (g, sx_false);

                    p = np;

                    graph_node_add_edge (c, ne, sx_false);

                    last = c;
                    c = ne;

                    rx_compile_recurse (g, n, ne, s, &p);
                    goto quit;
                }
                break;
            case '.':
                {
                    struct graph_node *t =
                            graph_search_node (g, make_integer(p));

                    graph_node_add_edge (c, t, sx_true);

                    last = c;
                    c = t;
                }
                break;
            default:
                add:
                {
                    struct graph_node *t =
                            graph_search_node (g, make_integer(p));

                    graph_node_add_edge (c, t, make_integer(ra));

                    last = c;
                    c = t;
                }
                break;
        }

        p = np;

        next:;
    }

    quit:
    *pp = p;
    graph_node_add_edge (c, e, el);
    return c;
}

struct graph *rx_compile_sx (sexpr sx)
{
    if (consp(sx))
    {
        return sexpr_to_graph(sx);
    }
    else if (stringp(sx))
    {
        return rx_compile (sx_string (sx));
    }

    return graph_create();
}

struct graph *rx_compile (const char *s)
{
    struct graph *g = graph_create();

    unsigned int p = 0;
    struct graph_node *n = graph_add_node (g, sx_nil);
    struct graph_node *e = graph_add_node (g, sx_true);

    rx_compile_add_nodes(g, (const unsigned char *)s);
    (void)rx_compile_recurse(g, n, e, (const unsigned char *)s, &p);

    return g;
}

struct nfa_state
{
    struct graph_node *n;
    unsigned int p;
    struct nfa_state *next;
};

static struct memory_pool nfa_pool =
        MEMORY_POOL_INITIALISER (sizeof(struct nfa_state));

static void rx_match_add_nfa_state
    (struct nfa_state **s, struct graph_node *n, unsigned int p)
{
    /* don't add nfa states that are already in the list */
    for (struct nfa_state *c = *s; c != (struct nfa_state *)0; c = c->next)
    {
        if ((c->n == n) && (c->p == p)) return;
    }

    struct nfa_state *ns = get_pool_mem (&nfa_pool);

    ns->n = n;
    ns->p = p;

    ns->next = (*s);
    *s = ns;
}

static sexpr rx_match_nfa_state_progress
    (struct nfa_state *ns, struct nfa_state **r, const unsigned char *s)
{
    unsigned int p = ns->p, np;
    struct graph_node *n = ns->n;
    char haveedge = 0;

    if (truep(n->label)) return sx_true;

    int_32 ra;
    np = utf8_get_character (s, p, &ra);

    sexpr sx = make_integer(ra);

    for (unsigned int i = 0; i < n->edge_count; i++)
    {
        struct graph_edge *e = n->edges[i];
        sexpr l = e->label;

        if (haveedge)
        {
            if (falsep(l))
            {
                rx_match_add_nfa_state (r, e->target, p);
            }
            else if (((ra != 0) && truep(l)) || truep(equalp(l, sx)))
            {
                struct graph_node *nt = e->target;

                rx_match_add_nfa_state (r, nt, p + ((ra == 0) ? 0 : 1));
            }
        }
        else
        {
            if (falsep(l))
            {
                ns->n = e->target;

                haveedge = 1;
            }
            else if (((ra != 0) && truep(l)) || truep(equalp(l, sx)))
            {
                ns->n = e->target;

                if (ra != 0) ns->p = np;

                haveedge = 1;
            }
        }
    }

    if (!haveedge)
    {
        return sx_nonexistent;
    }

    return sx_false;
}

static sexpr rx_match_recurse
    (struct nfa_state **ns, const unsigned char *s)
{
    while ((*ns) != (struct nfa_state *)0)
    {
        struct nfa_state *c = (*ns);

        while (c != (struct nfa_state *)0)
        {
            sexpr sx = rx_match_nfa_state_progress (c, ns, s);

            if (truep (sx))
            {
                return sx_true;
            }
            else if (nexp (sx))
            {
                struct nfa_state *f = c;

                c = c->next;

                free_pool_mem (f);

                if ((*ns) == f)
                {
                    *ns = c;
                }
                else for (struct nfa_state *y = *ns;
                          y != (struct nfa_state *)0;
                          y = y->next)
                {
                    if (y->next == f)
                    {
                        y->next = c;
                        break;
                    }
                }
            }
            else
            {
                c = c->next;
            }
        }
    }

    return sx_false;
}

sexpr rx_match_sx (struct graph *g, sexpr sx)
{
    if (stringp(sx))
    {
        return rx_match (g, sx_string(sx));
    }

    return sx_false;
}

sexpr rx_match (struct graph *g, const char *s)
{
    sexpr rv = sx_false;

    struct graph_node *n = graph_search_node (g, sx_nil);
    struct nfa_state *ns = get_pool_mem (&nfa_pool);

    if (n == (struct graph_node *)0) return sx_false;

    if (truep(n->label)) return sx_true;

    ns->next = (struct nfa_state *)0;
    ns->n = n;
    ns->p = 0;

    rv = rx_match_recurse (&ns, (const unsigned char *)s);

    while (ns != (struct nfa_state *)0)
    {
        struct nfa_state *f = ns;
        ns = ns->next;

        free_pool_mem (f);
    }

    return rv;
}