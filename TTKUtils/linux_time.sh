#!/bin/sh  
find . -exec touch -c -m -d "2024-10-22 12:00" {} \;
