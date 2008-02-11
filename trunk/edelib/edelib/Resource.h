/*
 * $Id$
 *
 * Resource reader and writer
 * Part of edelib.
 * Copyright (c) 2008 EDE Authors.
 *
 * This program is licensed under terms of the 
 * GNU General Public License version 2 or newer.
 * See COPYING for details.
 */

#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "econfig.h"

EDELIB_NS_BEGIN

/**
 * \enum ResourceType
 * \brief Flags showing what resource to load first or to load it only
 */
enum ResourceType {
	RES_SYS_FIRST,   ///< Load system resource first
	RES_USER_FIRST,  ///< Load user resource first
	RES_SYS_ONLY,    ///< Load system resource ignoring user one
	RES_USER_ONLY    ///< Load user resource ignoring system one
};

class Config;

/**
 * \class Resource
 * \brief Resource loader and saver.
 *
 * Resource class representing the way of loading and storing application settings 
 * and retrieving paths for application data. Application settings are read and
 * saved (by default) in config files, handled by Config class (for format of config
 * files, see documentation for Config class).
 *
 * When you want to load application settings from it's specific config file, you load
 * it by <em>domain</em>. <em>Domain</em> can be any name and prefered way is to use
 * the same name as the name of application. Then Resource will append <em>.conf</em>
 * suffix and try to find or save that as file name. With this way, Resource can be instructed
 * to use another backend for storage (e.g. database).
 *
 * How it will lookup for files? Resource uses XDG Base Directory Specification (see
 * user_config_dir(), system_config_dirs(), user_data_dir(), system_data_dirs() and user_cache_dir())
 * for finding proper locations.
 *
 * Resource class will also <em>merge</em> settings from config files, depending on
 * ResourceType parameter for any of get() members. <em>Merging</em> can be described as
 * overriding certain system settings, but keeping those that are not overridden (huh ;)).
 * Here is simple example:
 * \code
 *   # Let say you have foo.conf installed in $XDG_CONFIG_DIRS/foo.conf
 *   # with this content
 *   [globals]
 *   first_value = 1
 *   second_value = 2
 *
 *   # Then if user want to override only first_value, but not copy the whole foo.conf
 *   # in $XDG_CONFIG_HOME/foo.conf, it will create $XDG_CONFIG_HOME/foo.conf with
 *   # this content
 *   [globals]
 *   first_value = 34
 *
 *   # And application will get 34 for first_value and 2 for second_value.
 * \endcode
 *
 * You can also limit what key/values user can override or not using ResourceType parameter.
 * A short sample:
 * \code
 *   // You have foo.conf in $XDG_CONFIG_DIRS with this content:
 *   //  [prefs]
 *   //  value_one = 1
 *   //  value_two = 2
 *   //
 *   // and you don't want user override value_two in any chance by placing
 *   // foo.conf in $XDG_CONFIG_HOME directory; overriding value_one is ok.
 *   // You will do this:
 *
 *   int fetched;
 *   Resource r;
 *   if(!r.load("foo"))
 *     // bail
 *
 *   r.get("prefs", "value_one", fetched, 0);
 *   // this can't be overriden, unless you do it in $XDG_CONFIG_DIRS/foo.conf
 *   r.get("prefs", "value_two", fetched, 0, RES_SYS_ONLY);
 * \endcode
 *
 * When you want to save settings, you do it with save(<em>domain</em>). Resource will save
 * this file in $XDG_CONFIG_HOME appending <em>.conf</em> suffix. All user modifications
 * will be preserved.
 *
 * So, to sum things up, here is the complete example of loading and storing settings
 * for some application:
 * \code
 *   // Application will load myapp.conf and user can tune some
 *   // settings and some settings only administrator (or one who have access to
 *   // $XDG_CONFIG_DIRS/myapp.conf). This is the content of myapp.conf:
 *   // [settings]
 *   //  debug_print = true
 *   //  show_titlebar = true
 *   //  show_in_tray = true
 *
 *   // ...
 *   Resource r;
 *   if(!r.load("myapp"))
 *     // bail
 *
 *   bool dprint, titlebar, in_tray;
 *   r.get("settings", "debug_print", dprint, false);
 *   r.get("settings", "show_titlebar", titlebar, false);
 *
 *   // user can't override this one
 *   r.get("settings", "show_in_tray", in_tray, false, RES_SYS_ONLY);
 *
 *   // ...
 *   
 *   // storing
 *   r.set("settings", "debug_print", dprint);
 *   r.set("settings", "show_titlebar", titlebar);
 *
 *   // since file will be saved in user $XDG_CONFIG_HOME directory, where user can
 *   // change it, this item will be ignored, so watch for this things !
 *   r.set("settings", "show_in_tray", in_tray);
 *
 *   // and save it using only domain name
 *   r.save("myapp");
 * \endcode
 */
class EDELIB_API Resource {
	private:
		Config* sys_conf;
		Config* user_conf;

		Resource(const Resource&);
		Resource& operator=(const Resource&);
	public:
		/**
		 * Empty constructor
		 */
		Resource();

		/**
		 * Clears internals
		 */
		~Resource();

		/**
		 * Load resource settings file. Internals will be cleared.
		 *
		 * \return true if was able to find either user or system file
		 * \param domain is application specific settings file; by default it
		 * will search for <em>domain</em>.conf file
		 */
		bool load(const char* domain);

		/**
		 * Save content to the file.
		 *
		 * \return true if file saving was ok, otherwise false.
		 * \param domain is application specific settings file; by default it
		 * will be <em>domain</em>.conf and will be stored in $XDG_CONFIG_HOME directory
		 */
		bool save(const char* domain);

		/**
		 * Clears all loaded data. Calls to any get() member after this will
		 * return false.
		 */
		void clear(void);

		/**
		 * With this, you can use:
		 * \code
		 *   Resource r;
		 *   r.load(...);
		 *   if(!r)
		 *     ...
		 * \endcode
		 */
		operator bool(void) const   { return (sys_conf != NULL || user_conf != NULL); }

		/**
		 * Get char* value from named section.
		 *
		 * \return true if value exist
		 * \param section name of target section
		 * \param key name of target key
		 * \param ret buffer to copy returned value
		 * \param size size of buffer
		 * \param rt is ResourceType variable
		 */
		bool get(const char* section, const char* key, char* ret, unsigned int size, 
				ResourceType rt = RES_USER_FIRST);

		/**
		 * Get localized character data. Check get_localized() from Config for the details.
		 *
		 * \return true if value exist
		 * \param section name of target section
		 * \param key name of target key
		 * \param ret buffer to copy returned value
		 * \param size size of buffer
		 * \param rt is ResourceType variable
		 */
		bool get_localized(const char* section, const char* key, char* ret, unsigned int size, 
				ResourceType rt = RES_USER_FIRST);

		/**
		 * This function is the same as get() with char*, except returned data is allocated.
		 * Use <b>must</b> call delete[] on returned data. If this function returns false,
		 * returned data will not be allocated and <em>retsize</em> will be 0.
		 *
		 * \return true if value exists
		 * \param section name of target section
		 * \param key name of target key
		 * \param ret is allocated by this function ad returned value will be copied in it
		 * \param retsize size of buffer
		 * \param rt is ResourceType variable
		 */
		bool get_allocated(const char* section, const char* key, char** ret, unsigned int& retsize, 
				ResourceType rt = RES_USER_FIRST);

		/**
		 * Get int value from named section.
		 *
		 * \return true if value exist
		 * \param section name of target section
		 * \param key name of target key
		 * \param ret where to place returned value
		 * \param dfl default value, in case real value not found
		 * \param rt is ResourceType variable
		 */
		bool get(const char* section, const char* key, int& ret, int dfl = 0, ResourceType rt = RES_USER_FIRST);

		/**
		 * Get bool value from named section.
		 *
		 * \return true if value exist
		 * \param section name of target section
		 * \param key name of target key
		 * \param ret where to place returned value
		 * \param dfl default value, in case real value not found
		 * \param rt is ResourceType variable
		 */
		bool get(const char* section, const char* key, bool& ret, bool dfl = false, ResourceType rt = RES_USER_FIRST);

		/**
		 * Get float value from named section.
		 *
		 * \return true if value exist
		 * \param section name of target section
		 * \param key name of target key
		 * \param ret where to place returned value
		 * \param dfl default value, in case real value not found
		 * \param rt is ResourceType variable
		 */
		bool get(const char* section, const char* key, float& ret, float dfl = 0, ResourceType rt = RES_USER_FIRST);

		/**
		 * Get long value from named section.
		 *
		 * \return true if value exist
		 * \param section name of target section
		 * \param key name of target key
		 * \param ret where to place returned value
		 * \param dfl default value, in case real value not found
		 * \param rt is ResourceType variable
		 */
		bool get(const char* section, const char* key, long& ret, long dfl = 0, ResourceType rt = RES_USER_FIRST);

		/**
		 * Get double value from named section.
		 *
		 * \return true if value exist
		 * \param section name of target section
		 * \param key name of target key
		 * \param ret where to place returned value
		 * \param dfl default value, in case real value not found
		 * \param rt is ResourceType variable
		 */
		bool get(const char* section, const char* key, double& ret, double dfl = 0, ResourceType rt = RES_USER_FIRST);

		/**
		 * Get char value from named section.
		 *
		 * \return true if value exist
		 * \param section name of target section
		 * \param key name of target key
		 * \param ret where to place returned value
		 * \param dfl default value, in case real value not found
		 * \param rt is ResourceType variable
		 */
		bool get(const char* section, const char* key, char& ret, char dfl = 0, ResourceType rt = RES_USER_FIRST);

		/**
		 * Set char* value in named section. If section, or key does not
		 * exists, they will be created.
		 *
		 * \param section name of target section
		 * \param key name of target key
		 * \param val value to store within key
		 */
		void set(const char* section, const char* key, char* val);

		/**
		 * Set const char* value in named section. If section, or key does not
		 * exists, they will be created.
		 *
		 * \param section name of target section
		 * \param key name of target key
		 * \param val value to store within key
		 */
		void set(const char* section, const char* key, const char* val);

		/**
		 * Set char* value for localized named section. Description for get_localized()
		 * applies here too.
		 *
		 * \param section name of target section
		 * \param key name of target key
		 * \param val value to store within key
		 */
		void set_localized(const char* section, const char* key, char* val);

		/**
		 * Set const char* value for localized named section. Description for get_localized()
		 * applies here too.
		 *
		 * \param section name of target section
		 * \param key name of target key
		 * \param val value to store within key
		 */
		void set_localized(const char* section, const char* key, const char* val);

		/**
		 * Set bool value in named section. If section, or key does not
		 * exists, they will be created.
		 *
		 * \param section name of target section
		 * \param key name of target key
		 * \param val value to store within key
		 */
		void set(const char* section, const char* key, bool val);

		/**
		 * Set int value in named section. If section, or key does not
		 * exists, they will be created.
		 *
		 * \param section name of target section
		 * \param key name of target key
		 * \param val value to store within key
		 */
		void set(const char* section, const char* key, int val);

		/**
		 * Set long value in named section. If section, or key does not
		 * exists, they will be created.
		 *
		 * \param section name of target section
		 * \param key name of target key
		 * \param val value to store within key
		 */
		void set(const char* section, const char* key, long val);

		/**
		 * Set float value in named section. If section, or key does not
		 * exists, they will be created.
		 *
		 * \param section name of target section
		 * \param key name of target key
		 * \param val value to store within key
		 */
		void set(const char* section, const char* key, float val);

		/**
		 * Set double value in named section. If section, or key does not
		 * exists, they will be created.
		 *
		 * \param section name of target section
		 * \param key name of target key
		 * \param val value to store within key
		 */
		void set(const char* section, const char* key, double val);
};

EDELIB_NS_END
#endif // __RESOURCE_H__
