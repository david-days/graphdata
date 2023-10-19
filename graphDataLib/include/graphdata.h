/**
 * @brief Basic graph data definitions.
 *
 * This header contains the basic structures and definitions for graph data.
 */

#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#define GRAPHDATA_VERSION_MAJOR @GRAPHDATA_VERSION_MAJOR@
#define GRAPHDATA_VERSION_MINOR @GRAPHDATA_VERSION_MINOR@
#define GRAPHDATA_VERSION_PATCH @GRAPHDATA_VERSION_PATCH@

#include<stdio.h>

/**
 * @brief Descriptor of the graph structure and usage.
 *
 * This enum is used to described, as best as possible, the types of operations that will be necessary for this graph to
 * fulfill the consumer's needs.
 */
enum GRAPHDOMAIN {
    //Graph type values
    /**
     * @brief An undirected graph structure e(u,v) == e(v,u)
     *
     * The minimum number of edges will be used to represent connectivity between nodes.
     */
    UNDIRECTED  = 0x0001,
    /**
     * @brief A directed graph structure e(u,v) != e(v,u)
     *
     * Each edge description {u,v} will be distinct.
     */
    DIRECTED    = 0x0002,
    //Underlying Implementation
    /**
     * @brief The underlying implementation is an array-based structure
     */
    ARRAY       = 0x0010,
    /**
     * @brief The underlying structure is a double-linked list of nodes and edges
     */
    LINKED      = 0x0020,
    /**
     * @brief The underlying structure is a set of adjacency lists in hashtable form.
     */
    HASHED      = 0x0040,
    //Graph domain
    /**
     * @brief The graph represents a generic structure with no particular standardized relationship between nodes
     */
    GENERIC     = 0x0100,
    /**
     * @brief The graph represents an ordered set of nodes within a coordinate system, and edges are related to spatial
     * "neighborhoods".
     */
    SPATIAL     = 0x0200,
    //Specialization
    /**
     * @brief The graph has no specialized node values
     */
    UNLABELED   = 0x1000,
    /**
     * @brief The graph has a set of nodes representing categorical labeling of associated nodes.
     */
    LABELED     = 0x2000

};

/**
 * @brief Feature/attribute structure for a graph item.
 *
 * Features hold attribute values for the associated graph structure member (node, edge).  This structure can be used
 * to store additional information or computation results.
 */
struct feature_t {
    /**
     * @brief identifier for the feature type
     *
     * Feature identifier, unique for a feature type.  Basic concept is a hash of the feature name, though other implementations
     * are possible.
     */
    size_t hashid;
    /**
     * @brief text name of the feature type.
     */
    char *featurename;
    /**
     * Value of the feature.
     */
    double val;
    /**
     * Associated feature data.
     */
    void *fdata;
    /**
     * Previous feature in the list.
     */
    struct feature_t *prev;
    /**
     * Next feature in the list.
     */
    struct feature_t *next;
};

/**
 * @brief Graph node structure
 *
 * This structure represents individual graph nodes and associated structures.  Actual usage of the structure associations
 * is up to the individual implementations.
 */
struct node_t {
    /**
     * Node identifier
     */
    size_t nodeid;
    /**
     * Attached features, if any.  Initialized as NULL for all implementations.
     */
    struct feature_t *attrs;
    /**
     * Attached edges, if any.
     */
    struct edge_t *edges;
    /**
     * @brief Previous  node, as required.  Typical usage would be in node-based path list.
     */
    struct node_t *prev;
    /**
     * @brief Next node, as required.  Typical usage would be in node-based path list.
     */
    struct node_t *next;
};

/**
 * @brief Graph edge structure, representing linkages between nodes.
 *
 * The interpretation of an edge (path, traversal, relationship, association) is usage-dependent.  In general, this structure contains
 * enough information to carry out typical graph operations and algorithms.
 */
struct edge_t {
    /**
     * Starting node
     */
    size_t u;
    /**
     * Ending node
     */
    size_t v;
    /**
     * Baseline capacity or cost value.
     */
    double cap;
    /**
     * Additional flow (change in capacity value) for the edge.
     */
    double flow;
    /**
     * Features associated with the edge.
     */
    struct feature_t *attrs;
    /**
     * @brief Previous edge in the list.
     *
     * Typical usage would be either for normal edge lists, or for edge-based path values.
     */
    struct edge_t *prev;
    /**
     * @brief Next edge in the list.
     *
     * Typical usage would be either for normal adjacency lists, or for edge-based path values.
     */
    struct edge_t *next;
};



/**
 * @brief Structure to hold values for dimensional graphs
 *
 * This structure holds values defining the boundaries of a dimensional graph, such as one representing a spatial domain.
 */
struct dimensions_t {
    /**
     * Number of dimensions represented.
     */
    size_t dimcount;
    /**
     * Array pointer containing the upper dimensional boundary values (width, height, depth, etc).
     */
    size_t *dimarr;
};


/**
 * @brief Structure holding identifiers for "label" nodes.
 *
 * Generally, a label node is one that holds a unique or preferential status within a graph.  For segmentation or partition operations,
 * this may be the representative for a group.  For other operations, it may designate certain starting or ending points, or roots
 * to max- or min-trees or forests.
 */
struct labels_t {
    /**
     * Number of labels in the graph.
     */
    size_t labelcount;
    /**
     * Array pointer to the labels.
     */
    size_t *labelarr;
};



/**
 * @brief Structure containing the backing data for the graph representation.
 *
 * This is the representation of the graph, with necessary metadata (type, spatial dimensions, etc) as necessary. Consumers
 * choose the best implementation for their purposes, and pass around references as necessary.
 */
struct graph_t {
    /**
     * @brief Type of graph being represented.
     */
    enum GRAPHDOMAIN gtype;

    /**
     * @brief Dimensions structure for spatial graphs
     */
    struct dimensions_t *dims;

    /**
     * @brief Label data for the graph.
     */
    struct labels_t *labels;

    /**
     * @brief Implementation-specific pointer to metadata for graph structure.
     */
    void *metaImpl;

    /**
     * @brief Implementation-specific pointer to node backing data
     *
     * Central pointer to node data, to be interpreted by the specific graphops implementations
     */
    void *nodeImpl;
    /**
     * @brief Implementation-specific pointer to the edge backing data
     *
     * Central pointer to the edge data, to be interpreted by the specific graphops implementations.
     */
    void *edgeImpl;
    /**
     * @brief Implementation-specific pointer to capacity data.
     *
     * Central pointer to the capacity values, to be interpreted by the specific graphops implementations.
     */
    void *capImpl;
    /**
     * @brief Implementation-specific pointer to the flow data
     *
     * Central pointer to the flow values, to be interpreted by the specific graphops implementations.
     */
    void *flowImpl;
};


#endif //GRAPHDATA_H