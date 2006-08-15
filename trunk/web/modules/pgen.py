import template
from pparser import Parser


class Page:
	""" page generator """

	content = []
	def __init__(self, title, activate=""):
		self.title = title
		self.content.append( template.header(title, activate) )
		self.file_error = 0

	def __lshift__(self, text):
		self.add_text(text)

	def add_text(self, text):
		self.content.append(text)

	def file(self, filename):
		fcontent = ""
		try:
			f = open(filename)
			par = Parser()
			fcontent = par.parse( f.read() )
			f.close()

		except IOError, (errno, strerror):
			self.file_error = 1

			# clear the list, since we placed header in it
			self.content = []
			#self.content.append( cgi_header() )
			errorstr="%s named as: %s" %(strerror, filename)
			fcontent = template.error(strerror, errorstr)

		self.content.append(fcontent)

	def display(self):
		if not self.file_error:
			self.content.append( template.footer() )

		print "".join(self.content)

#--- sample ---
# plan = Page(".plan")
# plan<<'''This is simple plan file :)'''
# plan.file("pagebla.txt")
# plan.display()
