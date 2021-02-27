#!/bin/sh  
find . -exec touch -c -m -d "2021-02-27 12:00" {} \;
