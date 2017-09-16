#!/usr/bin/env bash
cc -fsanitize=leak -g -std=c99 -Wall pixav.c fsops.c vidops.c imgops.c -ledit -lpng -lm -o pixav
