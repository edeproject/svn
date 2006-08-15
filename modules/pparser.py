# vim:ts=3:
#
# Usage:
# {i: str} will be matched
# \{i: str} will not be matched
import re
from string import split, strip

MAGIC  = "[A-Za-z0-9_@\.\-\+\*\?#\\\/:,;\"\'\[\]\(\)\|\&\=\$\s+]+"
TAGLIST= "i|b|l|title|h1|h2|h3|italic|bold|link|img|image|imglink|imagelink|lname|br|break|rev"

# for formating
FORMAT = "(?P<pre>\[\[\[|\]\]\])|(?P<rule>-{10,30})|(?P<list>^\t\s*\*\s+.*?$)|(?P<ul>x{10,30})"

class Parser:
	def __init__(self):
		self.__content = []
		self.__reg = None
		self.__reg_format = None

		self.prepare()

		self.in_pre = 0
		self.in_ul  = 0

	def prepare(self):
		self.__reg = re.compile("(?<!\\\){(?P<tags>%s)\s*:\s*(?P<data>%s)}" %(TAGLIST,MAGIC), re.M)
		self.__reg_format = re.compile("%s"% FORMAT, re.M)

	def matcher(self, word):
		tag = word.group("tags")
		data= word.group("data")

		if tag == "i" or tag == "italic":
			return "<i>" + data + "</i>"

		elif tag == "b" or tag == "bold":
			return "<b>" + data + "</b>"

		elif tag == "h1" or tag == "title":
			return "<h1>" + data + "</h1>"

		elif tag == "h2":
			return "<h2>" + data + "</h2>"

		elif tag == "h3":
			return "<h3>" + data + "</h3>"

		elif tag == "rev":
			return "<div class=\"revision\">" + data + "</div>"

		elif tag == "img" or tag == "image":
			return "<img src=\"%s\">" % data

		elif tag == "br" or tag == "break":
			return "<br />"

		elif tag == "l" or tag == "link":
			str = split(data, "|")
			if len(str) <= 1:
				return data

			return "<a href=\"%s\">%s</a>" %(strip(str[1]), strip(str[0]))

		elif tag == "lname":
			str = split(data, "|")
			if len(str) <= 1:
				return data

			return "<a name=\"%s\">%s</a>" %(strip(str[1]), strip(str[0]))

		elif tag == "imglink" or tag == "imagelink":
			str = split(data, "|")
			if len(str) <= 1:
				return data;

			return "<a href=\"%s\"><img height=\"120\" alt=\"%s\" src=\"%s\" width=\"160\" border=\"0\" /></a>"\
					  %(str[1], str[1], str[0])

		else:
			return word


	def matcher_format(self, word):
		if word.group("pre"):
			if self.in_pre:
				self.in_pre = 0
				return "</pre>"
			else:
				self.in_pre = 1
				return "<pre>"

		if word.group("rule"):
			return "<hr />"

		if word.group("list"):
			# this will be matched iff we are in <ul> tag
			# else not
			#
			# first remove 'tab' and *
			if self.in_ul:
				line = re.sub("^\t\*","", word.group())
				return "<li>"+line+"</li>"
			else:
				return word.group()

		if word.group("ul"):
			tag = ""
			if self.in_ul:		
				tag = "</ul>"
			else:
				tag = "<ul>"

			self.in_ul ^= 1
			return tag

	def reparagraf(self, text):
		lines = split(text, "\n\n")
		#lines = re.split("(\n\s*\n)",text, re.M)
		# print lines
		content = []

		# according the xhtml 1.0 standard, listed tags can't
		# have parent <p> tag, so we must scan for them
		# and if found, don't emit it
		skip = "<[div|h1|h2|h3|pre|ul|li]"

		for i in lines:
			size = len(i)-1
			if re.search(skip, i) and size > 0:
				content.append("<p />\n")
				content.append(i)
				content.append("<p />\n")
			
			elif size > 0:  # and i[size] in ".?!":
				content.append("\n<p>")
				content.append(i)
				content.append("</p>\n")

			else:
				content.append("<br />\n"+i)
			
		return "".join(content)

	def parse(self, text):
		pass1 = self.__reg_format.sub(self.matcher_format, text)
		pass2 = self.__reg.sub(self.matcher, pass1)
		return self.reparagraf(pass2)




