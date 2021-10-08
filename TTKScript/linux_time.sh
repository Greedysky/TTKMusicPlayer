#!/bin/sh  
find . -exec touch -c -m -d "2021-10-08 12:00" {} \;
