#!/bin/sh

find . -exec touch -c -m -d "2025-01-20 12:00" {} \;
