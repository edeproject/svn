#include <string.h>
#include <stdio.h>
#include <edelib/EdbusConnection.h>
#include <edelib/EdbusList.h>
#include <edelib/ForeignCallback.h>
#include <edelib/Resource.h>

#include "tinyscheme/scheme-private.h"
#include "tinyscheme/scheme.h"
#include "comm.h"

#define EDE_XSETTINGS_SERVICE     "org.equinoxproject.Xsettings"
#define EDE_XSETTINGS_OBJECT_PATH "/org/equinoxproject/Xsettings"
#define EDE_XSETTINGS_IFACE       "org.equinoxproject.Xsettings"

EDELIB_NS_USING(EdbusConnection)
EDELIB_NS_USING(EdbusData)
EDELIB_NS_USING(EdbusMessage)
EDELIB_NS_USING(EdbusList)
EDELIB_NS_USING(Resource)
EDELIB_NS_USING(foreign_callback_call)
EDELIB_NS_USING(EDBUS_SYSTEM)
EDELIB_NS_USING(EDBUS_SESSION)

/* (foreign-callback-call "name") */
static pointer s_fc_call(scheme* sc, pointer args) {
	if(args == sc->NIL)
		return sc->F;

	pointer a = sc->vptr->pair_car(args);
	if(a == sc->NIL || !sc->vptr->is_string(a))
		return sc->F;

	const char *service = sc->vptr->string_value(a);
	foreign_callback_call(service);
	return sc->T;
}

/* (query "app-name" "section" "variable") */
static pointer s_query(scheme *sc, pointer args) {
	if(args == sc->NIL)
		return sc->F;

	const char *app, *section, *var;

	pointer a = sc->vptr->pair_car(args);
	if(a == sc->NIL || !sc->vptr->is_string(a))
		return sc->F;
	app = sc->vptr->string_value(a);

	args = sc->vptr->pair_cdr(args);

	a = sc->vptr->pair_car(args);
	if(a == sc->NIL || !sc->vptr->is_string(a))
		return sc->F;
	section = sc->vptr->string_value(a);

	args = sc->vptr->pair_cdr(args);

	a = sc->vptr->pair_car(args);
	if(a == sc->NIL || !sc->vptr->is_string(a))
		return sc->F;
	var = sc->vptr->string_value(a);

	char     buf[256];
	Resource conf;

	if(!conf.load(app))
		return sc->F;

	if(!conf.get(section, var, buf, sizeof(buf)))
		return sc->F;

	return mk_string(sc, buf);
}

/* (query-xsettings-all) */
static pointer s_query_xsettings_all(scheme *sc, pointer args) {
	pointer lst_ret = sc->NIL;

	EdbusConnection c;
	if(!c.connect(EDBUS_SESSION))
		return lst_ret;

	EdbusMessage msg, ret;
	msg.create_method_call(EDE_XSETTINGS_SERVICE, 
						   EDE_XSETTINGS_OBJECT_PATH, 
						   EDE_XSETTINGS_IFACE, 
						   "GetAll");

	c.send_with_reply_and_block(msg, 1000, ret);

	EdbusMessage::const_iterator el = ret.begin();
	if((*el).is_array()) {
		EdbusList arr = (*el).to_array();

		EdbusMessage::const_iterator it = arr.begin(), it_end = arr.end();
		for(; it != it_end; ++it) {
			if(!(*it).is_string())
				continue;

			lst_ret = cons(sc, mk_string(sc, (*it).to_string()), lst_ret);
		}

		lst_ret = scheme_reverse(sc, lst_ret);
	}

	return lst_ret;
}

/* (query-xsettings-type "name") => string-type (int|color|string) */
static pointer s_query_xsettings_type(scheme *sc, pointer args) {
	const char *name;
	pointer a = sc->vptr->pair_car(args);
	if(a == sc->NIL || !sc->vptr->is_string(a))
		return sc->F;
	name = sc->vptr->string_value(a);

	EdbusConnection c;
	if(!c.connect(EDBUS_SESSION))
		return sc->F;

	EdbusMessage msg, ret;
	msg.create_method_call(EDE_XSETTINGS_SERVICE, 
						   EDE_XSETTINGS_OBJECT_PATH, 
						   EDE_XSETTINGS_IFACE, 
						   "GetType");

	msg << EdbusData::from_string(name);
	c.send_with_reply_and_block(msg, 1000, ret);

	if(ret.size() != 1)
		return sc->F;

	EdbusMessage::const_iterator it = ret.begin();
	if(!(*it).is_string())
		return sc->F;

	return mk_string(sc, (*it).to_string());
}

/* (query-xsettings "name") => (int|color|string) */
static pointer s_query_xsettings(scheme *sc, pointer args) {
	const char *name;
	pointer a = sc->vptr->pair_car(args);
	if(a == sc->NIL || !sc->vptr->is_string(a))
		return sc->F;
	name = sc->vptr->string_value(a);

	EdbusConnection c;
	if(!c.connect(EDBUS_SESSION))
		return sc->F;

	EdbusMessage msg, ret;
	msg.create_method_call(EDE_XSETTINGS_SERVICE, 
						   EDE_XSETTINGS_OBJECT_PATH, 
						   EDE_XSETTINGS_IFACE, 
						   "GetValue");

	msg << EdbusData::from_string(name);
	c.send_with_reply_and_block(msg, 1000, ret);

	if(ret.size() != 1)
		return sc->F;

	EdbusMessage::const_iterator el = ret.begin();
	if((*el).is_string())
		return mk_string(sc, (*el).to_string());
	if((*el).is_int32())
		return mk_integer(sc, (*el).to_int32());
	if((*el).is_array()) {
		EdbusList arr = (*el).to_array();
		EdbusMessage::const_iterator it = arr.begin(), it_end = arr.end();
		pointer lst_ret = sc->NIL;

		for(; it != it_end; ++it) {
			if(!(*it).is_int32())
				continue;

			lst_ret = cons(sc, mk_integer(sc, (*it).to_int32()), lst_ret);
		}

		lst_ret = scheme_reverse(sc, lst_ret);
		return lst_ret;
	}

	return sc->F;
}

/* (flush-xsettings) */
static pointer s_flush_xsettings(scheme *sc, pointer args) {
	EdbusConnection c;
	if(!c.connect(EDBUS_SESSION))
		return sc->F;

	EdbusMessage msg, ret;
	msg.create_method_call(EDE_XSETTINGS_SERVICE, 
						   EDE_XSETTINGS_OBJECT_PATH, 
						   EDE_XSETTINGS_IFACE, 
						   "Flush");

	c.send(msg);
	return sc->T;
}

/* (remove-xsettings name) */
static pointer s_remove_xsettings(scheme *sc, pointer args) {
	const char *name;
	pointer a = sc->vptr->pair_car(args);
	if(a == sc->NIL || !sc->vptr->is_string(a))
		return sc->F;
	name = sc->vptr->string_value(a);

	EdbusConnection c;
	if(!c.connect(EDBUS_SESSION))
		return sc->F;

	EdbusMessage msg, ret;
	msg.create_method_call(EDE_XSETTINGS_SERVICE, 
						   EDE_XSETTINGS_OBJECT_PATH, 
						   EDE_XSETTINGS_IFACE, 
						   "Remove");

	msg << EdbusData::from_string(name);
	c.send(msg);
	return sc->T;
}

/* (set-xsettings name [string|int|rgba-list]) */
static pointer s_set_xsettings(scheme *sc, pointer args) {
	const char *name;
	pointer ar = sc->vptr->pair_car(args);
	if(ar == sc->NIL || !sc->vptr->is_string(ar))
		return sc->F;
	name = sc->vptr->string_value(ar);

	EdbusConnection c;
	if(!c.connect(EDBUS_SESSION))
		return sc->F;

	EdbusMessage msg, ret;
	msg.create_method_call(EDE_XSETTINGS_SERVICE, 
						   EDE_XSETTINGS_OBJECT_PATH, 
						   EDE_XSETTINGS_IFACE, 
						   "Set");

	args = sc->vptr->pair_cdr(args);

	ar = sc->vptr->pair_car(args);
	if(ar == sc->NIL)
		return sc->F;

	msg << EdbusData::from_string(name);

	/* figure out args */
	if(sc->vptr->is_string(ar))
		msg << EdbusData::from_string(sc->vptr->string_value(ar));
	else if(sc->vptr->is_number(ar))
		msg << EdbusData::from_int32((int32_t)sc->vptr->ivalue(ar));
	else if(sc->vptr->is_pair(ar)) {
		int     r, g, b, a;
		pointer val = sc->vptr->pair_car(ar);

		if(val == sc->NIL || !sc->vptr->is_number(val))
			return sc->F;
		r = sc->vptr->ivalue(val);

		/* next */
		ar = sc->vptr->pair_cdr(ar);
		val = sc->vptr->pair_car(ar);

		if(val == sc->NIL || !sc->vptr->is_number(val))
			return sc->F;
		g = sc->vptr->ivalue(val);

		/* next */
		ar = sc->vptr->pair_cdr(ar);
		val = sc->vptr->pair_car(ar);

		if(val == sc->NIL || !sc->vptr->is_number(val))
			return sc->F;
		b = sc->vptr->ivalue(val);

		/* next */
		ar = sc->vptr->pair_cdr(ar);
		val = sc->vptr->pair_car(ar);

		if(val == sc->NIL || !sc->vptr->is_number(val))
			return sc->F;

		a = sc->vptr->ivalue(val);

		/* fix values */
		if(r < 0)   r = 0;
		if(r > 255) r = 255;

		if(g < 0)   g = 0;
		if(g > 255) g = 255;

		if(b < 0)   b = 0;
		if(b > 255) b = 255;

		if(a < 0)   a = 0;
		if(a > 255) a = 255;

		EdbusList arr = EdbusList::create_array();
		arr << r << g << b << a;

		/* add it to the message */
		msg << EdbusData::from_array(arr);
	} else {
		return sc->F;
	}

	c.send(msg);
	return sc->T;
}

void register_communication_functions(scheme* sc) {
	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "foreign-callback-call"),
		sc->vptr->mk_foreign_func(sc, s_fc_call));

	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "query"),
		sc->vptr->mk_foreign_func(sc, s_query));

	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "query-xsettings-all"),
		sc->vptr->mk_foreign_func(sc, s_query_xsettings_all));

	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "query-xsettings-type"),
		sc->vptr->mk_foreign_func(sc, s_query_xsettings_type));

	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "query-xsettings"),
		sc->vptr->mk_foreign_func(sc, s_query_xsettings));

	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "flush-xsettings"),
		sc->vptr->mk_foreign_func(sc, s_flush_xsettings));

	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "remove-xsettings"),
		sc->vptr->mk_foreign_func(sc, s_remove_xsettings));

	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "set-xsettings"),
		sc->vptr->mk_foreign_func(sc, s_set_xsettings));
}
