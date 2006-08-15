#!/bin/sh

cvs update

echo ""
echo "Remember:"
echo "If you changed something in page.py, htaccess or index.html"
echo "place them like this:"
echo "page.py    -> cgi-bin/page.py (first chmod +x)"
echo "index.html -> /index.html"
echo "htaccess   -> /.htacces"
