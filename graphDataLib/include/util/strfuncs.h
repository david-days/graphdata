//
// Created by david on 11/24/23.
//

#ifndef STRFUNCS_H
#define STRFUNCS_H

/**
 * Generates a UUID string and places it in the allocated uchars array
 *
 * @param uchars Pre-allocated array of UUID_STR_LEN length
 * @return OP_SUCCESS if successful, otherwise OP_FAIL
 */
short gen_uuid(char *uchars);

#endif //STRFUNCS_H
