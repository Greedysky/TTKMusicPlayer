#!/bin/sh  
find . -exec touch -c -m -d "2022-09-15 12:00" {} \;
