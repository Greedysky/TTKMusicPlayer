#!/bin/sh  
find . -exec touch -c -m -d "2022-04-30 12:00" {} \;
