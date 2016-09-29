reset 
set ylabel 'time(sec)'
set xlabel 'N'
set style fill solid 
set title "Spent Time"
set key left box
set term png enhanced font 'Mono,10'
set output 'time.png'

plot 'time.csv' using 1:2 title 'Iteration' with linespoints , \
'time.csv' using 1:3 title 'Binary Search' with linespoints , \
'time.csv' using 1:4 title 'Byte_shift' with linespoints , \
'time.csv' using 1:5 title 'Recursive' with linespoints , \
'time.csv' using 1:6 title 'Tail_Recursive' with linespoints