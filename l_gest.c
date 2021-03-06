#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h> /* for rseed */
#include "sndkit/lil/lil.h"
#include "sndkit/graforge/graforge.h"
#include "sndkit/core.h"
#include "sndkit/nodes/sklil.h"
#include "gest.h"

int sk_node_gesticulate(sk_core *core);
int sk_node_gestweight(sk_core *core);
gest_d * sk_node_gestnew(sk_core *core);
gest_scalar * sk_node_scalarnew(sk_core *core);
int sk_node_gescalar(sk_core *core);
int sk_node_gestick(sk_core *core);
int sk_node_conductor(sk_core *core);

static lil_value_t gesticulate(lil_t lil,
                               size_t argc,
                               lil_value_t *argv)
{
    sk_core *core;
    int rc;
    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "gesticulate", argc, 2);

    rc = sk_node_gesticulate(core);
    SKLIL_ERROR_CHECK(lil, rc, "gesticulate didn't work out.");
    return NULL;
}

static lil_value_t gestweight(lil_t lil,
                              size_t argc,
                              lil_value_t *argv)
{
    sk_core *core;
    int rc;
    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "gestweight", argc, 1);

    rc = sk_node_gestweight(core);
    SKLIL_ERROR_CHECK(lil, rc, "gestweight didn't work out.");
    return NULL;
}

static lil_value_t gest_new(lil_t lil,
                            size_t argc,
                            lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    core = lil_get_data(lil);
    g = sk_node_gestnew(core);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_begin(lil_t lil,
                                size_t argc,
                                lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    int nbeats, div;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "gest_begin", argc, 2);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    nbeats = lil_to_integer(argv[0]);
    div = lil_to_integer(argv[1]);

    gest_begin(g, nbeats, div);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_end(lil_t lil,
                              size_t argc,
                              lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    rc = gest_end(g);

    if (rc) {
        lil_set_error(lil, "phrase did not close properly\n");
    }

    sk_core_generic_push(core, g);

    return NULL;
}

static lil_value_t l_gest_finish(lil_t lil,
                                 size_t argc,
                                 lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    gest_finish(g);
    return NULL;
}

static lil_value_t l_gest_addtarget(lil_t lil,
                                    size_t argc,
                                    lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    SKFLT val;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "gest_target", argc, 1);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    val = lil_to_double(argv[0]);

    rc = gest_addtarget(g, val);

    if (rc) {
        lil_set_error(lil, "Could not add target\n");
        return NULL;
    }

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_polyramp(lil_t lil,
                                   size_t argc,
                                   lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    int div;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "gest_polyramp", argc, 1);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    div = lil_to_integer(argv[0]);

    gest_polyramp(g, div);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_monoramp(lil_t lil,
                                   size_t argc,
                                   lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    int nbeats;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "gest_monoramp", argc, 1);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    nbeats = lil_to_integer(argv[0]);

    gest_monoramp(g, nbeats);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_linear(lil_t lil,
                                 size_t argc,
                                 lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    gest_behavior_linear(g);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_step(lil_t lil,
                               size_t argc,
                               lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    gest_behavior_step(g);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_inertia(lil_t lil,
                                  size_t argc,
                                  lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    SKFLT val;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "gest_inertia", argc, 1);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    val = lil_to_double(argv[0]);

    gest_inertia(g, val);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_mass(lil_t lil,
                               size_t argc,
                               lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    SKFLT val;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "gest_mass", argc, 1);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    val = lil_to_double(argv[0]);

    gest_mass(g, val);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_loopit(lil_t lil,
                                 size_t argc,
                                 lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    gest_loopit(g);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_gliss(lil_t lil,
                                size_t argc,
                                lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    gest_behavior_gliss(g);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_smallgliss(lil_t lil,
                                     size_t argc,
                                     lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    gest_behavior_smallgliss(g);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_exponential(lil_t lil,
                                      size_t argc,
                                      lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    SKFLT val;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "gest_exponential", argc, 1);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    val = lil_to_double(argv[0]);

    gest_behavior_exponential(g, val);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_bezier(lil_t lil,
                                 size_t argc,
                                 lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    SKFLT cx;
    SKFLT cy;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "gest_bezier", argc, 2);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    cx = lil_to_double(argv[0]);
    cy = lil_to_double(argv[1]);

    gest_behavior_bezier(g, cx, cy);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_metatarget(lil_t lil,
                                    size_t argc,
                                    lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    int sz;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "gest_metatarget", argc, 1);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    sz = lil_to_integer(argv[0]);

    rc = gest_addmetatarget(g, sz);

    if (rc) {
        lil_set_error(lil, "Could not add metatarget\n");
        return NULL;
    }

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_mediumgliss(lil_t lil,
                                     size_t argc,
                                     lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    gest_behavior_mediumgliss(g);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_metabehavior(lil_t lil,
                                       size_t argc,
                                       lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    int sz;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "gest_target", argc, 1);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    sz = lil_to_integer(argv[0]);

    rc = gest_addmetabehavior(g, sz);

    if (rc) {
        lil_set_error(lil, "Could not add metabehavior\n");
        return NULL;
    }

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_metanode(lil_t lil,
                                   size_t argc,
                                   lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    int sz;
    int nbeats;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "gest_metanode", argc, 2);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    nbeats = lil_to_integer(argv[0]);
    sz = lil_to_integer(argv[1]);

    rc = gest_addmetanode(g, nbeats, sz);

    if (rc) {
        lil_set_error(lil, "Could not add metanode.");
        return NULL;
    }

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_metaphrase(lil_t lil,
                                     size_t argc,
                                     lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    int sz;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "gest_metaphrase", argc, 1);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    sz = lil_to_integer(argv[0]);

    rc = gest_addmetaphrase(g, sz);

    if (rc) {
        lil_set_error(lil, "Could not add metaphrase.");
        return NULL;
    }

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_randtarget(lil_t lil,
                                     size_t argc,
                                     lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    rc = gest_randtarget(g);

    if (rc) {
        lil_set_error(lil, "Could not configure randtarget.");
        return NULL;
    }

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_randbehavior(lil_t lil,
                                     size_t argc,
                                     lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    rc = gest_randbehavior(g);

    if (rc) {
        lil_set_error(lil, "Could not configure randbehavior.");
        return NULL;
    }

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_randnode(lil_t lil,
                                   size_t argc,
                                   lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    rc = gest_randnode(g);

    if (rc) {
        lil_set_error(lil, "Could not configure randnode.");
        return NULL;
    }

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_randphrase(lil_t lil,
                                   size_t argc,
                                   lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    rc = gest_randphrase(g);

    if (rc) {
        lil_set_error(lil, "Could not configure randphrase.");
        return NULL;
    }

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_scalarnew(lil_t lil,
                                    size_t argc,
                                    lil_value_t *argv)
{
    sk_core *core;
    gest_scalar *s;
    core = lil_get_data(lil);
    s = sk_node_scalarnew(core);

    sk_core_generic_push(core, s);
    return NULL;
}

static lil_value_t l_gest_setscalar(lil_t lil,
                                    size_t argc,
                                    lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    gest_scalar *s;
    SKFLT val;
    int rc;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "scalar", argc, 2);

    /* get scalar from sndkit stack */
    /* the first argument in LIL is a placeholder for this */
    rc = sk_core_generic_pop(core, (void **)&s);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get scalar data.");

    /* gest is an implicit argument. */
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");


    val = lil_to_double(argv[1]);
    rc = gest_setscalar(g, s, val);

    if (rc) {
        lil_set_error(lil, "Problems with setscalar.");
        return NULL;
    }

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gescalar(lil_t lil,
                                size_t argc,
                                lil_value_t *argv)
{
    sk_core *core;
    int rc;
    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "gescalar", argc, 1);

    /* argument is implictely on the sndkit stack */
    /* the arity check in LIL is mainly symbolic */

    rc = sk_node_gescalar(core);
    SKLIL_ERROR_CHECK(lil, rc, "gescalar didn't work out.");
    return NULL;
}

static lil_value_t l_gestick(lil_t lil,
                             size_t argc,
                             lil_value_t *argv)
{
    sk_core *core;
    int rc;
    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "gestick", argc, 1);

    /* argument is implictely on the sndkit stack */
    /* the arity check in LIL is mainly symbolic */

    rc = sk_node_gestick(core);
    SKLIL_ERROR_CHECK(lil, rc, "gestick didn't work out.");
    return NULL;
}

static lil_value_t l_gest_repeat(lil_t lil,
                                 size_t argc,
                                 lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    int nreps;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "gest_repeat", argc, 1);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    nreps = lil_to_integer(argv[0]);

    rc = gest_repeat(g, nreps);

    if (rc) {
        lil_set_error(lil, "Could not add metaphrase.");
        return NULL;
    }

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_rseed(lil_t lil,
                                size_t argc,
                                lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    SKLIL_ARITY_CHECK(lil, "gest_rseed", argc, 1);
    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    if (argc > 1) {
        int seed;
        seed = lil_to_integer(argv[1]);
        gest_seed(g, seed);
    } else {
        gest_seed(g, sk_core_rand(core));
    }

    return NULL;
}

static lil_value_t l_gest_mark(lil_t lil,
                               size_t argc,
                               lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    rc = gest_mark(g);

    SKLIL_ERROR_CHECK(lil, rc, "mark failed. Is there a phrase?");
    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_return(lil_t lil,
                                 size_t argc,
                                 lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    int ntimes;

    SKLIL_ARITY_CHECK(lil, "gest_return", argc, 1);
    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");


    ntimes = lil_to_integer(argv[0]);
    rc = gest_return(g, ntimes);

    SKLIL_ERROR_CHECK(lil, rc, "gest return failed.");
    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_skewquad(lil_t lil,
                                    size_t argc,
                                    lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    int dir;

    SKLIL_ARITY_CHECK(lil, "gest_skewquad", argc, 1);
    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");


    dir = lil_to_integer(argv[0]);
    rc = gest_skewquad(g, dir);

    SKLIL_ERROR_CHECK(lil, rc, "gest return failed.");
    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_skewexp(lil_t lil,
                                    size_t argc,
                                    lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    int dir;

    SKLIL_ARITY_CHECK(lil, "gest_skewexp", argc, 1);
    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");


    dir = lil_to_integer(argv[0]);
    rc = gest_skewexp(g, dir);

    SKLIL_ERROR_CHECK(lil, rc, "gest skewexp failed.");
    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_skewshuf(lil_t lil,
                                   size_t argc,
                                   lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    rc = gest_skewshuf(g);

    SKLIL_ERROR_CHECK(lil, rc, "gest skewshuf failed.");
    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_shrink(lil_t lil,
                                 size_t argc,
                                 lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    SKFLT amt;

    SKLIL_ARITY_CHECK(lil, "gest_shrink", argc, 1);
    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    amt = lil_to_double(argv[0]);
    rc = gest_shrink(g, amt);

    SKLIL_ERROR_CHECK(lil, rc, "gest shrink failed.");
    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_grow(lil_t lil,
                               size_t argc,
                               lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    SKFLT amt;

    SKLIL_ARITY_CHECK(lil, "gest_grow", argc, 1);
    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    amt = lil_to_double(argv[0]);
    rc = gest_grow(g, amt);

    SKLIL_ERROR_CHECK(lil, rc, "gest grow failed.");
    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_smoothstep(lil_t lil,
                                     size_t argc,
                                     lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    gest_behavior_smoothstep(g);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_ramp(lil_t lil,
                               size_t argc,
                               lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    gest_ramp(g);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_invramp(lil_t lil,
                                  size_t argc,
                                  lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    gest_invramp(g);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_biramp(lil_t lil,
                                 size_t argc,
                                 lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    SKFLT pos;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "gest_biramp", argc, 1);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    pos = lil_to_double(argv[0]);

    gest_behavior_biramp(g, pos);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_gate(lil_t lil,
                               size_t argc,
                               lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    SKFLT pos;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "gest_gate", argc, 1);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    pos = lil_to_double(argv[0]);

    gest_behavior_gate(g, pos);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gestcnd(lil_t lil,
                             size_t argc,
                             lil_value_t *argv)
{
    sk_core *core;
    int rc;
    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "gestcnd", argc, 1);

    rc = sklil_param(core, argv[0]);
    SKLIL_PARAM_CHECK(lil, rc, "gstcnd");

    rc = sk_node_conductor(core);
    SKLIL_ERROR_CHECK(lil, rc, "gestcnd didn't work out.");

    return NULL;
}

static lil_value_t l_gest_squawk(lil_t lil,
                                 size_t argc,
                                 lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    int squawk;

    SKLIL_ARITY_CHECK(lil, "gest_squawk", argc, 1);

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    squawk = lil_to_integer(argv[0]);
    gest_squawk(g, squawk);

    sk_core_generic_push(core, g);
    return NULL;
}

static lil_value_t l_gest_tolerance(lil_t lil,
                                    size_t argc,
                                    lil_value_t *argv)
{
    sk_core *core;
    gest_d *g;
    int rc;
    SKFLT tol;

    SKLIL_ARITY_CHECK(lil, "gest_tolerance", argc, 1);

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, (void **)&g);
    SKLIL_ERROR_CHECK(lil, rc, "couldn't get gest data.");

    tol = lil_to_double(argv[0]);
    gest_tolerance(g, tol);

    sk_core_generic_push(core, g);
    return NULL;
}

void sklil_load_gest(lil_t lil)
{
    lil_register(lil, "gest_new", gest_new);
    lil_register(lil, "gesticulate", gesticulate);
    lil_register(lil, "gestweight", gestweight);
    lil_register(lil, "gest_begin", l_gest_begin);
    lil_register(lil, "gest_end", l_gest_end);
    lil_register(lil, "gest_finish", l_gest_finish);
    lil_register(lil, "gest_target", l_gest_addtarget);
    lil_register(lil, "gest_polyramp", l_gest_polyramp);
    lil_register(lil, "gest_monoramp", l_gest_monoramp);
    lil_register(lil, "gest_linear", l_gest_linear);
    lil_register(lil, "gest_step", l_gest_step);
    lil_register(lil, "gest_inertia", l_gest_inertia);
    lil_register(lil, "gest_mass", l_gest_mass);
    lil_register(lil, "gest_loopit", l_gest_loopit);
    lil_register(lil, "gest_gliss", l_gest_gliss);
    lil_register(lil, "gest_smallgliss", l_gest_smallgliss);
    lil_register(lil, "gest_exponential", l_gest_exponential);
    lil_register(lil, "gest_bezier", l_gest_bezier);
    lil_register(lil, "gest_metatarget", l_gest_metatarget);
    lil_register(lil, "gest_mediumgliss", l_gest_mediumgliss);
    lil_register(lil, "gest_metabehavior", l_gest_metabehavior);
    lil_register(lil, "gest_metanode", l_gest_metanode);
    lil_register(lil, "gest_metaphrase", l_gest_metaphrase);
    lil_register(lil, "gest_randtarget", l_gest_randtarget);
    lil_register(lil, "gest_randbehavior", l_gest_randbehavior);
    lil_register(lil, "gest_randnode", l_gest_randnode);
    lil_register(lil, "gest_randphrase", l_gest_randphrase);
    lil_register(lil, "gest_scalarnew", l_gest_scalarnew);
    lil_register(lil, "gest_setscalar", l_gest_setscalar);
    lil_register(lil, "gescalar", l_gescalar);
    lil_register(lil, "gestick", l_gestick);
    lil_register(lil, "gest_repeat", l_gest_repeat);
    lil_register(lil, "gest_rseed", l_gest_rseed);
    lil_register(lil, "gest_mark", l_gest_mark);
    lil_register(lil, "gest_return", l_gest_return);
    lil_register(lil, "gest_skewquad", l_gest_skewquad);
    lil_register(lil, "gest_skewexp", l_gest_skewexp);
    lil_register(lil, "gest_skewshuf", l_gest_skewshuf);
    lil_register(lil, "gest_shrink", l_gest_shrink);
    lil_register(lil, "gest_grow", l_gest_grow);
    lil_register(lil, "gest_smoothstep", l_gest_smoothstep);
    lil_register(lil, "gest_ramp", l_gest_ramp);
    lil_register(lil, "gest_invramp", l_gest_invramp);
    lil_register(lil, "gest_biramp", l_gest_biramp);
    lil_register(lil, "gest_gate", l_gest_gate);
    lil_register(lil, "gestcnd", l_gestcnd);
    lil_register(lil, "gest_squawk", l_gest_squawk);
    lil_register(lil, "gest_tolerance", l_gest_tolerance);
}
