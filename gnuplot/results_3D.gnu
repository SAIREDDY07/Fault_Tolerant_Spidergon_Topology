#------------------------------------------------------------------------------------------------
#added
# avg Power
reset
set term postscript eps enhanced color
set pm3d
set output "results/graphs/power"
set xlabel "Routers along X-direction"
set ylabel "Routers along Y-direction"
#set zlabel "Power(in Watt)" center rotate by 90
set title "Power Consumption at routers (in Watt)"
set label "R0" at 0.7,0.5
#set arrow 1 from 0.2,0.5 to 0,0.7,0.3
#set label "N" at 0,0.5,0.6
#set arrow 2 from 0.8,0.5 to 0,1.7,0.3
#set label "S" at 0,1.5,0.6
set ticslevel 0
set hidden3d
splot "log/gnuplot/power" using 1:2:3 with lines 
#------------------------------------------------------------------------------------------------
# plotting the two graphs for LinkUsage One Die at a time on one graph
reset
set term postscript eps color
set output "results/graphs/LinkUsage_3D"
set ticslevel 0
set hidden3d
x = 2
y = 2
set size x,y
set origin 0,0
set multiplot

set size 1,1
set origin 0,1
splot "log/gnuplot/LinkUsage" using 1:2:3 with lines

set size 1,1
set origin 1,1
splot "log/gnuplot/LinkUsage1" using 1:2:3 with lines

set size 1,1
set origin 0,0
splot "log/gnuplot/LinkUsage2" using 1:2:3 with lines

set size 1,1
set origin 1,0
splot "log/gnuplot/LinkUsage3" using 1:2:3 with lines

unset multiplot
reset

