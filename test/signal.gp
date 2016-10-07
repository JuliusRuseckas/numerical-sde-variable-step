#!/usr/bin/gnuplot

set terminal qt

set xtics nomirror
set ytics nomirror
set mxtics 5
set mytics 5
set tics scale 2
set grid xtics ytics

#set logscale y

plot [0:] 'signal.dat' with lines

pause -1 "Hit return to continue"

# vim: filetype=gnuplot