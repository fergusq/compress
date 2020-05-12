#!/bin/bash

testdir="$1"

echo "Testataan $testdir"

rm "$testdir"/*.compressed "$testdir"/*.uncompressed "$testdir"/*.gcompressed "$testdir".times "$testdir".gtimes

uncompressed_size=0
compressed_size=0
gcompressed_size=0

for file in $(ls "$testdir"); do
    file="$testdir/$file"
    
    start=$(date +%s.%N)
    ../compress "$file" >"$file".compressed 2>/dev/null
    ../decompress "$file".compressed >"$file".uncompressed 2>/dev/null
    dur=$(echo "$(date +%s.%N) - $start" | bc)

    diff "$file" "$file".uncompressed || (echo "Ongelma tiedoston $file kanssa"; exit)

    start=$(date +%s.%N)
    gzip -c "$file" >"$file".gcompressed 2>/dev/null
    gunzip -c "$file".gcompressed >/dev/null
    gdur=$(echo "$(date +%s.%N) - $start" | bc)

    usize=$(stat -c '%s' "$file")
    csize=$(stat -c '%s' "$file".compressed)
    gsize=$(stat -c '%s' "$file".gcompressed)
    ((uncompressed_size+=usize))
    ((compressed_size+=csize))
    ((gcompressed_size+=gsize))

    echo $usize $dur >> "$testdir".times
    echo $usize $gdur >> "$testdir".gtimes
done

printf "Pakkaamaton: %d\n" $uncompressed_size
printf "Pakattu: %d (%d %%)\n" $compressed_size $((100*compressed_size/uncompressed_size))
printf "Pakattu (gzip): %d (%d %%)\n" $gcompressed_size $((100*gcompressed_size/uncompressed_size))