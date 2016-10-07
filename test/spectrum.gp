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

ss(f)=xmin**2/f

plot [f=:] 'spectrum.dat' with points ps 0.1,  ss(f) with lines, 'spectrum_teor' with lines

pause -1 "Hit return to continue"

# vim: filetype=gnuplot
