/*
// Full copyright information is available in the file ../doc/CREDITS
//
// Floating point functions (including trig) originally coded by
// Andy Selle (andy@positronic.res.cmu.edu)
//
// converted to natives by Brandon Gillespie, a bit of optimization performed
// on how args are handled.  Standardized floats to float hooks, ColdC ints
// to double hooks in the math libs.  Added a few more hooks.
*/

#include "defs.h"

#include <time.h>
#include <math.h>
#include "cdc_pcode.h"
#include "util.h"
#include "sig.h"

#ifndef PI
#define PI 3.141592654
#endif

#define HANDLE_FPE \
        if (caught_fpe) { \
            caught_fpe = 0; \
            THROW((fpe_id, "floating-point exception")); \
        }

#define MATH_HOOK_FPE_2(_name_) \
    COLDC_FUNC(_name_) { \
        double r = 0; \
        cData * args; \
        if (!func_init_2(&args, FLOAT, FLOAT))\
            return; \
        r = _name_ ((double) _FLOAT(ARG1), (double) _FLOAT(ARG2)); \
        HANDLE_FPE; \
        pop(2); push_float((cFloat) r);\
    }

#define MATH_HOOK_FPE( _name_ ) \
    COLDC_FUNC(_name_) { \
        double r = 0; \
        cData * args; \
        if (!func_init_1(&args, FLOAT))\
            return; \
        r = _name_ ((double) _FLOAT(ARG1)); \
        HANDLE_FPE; \
        pop(1); push_float((cFloat) r);\
    }

#define MATH_HOOK(_name_) \
    COLDC_FUNC(_name_) { \
        cData * args; \
        if (!func_init_1(&args, FLOAT))\
            return; \
        pop(1); push_float((cFloat) _name_ ((double) _FLOAT(ARG1))); \
    }

MATH_HOOK(sin)
MATH_HOOK(exp)
MATH_HOOK_FPE(log)
MATH_HOOK(cos)
MATH_HOOK_FPE(tan)
MATH_HOOK_FPE(sqrt)
MATH_HOOK_FPE(asin)
MATH_HOOK_FPE(acos)
MATH_HOOK(atan)
MATH_HOOK_FPE_2(pow)
MATH_HOOK_FPE_2(atan2)

COLDC_FUNC(random) {
    cData * args;

    /* Take one integer argument. */
    if (!func_init_1(&args, INTEGER))
        return;

    /* Replace argument on stack with a random number. */
    _INT(ARG1) = random_number(_INT(ARG1)) + 1;
}

/* which is 1 for max, -1 for min. */
INTERNAL void find_extreme(Int which) {
    Int arg_start, num_args, i, type;
    cData *args, *extreme, d;

    arg_start = arg_starts[--arg_pos];
    args = &stack[arg_start];
    num_args = stack_pos - arg_start;

    if (!num_args) {
        cthrow(numargs_id, "Called with no arguments, requires at least one.");
        return;
    }

    type = args[0].type;
    if (type != INTEGER && type != STRING && type != FLOAT) {
        cthrow(type_id, "First argument (%D) not an integer, float or string.",
              &args[0]);
        return;
    }

    extreme = &args[0];
    for (i = 1; i < num_args; i++) {
        if (args[i].type != type) {
            cthrow(type_id, "Arguments are not all of same type.");
            return;
        }
        if (data_cmp(&args[i], extreme) * which > 0)
            extreme = &args[i];
    }

    /* Replace args[0] with extreme, and pop other arguments. */
    data_dup(&d, extreme);
    data_discard(&args[0]);
    args[0] = d;
    pop(num_args - 1);
}

COLDC_FUNC(max) {
    find_extreme(1);
}

COLDC_FUNC(min) {
    find_extreme(-1);
}

COLDC_FUNC(abs) {
    cData * args;

    if (!func_init_1(&args, ANY_TYPE))
        return;

    if (args[0].type == INTEGER) {
        if (_INT(ARG1) < 0)
            _INT(ARG1) = -_INT(ARG1);
    } else if (args[0].type == FLOAT) {
        _FLOAT(ARG1) = (cFloat) fabs((double) _FLOAT(ARG1));
    }
}

