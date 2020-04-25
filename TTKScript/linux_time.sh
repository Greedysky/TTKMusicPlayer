#!/bin/sh  
find . -exec touch -c -m -d "2020-04-25 10:30" {} \;
