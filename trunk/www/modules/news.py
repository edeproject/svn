#
# Create html document from news xml template
#

from xml.sax import make_parser, handler
from xml.sax import SAXParseException

def reconstruct_tag(name, attrs):
	"""Again reconstruct tag with attributes"""
	if len(attrs) == 0:
		return "<%s>" % name
	else:
		buff = []
		buff.append("<%s " % name)
		for i in attrs.getNames():
			buff.append("%s=\"%s\"" % (i, attrs.getValue(i)))
		buff.append(">")
		return "".join(buff)

class NewsParser(handler.ContentHandler):
	def __init__(self):
		self.data = []

	def startElement(self, name, attrs):
		if name == "document":
			return
		elif name == "news":
			# yes, this is ugly, but it is only
			# way we set item in order, without messing
			# with sort functions, which is uneeded indeed
			t = attrs.get("title")
			if t:
				tag = "<h1 class=\"news\">%s</h1>" % t
				self.data.append(tag)

			t = attrs.get("date")
			if t:
				tag  = "<div class=\"date\">%s</div>" % t
				self.data.append(tag)

			t = attrs.get("author")
			if t:
				tag  = "<div class=\"author\">%s</div>" % t
				self.data.append(tag)
		else:
			self.data.append(reconstruct_tag(name,attrs))

	def endElement(self, name):
		if name == "document":
			return
		elif name == "news":
			self.data.append("<br />")
		else:
			self.data.append("</%s>" % name)

	def characters(self, content):
		self.data.append(content)

	def get_data(self):
		return "".join(self.data)

def render_news(file):
	parser = make_parser()
	np = NewsParser()
	parser.setContentHandler(np)
	try:
		parser.parse(file)
		return np.get_data()
	except SAXParseException, msg:
		return "Error: %s" % msg
