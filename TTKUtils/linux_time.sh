#!/bin/sh

find . -exec touch -c -m -d "2025-07-18 12:00" {} \;
