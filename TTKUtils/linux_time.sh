#!/bin/sh  
find . -exec touch -c -m -d "2023-04-22 12:00" {} \;
