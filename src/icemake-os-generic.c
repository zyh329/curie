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

#include <curie/memory.h>
#include <curie/multiplex.h>
#include <curie/filesystem.h>

#if defined(_WIN32)
/* windows comes with dedicated file copy functions, so let's use those... */
static int install_file
    (struct icemake *im, sexpr spec)
{
    sexpr source = car (spec);
    sexpr target = cdr (spec);
    int mode = 0444;
    struct io *in, *out;

    if (integerp (source))
    {
        mode   = sx_integer (source);
        source = car (target);
        target = cdr (target);
    }

    if (truep(equalp(source, sym_symlink))) /* no symlinks on windows */
    {
/*        source = car (target);
        target = cdr (target);

        sx_write (stdio, cons (sym_symlink,
                  cons (source, cons (target, sx_end_of_list))));

        symlink (sx_string(source), sx_string(target));*/
    }
    else if (truep (filep (source)))
    {
        sx_write (stdio, cons (sym_install, cons (make_integer (mode),
                  cons (source, cons (target, sx_end_of_list)))));

        mkdir_pi (target);
        CopyFile (sx_string (source), sx_string (target), FALSE);
    }

    return 0;
}

#else

struct file_metadata
{
    struct io      *out;
    struct icemake *icemake;
};

static void install_read (struct io *in, void *aux)
{
    struct file_metadata *meta = (struct file_metadata *)aux;

    if (((in->length) - (in->position)) > 0)
    {
        io_write (meta->out,
                  (in->buffer) + (in->position),
                  (in->length) - (in->position));

        in->position = in->length;
    }
}

static void install_close (struct io *in, void *aux)
{
    struct file_metadata *meta = (struct file_metadata *)aux;

    if (((in->length) - (in->position)) > 0)
    {
        io_write (meta->out,
                  (in->buffer) + (in->position),
                  (in->length) - (in->position));

        in->position = in->length;
    }

    (meta->icemake->alive_processes)--;

    free_pool_mem (aux);
}

static int install_file
    (struct icemake *im, sexpr spec)
{
    sexpr source = car (spec);
    sexpr target = cdr (spec);
    int mode = 0444;
    struct io *in, *out;
    static struct memory_pool pool =
        MEMORY_POOL_INITIALISER (sizeof (struct file_metadata));
    struct file_metadata *meta;

    if (integerp (source))
    {
        mode   = sx_integer (source);
        source = car (target);
        target = cdr (target);
    }

    if (truep(equalp(source, sym_symlink)))
    {
        source = car (target);
        target = cdr (target);

        sx_write (stdio, cons (sym_symlink,
                  cons (source, cons (target, sx_end_of_list))));

        symlink (sx_string(source), sx_string(target));
    }
    else if (truep (filep (source)))
    {
        sx_write (stdio, cons (sym_install, cons (make_integer (mode),
                  cons (source, cons (target, sx_end_of_list)))));

        mkdir_pi (target);

        meta = (struct file_metadata *)get_pool_mem (&pool);

        in  = io_open_read   (sx_string (source));
        out = io_open_create (sx_string (target), mode);

        meta->out     = out;
        meta->icemake = im;

        (im->alive_processes)++;

        multiplex_add_io (in, install_read, install_close, (void *)meta);
        multiplex_add_io_no_callback (out);
    }

    return 0;
}
#endif

int icemake_prepare_operating_system_generic (struct toolchain_descriptor *td)
{
    td->install_file = install_file;
    return 0;
}