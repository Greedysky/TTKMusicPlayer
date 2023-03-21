#!/bin/sh  
find . -exec touch -c -m -d "2023-01-31 12:00" {} \;
