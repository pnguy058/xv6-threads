#!/usr/bin/env bash
cd kernel
make qemu-nox | ls
ls
exit 0
exec $SHELL
