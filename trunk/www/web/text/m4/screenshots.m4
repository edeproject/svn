dnl
dnl help file to produce screenshots.txt file
dnl m4 scr.m4 screenshots.tmpl > screenshots.txt
dnl
dnl use: THUMB(image_name)
dnl
dnl Note that image must have thumbnail with prefix th_ (like th_image_name)
changequote([,])dnl
## skip-markdown ##
define([THUMB],[<a href="/sitemedia/screenshots/$1"><img src="/sitemedia/screenshots/th_$1" width="160" height="120" /></a>])dnl
define([BEGIN],[<div class="gallery">])dnl
define([END],[</div>])dnl
define([TITLE],[<h2>$@</h2>])dnl
