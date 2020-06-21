load "average.gnu"

set style data lines

plot "all-sizes-xml-test-files.gtimes" using 1:2 with points, \
    "all-sizes-xml-test-files.gtimes" using 1:(avg_n($2)) title "100 edellisen keskiarvo" lw 2 lc rgb "blue";

