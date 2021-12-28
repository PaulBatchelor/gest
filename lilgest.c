#include <stdio.h>
#include <sndkit/graforge/graforge.h>
#include <sndkit/core.h>
#include <sndkit/lil/lil.h>
#include <sndkit/nodes/sklil.h>

void sklil_load_gest(lil_t lil);
void sklil_load_gestlang(lil_t lil);
void sklil_loader_withextra(lil_t lil);

static void load(lil_t lil)
{
    sklil_loader_withextra(lil);
    sklil_load_gest(lil);
    sklil_load_gestlang(lil);
}

static void clean(lil_t lil)
{
    sklil_clean(lil);
}

int main(int argc, char *argv[])
{
    return lil_main(argc, argv, load, clean);
}
