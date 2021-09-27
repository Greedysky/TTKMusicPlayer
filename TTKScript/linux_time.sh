#!/bin/sh  
find . -exec touch -c -m -d "2021-09-27 12:00" {} \;
