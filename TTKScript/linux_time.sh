#!/bin/sh  
find . -exec touch -c -m -d "2020-11-21 12:00" {} \;
