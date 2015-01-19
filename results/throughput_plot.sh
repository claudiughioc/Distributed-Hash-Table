#!/usr/bin/gnuplot 
reset
set title "Transfer Duration of 1MB of Data"
set terminal png
set xlabel "Size of Chunks (KB)"
set ylabel "Duration (s)"

set logscale x
set key reverse Left outside
set grid

set xrange[0.01:1001]

set boxwidth 0.3
set style fill solid

set output 'throughput.png'
plot "throughput" using 1:2 with boxes lt rgb "#406090" title "Transfer Duration"
