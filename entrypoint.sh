#!/usr/bin/env bash
cd kernel
"echo ls" | make qemu-nox
ls

exec $SHELL
