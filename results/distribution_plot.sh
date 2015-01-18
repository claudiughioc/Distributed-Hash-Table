#!/usr/bin/gnuplot 
reset
set title "Distribution of 1000 Objects"
set terminal png
set xlabel "Server Index"
set ylabel "Nr Objects"
set xrange[0:11]


set boxwidth 0.5
set style fill solid

set output 'distribution.png'
plot "distribution" using 1:2 with boxes lt rgb "#406090" title "Distribution"
