#!/bin/sh  
find . -exec touch -c -m -d "2021-05-02 12:00" {} \;
