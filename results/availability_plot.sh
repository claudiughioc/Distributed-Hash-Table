#!/usr/bin/gnuplot 
reset
set title "Availability of DHT"
set terminal png
set xlabel "Number of Offline Servers"
set ylabel "Percentage of Successful GETs"
set xrange[0:11]

set key reverse Left outside
set grid

set style data linespoints

set output 'availability.png'
plot "availability" using 1:2 title "Availability"
