//
// Created by david on 11/11/17.
//

#ifndef GRAPHDATA_GRAPHINIT_H
#define GRAPHDATA_GRAPHINIT_H

#include <graphdata.h>
#include <graphops.h>

/**
 * @brief Create a dimension structure containing the given values in order (x, y, z, etc)
 *
 * This is used to initialize (and contain) the ordered size of the spatial dimensions given.  Consumers are responsible
 * for calling free() on the structure, or passing it to a standard clearing function
 *
 * @param dimval First dimension in the ordered list of dimension limits
 * @param ... Additional dimension values passed
 * @return Pointer to a dimensions_t structure that holds the count and values of the structure
 */
struct dimensions_t * createDimensions(size_t dimval, ...);

/**
 * @brief Raw initializer for graphops_t structure
 *
 * Consumers of this object are responsible for calling free() on the pointer when finished, or passing it to
 * a standard cleanup function.
 *
 * @return Pointer to a graphops structure, or NULL if there was a problem with memory allocation
 */
struct graphops_t * initGraphops();


#endif //GRAPHDATA_GRAPHINIT_H
