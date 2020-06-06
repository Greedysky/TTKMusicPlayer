#!/bin/sh  
find . -exec touch -c -m -d "2020-06-06 10:30" {} \;
