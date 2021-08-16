#!/bin/sh  
find . -exec touch -c -m -d "2021-08-16 12:00" {} \;
