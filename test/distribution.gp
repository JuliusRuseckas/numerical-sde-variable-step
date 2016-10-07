#!/usr/bin/gnuplot

set terminal qt

set xtics nomirror
set ytics nomirror
set mxtics 5
set mytics 5
set tics scale 2
set grid xtics ytics
set logscale xy

xmin=1.0
p(x)=xmin**2/x**3*exp(-xmin/x)

plot [0.1:] 'distribution.dat' with points, p(x) with lines

pause -1 "Hit return to continue"

# vim: filetype=gnuplot
