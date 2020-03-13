#!/usr/bin/env bash
cd kernel
make qemu-nox && make clean
ls
exit 0
exec $SHELL
