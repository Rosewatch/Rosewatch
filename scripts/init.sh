#!/usr/bin/bash
TouchScreenDevice='7'
# https://gist.github.com/rubo77/daa262e0229f6e398766
TouchTransform='0 1 0 -1 0 1 0 0 1'

xrandr -o right

# Find all touchscreen devices and apply transform
xinput list --short | \
grep -i 'touch' | \
sed -n 's/.*id=\([0-9]\+\).*/\1/p' | \
while read -r id; do
    echo "Applying transform to touchscreen id $id"
    xinput set-prop "$id" 'Coordinate Transformation Matrix' $TouchTransform
done

/var/rosewatch/build/src/shell/rosewatch