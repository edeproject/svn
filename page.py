#!/usr/bin/python

import sys

# modules directory
#sys.path.append("../htdocs/web/modules/")
sys.path.append("../web/modules/")

# for debugging
sys.stderr = sys.stdout

from pgen import Page
import template

def cgi_header():
	# two '\n' characters in cgi header is
	# not needed since 'print' statement add one
	# but this is ONLY used when issue 'print cgi_header()'
	return "Content-Type: text/html\n"


def main():
	title = "EDE - "
	title_alt = "Equinox Desktop Environment"
	txt_dir = "../htdocs/web/text/"
    intro_txt = "/web/images/intro_txt.png"
    intro_sample = "/web/images/intro_sample.png"
    #txt_dir = "../web/text/"
    #intro_txt = "../web/images/intro_txt.png"
    #intro_sample = "../web/images/intro_sample.png"
	txt_ext = ".txt"

	if len(sys.argv) == 2:
		name = sys.argv[1]
		page = Page(title + name, name)
		if name == "home": 
			page << template.intro(intro_txt, intro_sample)

		page.file("%s%s%s" % (txt_dir, name, txt_ext))

	else:
		page = Page(title_alt, "home")
		page << template.intro(intro_txt, intro_sample)
		page.file(txt_dir + "home" + txt_ext)

	page.display()


if __name__ == "__main__":
	print cgi_header()
	main()
