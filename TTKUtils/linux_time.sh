#!/bin/sh

find . -exec touch -c -m -d "2026-02-12 00:00" {} \;
