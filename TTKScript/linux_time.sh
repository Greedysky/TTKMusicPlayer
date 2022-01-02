#!/bin/sh  
find . -exec touch -c -m -d "2022-01-02 12:00" {} \;
