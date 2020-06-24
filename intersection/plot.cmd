set terminal png size 1000,1000
set output 'result.png'
set palette model RGB defined ( 1 'violet', 2 'red', 3 'blue', 4 'yellow', 5 'green', 6 'orange', 7 'cyan')
unset xtics
unset ytics
unset border
plot 'lines.txt' using 1:2:($3-$1):($4-$2):5 with vectors palette nohead notitle
