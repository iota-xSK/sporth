#include "plumber.h"

int sporth_smoothdelay(sporth_stack *stack, void *ud)
{
    plumber_data *pd = ud;
    SPFLOAT in;
    SPFLOAT out;
    SPFLOAT maxdel;
    uint32_t interp;
    SPFLOAT feedback;
    SPFLOAT del;
    sp_smoothdelay *smoothdelay;

    switch(pd->mode) {
        case PLUMBER_CREATE:

#ifdef DEBUG_MODE
            fprintf(stderr, "smoothdelay: Creating\n");
#endif

            sp_smoothdelay_create(&smoothdelay);
            plumber_add_ugen(pd, SPORTH_SMOOTHDELAY, smoothdelay);
            if(sporth_check_args(stack, "fffff") != SPORTH_OK) {
                fprintf(stderr,"Not enough arguments for smoothdelay\n");
                stack->error++;
                return PLUMBER_NOTOK;
            }
            maxdel = sporth_stack_pop_float(stack);
            interp = sporth_stack_pop_float(stack);
            del = sporth_stack_pop_float(stack);
            feedback = sporth_stack_pop_float(stack);
            in = sporth_stack_pop_float(stack);
            sporth_stack_push_float(stack, 0);
            break;
        case PLUMBER_INIT:

#ifdef DEBUG_MODE
            fprintf(stderr, "smoothdelay: Initialising\n");
#endif

            maxdel = sporth_stack_pop_float(stack);
            interp = sporth_stack_pop_float(stack);
            del = sporth_stack_pop_float(stack);
            feedback = sporth_stack_pop_float(stack);
            in = sporth_stack_pop_float(stack);
            smoothdelay = pd->last->ud;
            sp_smoothdelay_init(pd->sp, smoothdelay, maxdel, interp);
            sporth_stack_push_float(stack, 0);
            break;
        case PLUMBER_COMPUTE:
            maxdel = sporth_stack_pop_float(stack);
            interp = sporth_stack_pop_float(stack);
            del = sporth_stack_pop_float(stack);
            feedback = sporth_stack_pop_float(stack);
            in = sporth_stack_pop_float(stack);
            smoothdelay = pd->last->ud;
            smoothdelay->feedback = feedback;
            smoothdelay->del = del;
            sp_smoothdelay_compute(pd->sp, smoothdelay, &in, &out);
            sporth_stack_push_float(stack, out);
            break;
        case PLUMBER_DESTROY:
            smoothdelay = pd->last->ud;
            sp_smoothdelay_destroy(&smoothdelay);
            break;
        default:
            fprintf(stderr, "smoothdelay: Unknown mode!\n");
            break;
    }
    return PLUMBER_OK;
}
