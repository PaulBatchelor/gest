/* gcc -I.. example.c ../gest.c ../skgest.c -o example -lsndkit */

#include <stdlib.h>
#include <sndkit/graforge.h>
#include <sndkit/core.h>
#include <sndkit/sknodes.h>

#define GEST_PRIVATE
#include "gest.h"

int sk_node_gesticulate(sk_core *core);
int sk_node_gestweight(sk_core *core);

void testgest(gest_d *g)
{
    gest_begin(g, 3, 3);
    gest_addtarget(g, 0);
    gest_addtarget(g, 0);
    gest_addtarget(g, 0);
    gest_end(g);
    gest_finish(g);
}

void mkgesture(gest_d *g)
{
    float lo, mid, hi;

    lo = 60;
    hi = 72;
    mid = 62;

    gest_begin(g, 3, 3);
    gest_polyramp(g, 2);
    gest_addtarget(g, hi);
    gest_addtarget(g, lo);

    gest_addtarget(g, mid);

    gest_polyramp(g, 3);
    gest_addtarget(g, lo);
    gest_addtarget(g, hi);
    gest_addtarget(g, lo);

    gest_end(g);
}

void polyramptest(gest_d *g)
{
    float base;
    static float scale[] = {0, 2, 3, 7, 11, 12};

    base = 67;

    gest_begin(g, 3, 3);
    gest_polyramp(g, 2);
    gest_addtarget(g, scale[0] + base);
    gest_behavior_step(g);

    gest_polyramp(g, 5);
    gest_addtarget(g, scale[2] + base);
    gest_behavior_step(g);
    gest_addtarget(g, scale[0] + base);
    gest_behavior_step(g);
    gest_addtarget(g, scale[1] + base);
    gest_behavior_step(g);
    gest_addtarget(g, scale[2] + base);
    gest_behavior_step(g);
    gest_addtarget(g, scale[3] + base);
    gest_behavior_step(g);

    gest_monoramp(g, 2);
    gest_polyramp(g, 3);
    gest_addtarget(g, scale[5] + base);
    gest_behavior_step(g);

    gest_polyramp(g, 2);
    gest_addtarget(g, scale[0] + base);
    gest_behavior_step(g);
    gest_addtarget(g, scale[4] + base);
    gest_behavior_step(g);

    gest_addtarget(g, scale[2] + base);
    /* gest_behavior_step(g); */

    gest_end(g);
}

void weighttest(gest_d *g)
{
    float base;
    static float scale[] = {0, 2, 3, 7, 11, 12};

    base = 67;

    gest_begin(g, 3, 3);
    gest_polyramp(g, 2);
    gest_addtarget(g, scale[0] + base);
    gest_behavior_step(g);

    gest_mass(g, 0);
    gest_inertia(g, 0);
    gest_polyramp(g, 5);
    gest_addtarget(g, scale[2] + base);

    gest_mass(g, 120);
    gest_inertia(g, 0);

    gest_behavior_step(g);
    gest_addtarget(g, scale[0] + base);
    gest_behavior_step(g);
    gest_addtarget(g, scale[1] + base);
    gest_behavior_step(g);

    gest_mass(g, -120);
    gest_inertia(g, 0.005);

    gest_addtarget(g, scale[2] + base);
    gest_behavior_step(g);
    gest_addtarget(g, scale[3] + base);
    gest_behavior_step(g);

    gest_monoramp(g, 2);
    gest_polyramp(g, 3);
    gest_addtarget(g, scale[5] + base);

    gest_inertia(g, 0);

    gest_behavior_step(g);

    gest_mass(g, 30);
    gest_inertia(g, 0.01);

    gest_polyramp(g, 2);
    gest_addtarget(g, scale[0] + base);
    gest_behavior_step(g);
    gest_addtarget(g, scale[4] + base);
    gest_behavior_step(g);

    gest_addtarget(g, scale[2] + base);

    gest_end(g);

    /* alternate interpretation */

    gest_begin(g, 3, 3);
    gest_polyramp(g, 2);
    gest_addtarget(g, scale[0] + base);
    gest_behavior_step(g);
    gest_mass(g, 0);
    gest_inertia(g, 0);

    gest_polyramp(g, 5);
    gest_addtarget(g, scale[2] + base);

    gest_mass(g, -110);
    gest_inertia(g, 0);

    gest_behavior_step(g);
    gest_addtarget(g, scale[0] + base);
    gest_behavior_step(g);
    gest_addtarget(g, scale[1] + base);
    gest_behavior_step(g);

    gest_mass(g, 110);
    gest_inertia(g, 0.1);

    gest_addtarget(g, scale[2] + base);
    gest_behavior_step(g);
    gest_addtarget(g, scale[3] + base);
    gest_behavior_step(g);

    gest_monoramp(g, 2);
    gest_polyramp(g, 3);
    gest_addtarget(g, scale[5] + base);

    gest_mass(g, 0);
    gest_inertia(g, 0);

    gest_behavior_step(g);

    gest_mass(g, -30);
    gest_inertia(g, 0.1);

    gest_polyramp(g, 2);
    gest_addtarget(g, scale[0] + base);
    gest_behavior_step(g);
    gest_addtarget(g, scale[4] + base);
    gest_behavior_step(g);

    gest_addtarget(g, scale[2] + base);
    gest_end(g);
}

void nested_polyramps(gest_d *g)
{
    gest_begin(g, 3, 3);
    gest_addtarget(g, 60);
    gest_behavior_step(g);

    gest_polyramp(g, 2);
        gest_addtarget(g, 63);
        gest_behavior_step(g);
        gest_polyramp(g, 2);
            gest_addtarget(g, 67);
            gest_behavior_step(g);
            gest_addtarget(g, 63);
            gest_behavior_step(g);

    gest_addtarget(g, 68);
    gest_behavior_step(g);
    gest_end(g);
}

void doitall(gest_d *g)
{
    mkgesture(g);
    polyramptest(g);
    nested_polyramps(g);
    weighttest(g);
    gest_loopit(g);
    gest_finish(g);
}

int main(int argc, char *argv[])
{
    sk_core *core;
    gest_d gest;

    core = sk_core_new(44100);

    gest_init(&gest);

    sk_core_generic_push(core, &gest);
    sk_core_regset(core, 1);

    sk_core_constant(core, 60);
    sk_core_regget(core, 1);
    sk_node_gestweight(core);
    sk_core_dup(core);
    sk_node_wavout(core, "weight.wav");
    sk_core_constant(core, 25);
    sk_node_mul(core);
    sk_node_add(core);

    sk_core_constant(core, 1.0/60);
    sk_node_mul(core);
    sk_node_phasor(core, 0);
    sk_core_hold(core);
    sk_core_regset(core, 0);

    sk_core_table_new(core, 8192);
    sk_node_gensine(core);

    sk_core_regget(core, 1);
    sk_core_regget(core, 0);
    sk_node_gesticulate(core);

    /* mkgesture(&gest); */
    /* polyramptest(&gest); */
    /* weighttest(&gest); */
    /* nested_polyramps(&gest); */
    /* testgest(&gest); */
    /* sk_core_dup(core); */
    /* sk_node_wavout(core, "gest.wav"); */
    doitall(&gest);
    sk_node_mtof(core);
    sk_core_constant(core, 1);
    sk_core_constant(core, 1);
    sk_core_constant(core, 1);
    sk_core_constant(core, 0);
    sk_node_fmpair(core);
    sk_core_constant(core, 0.3);
    sk_node_mul(core);


    /* make a metronome signal */

    sk_core_regget(core, 0);
    sk_core_constant(core, 1);
    sk_node_phsclk(core);
    sk_core_constant(core, 0.001);
    sk_core_constant(core, 0.01);
    sk_core_constant(core, 0.001);
    sk_node_env(core);
    sk_core_constant(core, 1000);
    sk_core_constant(core, 0.3);
    sk_node_sine(core);
    sk_node_mul(core);

    sk_node_add(core);

    sk_node_wavout(core, "test.wav");

    sk_core_computes(core, 30);
    /* sk_core_compute(core); */

    sk_core_regget(core, 0);
    sk_core_unhold(core);
    sk_core_del(core);

    gest_cleanup(&gest);
    return 0;
}
