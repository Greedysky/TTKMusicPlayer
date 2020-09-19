#!/bin/sh  
find . -exec touch -c -m -d "2020-09-19 12:00" {} \;
