//
// Created by david on 11/24/23.
//

#include <stdlib.h>
#include <stdio.h>
#include <uuid/uuid.h>
#include <graphData.h>

short gen_uuid(char *uchars) {
    short retVal = OP_SUCCESS;
    uuid_t binuuid;
    uuid_generate_random(binuuid);
    uuid_unparse(binuuid, uchars);
    return retVal;
}
