/*
 *  multiplex.h
 *  atomic-libc
 *
 *  Created by Magnus Deininger on 03/06/2008.
 *  Copyright 2008 Magnus Deininger. All rights reserved.
 *
 */

/*
 * Copyright (c) 2008, Magnus Deininger All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer. *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution. *
 * Neither the name of the project nor the names of its contributors may
 * be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ATOMIC_MULTIPLEX_H
#define ATOMIC_MULTIPLEX_H

#include <atomic/io.h>
#include <atomic/sexpr.h>
#include <atomic/exec.h>

struct multiplex_functions {
    void (*count)(int *, int *);
    void (*augment)(int *, int *, int *, int *);
    void (*callback)(int *, int, int *, int);

    struct multiplex_functions *next;
};

enum multiplex_result {
    mx_ok = 0,
    mx_nothing_to_do = 1
};

enum multiplex_result multiplex ();

void multiplex_add (struct multiplex_functions *);

void multiplex_io ();
void multiplex_process ();
void multiplex_signal ();
void multiplex_sexpr ();

void multiplex_add_io (struct io *io, void (*on_read)(struct io *));
void multiplex_add_process (struct exec_context *context, void (*on_death)(struct exec_context *));
void multiplex_add_sexpr (struct sexpr_io *io, void (*on_read)(struct sexpr *));

#endif
