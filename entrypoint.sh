#!/usr/bin/env bash
cd kernel
make qemu-nox && make clean

exit 0
exec $SHELL
