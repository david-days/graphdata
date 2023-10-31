//
// Created by david on 11/18/17.
//

#include <util/cartesian.h>
#include <stdarg.h>

/**
 * @brief Sets array index value of the given set of dimensions, when calculated against the given dimensional array.
 *
 * This is function is used to provide transform between zero-based coordinates to a zero-based array index, for example.
 *
 * @param dims Dimensions to be use for index calculation
 * @param idx size_t reference to be set
 * @param coords array pointer of length dims->dimcount, holding the coordinates in question
 * @return 0 if successful; 1 if the coordinates are outside the bounds of the dimensions; -1 if there is a problem with the
 * values passed.
 */
short indexFromCartesian(struct dimensions_t *dims, size_t *idx, size_t *coords) {
    short retval = OP_FAIL;
    if (NULL != dims && NULL != idx && NULL != coords) {
        size_t dxval = coords[0];
        size_t dimmult = dims->dimarr[0];
        for (size_t i=1;i<dims->dimcount;i++) {
            dxval += dimmult*coords[i];
            dimmult *= dims->dimarr[i];
        }
        size_t dlen = cartesianIndexLength(dims);
        if (dxval >= dlen) {
            retval = OP_SUCCESS;
        } else {
            retval = OP_FAIL;
        }
        *idx = dxval;
    }
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
short cartesianFromIndex(size_t *idx, size_t *coords, struct dimensions_t *dims) {
    short retval = OP_FAIL;
    if (NULL != idx && NULL != coords && NULL != dims) {
        size_t dimdiv = cartesianIndexLength(dims);
        if (*idx < dimdiv) {
            size_t remval = *idx;
            if (dimdiv > 0) {
                //we have a real coordinate system
                for (size_t i = dims->dimcount - 1; i > 0; i--) {
                    dimdiv /= dims->dimarr[i];
                    coords[i] = remval / dimdiv;
                    remval = *idx % dimdiv;
                }
                coords[0] = remval;
            }
            retval = OP_SUCCESS;
        } else {
            retval = OP_FAIL;
        }
    }
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
size_t cartesianIndexLength(struct dimensions_t *dims) {
    size_t retval = 0;
    if (NULL != dims) {
        retval = 1;
        for (int i=0;i<dims->dimcount;i++) {
            retval *= dims->dimarr[i];
        }
    }
    return retval;
}
