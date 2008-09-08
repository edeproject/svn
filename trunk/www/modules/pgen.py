#
# Simple page generator
#

from markdown import markdown
from mako.template import Template
from mako.lookup import TemplateLookup
from news import render_news
import re
import os

skip_re = re.compile("^\s*##\s*skip-markdown\s*##$")

def file_error(file, errno, strerr):
	return """<h1>Internal error</h1>
<br /><br />
<b>%s: (%i) %s</b>""" % (file, errno, strerr)

def file_404(file):
	return "Ups, looks like file does not exists any more, or you typed a bad path :-)"

def file_exists(path):
	return os.access(path, os.F_OK | os.R_OK)

class PageGenerator:
	def __init__(self, template_dir, template):
		self.title = None
		self.content = []
		self.template_dir = template_dir
		self.template = template

	def load(self, file):
		if not file_exists(file):
			self.content = file_404(file)
			return

		f = open(file)
		domark = True
		for i,j in enumerate(f):
			if i < 5:
				if domark and skip_re.search(j) > -1:
					domark = False
					continue
			self.content.append(j)

		self.content = "".join(self.content)
		if domark:
			self.content = markdown(self.content)

	def load_news(self, file):
		if file_exists(file):
			self.content = render_news(file)
		else:
			self.content = file_404(file)

	def set_title(self, title, prepend = None):
		"""Set title; if prepend is given, it will look for '-' chars
		which will be delimiters in output; first item will be capitalized."""
		self.title = title
		if prepend != None:
			p = prepend.split("-")
			self.title = "%s %s" % (self.title, (" ".join(p)).capitalize())

	def display(self):
		try:
			if self.title == None:
				self.title = ""

			#t = Template(filename = self.template)
			lookup = TemplateLookup(self.template_dir)
			t = lookup.get_template(self.template)
			print t.render(title = self.title, content = self.content)
		except IOError, (errno, strerr):
			print file_error(self.template, errno, strerr)
