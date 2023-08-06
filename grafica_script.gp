set datafile separator ','
set terminal png
set output 'grafica.png'
plot 'estadisticas.csv' using 1:2 title 'Uso de CPU' with lines, '' using 1:3 title 'Uso de Memoria' with lines
