#!/bin/bash
for f in *; do
    echo "/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */" > tmpfile
    cat $f >> tmpfile
    mv tmpfile $f
done

exit 0
