#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sndkit/lil.h"
#include "sndkit/patchwerk.h"
#include "sndkit/core.h"
#include "sndkit/sklil.h"
#include "gest.h"

int sk_node_gesticulate(sk_core *core);
int sk_node_gestweight(sk_core *core);
gest_d * sk_node_gestnew(sk_core *core);

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
    SKLIL_ARITY_CHECK(lil, "gest_target", argc, 1);
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
}
