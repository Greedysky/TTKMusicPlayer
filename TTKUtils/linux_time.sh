#!/bin/sh  
find . -exec touch -c -m -d "2024-04-25 12:00" {} \;
