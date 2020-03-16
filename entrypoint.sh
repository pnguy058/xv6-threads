#!/usr/bin/env bash
cd kernel
make
timeout 30 bash -c "echo ' test_thread' | make qemu-nox"; exit 0
exec $SHELL
