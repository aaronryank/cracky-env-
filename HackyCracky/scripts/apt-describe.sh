#!/bin/bash

pkglist="$(cat $1)"

for i in $pkglist; do
  apt-cache search --names-only "^$i\$"
done
