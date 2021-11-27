#include <math.h>
#include <stdlib.h>
#include "sndkit/graforge.h"
#include "sndkit/core.h"
#include "gest.h"

struct gest_n {
    gf_cable *conductor;
    gf_cable *out;
    gest_d *gest;
};

struct gestweight_n {
    gf_cable *out;
    gest_d *gest;
    int sr;
    SKFLT a;
    SKFLT inertia;
    SKFLT prev;
    SKFLT scale;
};

static void compute(gf_node *node)
{
    int blksize;
    int n;
    struct gest_n *gest;

    blksize = gf_node_blksize(node);

    gest = (struct gest_n *)gf_node_get_data(node);

    for (n = 0; n < blksize; n++) {
        PWFLT out, conductor;
        conductor = gf_cable_get(gest->conductor, n);

        out = gest_tick(gest->gest, conductor, n);
        gf_cable_set(gest->out, n, out);
    }
}

static void compute_weight(gf_node *node)
{
    int blksize;
    int n;
    struct gestweight_n *gw;

    blksize = gf_node_blksize(node);

    gw = (struct gestweight_n *)gf_node_get_data(node);

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
        gf_cable_set(gw->out, n, out);
    }
}

static void destroy(gf_node *node)
{
    gf_patch *patch;
    int rc;
    void *ud;
    rc = gf_node_get_patch(node, &patch);
    if (rc != GF_OK) return;
    gf_node_cables_free(node);
    ud = gf_node_get_data(node);
    gf_memory_free(patch, &ud);
}

int gf_node_gesticulate(gf_node *node, gest_d *g)
{
    int rc;
    gf_patch *patch;
    struct gest_n *gest;
    void *ud;

    rc = gf_node_get_patch(node, &patch);
    if (rc != GF_OK) return rc;

    rc = gf_memory_alloc(patch, sizeof(struct gest_n), &ud);
    SK_GF_ERROR_CHECK(rc);
    gest = (struct gest_n *)ud;

    rc = gf_node_cables_alloc(node, 2);

    if (rc != GF_OK) return rc;

    gf_node_set_block(node, 1);

    gf_node_get_cable(node, 0, &gest->conductor);
    gf_node_get_cable(node, 1, &gest->out);
    gest->gest = g;

    gf_node_set_data(node, gest);
    gf_node_set_compute(node, compute);
    gf_node_set_destroy(node, destroy);
    return GF_OK;
}

int sk_node_gesticulate(sk_core *core)
{
    gf_patch *patch;
    gf_node *node;
    int rc;
    sk_param conductor;
    gest_d *g;

    rc = sk_param_get(core, &conductor);
    SK_ERROR_CHECK(rc);

    rc = sk_core_generic_pop(core, (void **)&g);
    SK_ERROR_CHECK(rc);

    patch = sk_core_patch(core);

    rc = gf_patch_new_node(patch, &node);
    SK_GF_ERROR_CHECK(rc);

    rc = gf_node_gesticulate(node, g);
    SK_GF_ERROR_CHECK(rc);

    sk_param_set(core, node, &conductor, 0);
    sk_param_out(core, node, 1);
    return 0;
}

int gf_node_gestweight(gf_node *node, gest_d *g)
{
    struct gestweight_n *gw;
    void *ud;
    int rc;
    gf_patch *patch;

    rc = gf_node_get_patch(node, &patch);
    if (rc != GF_OK) return rc;

    rc = gf_memory_alloc(patch, sizeof(struct gestweight_n), &ud);
    if (rc != GF_OK) return rc;
    gw = (struct gestweight_n *)ud;

    gw->gest = g;
    gw->sr = gf_patch_srate_get(patch);
    gw->inertia = 0;
    gw->a = 0;
    gw->scale = 1.0 / 120.0;
    gw->prev = 0;

    rc = gf_node_cables_alloc(node, 1);
    if (rc != GF_OK) return rc;

    gf_node_set_block(node, 0);

    gf_node_get_cable(node, 0, &gw->out);

    gf_node_set_data(node, gw);
    gf_node_set_compute(node, compute_weight);
    gf_node_set_destroy(node, destroy);


    return GF_OK;
}

int sk_node_gestweight(sk_core *core)
{
    gf_patch *patch;
    gf_node *node;
    int rc;
    gest_d *g;

    rc = sk_core_generic_pop(core, (void **)&g);
    SK_ERROR_CHECK(rc);

    patch = sk_core_patch(core);

    rc = gf_patch_new_node(patch, &node);
    SK_GF_ERROR_CHECK(rc);

    rc = gf_node_gestweight(node, g);
    SK_GF_ERROR_CHECK(rc);

    sk_param_out(core, node, 0);
    return 0;
}

static void cleanup_gest(gf_pointer *p)
{
    gest_d *g;
    g = (gest_d *)gf_pointer_data(p);
    gest_cleanup(g);
    free(g);
}

static void cleanup_scalar(gf_pointer *p)
{
    gest_scalar *s;
    s = (gest_scalar *)gf_pointer_data(p);
    free(s);
}

gest_d *gest_gf_alloc(gf_patch *patch)
{
    gest_d *g;
    g = malloc(gest_sizeof());
    gest_init(g);
    gf_patch_append_userdata(patch, cleanup_gest, g);
    return g;
}

gest_d * sk_node_gestnew(sk_core *core)
{
    gf_patch *patch;
    patch = sk_core_patch(core);
    return gest_gf_alloc(patch);
}

gest_scalar* gest_scalar_gf_alloc(gf_patch *patch)
{
    gest_scalar *s;
    s = malloc(sizeof(gest_scalar));
    gest_scalar_init(s);
    gf_patch_append_userdata(patch, cleanup_scalar, s);
    return s;
}

gest_scalar * sk_node_scalarnew(sk_core *core)
{
    gf_patch *patch;
    patch = sk_core_patch(core);
    return gest_scalar_gf_alloc(patch);
}

static void scalar_compute(gf_node *node)
{
    int blksize;
    int n;
    gest_scalar *s;
    gf_cable *out;

    blksize = gf_node_blksize(node);

    s = gf_node_get_data(node);

    gf_node_get_cable(node, 0, &out);

    for (n = 0; n < blksize; n++) {
        SKFLT x;

        x = s->cur;
        if (s->pos >= 0 && n >= s->pos) x = s->nxt;
        gf_cable_set(out, n, x);
    }

    /* reset */
    s->cur = s->nxt;
    s->pos = -1;
}

int gf_node_gescalar(gf_node *node, gest_scalar *s)
{
    gf_patch *patch;
    int rc;

    rc = gf_node_get_patch(node, &patch);
    if (rc != GF_OK) return rc;

    rc = gf_node_cables_alloc(node, 1);
    gf_node_set_block(node, 0);

    if (rc != GF_OK) return rc;

    gf_node_set_compute(node, scalar_compute);
    gf_node_set_data(node, s);

    return GF_OK;
}

int sk_node_gescalar(sk_core *core)
{
    gf_patch *patch;
    gf_node *node;
    int rc;
    gest_scalar *s;

    rc = sk_core_generic_pop(core, (void **)&s);
    SK_ERROR_CHECK(rc);

    patch = sk_core_patch(core);

    rc = gf_patch_new_node(patch, &node);
    SK_GF_ERROR_CHECK(rc);

    rc = gf_node_gescalar(node, s);
    SK_GF_ERROR_CHECK(rc);

    sk_param_out(core, node, 0);
    return 0;
}

/* stick: ticks when a new scalar value has been set.
 * discards the actual value. This is useful for rhythmic
 * sequencing.
 */

static void stick_compute(gf_node *node)
{
    int blksize;
    int n;
    gest_scalar *s;
    gf_cable *out;

    blksize = gf_node_blksize(node);

    s = gf_node_get_data(node);

    gf_node_get_cable(node, 0, &out);

    for (n = 0; n < blksize; n++) {
        SKFLT x;

        x = 0;
        if (s->pos >= 0 && n == s->pos) x = 1;
        gf_cable_set(out, n, x);
    }

    /* reset */
    s->cur = s->nxt;
    s->pos = -1;
}

int gf_node_gestick(gf_node *node, gest_scalar *s)
{
    gf_patch *patch;
    int rc;

    rc = gf_node_get_patch(node, &patch);
    if (rc != GF_OK) return rc;

    rc = gf_node_cables_alloc(node, 1);
    gf_node_set_block(node, 0);

    if (rc != GF_OK) return rc;

    gf_node_set_compute(node, stick_compute);
    gf_node_set_data(node, s);

    return GF_OK;
}

int sk_node_gestick(sk_core *core)
{
    gf_patch *patch;
    gf_node *node;
    int rc;
    gest_scalar *s;

    rc = sk_core_generic_pop(core, (void **)&s);
    SK_ERROR_CHECK(rc);

    patch = sk_core_patch(core);

    rc = gf_patch_new_node(patch, &node);
    SK_GF_ERROR_CHECK(rc);

    rc = gf_node_gestick(node, s);
    SK_GF_ERROR_CHECK(rc);

    sk_param_out(core, node, 0);
    return 0;
}


typedef struct {
    SKFLT bpm, pbpm;
    SKFLT phs;
    SKFLT inc;
    SKFLT onedsr;
} conductor_dsp;

typedef struct {
    gf_cable *bpm;
    gf_cable *out;
    conductor_dsp dsp;
} conductor_n;

static void conductor_compute(gf_node *node)
{
    conductor_n *cnd;
    conductor_dsp *dsp;
    int n, blksize;

    blksize = gf_node_blksize(node);
    cnd = gf_node_get_data(node);
    dsp = &cnd->dsp;

    for (n = 0; n < blksize; n++) {
        SKFLT bpm, out;
        SKFLT phs;
        out = 0;

        bpm = gf_cable_get(cnd->bpm, n);
        dsp->bpm = bpm;

        if (dsp->bpm != dsp->pbpm) {
            dsp->pbpm = dsp->bpm;
            dsp->inc = (dsp->bpm / 60.0) * dsp->onedsr;
        }

        phs = dsp->phs;
        out = dsp->phs;

        phs += dsp->inc;

        if (phs > 1) phs = 0;

        dsp->phs = phs;
        gf_cable_set(cnd->out, n, out);
    }
}

static void conductor_destroy(gf_node *node)
{
    gf_patch *patch;
    int rc;
    void *ud;
    rc = gf_node_get_patch(node, &patch);
    if (rc != GF_OK) return;
    ud = gf_node_get_data(node);
    gf_memory_free(patch, &ud);
    gf_node_cables_free(node);
}

int gf_node_conductor(gf_node *node)
{
    int rc;
    gf_patch *patch;
    conductor_n *cnd;
    void *ud;

    rc = gf_node_get_patch(node, &patch);
    if (rc != GF_OK) return rc;

    rc = gf_memory_alloc(patch, sizeof(conductor_n), &ud);

    if (rc != GF_OK) return GF_OK;

    cnd = (conductor_n *)ud;

    rc = gf_node_cables_alloc(node, 2);

    if (rc != GF_OK) return rc;

    gf_node_get_cable(node, 0, &cnd->bpm);
    gf_node_set_block(node, 1);
    gf_node_get_cable(node, 1, &cnd->out);

    cnd->dsp.bpm = 0;
    cnd->dsp.pbpm = -1;
    cnd->dsp.phs = 0;
    cnd->dsp.inc = 0;
    cnd->dsp.onedsr = 1.0 / (SKFLT)gf_patch_srate_get(patch);

    gf_node_set_data(node, cnd);
    gf_node_set_compute(node, conductor_compute);
    gf_node_set_destroy(node, conductor_destroy);
    return GF_OK;
}
