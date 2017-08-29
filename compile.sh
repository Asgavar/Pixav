#!/usr/bin/env bash
cc -std=c99 -Wall pixav.c fsops.c vidops.c imgops.c -ledit -o pixav
