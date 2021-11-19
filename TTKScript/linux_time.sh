#!/bin/sh  
find . -exec touch -c -m -d "2021-11-19 12:00" {} \;
