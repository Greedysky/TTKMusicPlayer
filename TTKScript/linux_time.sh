#!/bin/sh  
find . -exec touch -c -m -d "2021-01-09 12:00" {} \;
