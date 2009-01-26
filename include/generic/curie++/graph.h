/*
 *  graph.h
 *  libcurie
 *
 *  Created by Nadja Klein on 25/01/2009.
 *  Copyright 2008/2009 Magnus Deininger. All rights reserved.
 *  Copyright 2008/2009 Nadja Klein. All rights reserved.
 *
 */

/*
 * Copyright (c) 2008/2009, Magnus Deininger All rights reserved.
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

#ifndef LIBCURIEPP_GRAPH_H
#define LIBCURIEPP_GRAPH_H

#include "sexpr.h"

namespace curiepp {

  class Graph {
    private:
      int_32 nodeCount;
      Node **nodes;

    public:
      Graph();
      Graph(SExpr *);
      ~Graph();
      void addNode();
      Node *searchNode(SExpr *s);

      // converts a given graph to a S-expression
      SExpr* operator= ();

  };

  class Node {
    private:
      int_32 edgeCount;
      Edge **edges;

    public:
      Node(SExpr *l);
      ~Node();
      SExpr *label;
      void addEdge();
      Edge *searchEdge(SExpr *s);
  };

  class Edge {
    private:

    public:
      Edge(SExpr *l);
      ~Edge();
      SExpr *label;
      Node *target;

  };


}

#endif