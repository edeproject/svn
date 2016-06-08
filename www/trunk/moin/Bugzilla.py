"""
Macro for linking to the bugzilla page for a particular bug

Usage: <<Bugzilla(integer)>>

	31-October-2007 - Initial version - Leif Gruenwoldt
	06-July-2007 - Rewrite of bugURL syntax - Adrian Parker
	16-July-2009 - Fixed 'Usage' sample - Sanel Zukan
	21-July-2009 - Allow reading 'bugzilla_url_prefix' from wikiconfig.py - Sanel Zukan
"""

Dependencies = []

def execute(macro, args):
	bugzillaURL = "http://bugs.equinox-project.org"

	if getattr(macro.request.cfg, u'bugzilla_url_prefix', None):
		bugzillaURL = macro.request.cfg.bugzilla_url_prefix

	bugURL = "%s/show_bug.cgi?id=%s" % (bugzillaURL, args)
	linkName = "Bug #%s" % (args)

	return macro.formatter.text("(") + macro.formatter.url( 1, bugURL ) + linkName + macro.formatter.url(0) + macro.formatter.text(")")
