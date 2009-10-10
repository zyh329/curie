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

#include <curie/string.h>
#include <curie/memory.h>

/* this hash function is based on this one:
 * http://www.azillionmonkeys.com/qed/hash.html */

/**** begin ***** markos' hashing function ************************************/

#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const int_16 *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((int_32)(((const int_8 *)(d))[1])) << 8)\
                       +(int_32)(((const int_8 *)(d))[0]) )
#endif

// Taken from
// http://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
#define HAS_ZERO_BYTE(v) (~((((v & 0x7F7F7F7FUL) + 0x7F7F7F7FUL) | v) | 0x7F7F7F7FUL))

// data is guarranteed to be 32-bit aligned)
int_32 str_hash(const char *data, unsigned long *len)
{
    int_32 hash = 0, tmp, mask;
    int rem = 0;
    unsigned long lent = 0;

    // this unrolls to better optimized code on most compilers
    do {
        mask = HAS_ZERO_BYTE(*((int_32 *)(data)));
        if (mask == 0) {
            // Do the hash calculation
            hash += get16bits(data);
            tmp   = (get16bits(data + 2) << 11) ^ hash;
            hash  = (hash << 16) ^ tmp;
            hash += hash >> 11;
            data += 4;
            // Increase len as well
            lent += 4;
        }
    } while (mask == 0);

    // Now we've got out of the loop, because we hit a zero byte,
    // find out which exactly
    if (data[0] == 0)
        rem = 0;
    else if (data[1] == 0)
        rem = 1;
    else if (data[2] == 0)
        rem = 2;
    else if (data[3] == 0)
        rem = 3;

    /*
     * Handle end cases 
     */
    switch (rem) {
    case 3:
        hash += get16bits(data);
        hash ^= hash << 16;
        hash ^= data[2] << 18;
        hash += hash >> 11;
        break;
    case 2:
        hash += get16bits(data);
        hash ^= hash << 11;
        hash += hash >> 17;
        break;
    case 1:
        hash += *data;
        hash ^= hash << 10;
        hash += hash >> 1;
    }
    // len was calculated in 4-byte tuples,
    // multiply it by 4 to get the number in bytes
    *len = lent + rem;

    /*
     * Force "avalanching" of final 127 bits 
     */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}

/**** end ******* markos' hashing function ************************************/
