dnl
dnl help file to produce screenshots.txt file
dnl m4 scr.m4 screenshots.tmpl > screenshots.txt
dnl
dnl use: THUMB(image_name)
dnl
dnl Note that image must have thumbnail with prefix th_ (like th_image_name)
changequote([,])dnl
define([THUMB],[{imglink: ../screens/th_$1 | ../screens/$1}])dnl
