#!/bin/sh
name="InnerResource"
macro="InnerResource"
tmp="tmpdata"
find ../imageSource -name '*.png' >$tmp
sed -n 's/^\.\.\/imageSource\///p' $tmp>list
rm -f $tmp 
./inner-res-trans -i ../imageSource -l list -o . -n $name -m $macro
genFile=$name.c

# for g++ compile error
sed -e '/static\s*INNER_RES\s*.*\[\]=/,/G/s/void/Uint8/p' $genFile >$tmp
sed -e '/static\s*INNER_RES\s*.*\[\]=/,/G/s/"png"/(char*)"png"/p' $tmp>$genFile 
rm -f $tmp
