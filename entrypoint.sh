#!/usr/bin/env bash
cd kernel
make
timeout 30 bash -c "echo ' test_lock2 20' | make qemu-nox"; exit 0
exec $SHELL
