#!/bin/sh  
find . -exec touch -c -m -d "2024-12-06 12:00" {} \;
