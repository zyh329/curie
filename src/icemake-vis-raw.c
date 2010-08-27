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

#include <icemake/icemake.h>
#include <curie/multiplex.h>

static void items
    (struct icemake *im, int count)
{
    sx_write (im->visualiser.meta.raw.stdio,
              cons (sym_items_total,
                    cons (make_integer (count), sx_end_of_list)));
}

static void on_command
    (struct icemake *im, sexpr cmd)
{
    sx_write (im->visualiser.meta.raw.stdio, cmd);
}

static void on_command_done
    (struct icemake *im, sexpr cmd)
{
    sx_write (im->visualiser.meta.raw.stdio,
              cons (sym_completed, cons (cmd, sx_end_of_list)));
}

static void on_error
    (struct icemake *im, enum icemake_error error, sexpr sx)
{
    if (error != ie_problematic_signal)
    {
        sx_write (im->visualiser.meta.raw.stdio, cons (sym_error, sx));
    }
}

static void on_warning
    (struct icemake *im, enum icemake_error error, sexpr sx)
{
    sx_write (im->visualiser.meta.raw.stdio, cons (sym_warning, sx));
}

int icemake_prepare_visualiser_raw (struct icemake *im)
{
    im->visualiser.visualiser      = vs_raw;

    im->visualiser.items           = items;
    im->visualiser.on_command      = on_command;
    im->visualiser.on_command_done = on_command_done;
    im->visualiser.on_error        = on_error;
    im->visualiser.on_warning      = on_warning;
    im->visualiser.meta.raw.stdio  = sx_open_stdout();

    multiplex_add_sexpr (im->visualiser.meta.raw.stdio, (void *)0, (void *)0);

    return 0;
}

