#!/bin/sh  
find . -exec touch -c -m -d "2024-10-16 12:00" {} \;
