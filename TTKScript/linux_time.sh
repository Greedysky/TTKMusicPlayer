#!/bin/sh  
find . -exec touch -c -m -d "2022-03-05 12:00" {} \;
