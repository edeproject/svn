divert(-1)
changequote([,])

dnl help file to produce screenshots.html file
dnl m4 screenshots.tmpl > screenshots.html
dnl use: THUMB(image_name)
dnl Note that image must have thumbnail with prefix th_ (like th_image_name)

define([THUMB],[dnl
  <li class="span2">
    <a href="<% (site-media {screenshots/$1}) %>" class="thumbnail">
      <img src="<% (site-media {screenshots/th_$1}) %>" alt="th_$1">
    </a>
  </li>
])

define([__begin_screenshots], [dnl
<%
 (define *page-title* "- screenshots")
 (define *template-custom-css* "css/screenshots.css")
%>
<%include header.html %>
<h2>Screenshots</h2><br/>
<div class="gallery">])

define([__end_screenshots], [dnl
</div> <!-- gallery -->
<%include footer.html %>])

define([__begin_thumbs], [dnl
 <ul class="thumbnails">
  <div class="row">])

define([__end_thumbs], [dnl
  </div>
 </ul>
 <br />])

define([TITLE], [  <h4>$@</h4>])
divert(0)dnl
