#!/bin/sh  
find . -exec touch -c -m -d "2023-06-30 12:00" {} \;
