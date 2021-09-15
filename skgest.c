#include <math.h>
#include <stdlib.h>
#include "sndkit/patchwerk.h"
#include "sndkit/core.h"
#include "gest.h"

struct gest_n {
    pw_cable *conductor;
    pw_cable *out;
    gest_d *gest;
};

struct gestweight_n {
    pw_cable *out;
    gest_d *gest;
    int sr;
    SKFLT a;
    SKFLT inertia;
    SKFLT prev;
    SKFLT scale;
};

static void compute(pw_node *node)
{
    int blksize;
    int n;
    struct gest_n *gest;

    blksize = pw_node_blksize(node);

    gest = (struct gest_n *)pw_node_get_data(node);

    for (n = 0; n < blksize; n++) {
        PWFLT out, conductor;
        conductor = pw_cable_get(gest->conductor, n);

        out = gest_tick(gest->gest, conductor);
        pw_cable_set(gest->out, n, out);
    }
}

static void compute_weight(pw_node *node)
{
    int blksize;
    int n;
    struct gestweight_n *gw;

    blksize = pw_node_blksize(node);

    gw = (struct gestweight_n *)pw_node_get_data(node);

    for (n = 0; n < blksize; n++) {
        PWFLT out;
        PWFLT i;

        i = gest_inertia_get(gw->gest);

        if (i != gw->inertia) {
            if (i == 0) {
                gw->a = 0;
                gw->prev = i;
            } else {
                gw->a = exp(-1.0 / (gw->sr * i));
            }
        }

        out =
            (1 - gw->a) * gest_mass_get(gw->gest) * gw->scale +
            gw->a * gw->prev;
        gw->prev = out;
        pw_cable_set(gw->out, n, out);
    }
}

static void destroy(pw_node *node)
{
    pw_patch *patch;
    int rc;
    void *ud;
    rc = pw_node_get_patch(node, &patch);
    if (rc != PW_OK) return;
    pw_node_cables_free(node);
    ud = pw_node_get_data(node);
    pw_memory_free(patch, &ud);
}

int pw_node_gesticulate(pw_node *node, gest_d *g)
{
    int rc;
    pw_patch *patch;
    struct gest_n *gest;
    void *ud;

    rc = pw_node_get_patch(node, &patch);
    if (rc != PW_OK) return rc;

    rc = pw_memory_alloc(patch, sizeof(struct gest_n), &ud);
    SK_PW_ERROR_CHECK(rc);
    gest = (struct gest_n *)ud;

    rc = pw_node_cables_alloc(node, 2);


    if (rc != PW_OK) return rc;

    pw_node_set_block(node, 1);

    pw_node_get_cable(node, 0, &gest->conductor);
    pw_node_get_cable(node, 1, &gest->out);
    gest->gest = g;

    pw_node_set_data(node, gest);
    pw_node_set_compute(node, compute);
    pw_node_set_destroy(node, destroy);
    return PW_OK;
}

int sk_node_gesticulate(sk_core *core)
{
    pw_patch *patch;
    pw_node *node;
    int rc;
    sk_param conductor;
    gest_d *g;

    rc = sk_param_get(core, &conductor);
    SK_ERROR_CHECK(rc);

    rc = sk_core_generic_pop(core, (void **)&g);
    SK_ERROR_CHECK(rc);

    patch = sk_core_patch(core);

    rc = pw_patch_new_node(patch, &node);
    SK_PW_ERROR_CHECK(rc);

    rc = pw_node_gesticulate(node, g);
    SK_PW_ERROR_CHECK(rc);

    sk_param_set(core, node, &conductor, 0);
    sk_param_out(core, node, 1);
    return 0;
}

int pw_node_gestweight(pw_node *node, gest_d *g)
{
    struct gestweight_n *gw;
    void *ud;
    int rc;
    pw_patch *patch;

    rc = pw_node_get_patch(node, &patch);
    if (rc != PW_OK) return rc;

    rc = pw_memory_alloc(patch, sizeof(struct gestweight_n), &ud);
    if (rc != PW_OK) return rc;
    gw = (struct gestweight_n *)ud;

    gw->gest = g;
    gw->sr = pw_patch_srate_get(patch);
    gw->inertia = 0;
    gw->a = 0;
    gw->scale = 1.0 / 120.0;
    gw->prev = 0;

    rc = pw_node_cables_alloc(node, 1);
    if (rc != PW_OK) return rc;

    pw_node_set_block(node, 0);

    pw_node_get_cable(node, 0, &gw->out);

    pw_node_set_data(node, gw);
    pw_node_set_compute(node, compute_weight);
    pw_node_set_destroy(node, destroy);


    return PW_OK;
}

int sk_node_gestweight(sk_core *core)
{
    pw_patch *patch;
    pw_node *node;
    int rc;
    gest_d *g;

    rc = sk_core_generic_pop(core, (void **)&g);
    SK_ERROR_CHECK(rc);

    patch = sk_core_patch(core);

    rc = pw_patch_new_node(patch, &node);
    SK_PW_ERROR_CHECK(rc);

    rc = pw_node_gestweight(node, g);
    SK_PW_ERROR_CHECK(rc);

    sk_param_out(core, node, 0);
    return 0;
}

static void cleanup_gest(pw_pointer *p)
{
    gest_d *g;
    g = (gest_d *)pw_pointer_data(p);
    gest_cleanup(g);
    free(g);
}

static void cleanup_scalar(pw_pointer *p)
{
    gest_scalar *s;
    s = (gest_scalar *)pw_pointer_data(p);
    free(s);
}

gest_d *gest_pw_alloc(pw_patch *patch)
{
    gest_d *g;
    g = malloc(gest_sizeof());
    gest_init(g);
    pw_patch_append_userdata(patch, cleanup_gest, g);
    return g;
}

gest_d * sk_node_gestnew(sk_core *core)
{
    pw_patch *patch;
    patch = sk_core_patch(core);
    return gest_pw_alloc(patch);
}

gest_scalar* gest_scalar_pw_alloc(pw_patch *patch)
{
    gest_scalar *s;
    s = malloc(sizeof(gest_scalar));
    gest_scalar_init(s);
    pw_patch_append_userdata(patch, cleanup_scalar, s);
    return s;
}

gest_scalar * sk_node_scalarnew(sk_core *core)
{
    pw_patch *patch;
    patch = sk_core_patch(core);
    return gest_scalar_pw_alloc(patch);
}

static void scalar_compute(pw_node *node)
{
    int blksize;
    int n;
    gest_scalar *s;
    pw_cable *out;

    blksize = pw_node_blksize(node);

    s = pw_node_get_data(node);

    pw_node_get_cable(node, 0, &out);

    for (n = 0; n < blksize; n++) {
        SKFLT x;

        x = s->cur;
        if (s->pos >= 0 && n >= s->pos) x = s->nxt;
        pw_cable_set(out, n, x);
    }

    /* reset */
    s->cur = s->nxt;
    s->pos = -1;
}

int pw_node_scalar(pw_node *node, gest_scalar *s)
{
    pw_patch *patch;
    int rc;

    rc = pw_node_get_patch(node, &patch);
    if (rc != PW_OK) return rc;

    rc = pw_node_cables_alloc(node, 1);

    if (rc != PW_OK) return rc;

    pw_node_set_compute(node, scalar_compute);
    pw_node_set_data(node, s);

    return PW_OK;
}
