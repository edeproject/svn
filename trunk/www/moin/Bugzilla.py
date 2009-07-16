"""
Macro for linking to the bugzilla page for a particular bug

Usage: <<Bugzilla(integer)>>

	31-October-2007 - Initial version - Leif Gruenwoldt
	06-July-2007 - Rewrite of bugURL syntax - Adrian Parker
	16-July-2009 - Fixed 'Usage' sample - Sanel Zukan
"""

Dependencies = []

def execute(macro, args):
	bugURL = ( "http://bugs.equinox-project.org/show_bug.cgi?id=%s" % (args) )
	linkName = ( "Bug #%s" % (args))

	return macro.formatter.text("(") + macro.formatter.url( 1, bugURL ) + linkName + macro.formatter.url(0) + macro.formatter.text(")")
