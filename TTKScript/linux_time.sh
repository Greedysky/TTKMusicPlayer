#!/bin/sh  
find . -exec touch -c -m -d "2022-11-11 12:00" {} \;
