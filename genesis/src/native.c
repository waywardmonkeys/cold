/*
// Full copyright information is available in the file ../doc/CREDITS
//
// this file handles native methods
*/

#define _native_

#include "defs.h"
#include "cdc_pcode.h"
#include "cdc_db.h"
#include "moddef.h"

Int init_modules(Int argc, char ** argv) {
    Int m;

    for (m=0; m < NUM_MODULES; m++) {
        if (cold_modules[m]->init != (void *) NULL)
            cold_modules[m]->init(argc, argv);
    }

    return 1;
}

Int uninit_modules(void) {
    Int m;

    for (m=0; m < NUM_MODULES; m++) {
        if (cold_modules[m]->init != (void *) NULL)
            cold_modules[m]->uninit();
    }

    return 1;
}

#undef _native_
