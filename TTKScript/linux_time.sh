#!/bin/sh  
find . -exec touch -c -m -d "2020-07-12 12:00" {} \;
