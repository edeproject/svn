dnl
dnl parse themes.tmpl throught m4 and this file
dnl to get themes.txt
dnl example: m4 themes.m4 themes.tmpl > themes.txt
changequote([,])dnl
## skip-markdown ##
define([ITALIC],[<i>$@</i>])dnl
define([TITLE],[<h1>$1</h1>])dnl
define([IMG],[<a href="/themes/$1/$1.png"><img src="/themes/$1/$1-small.png" width="160" height="120" /></a>])dnl
define([DOWNLOAD], [<h3>Download</h3>
<a href="/themes/$1/$1.tar.bz2">$1.tar.bz2</a>])dnl
define([THEME], [<div class="themes">
$1</div>])dnl
define([ABOUT], [<h3>About:</h3>])dnl
