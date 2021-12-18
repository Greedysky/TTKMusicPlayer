#!/bin/sh  
find . -exec touch -c -m -d "2021-12-18 12:00" {} \;
