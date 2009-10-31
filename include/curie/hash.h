/*
 * This file is part of the kyuba.org Curie project.
 * See the appropriate repository at http://git.kyuba.org/ for exact file
 * modification records.
*/

/*
 * Copyright (c) 2009, Kyuba Project Members
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

/*! \file
 *  \brief Hash Functions
 *
 *  This file will contain a number of hash functions. Hash functions are rather
 *  useful in general, and needed for a variety of problems, including some
 *  of the basic functionality in curie.
 *
 *  The hash functions in this file (should) be in the public domain, as far as
 *  I know, anyway.
 */

#ifndef LIBCURIE_HASH_H
#define LIBCURIE_HASH_H

#include <curie/int.h>

/*! \brief MurmurHash2, 32-Bit
 *  \param[in] key  The data to hash.
 *  \param[in] len  Length (in bytes) of key.
 *  \param[in] seed Start value for the hash.
 *
 *  This is the MurmurHash2, written by Austin Appleby. For more details, see
 *  http://murmurhash.googlepages.com/ -- this is the website where the hash
 *  is available in its original form.
 *
 *  This variant in particular is the regular 32-bit version of the hash.
 */
int_32      hash_murmur2_32 ( const void * key, int len, unsigned int seed );

/*! \brief MurmurHash2, 64-Bit
 *  \param[in] key  The data to hash.
 *  \param[in] len  Length (in bytes) of key.
 *  \param[in] seed Start value for the hash.
 *
 *  This is the MurmurHash2, written by Austin Appleby. For more details, see
 *  http://murmurhash.googlepages.com/ -- this is the website where the hash
 *  is available in its original form.
 *
 *  This variant in particular is either the native 64-bit version, or the
 *  version for 32-bit architectures, depending on the current architecture.
 *  Thus it's perfectly fine to use on 32-bit platforms.
 */
int_64      hash_murmur2_64 ( const void * key, int len, unsigned int seed );

/*! \brief MurmurHash2, Pointer-Sized
 *  \param[in] key  The data to hash.
 *  \param[in] len  Length (in bytes) of key.
 *  \param[in] seed Start value for the hash.
 *
 *  This is the MurmurHash2, written by Austin Appleby. For more details, see
 *  http://murmurhash.googlepages.com/ -- this is the website where the hash
 *  is available in its original form.
 *
 *  This variant is either the 32-bit or the 64-bit version of the hash,
 *  depending on the pointer size of the target architecture.
 */
int_pointer hash_murmur2_pt ( const void * key, int len, unsigned int seed );

/*! \brief SuperFastHash
 *  \param[in] data The data to hash.
 *  \param[in] len  Length (in bytes) of key.
 *
 *  This is the SuperFastHash, written by Paul Hsieh. For more details, see
 *  http://www.azillionmonkeys.com/qed/hash.html -- this is the website where
 *  the hash is available in its original form.
 *
 *  This hash is always 32-bit. This version has been slightly modified to
 *  accomodate for one of the curie rules: if the user passes a NULL pointer
 *  or does other things that are meaningless, things oughta blow up. For this
 *  reason, the original check for validity of data and len is skipped.
 */
int_32      hash_superfast ( const int_8 * data, int len );

#endif

/*! @} */