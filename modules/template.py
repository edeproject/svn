def __link(name, where, title, activate_name):
	""" used only to handle activate tag """

	activate_tag = 'class="active"'

	if name == activate_name:
		return '<a href="%s" %s title="%s">%s</a>'%(where, activate_tag, title, name)
	else:
		return '<a href="%s" title="%s">%s</a>'%(where, title, name)


def header(title = "", activate = ""):
	return '''\
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
  <meta content="text/html; charset=ISO-8859-1" http-equiv="content-type" />
  <meta name="keywords" content="equinox, equinox screenshots, equinox desktop environment, equinox themes, desktop environment, linux desktop, freebsd desktop, desktop fltk, window manager, x11, linux, ede, FLTK, eFLTK, equinox dotplans, equinox .plans, dotplan, .plan" />
  <meta name="description" content="Equinox Desktop Environment is simple and small graphical desktop environment for UNIX operating systems" />
  <meta name="author" content="ede team" />
  <link type="text/css" href="../web/css/style.css" rel="stylesheet" title="default" />
  <title>%s</title>
</head>
<body>
<div id="container">
<div class="bottom_left"><div class="bottom_right">
<div id="top">
   <div id="title">
      <img id="logo" src="../web/images/ede-logo.gif" alt="Equinox Desktop Environment" />
   </div>
   <div id="toplinks">
      <ul>
         <li>%s</li>
         <li>%s</li>
         <li>%s</li>
         <li>%s</li>
         <li>%s</li>
         <li>%s</li>
         <li>%s</li>
         <li>%s</li>
      </ul>
   </div>
</div>
<div class="hidden">
More:
</div>

<div id="leftbar">
<h3>Forum</h3>
<ul>
   <li><a href="http://sourceforge.net/forum/forum.php?forum_id=249616">ede</a></li>
   <li><a href="http://sourceforge.net/forum/forum.php?forum_id=249614">efltk</a></li>
   <li><a href="http://sourceforge.net/forum/forum.php?forum_id=164319">general help</a></li>
</ul>
<h3>CVS</h3>
<ul>
   <li><a href="http://ede.cvs.sourceforge.net/ede">browse</a></li>
   <li><a href="http://sourceforge.net/cvs/?group_id=49891">help page</a></li>
</ul>
<h3>Tracker</h3>
<ul>
   <li><a href="http://sourceforge.net/tracker/?group_id=49891&amp;atid=457858">bugs</a></li>
   <li><a href="http://sourceforge.net/tracker/?group_id=49891&amp;atid=457860">patches</a></li>
   <li><a href="http://sourceforge.net/tracker/?atid=457861&amp;group_id=49891&amp;func=browse">feature requests</a></li>
   <li><a href="http://sourceforge.net/tracker/?atid=457859&amp;group_id=49891&amp;func=browse">support</a></li>
</ul>
<h3>RSS</h3>
<ul>
   <li><a href="http://sourceforge.net/export/rss2_projnews.php?group_id=49891&amp;rss_fulltext=1">project news</a></li>
   <li><a href="http://sourceforge.net/export/rss2_projfiles.php?group_id=49891">file releases</a></li>
</ul>
</div>
<div id="content">
''' % (title, __link("home", "/page/home", "start page", activate),
			  __link("about", "/page/about", "short introduction", activate),
			  __link("screenshots", "/page/screenshots", "picture gallery", activate),
			  __link("documentation", "/page/documentation", "howto's, papers", activate),
			  __link("faq", "/page/faq", "frequently asked questions", activate),
			  __link("themes", "/page/themes", "enlight your desktop", activate),
			  __link("download", "/page/download", "sources and binaries", activate),
			  __link("links", "/page/links", "useful pages", activate))


def footer():
	return '''\
</div>
<br /><br />
<div id="bottom">
<br /><br />
<a href="http://sourceforge.net"><img src="http://sourceforge.net/sflogo.php?group_id=49891&amp;type=1" width="88"
 height="31" border="0" alt="SourceForge.net Logo" /></a>
<br /><br />
   Copyright &copy; <a href="/page/bad_design" title="aka. ede team">bad design</a> 2002-2006 |
   <a href="http://jigsaw.w3.org/css-validator/validator?uri=http://ede.sourceforge.net/web/css/style.css"
   title="check css validity">css</a> | 
   <a href="http://validator.w3.org/check/referer" title="check xhtml 1.0 validity">xhtml</a>
</div>
<!-- bottom gifs part -->
</div></div>
<!-- gifs part end -->
</div>
</body>
</html> '''


# used in index page

def intro(title_img = "", sample_img = ""):
	return '''\
<div id="introduction">
      <a href="/page/about"><img class="intro_img" src="%s" alt="More..." /></a>
      <a href="/page/screenshots"><img src="%s" alt="intro" title="sample screenshot"/></a>
</div>
	''' %(title_img, sample_img)



# error page

def error(title, string):
	return '''\
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
  <meta content="text/html; charset=ISO-8859-1" http-equiv="content-type">
  <style type="text/css">
  <!--
	body 
	{
		margin-left: 5em;
		margin-right: 5em;
	}
	.title
	{
		text-align: center;
		background: black;
		color: white;
		padding: 30px;
		font: 18px "Lucida Grande", verdana, sans-serif;
		border: 1px solid black;
	}
	.description
	{
		background: white;
		padding: 25px;
		border: 1px solid black;
	}
  -->
  </style>
 <title>Error - %s</title>
</head>
<body>
<div class="title">
	%s
</div>
<div class="description">
	It seems that you entered wrong url or existing page is removed.
	Please check url string again.<br><br>
	If problem persist, then page is
	surrerly removed. In that case, the only solution is to go
	on home page at: <a href="http://ede.sourceforge.net">http://ede.sourceforge.net</a>.
</div>
</body>
</html>''' %(title, string)
