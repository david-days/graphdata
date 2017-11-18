//
// Created by david on 11/18/17.
//

#include <spatial.h>

/**
 * @brief Sets array index value of the given set of dimensions, when calculated against the given dimensional array.
 *
 * This is function is used to provide transform between zero-based coordinates to a zero-based array index, for example.
 *
 * @param dims Dimensions to be use for index calculation
 * @param idx size_t reference to be set
 * @param dimsz Number of dimensions in question--can pass dims->dimcount for consistency
 * @param ... zero-based spatial coordinates for calculation
 * @return 0 if successful; 1 if the coordinates are outside the bounds of the dimensions; -1 if there is a problem with the
 * values passed.
 */
int indexFromDimensions(struct dimensions_t *dims, size_t *idx, size_t dimsz, ...) {
    int retval = -1;

    return retval;
}

/**
 * @brief Sets the size_t array (whose length must be initialized to match dims->dimcount) to the coordinates within
 * the given spatial dimensions
 *
 * This function is used to transform between zero-based coordinates to a zero-based array index, for example.
 *
 * @param idx Index value to be used
 * @param coords Array to hold the result
 * @param dims Dimensional value to be calculated against
 * @return 0 if successful; 1 if the index is outside the bounds of the dimensions; -1 if there is a problem with the values passed.
 */
int spatialFromIndex(size_t *idx, size_t *coords, struct dimensions_t *dims) {
    int retval = -1;


    return retval;
}

/**
 * @brief Calculates the index length, based on the given coordinates
 *
 * This function multiplies the given dimensions and returns the size of an array that would be necessary to
 * hold all possible dimension values.  This is provided both as a consistent check and a means to consistently
 * return the proper value.
 *
 * @param dims Dimensions to be checked
 * @return Size of index calculation, or 0 if the dims pointer is NULL.
 */
size_t indexLength(struct dimensions_t *dims) {
    size_t retval = 0;
    if (NULL != dims) {
        retval = 1;
        for (int i=0;i<dims->dimcount;i++) {
            retval *= dims->dimarr[i];
        }
    }
    return retval;
}
