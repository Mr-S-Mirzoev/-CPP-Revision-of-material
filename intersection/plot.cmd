set terminal png size 1000,1000
set output 'result.png'
unset xtics
unset ytics
unset border
plot 'lines.txt' using 1:2:($3-$1):($4-$2) with vectors nohead notitle