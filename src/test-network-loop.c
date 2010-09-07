/*
 * This file is part of the kyuba.org Curie project.
 * See the appropriate repository at http://git.kyuba.org/ for exact file
 * modification records.
*/

/*
 * Copyright (c) 2008-2010, Kyuba Project Members
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

#include "curie/network.h"
#include "curie/sexpr.h"

#include <curie/sexpr-internal.h>

define_symbol(test_symbol_1, "hello-world!");
define_string(test_string_1, "hello \"world\"!");
define_symbol(sym_success, "success");
define_symbol(sym_failure, "failure");

int cmain(void) {
    struct io *in, *out;
    struct sexpr_io *io;
    sexpr i, t1, t2;
    int rv = 0;

    net_open_loop(&in, &out);

    io = sx_open_io(in, out);

    t1 = test_symbol_1;
    t2 = test_string_1;

    sx_write (io, t1);
    sx_write (io, t2);
    while (io_commit (io->out) != io_complete);

    /* test transmission of a single symbol*/
    do {
        i = sx_read (io);
    } while (nexp(i));

    rv |= (truep(equalp(t1, i)) ? 0 : 1) << 1;

    /* test transmission of a single string */

    while (io_commit (io->out) == io_incomplete);
    do {
        i = sx_read (io);
    } while (i == sx_nonexistent);

    rv |= (truep(equalp(i, t2)) ? 0 : 1) << 2;

    i = sx_read (io);

    rv |= (nexp(i) ? 0 : 1) << 3;

    sx_close_io (io);

    return rv;
}
