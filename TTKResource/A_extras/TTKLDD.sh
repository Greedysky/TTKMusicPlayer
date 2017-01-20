#!/bin/sh  

exe="TTKService"
des="./"

deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')  
cp $deplist $des  
