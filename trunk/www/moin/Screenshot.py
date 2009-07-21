"""
Macro for creating screenshot links without giving the full path. shotURL must be filled
with the directory where are the images. It is assumed how thumbnails and full images are
in the same directory.

Usage: <<Screenshot(thumbnail_name, image_name)>>
	21-July-2009 - Initial version- Sanel Zukan
"""

Dependencies = []

def macro_Screenshot(macro, thumbnail_name, image_name):
	shots_url = "/sitemedia/screenshots"

	if getattr(macro.request.cfg, u'screenshots_url_prefix', None):
		shots_url = macro.request.cfg.screenshots_url_prefix

	thumb = "%s/%s" % (shots_url, thumbnail_name)
	img_url = "%s/%s" % (shots_url, image_name)

	return macro.formatter.url(1, img_url) + macro.formatter.image(thumb) + macro.formatter.url(0)
