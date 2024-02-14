#!/bin/sh  
find . -exec touch -c -m -d "2024-02-14 12:00" {} \;
