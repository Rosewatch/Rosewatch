#!/usr/bin/bash
cd /var/rosewatch
if [ -d "build" ]; then
make build-bundle
else
make full-build
fi
exit 0