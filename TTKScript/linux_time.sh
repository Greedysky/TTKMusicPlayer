#!/bin/sh  
find . -exec touch -c -m -d "2022-05-28 12:00" {} \;
