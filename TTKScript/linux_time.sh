#!/bin/sh  
find . -exec touch -c -m -d "2021-06-26 12:00" {} \;
