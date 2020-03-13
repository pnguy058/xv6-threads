#!/usr/bin/env bash
cd kernel
echo 'ls' | make qemu-nox
exit 0
exec $SHELL
