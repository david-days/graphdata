#include "bfs_shortest_path.h"

#include <stdlib.h>

static int indexOf(const size_t *ids, size_t count, size_t id) {
    for (size_t i = 0; i < count; i++) {
        if (ids[i] == id) {
            return (int)i;
        }
    }
    return -1;
}

int bfsShortestPath(struct graphops_t *ops, struct graph_t *g,
                    size_t source, size_t target,
                    size_t *path_out, size_t path_cap, size_t *path_len) {
    struct node_t *listed = NULL;
    size_t *ids = NULL;
    size_t *dist = NULL;
    size_t *parent = NULL;
    size_t *queue = NULL;
    unsigned char *seen = NULL;
    size_t count = 0;
    size_t qhead = 0;
    size_t qtail = 0;
    int result = GRAPH_ERR;
    int source_at = -1;
    int target_at = -1;

    if (ops == NULL || g == NULL || path_out == NULL || path_len == NULL
        || ops->listNodes == NULL || ops->getNeighbors == NULL) {
        return GRAPH_ERR;
    }

    listed = ops->listNodes(g);
    for (struct node_t *node = listed; node != NULL; node = node->next) {
        count++;
    }
    if (count == 0 || path_cap < 1) {
        goto done;
    }

    ids = (size_t *)calloc(count, sizeof(size_t));
    dist = (size_t *)calloc(count, sizeof(size_t));
    parent = (size_t *)calloc(count, sizeof(size_t));
    queue = (size_t *)calloc(count, sizeof(size_t));
    seen = (unsigned char *)calloc(count, sizeof(unsigned char));
    if (ids == NULL || dist == NULL || parent == NULL || queue == NULL || seen == NULL) {
        goto done;
    }

    {
        size_t i = 0;
        for (struct node_t *node = listed; node != NULL; node = node->next) {
            ids[i] = node->nodeid;
            parent[i] = SIZE_MAX;
            i++;
        }
    }

    source_at = indexOf(ids, count, source);
    target_at = indexOf(ids, count, target);
    if (source_at < 0 || target_at < 0) {
        goto done;
    }

    seen[source_at] = 1;
    dist[source_at] = 0;
    queue[qtail++] = source;

    while (qhead < qtail) {
        size_t current = queue[qhead++];
        struct node_t *neighbors = NULL;
        int current_at = indexOf(ids, count, current);
        if (current == target) {
            break;
        }
        neighbors = ops->getNeighbors(&current, g);
        for (struct node_t *neighbor = neighbors; neighbor != NULL; neighbor = neighbor->next) {
            int next_at = indexOf(ids, count, neighbor->nodeid);
            if (next_at < 0 || seen[next_at]) {
                continue;
            }
            seen[next_at] = 1;
            dist[next_at] = dist[current_at] + 1;
            parent[next_at] = current;
            queue[qtail++] = neighbor->nodeid;
        }
        if (neighbors != NULL) {
            destroyNodes((void **)&neighbors);
        }
    }

    if (!seen[target_at]) {
        goto done;
    }

    {
        size_t length = dist[target_at] + 1;
        size_t cursor = target;
        if (length > path_cap) {
            goto done;
        }
        for (size_t i = length; i > 0; i--) {
            path_out[i - 1] = cursor;
            if (i == 1) {
                break;
            }
            cursor = parent[indexOf(ids, count, cursor)];
        }
        *path_len = length;
        result = GRAPH_OK;
    }

done:
    if (listed != NULL) {
        destroyNodes((void **)&listed);
    }
    free(ids);
    free(dist);
    free(parent);
    free(queue);
    free(seen);
    return result;
}
