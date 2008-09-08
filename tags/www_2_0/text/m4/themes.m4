dnl
dnl parse themes.tmpl throught m4 and this file
dnl to get themes.txt
dnl example: m4 themes.m4 themes.tmpl > themes.txt
changequote([,])dnl
define([TITLE],[{h1: $1}])dnl
define([IMG],[{imglink: ../themes/$1/$1-small.png | ../themes/$1/$1.png}])dnl
define([DOWNLOAD], [{h2:Download:}
{link: $1.tar.bz2 | ../themes/$1/$1.tar.bz2}])dnl
define([THEME], [<div class="themes">
$1</div>])dnl
define([ABOUT], [{h2: About:}])dnl
