#!/usr/bin/env bash
cd kernel
make
timeout 30 bash -c "echo ' ls' | make qemu-nox"; exit 0
exec $SHELL
