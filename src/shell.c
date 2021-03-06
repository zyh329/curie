/**\file
 *
 * \copyright
 * Copyright (c) 2008-2014, Kyuba Project Members
 * \copyright
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * \copyright
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * \copyright
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * \see Project Documentation: http://ef.gy/documentation/curie
 * \see Project Source Code: http://git.becquerel.org/kyuba/curie.git
*/

#define BUFFERSIZE 4096

#include <curie/sexpr.h>
#include <curie/filesystem.h>
#include <sievert/shell.h>

sexpr ewhich (char **environment, sexpr programme)
{
    define_string (str_slash, "/");
    char *x = (char *)0, *y, buffer[BUFFERSIZE];

    for (int i = 0; environment[i]; i++)
    {
        y = environment[i];
        if ((y[0] == 'P') && (y[1] == 'A') && (y[2] == 'T') && (y[3] == 'H') &&
            (y[4] == '='))
        {
            x = y + 5;
            break;
        }
    }

    if (x == (char *)0)
    {
        x = "/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin";
    }

    y = buffer;

    while (y < (buffer + BUFFERSIZE - 1))
    {
        if ((*x == ':') || ((*x) == 0))
        {
            if (y != buffer) /* have at least one character */
            {
                *y = 0;
                y = buffer;
                sexpr b = make_string (buffer);
                sexpr f = sx_join (b, str_slash, programme);

                if (truep (filep (f)))
                {
                    return f;
                }
            }

            if ((*x) == 0)
            {
                return sx_false;
            }
        }
        else
        {
            *y = *x;
            y++;
        }

        x++;
    }

    return sx_false;
}
