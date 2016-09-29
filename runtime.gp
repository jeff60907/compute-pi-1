reset                              
set key width 2
set key left
set style fill solid

set grid
set datafile separator "," 
set title 'compute_pi  time'
set ylabel 'time(sec)'
set xlabel 'N'

set term png enhanced font 'Verdana,10'
set output 'runtime.png'


plot "result_clock_gettime.csv" using 1:2 with lines title 'baseline', \
'' using 1:4 with lines title 'omp_2', \
'' using 1:6 with lines title 'omp_4', \
'' using 1:8 with lines title 'avx', \
'' using 1:10 with lines title 'avxunroll'


set xlabel 'N'
set ylabel 'Error'
set datafile separator "," 
set logscale y
set style fill solid
set title 'Compute_pi Error'
set term png enhanced font 'Verdana,10'
set xrange[0:]
set output 'error.png'

plot "result_clock_gettime.csv" using 1:3 title 'baseline' with lines lt rgb 'blue', \
'' using 1:5 title 'openmp_2' with lines lt rgb 'red', \
'' using 1:7 title 'openmp_4' with lines lt rgb 'orange', \
'' using 1:9 title 'avx' with lines lt rgb 'green', \
'' using 1:11 title 'avxunroll' with lines lt rgb 'purple', \
