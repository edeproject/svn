#!/bin/sh
# clean the files older than one week

find ede* -mindepth 1 -type f -mtime +7 -exec rm {} \;
find twistd.log* -mtime +7 -exec rm {} \;
