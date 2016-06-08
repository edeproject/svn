#!/bin/sh
# small shell script to create 'screenshots.html' template from m4 sources

cd m4
m4 screenshots.tmpl > ../screenshots.html
