#include <stdio.h>
#include <sndkit/patchwerk.h>
#include <sndkit/core.h>
#include <sndkit/lil.h>
#include <sndkit/sklil.h>

void sklil_load_gest(lil_t lil);

static void load(lil_t lil)
{
    sklil_loader(lil);
    sklil_load_gest(lil);
}

static void clean(lil_t lil)
{
    sklil_clean(lil);
}

int main(int argc, char *argv[])
{
    return lil_main(argc, argv, load, clean);
}
