Gest
====

Gest is a gesture sequencer, a hybrid
between a step sequencer and automation curve editor.
It will produce audio-rate control curves externally
controlled by a clock signal.

The core gest program is written in ANSI C as a literate
program. Also included is the lilgest commandline utility
that allows Gest to interact with DSP algorithms from
[sndkit](https://pbat.ch/sndkit) using the LIL scripting
language.

Prerequisites
-------------

[Worgle](https://pbat.ch/proj/worgle): needed to tangle
things to C code.

[sndkit](https://pbat.ch/sndkit): required to build the
lilgest utility.

A standard C compiler and Make build system.

Building
--------

Build things by running "make".

Documentation and Example Code
------------------------------

An overview of Gest with examples using
sndkit can be found in `guide.org`, and
[viewed online with audio examples](https://pbat.ch/loom/gest_guide).

There is also a test C program called example.c that
shows how to interact with Gest in C via the sndkit C API.

For more detailed information on the guts of Gest, there
is [always the program itself](https://pbat.ch/loom/gest_program).