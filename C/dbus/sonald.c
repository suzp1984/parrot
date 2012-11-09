
/**
 *  * the demo is to demonstrate the whole process of setting up
 *   * a dbus message loop manually. for simplicity, we connect to
 *    * session bus instead of coding a server from the grand up.
 *     * the best of dbus is that we can register our own callbacks in
 *      * all most every step of message process. by using notify functions
 *       * the main loop of application awares of the changes, including coming
 *        * of raw message data from the transport, new outgoing message, incoming
 *         * message queue is empty and so on.
 *          */
#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/select.h>

/**
 *  * for every connection, there is a transport mechanism ( here
 *   * maybe is unix domain socket ). data come and go from the connection,
 *    * a watch is used to monitor incoming or outgoing data flow or exception.
 *     * a watch bind with a fd (i know here fd is owned by the socket),
 *      * when fd is ready to read data or write from transport, a notify
 *       * event occurs.
 *        */
struct watchlist_t {
	DBusWatch *watch;
	struct watchlist_t *next;
};

static struct watchlist_t *watchlist = NULL;

/**
 *  * when you send a message typed method_call with a reply, you
 *   * may need to set up a timeout handler. when a reply available,
 *    * a timeout event ocurrs.
 *     */
static struct timeoutlist_t {
	DBusTimeout *timeout;
	struct timeoutlist_t *next;
} *timeoutlist = NULL;

/**
 *  * you can register any path you like, just follow the dbus specification,
 *   * such as "/org", "/org/redflag", "/com" etc. i think dbus internally
 *    * splits all path into components ("org", "redflag", "com") and arranges
 *     * them into a tree, every node bind a handler. if one node has no
 *      * handler registered, it processed by dbus( use
 *       * DBUS_HANDLER_RESULT_NOT_YET_HANDLED to notify dbus).
 *        */
char *objectPaths[] = {
	"/org/freedesktop",
	"/com",
	"/com/redflag",
	"/org/freedesktop/DBus",
	"/org/freedesktop/csy",
	"/com/redflag/csy"
};

/** object handlers and object path handlers */
DBusHandlerResult object_dbus_handler(DBusConnection*, DBusMessage*, void*);
DBusHandlerResult object_csy_handler(DBusConnection*, DBusMessage*, void*);
DBusHandlerResult subsection_com_redflag_handler(DBusConnection*,
		DBusMessage*, void*);
DBusHandlerResult subsection_com_handler(DBusConnection*,
		DBusMessage*, void*);
DBusHandlerResult subsection_org_freedesktop_handler(DBusConnection*,
		DBusMessage*, void*);

void object_unregister_handler(DBusConnection*, void*);

DBusObjectPathVTable objectPathVTable[] = {
	{
		.unregister_function = NULL,
		.message_function = subsection_org_freedesktop_handler
	},
	{    
		.unregister_function = NULL,
		.message_function = subsection_com_handler
	},
	{
		.unregister_function = NULL,
		.message_function = subsection_com_redflag_handler
	},
	{
		.unregister_function = NULL,
		.message_function = object_dbus_handler
	},
	{
		.unregister_function = object_unregister_handler,
		.message_function = object_csy_handler
	},
	{
		.unregister_function = object_unregister_handler,
		.message_function = object_csy_handler
	}
};

static struct seriallist_t {
	dbus_uint32_t serial;
	struct seriallist_t *next;
} *seriallist = NULL;

/**------------- debug facilities --------------------------------*/
void err_quit(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, fmt, args);
	va_end(args);
	exit(1);    
}

/**
 *  * for debug purpose
 *   */
char* _verbose_watch(DBusWatch *watch) {
	char *s_flags[] = {
		"readable",
		"writable"
	};
	char p[1024] = "", ret[1024] = "";
	if (dbus_watch_get_flags(watch) & DBUS_WATCH_READABLE )
		strncpy(p, s_flags[0], strlen(s_flags[0]));
	else if (dbus_watch_get_flags(watch) & DBUS_WATCH_WRITABLE ) {
		if ( p[0] )
			strncat(p, "&", strlen("&"));
		strncat(p, s_flags[1], strlen(s_flags[1]));
	}
	sprintf(ret, "<fd=%d>:%s", dbus_watch_get_fd(watch), p);
	return ret;
}

char* _verbose_message(DBusMessage *msg) {
	char s_msg[1024] = "", ret[1024] = "";
	int bc = sprintf(s_msg, "\ttype: %s\n\tpath: %s\n"
			"\tmember: %s\n\t",
			dbus_message_type_to_string(dbus_message_get_type(msg)),
			dbus_message_get_path(msg),
			dbus_message_get_member(msg));
	strncpy(ret, s_msg, bc+1);

	if (dbus_message_get_serial(msg)) {
		bc = sprintf(s_msg, "serial: %ud\n\t",
				dbus_message_get_serial(msg));
		strncat(ret, s_msg, bc+1);
	}

	if (dbus_message_get_reply_serial(msg)) {
		bc = sprintf(s_msg, "reply_serial: %ud\n\t",
				dbus_message_get_reply_serial(msg));
		strncat(ret, s_msg, bc+1);
	}

	DBusMessageIter args, subargs;
	char *s;
	int i;
	dbus_message_iter_init(msg, &args);
	bc = sprintf(s_msg, "args: ");
	strncat(ret, s_msg, bc+1);

	/** here is not the best way to parse the params, i just want to test some of
	 *      * nested situations and different types of params. */
	while (DBUS_TYPE_INVALID != dbus_message_iter_get_arg_type(&args)) {
		// demo here: only care about int and string, igore other types

		switch (dbus_message_iter_get_arg_type(&args)) {
			case DBUS_TYPE_STRING:
				dbus_message_iter_get_basic(&args, &s);
				bc = sprintf(s_msg, " %s", s);
				strncat(ret, s_msg, bc+1);
				break;
			case DBUS_TYPE_INT32:
				dbus_message_iter_get_basic(&args, &i);
				bc = sprintf(s_msg, " %d", i);
				strncat(ret, s_msg, bc+1);
				break;
			case DBUS_TYPE_ARRAY:
				dbus_message_iter_recurse(&args, &subargs);
				strcat(ret, " [ ");
				while (dbus_message_iter_get_arg_type(&subargs)
						!= DBUS_TYPE_INVALID) {
					switch (dbus_message_iter_get_arg_type(&subargs)) {
						case DBUS_TYPE_STRING:
							dbus_message_iter_get_basic(&subargs, &s);
							bc = sprintf(s_msg, " %s", s);
							strncat(ret, s_msg, bc+1);
							break;
						case DBUS_TYPE_INT32:
							dbus_message_iter_get_basic(&subargs, &i);
							bc = sprintf(s_msg, " %d", i);
							strncat(ret, s_msg, bc+1);
							break;
					}
					dbus_message_iter_next(&subargs);
				}
				strcat(ret, " ] ");
		}
		dbus_message_iter_next(&args);
	}
	return ret;
}

char* _verbose_timeout(DBusTimeout *timeout) {
	char ret[1024] = "";
	sprintf(ret, "timeout<#%x>: <interval,%d>\n",
			(unsigned int)timeout, dbus_timeout_get_interval(timeout));
	return ret;        
}

/** -------------------------------------------------------*/

DBusHandlerResult handle_method_return(DBusConnection *conn,
		DBusMessage *reply) {
	struct seriallist_t *l = seriallist;
	while (l != NULL) {
		if (l->serial == dbus_message_get_reply_serial(reply)) {        
			printf("reply_msg:\t%s\n", _verbose_message(reply));
			return DBUS_HANDLER_RESULT_HANDLED;
		}
		l = l->next;
	}
	return DBUS_HANDLER_RESULT_HANDLED;
}

int reply_to_method_call(DBusMessage *msg, DBusConnection *conn) {
	DBusMessage *reply;
	DBusMessageIter args;
	DBusError err;

	// suppose we expect two argument, one is id, and second is

	// a string.

	if (!dbus_message_iter_init(msg, &args))
		err_quit("arg init error.\n");

	dbus_int32_t id;
	char *content;

	dbus_error_init( &err );
	dbus_message_get_args( msg, &err,
			DBUS_TYPE_INT32, &id,
			DBUS_TYPE_STRING, &content,
			DBUS_TYPE_INVALID );
	if (dbus_error_is_set(&err))
		err_quit("get arg error.\n");

	reply = dbus_message_new_method_return(msg);
	if (NULL == reply)
		err_quit("Memory is not enough.\n");
	printf("received call args: %d: %s\n", id, content);

	const char *comment = "reply to method call from com.redflag.csy.";
	dbus_message_iter_init_append( reply, &args );
	dbus_message_append_args ( reply,
			DBUS_TYPE_STRING, &comment,
			DBUS_TYPE_INVALID );

	dbus_uint32_t serial;
	dbus_connection_send( conn, reply, &serial );
	dbus_connection_flush( conn );

	printf( "build reply msg and send: \n\t%s\n", _verbose_message(reply) );
	dbus_message_unref( reply );
}

dbus_bool_t watchAddNotify(DBusWatch *watch, void *data) {
	struct watchlist_t *l;
	for (l = watchlist; l != NULL; l = l->next) {
		if (l->watch == watch)
			return TRUE;
	}

	printf("watchAdd: %s\n", _verbose_watch(watch));    
	l = dbus_new(struct watchlist_t, 1);
	if ( NULL == l )
		return FALSE;
	l->watch = watch;
	l->next = watchlist;
	watchlist = l;
	return TRUE;
}

void watchRemoveNotify(DBusWatch *watch, void *data) {
	struct watchlist_t *l, *pre;
	for ( pre = l = watchlist; l != NULL; pre = l, l = l->next ) {
		if (l->watch == watch) {
			printf("watchRemove: <fd = %d>\n", dbus_watch_get_fd(watch));

			if ( l == watchlist ) {
				watchlist = l->next;
				dbus_free(l);
			} else {
				pre->next = l->next;
				dbus_free(l);
			}
			break;
		}
	}        
}

void watchToggleNotify(DBusWatch *watch, void *data) {
	if (watch == NULL)
		err_quit("line %d: watch should not be null.", __LINE__);
	printf( "toggleNotify: watch %d toogled %s",
			dbus_watch_get_fd(watch),
			dbus_watch_get_enabled(watch)?"enable":"disabled" );
}

/**
 * process any watches added to connection
 */

dbus_bool_t watchHandler() {
	struct watchlist_t *l;
	fd_set rfds, wfds, efds;
	int maxid = -1, fd;
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&efds);

	// prepare all readble and writable fds

	for (l = watchlist; l != NULL; l = l->next) {
		if (!dbus_watch_get_enabled(l->watch))
			continue;

		fd = dbus_watch_get_fd(l->watch);
		if ( fd & DBUS_WATCH_READABLE ) {
			FD_SET(fd, &rfds);
			maxid = (maxid < fd? fd: maxid);
		}
		if ( fd & DBUS_WATCH_WRITABLE ) {
			FD_SET(fd, &wfds);
			maxid = (maxid < fd? fd: maxid);
		}
		if ( fd & DBUS_WATCH_ERROR ) {
			FD_SET(fd, &efds);
			maxid = (maxid < fd? fd: maxid);
		}
	}

	int ret = select(maxid+1, &rfds, &wfds, &efds, NULL);
	if (ret <= 0)
		return;

	// call dbus_watch_handle is a must, it uses internal predefined

	// watch handler to do some thing, but i am not quite sure what

	// it is right now.

	for (l = watchlist; l != NULL; l = l->next) {
		fd = dbus_watch_get_fd(l->watch);

		if (FD_ISSET(fd, &rfds))
			dbus_watch_handle(l->watch, DBUS_WATCH_READABLE);

		if (FD_ISSET(fd, &wfds))
			dbus_watch_handle(l->watch, DBUS_WATCH_WRITABLE);

		if (FD_ISSET(fd, &efds))
			dbus_watch_handle(l->watch, DBUS_WATCH_ERROR);    
	}
}

/**---- timeout process functions ------------------------------*/

dbus_bool_t timeoutAddNotify(DBusTimeout *timeout, void *data) {
	struct timeoutlist_t *l;
	for (l = timeoutlist; l != NULL; l = l->next) {
		if (l->timeout == timeout)
			return TRUE;
	}
	l = dbus_new(struct timeoutlist_t, 1);
	if (NULL == l)
		return FALSE;
	l->timeout = timeout;
	fprintf(stdout, "timeoutAdd:%s\n", _verbose_timeout(timeout));
	l->next = timeoutlist;
	timeoutlist = l;
	return TRUE;
}

void timeoutRemoveNotify(DBusTimeout *timeout, void *data) {
	struct timeoutlist_t *pre = NULL, *l = timeoutlist;
	while( l != NULL ) {
		if (l->timeout == timeout) {
			if (pre == NULL)
				timeoutlist = l->next;
			else
				pre->next = l->next;
			fprintf(stdout, "timeoutRemove:%s\n",
					_verbose_timeout(timeout));
			break;
		}
		pre = l;
		l = l->next;
	}
}

void timeoutToggleNotify(DBusTimeout *timeout, void *data) {
	fprintf(stdout, "timeoutToggle: %s\n", _verbose_timeout(timeout));
}

/**
 * in this function, we call dbus_timeout_handle to handle all timeout
 * events, it will call internal predefined handler to process.
 */
void timeoutHandle() {
	struct timeoutlist_t *l = timeoutlist;
	for (; l != NULL; l = l->next) {
		if (dbus_timeout_get_enabled(l->timeout)) {
			printf("timeoutHandle: %s\n", _verbose_timeout(l->timeout));
			dbus_timeout_handle(l->timeout);
		}
	}
}

/**----------- all handlers -------------------------------------*/

/**
 * filter messages that already stayed in the incoming queue,
 * decide whether a further process is needed.
 */
DBusHandlerResult msgFilter(DBusConnection *conn,
		DBusMessage *msg, void *data) {
	printf("incoming msg: %s\n", _verbose_message(msg));

	switch (dbus_message_get_type(msg)) {
		case DBUS_MESSAGE_TYPE_METHOD_CALL:
			if (!strcmp(dbus_message_get_member(msg), "ignore")) {
				DBusMessage *errMsg;
				errMsg = dbus_message_new_error(msg,
						"com.redflag.csy.IgnoreService",
						"this demonstrate the filter.");
				dbus_connection_send(conn, errMsg, NULL);
				return DBUS_HANDLER_RESULT_HANDLED;
			} else
				break;

		case DBUS_MESSAGE_TYPE_METHOD_RETURN:
			// never reach here.

			break;

		case DBUS_MESSAGE_TYPE_SIGNAL:
			break;

		case DBUS_MESSAGE_TYPE_ERROR:
			break;
	}

	// set this flag is very important, if not, dbus may not

	// process messages for you. it pass the control to dbus

	// default filter.

	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

DBusHandlerResult subsection_com_handler(DBusConnection* conn,
		DBusMessage* msg, void* data) {
	if ( strncmp(dbus_message_get_path(msg), objectPaths[1],
				strlen(objectPaths[1])) != 0 ) {
		printf("subsection_com_handler: something wrong.\n");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	if ( strncmp(dbus_message_get_member(msg), "pseudo", 6) == 0 ) {
		reply_to_method_call(msg, conn);
		printf("subsection_com_handler: handled.\n");
		return DBUS_HANDLER_RESULT_HANDLED;
	} else
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

DBusHandlerResult subsection_org_freedesktop_handler(
		DBusConnection* conn, DBusMessage* msg, void* data) {
	if ( strncmp(dbus_message_get_path(msg), objectPaths[0],
				strlen(objectPaths[0])) != 0 ) {
		printf("subsection_org_freedesktop__handler: something wrong.\n");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	if ( strncmp(dbus_message_get_member(msg), "error", 5) == 0 ) {
		printf("subsection_org_freedesktop_handler(error): handled.\n");
		return DBUS_HANDLER_RESULT_HANDLED;
	} else
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

DBusHandlerResult subsection_com_redflag_handler(DBusConnection* conn,
		DBusMessage* msg, void* data) {
	if ( strncmp(dbus_message_get_path(msg), objectPaths[2],
				strlen(objectPaths[2])) != 0 ) {
		printf("subsection_com_redflag_handler: something wrong.\n");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	if ( strncmp(dbus_message_get_member(msg), "pseudo", 6) == 0 ) {
		reply_to_method_call(msg, conn);
		printf("subsection_com_redflag_handler: handled.\n");
		return DBUS_HANDLER_RESULT_HANDLED;
	} else
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

DBusHandlerResult object_dbus_handler(DBusConnection* conn,
		DBusMessage* msg, void* data) {
	if (dbus_message_get_type(msg) == DBUS_MESSAGE_TYPE_METHOD_RETURN) {
		printf("object_dbus_handler: method_return.\n");
		int ret = handle_method_return( conn, msg );
		printf("object_dbus_handler: handled.\n");
		return ret;
	}

	fprintf(stdout, "object_dbus_handler: cannot handle.\n");
	fprintf(stdout, "\t%s\n", _verbose_message(msg));
	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

DBusHandlerResult object_csy_handler(DBusConnection* conn,
		DBusMessage* msg, void* data) {
	switch (dbus_message_get_type(msg)) {
		case DBUS_MESSAGE_TYPE_SIGNAL:
		case DBUS_MESSAGE_TYPE_ERROR:
			fprintf(stdout, "object_csy_handler(error/signal):\n\t%s\n",
					_verbose_message(msg));
			return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;

		case DBUS_MESSAGE_TYPE_METHOD_RETURN:
			return handle_method_return(conn, msg);

		case DBUS_MESSAGE_TYPE_METHOD_CALL:
			if ( strstr(dbus_message_get_path(msg), "sycao") != NULL ) {
				printf("object_csy_handler(call): "
						"cannot handle, through to tree.");
				return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
			}

			if ( !strcmp(dbus_message_get_member(msg), "pseudo") ) {
				reply_to_method_call(msg, conn);
				printf("object_csy_handler(call): handled.\n");
				return DBUS_HANDLER_RESULT_HANDLED;
			} else {
				fprintf(stdout, "object_csy_handler(call): cannot handle.\n");
				return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
			}
	}
}

void object_unregister_handler(DBusConnection* conn, void* data) {
	printf("object_unregister_handler:\n");
}

/**----------------testing routines ----------------------------------*/

/**
 * test send signal method call, send certain signal randomly
 */
void send_random_signal(DBusConnection *conn) {
	DBusMessage *msg;
	char *msg_args[] = {
		"/org/freedesktop/DBus\0org.freedesktop.DBus\0NotExist",
		"/org/freedesktop/HAL\0org.freedesktop.HAL\0Device",
		"/com/redflag/csy\0com.redflag.csy\0NewSignal",
		"\0", "\0"
	};

	char *pargs = msg_args[rand()%(sizeof(msg_args)/sizeof(msg_args[0]))];
	if (!*pargs)
		return;

	long chance = 0x0fffffff;
	if ((rand() % chance) > 100)
		return;

	char *path = pargs;
	char *intf, *member;
	while( *pargs++ );
	intf = pargs;
	while( *pargs++ );
	member = pargs;

	//    printf("%s,%s,%s\n", path, intf, member);

	dbus_uint32_t serial;
	msg = dbus_message_new_signal(path, intf, member);
	if (NULL == msg)
		err_quit("no enough memory.\n");

	if ( !dbus_connection_send(conn, msg, &serial) )
		fprintf( stderr, "no enough memory to send signal.\n" );
	fprintf(stdout, "SendSignal(%d): %s\n", serial, _verbose_message(msg));
}

void pendingCallNotify(DBusPendingCall *pending, void *data) {
	// one can process the pending and then read the reply message

	// alternatively, one can leave it to message handlers

	DBusMessage *msg = dbus_pending_call_steal_reply(pending);    
	printf( "pendingCallNotify: %s\n", _verbose_message(msg) );
}

void send_random_method_expecting_reply(DBusConnection *conn) {
	DBusMessage *msg;
	char *msg_args[] = {
		"org.freedesktop.DBus\0/org/freedesktop/DBus\0org.freedesktop.DBus\0ListNames",
		"org.freedesktop.DBus\0/org/freedesktop/DBus\0org.freedesktop.DBus.Peer\0Ping",
		"org.freedesktop.DBus\0/org/freedesktop/DBus\0org.freedesktop.DBus.Peer\0GetMachineId",
		"\0" // add possibility of not sending any msg this turn

	};

	long chance = 0x0fffffff;
	if ((rand() % chance) > 100)
		return;

	char *pargs = msg_args[rand()%(sizeof(msg_args)/sizeof(msg_args[0]))];
	if (!*pargs)
		return;    

	char *dest, *path, *intf, *member;
	dest = pargs;
	while( *pargs++ );
	path = pargs;
	while( *pargs++ );
	intf = pargs;
	while( *pargs++ );
	member = pargs;

	msg = dbus_message_new_method_call(dest, path, intf, member);
	if (NULL == msg)
		err_quit("no enough memory.\n");

	DBusPendingCall *pendingCall;
	if (!dbus_connection_send_with_reply(conn, msg, &pendingCall, 10000))
		fprintf( stderr, "no memory to send method_call.\n" );
	dbus_pending_call_set_notify(pendingCall, pendingCallNotify, NULL, NULL);
	struct seriallist_t *l;
	l = dbus_new(struct seriallist_t, 1);
	l->serial = dbus_message_get_serial(msg);
	l->next = seriallist;
	seriallist = l;

	fprintf(stdout, "SendMethod: %s\n", _verbose_message(msg));
}

int dbus_process_msg_loop() {
	DBusConnection *conn;
	DBusError *perr;

	// 1. connection to bus

	perr = dbus_new(DBusError, 1);
	dbus_error_init(perr);
	conn = dbus_bus_get(DBUS_BUS_SESSION, perr);
	if ( dbus_error_is_set(perr) )
		err_quit("connection failed.\n");

	// 2. request well-known name

	int ret = dbus_bus_request_name( conn, "com.redflag.csy",
			DBUS_NAME_FLAG_REPLACE_EXISTING, perr );
	if ( DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret )
		err_quit("not primary owner.\n");

	// 3. register any handler

	dbus_bus_add_match( conn,
			"type='method_call',path='/org/freedesktop/csy'"
			",interface='com.redflag.sycao',member='faked'"
			, perr);
	if (dbus_error_is_set(perr))
		err_quit("add match failed.\n");

	dbus_connection_add_filter( conn, msgFilter, NULL, NULL);
	dbus_connection_set_watch_functions( conn,
			watchAddNotify, watchRemoveNotify, watchToggleNotify,
			NULL, NULL );
	dbus_connection_set_timeout_functions( conn,
			timeoutAddNotify, timeoutRemoveNotify, timeoutToggleNotify,
			NULL, NULL );

	int i;
	for (i = 0; i < sizeof(objectPaths)/sizeof(objectPaths[0]); i++) {
		if (i < 3)
			dbus_connection_register_fallback( conn,
					objectPaths[i], &objectPathVTable[i], NULL );
		else
			dbus_connection_register_object_path( conn,
					objectPaths[i], &objectPathVTable[i], NULL);
	}
	printf("main: registered %d objectHandlers.\n", i);

	// 4. main loop: wait and process incoming msgs

	// there are several steps :

	//    a. check if any watch is ready for read (incoming data prepared)

	//    or write (outgoing data prepared), and process it.

	//    b. check if any timeout occurred ( i know now method_call that

	//    needs a reply will set a timeout ), and process it.

	//    c. call dispatch will do:

	//        i. parse incoming raw data if has.

	//        ii. process any pending call ( bind with a reply message );

	//        ii. call any filter registered.

	//        iv. call any object path handler registered.

	//        ps: a single dispatch call processes at most one message.

	//            

	while( 1 ) {
		DBusDispatchStatus status;

		while( 1 ) {
			watchHandler();
			timeoutHandle();
			status = dbus_connection_dispatch( conn );

			switch( status ) {
				case DBUS_DISPATCH_DATA_REMAINS:
					// there may be more msgs remain, go on

					printf("DBUS_DISPATCH_DATA_REMAINS\n");
					continue;

				case DBUS_DISPATCH_COMPLETE:
					break;

				case DBUS_DISPATCH_NEED_MEMORY:
					fprintf( stderr, "more memory is needed.\n" );
			}
			break;
		}

		send_random_signal( conn );
		send_random_method_expecting_reply( conn );
	}

	dbus_free(perr);
	return 0;
}

int main(int argc, char **argv) {
	return dbus_process_msg_loop();
}
