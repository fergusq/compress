load "average.gnu"

set style data lines

plot "all-sizes-xml-test-files.times" using 1:2 with points, \
    "all-sizes-xml-test-files.times" using 1:(avg_n($2)) title "500 edellisen keskiarvo" lw 2 lc rgb "blue";

