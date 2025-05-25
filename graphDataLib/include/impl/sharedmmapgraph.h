//
// Created by david on 10/21/23.
//

#ifndef GRAPHDATA_SHAREDMMAPGRAPH_H
#define GRAPHDATA_SHAREDMMAPGRAPH_H

#include <graphData.h>
#include <util/memmgt.h>

struct shmmapdata_t {
    size_t nodelen;
    size_t degree;
    size_t arraylen;

    char *nodefile;
    char *flowfile;
    char *capfile;

    int nodefd;
    int flowfd;
    int capfd;
};

#endif //GRAPHDATA_SHAREDMMAPGRAPH_H
