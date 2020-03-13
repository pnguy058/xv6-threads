#!/usr/bin/env bash
exec $SHELL

cd kernel
make qemu-nox
ls
