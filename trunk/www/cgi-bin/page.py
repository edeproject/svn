#!/usr/bin/python
#
# EDE web 3.0
# This time mako + markdown powered
#
# This file should be placed inside cgi-bin
# directory and be maded executable
#
# NOTE: index.txt and index-old.txt will be
# rendered as xml files via news.py

import sys
import time

# base path; must end with '/'
BASE_DIR = "/home/equinox/public_html/"

sys.path.append(BASE_DIR + "modules")

from pgen import PageGenerator
sys.stderr = sys.stdout

# directories, must be absolute path
TEMPLATE_DIR = BASE_DIR + "templates"
TEXT_DIR     = BASE_DIR + "text"

# extensions for text files
TEXT_EXT     = ".txt"

TITLE_LONG   = "Equinox Desktop Environment"
TITLE_SHORT  = "EDE"

def cgi_header(cache = True):
	if cache:
		# "Cache-Control: no-cache" will clear all user cache
		print "Cache-Control: max-age=3600"

	print "Content-Type: text/html\n"

def gentime(sec):
	print "<!-- page generated in %f sec. -->" % sec

def main():
	if len(sys.argv) == 2:
		name = sys.argv[1]
		tname = "main.html"
	else:
		name = "news"
		tname = "front.html"

	p = PageGenerator(TEMPLATE_DIR, tname)

	if name == "news" or name == "old-news":
		p.load_news("%s/%s%s" % (TEXT_DIR, name, TEXT_EXT))
		if name == "news":
			p.set_title(TITLE_LONG)
		else:
			b = "%s -" % TITLE_SHORT
			p.set_title(b, name)
	else:
		p.load("%s/%s%s" % (TEXT_DIR, name, TEXT_EXT))
		b = "%s -" % TITLE_SHORT
		p.set_title(b, name)
	p.display()

if __name__ == "__main__":
	cgi_header()
	start = time.time()
	main()
	end = time.time()
	gentime(end - start)
