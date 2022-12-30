#!/bin/sh  
find . -exec touch -c -m -d "2022-12-30 12:00" {} \;
