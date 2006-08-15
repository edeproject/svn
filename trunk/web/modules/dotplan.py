# simple .plan handling module
# NOTE: it is accessed not through finger service !

# from pwd module documentation getpwnam() returns tuple with fields:
# 0   	pw_name  	Login name
# 1  	pw_passwd 	Optional encrypted password
# 2  	pw_uid 	    Numerical user ID
# 3  	pw_gid 	    Numerical group ID
# 4  	pw_gecos 	User name or comment field
# 5  	pw_dir 	    User home directory
# 6  	pw_shell 	User command interpreter
#
# example print pwd.getpwnam("some_user").pw_gecos

import pwd

plan_name = ".plan"

def __read_plan(path):
	""" internal use only """
	try:
		f = open(path + "/"+ plan_name)
		data = f.read()
		f.close()
		return data

	except IOError:
		return "No Plan."

def __re_matcher(word):
	""" internal use only. get_html() use it"""

	if word.group("break"):
		return "</br>\n"
	if word.group("tab"):
		return "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"

def get(username):
	""" input is username, output is .plan content if user exist """
	try:
		home = pwd.getpwnam(username).pw_dir
		return __read_plan(home)

	except KeyError:
		return "No user such as %s." %username

def get_html(username):
	""" get .plan file as html (very similar as <pre>, but
		when we are in <pre> css can't break lines
	"""
	import re
	reg = re.compile("(?P<break>\n)|(?P<tab>\t)", re.M)

	return reg.sub(__re_matcher, get(username) )

