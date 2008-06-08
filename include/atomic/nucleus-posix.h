/*
 *  nucleus-posix.h
 *  atomic-libc
 *
 *  Created by Magnus Deininger on 04/06/2008.
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

#ifndef ATOMIC_NUCLEUS_H
#define ATOMIC_NUCLEUS_H

#ifdef __cplusplus
extern "C" {
#endif
  void   _atomic_exit  (int status);
  int    _atomic_read  (int fd, /*@out@*/ void *buf, int count)
   /*@globals errno;@*/;
  int    _atomic_write (int fd, const void *buf, int count)
   /*@globals errno;@*/;

  int    _atomic_open_read (const char *path)
   /*@globals errno;@*/;
  int    _atomic_open_write (const char *path)
   /*@globals errno;@*/;
  int    _atomic_create (const char *path, int mode)
   /*@globals errno;@*/;
  int    _atomic_close (int fd)
   /*@globals errno;@*/;

  /*@null@*/ void * _atomic_mmap (void *start, int length, int prot, int flags,
                                  int fd, int offset)
   /*@globals errno;@*/;
  int    _atomic_munmap (void *start, int length)
   /*@globals errno;@*/;

  int    _atomic_kill (int pid, int sig)
   /*@globals errno;@*/;

  int atomic_main();
  
  extern char last_error_recoverable_p;
#ifdef __cplusplus
}
#endif

#endif /* NUCLEUS_H */