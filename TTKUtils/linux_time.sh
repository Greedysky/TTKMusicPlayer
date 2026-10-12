#!/bin/sh

find . -exec touch -c -m -d "2026-10-12 00:00" {} \;
