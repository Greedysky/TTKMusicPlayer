#!/bin/sh  
find . -exec touch -c -m -d "2024-01-18 12:00" {} \;
