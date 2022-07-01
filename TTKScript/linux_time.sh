#!/bin/sh  
find . -exec touch -c -m -d "2022-07-01 12:00" {} \;
