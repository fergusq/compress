my @files = dir "xml-test-files";
for ^100 -> $size {
    for ^10 -> $i {
        for ^$size {
            "all-sizes-xml-test-files/file-$size-$i.xml".IO.spurt: @files.pick.IO.slurp(:bin), :append;
        }
    }
}
