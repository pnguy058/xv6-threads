#!/usr/bin/env bash
cd kernel
make qemu-nox
ls

exec $SHELL
