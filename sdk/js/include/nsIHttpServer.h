/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/netwerk/test/httpserver/nsIHttpServer.idl
 */

#ifndef __gen_nsIHttpServer_h__
#define __gen_nsIHttpServer_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIInputStream; /* forward declaration */

class nsILocalFile; /* forward declaration */

class nsIOutputStream; /* forward declaration */

class nsISimpleEnumerator; /* forward declaration */

class nsIHttpServer; /* forward declaration */

class nsIHttpServerStoppedCallback; /* forward declaration */

class nsIHttpRequestHandler; /* forward declaration */

class nsIHttpRequest; /* forward declaration */

class nsIHttpResponse; /* forward declaration */

class nsIHttpServerIdentity; /* forward declaration */


/* starting interface:    nsIHttpServer */
#define NS_IHTTPSERVER_IID_STR "71ecfba5-15cf-457f-9642-4b33f6e9baf4"

#define NS_IHTTPSERVER_IID \
  {0x71ecfba5, 0x15cf, 0x457f, \
    { 0x96, 0x42, 0x4b, 0x33, 0xf6, 0xe9, 0xba, 0xf4 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIHttpServer : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IHTTPSERVER_IID)

  /**
 * An interface which represents an HTTP server.
 */
/**
   * Starts up this server, listening upon the given port.
   *
   * @param port
   *   the port upon which listening should happen, or -1 if no specific port is
   *   desired
   * @throws NS_ERROR_ALREADY_INITIALIZED
   *   if this server is already started
   * @throws NS_ERROR_NOT_AVAILABLE
   *   if the server is not started and cannot be started on the desired port
   *   (perhaps because the port is already in use or because the process does
   *   not have privileges to do so)
   * @note
   *   Behavior is undefined if this method is called after stop() has been
   *   called on this but before the provided callback function has been
   *   called.
   */
  /* void start (in long port); */
  NS_SCRIPTABLE NS_IMETHOD Start(PRInt32 port) = 0;

  /**
   * Shuts down this server if it is running (including the period of time after
   * stop() has been called but before the provided callback has been called).
   *
   * @param callback
   *   an asynchronous callback used to notify the user when this server is
   *   stopped and all pending requests have been fully served
   * @throws NS_ERROR_NULL_POINTER
   *   if callback is null
   * @throws NS_ERROR_UNEXPECTED
   *   if this server is not running
   */
  /* void stop (in nsIHttpServerStoppedCallback callback); */
  NS_SCRIPTABLE NS_IMETHOD Stop(nsIHttpServerStoppedCallback *callback) = 0;

  /**
   * Associates the local file represented by the string file with all requests
   * which match request.
   *
   * @param path
   *   the path which is to be mapped to the given file; must begin with "/" and
   *   be a valid URI path (i.e., no query string, hash reference, etc.)
   * @param file
   *   the file to serve for the given path, or null to remove any mapping that
   *   might exist; this file must exist for the lifetime of the server
   */
  /* void registerFile (in string path, in nsILocalFile file); */
  NS_SCRIPTABLE NS_IMETHOD RegisterFile(const char *path, nsILocalFile *file) = 0;

  /**
   * Registers a custom path handler.
   *
   * @param path
   *   the path on the server (beginning with a "/") which is to be handled by
   *   handler; this path must not include a query string or hash component; it
   *   also should usually be canonicalized, since most browsers will do so
   *   before sending otherwise-matching requests
   * @param handler
   *   an object which will handle any requests for the given path, or null to
   *   remove any existing handler; if while the server is running the handler
   *   throws an exception while responding to a request, an HTTP 500 response
   *   will be returned
   * @throws NS_ERROR_INVALID_ARG
   *   if path does not begin with a "/"
   */
  /* void registerPathHandler (in string path, in nsIHttpRequestHandler handler); */
  NS_SCRIPTABLE NS_IMETHOD RegisterPathHandler(const char *path, nsIHttpRequestHandler *handler) = 0;

  /**
   * Registers a custom error page handler.
   *
   * @param code
   *   the error code which is to be handled by handler
   * @param handler
   *   an object which will handle any requests which generate the given status
   *   code, or null to remove any existing handler.  If the handler throws an
   *   exception during server operation, fallback is to the genericized error
   *   handler (the x00 version), then to 500, using a user-defined error
   *   handler if one exists or the server default handler otherwise.  Fallback
   *   will never occur from a user-provided handler that throws to the same
   *   handler as provided by the server, e.g. a throwing user 404 falls back to
   *   400, not a server-provided 404 that might not throw.
   * @note
   *   If the error handler handles HTTP 500 and throws, behavior is undefined.
   */
  /* void registerErrorHandler (in unsigned long code, in nsIHttpRequestHandler handler); */
  NS_SCRIPTABLE NS_IMETHOD RegisterErrorHandler(PRUint32 code, nsIHttpRequestHandler *handler) = 0;

  /**
   * Maps all requests to paths beneath path to the corresponding file beneath
   * dir.
   *
   * @param path
   *   the absolute path on the server against which requests will be served
   *   from dir (e.g., "/", "/foo/", etc.); must begin and end with a forward
   *   slash
   * @param dir
   *   the directory to be used to serve all requests for paths underneath path
   *   (except those further overridden by another, deeper path registered with
   *   another directory); if null, any current mapping for the given path is
   *   removed
   * @throws NS_ERROR_INVALID_ARG
   *   if dir is non-null and does not exist or is not a directory, or if path
   *   does not begin with and end with a forward slash
   */
  /* void registerDirectory (in string path, in nsILocalFile dir); */
  NS_SCRIPTABLE NS_IMETHOD RegisterDirectory(const char *path, nsILocalFile *dir) = 0;

  /**
   * Associates files with the given extension with the given Content-Type when
   * served by this server, in the absence of any file-specific information
   * about the desired Content-Type.  If type is empty, removes any extant
   * mapping, if one is present.
   *
   * @throws NS_ERROR_INVALID_ARG
   *   if the given type is not a valid header field value, i.e. if it doesn't
   *   match the field-value production in RFC 2616
   * @note
   *   No syntax checking is done of the given type, beyond ensuring that it is
   *   a valid header field value.  Behavior when not given a string matching
   *   the media-type production in RFC 2616 section 3.7 is undefined.
   *   Implementations may choose to define specific behavior for types which do
   *   not match the production, such as for CGI functionality.
   * @note
   *   Implementations MAY treat type as a trusted argument; users who fail to
   *   generate this string from trusted data risk security vulnerabilities.
   */
  /* void registerContentType (in string extension, in string type); */
  NS_SCRIPTABLE NS_IMETHOD RegisterContentType(const char *extension, const char *type) = 0;

  /**
   * Sets the handler used to display the contents of a directory if
   * the directory contains no index page.
   *
   * @param handler
   *   an object which will handle any requests for directories which
   *   do not contain index pages, or null to reset to the default
   *   index handler; if while the server is running the handler
   *   throws an exception while responding to a request, an HTTP 500
   *   response will be returned.  An nsIFile corresponding to the
   *   directory is available from the metadata object passed to the
   *   handler, under the key "directory".
   */
  /* void setIndexHandler (in nsIHttpRequestHandler handler); */
  NS_SCRIPTABLE NS_IMETHOD SetIndexHandler(nsIHttpRequestHandler *handler) = 0;

  /** Represents the locations at which this server is reachable. */
  /* readonly attribute nsIHttpServerIdentity identity; */
  NS_SCRIPTABLE NS_IMETHOD GetIdentity(nsIHttpServerIdentity **aIdentity) = 0;

  /**
   * Retrieves the string associated with the given key in this, for the given
   * path's saved state.  All keys are initially associated with the empty
   * string.
   */
  /* AString getState (in AString path, in AString key); */
  NS_SCRIPTABLE NS_IMETHOD GetState(const nsAString & path, const nsAString & key, nsAString & _retval NS_OUTPARAM) = 0;

  /**
   * Sets the string associated with the given key in this, for the given path's
   * saved state.
   */
  /* void setState (in AString path, in AString key, in AString value); */
  NS_SCRIPTABLE NS_IMETHOD SetState(const nsAString & path, const nsAString & key, const nsAString & value) = 0;

  /**
   * Retrieves the string associated with the given key in this, in
   * entire-server saved state.  All keys are initially associated with the
   * empty string.
   */
  /* AString getSharedState (in AString key); */
  NS_SCRIPTABLE NS_IMETHOD GetSharedState(const nsAString & key, nsAString & _retval NS_OUTPARAM) = 0;

  /**
   * Sets the string associated with the given key in this, in entire-server
   * saved state.
   */
  /* void setSharedState (in AString key, in AString value); */
  NS_SCRIPTABLE NS_IMETHOD SetSharedState(const nsAString & key, const nsAString & value) = 0;

  /**
   * Retrieves the object associated with the given key in this in
   * object-valued saved state.  All keys are initially associated with null.
   */
  /* nsISupports getObjectState (in AString key); */
  NS_SCRIPTABLE NS_IMETHOD GetObjectState(const nsAString & key, nsISupports **_retval NS_OUTPARAM) = 0;

  /**
   * Sets the object associated with the given key in this in object-valued
   * saved state.  The value may be null.
   */
  /* void setObjectState (in AString key, in nsISupports value); */
  NS_SCRIPTABLE NS_IMETHOD SetObjectState(const nsAString & key, nsISupports *value) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIHttpServer, NS_IHTTPSERVER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIHTTPSERVER \
  NS_SCRIPTABLE NS_IMETHOD Start(PRInt32 port); \
  NS_SCRIPTABLE NS_IMETHOD Stop(nsIHttpServerStoppedCallback *callback); \
  NS_SCRIPTABLE NS_IMETHOD RegisterFile(const char *path, nsILocalFile *file); \
  NS_SCRIPTABLE NS_IMETHOD RegisterPathHandler(const char *path, nsIHttpRequestHandler *handler); \
  NS_SCRIPTABLE NS_IMETHOD RegisterErrorHandler(PRUint32 code, nsIHttpRequestHandler *handler); \
  NS_SCRIPTABLE NS_IMETHOD RegisterDirectory(const char *path, nsILocalFile *dir); \
  NS_SCRIPTABLE NS_IMETHOD RegisterContentType(const char *extension, const char *type); \
  NS_SCRIPTABLE NS_IMETHOD SetIndexHandler(nsIHttpRequestHandler *handler); \
  NS_SCRIPTABLE NS_IMETHOD GetIdentity(nsIHttpServerIdentity **aIdentity); \
  NS_SCRIPTABLE NS_IMETHOD GetState(const nsAString & path, const nsAString & key, nsAString & _retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetState(const nsAString & path, const nsAString & key, const nsAString & value); \
  NS_SCRIPTABLE NS_IMETHOD GetSharedState(const nsAString & key, nsAString & _retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetSharedState(const nsAString & key, const nsAString & value); \
  NS_SCRIPTABLE NS_IMETHOD GetObjectState(const nsAString & key, nsISupports **_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetObjectState(const nsAString & key, nsISupports *value); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIHTTPSERVER(_to) \
  NS_SCRIPTABLE NS_IMETHOD Start(PRInt32 port) { return _to Start(port); } \
  NS_SCRIPTABLE NS_IMETHOD Stop(nsIHttpServerStoppedCallback *callback) { return _to Stop(callback); } \
  NS_SCRIPTABLE NS_IMETHOD RegisterFile(const char *path, nsILocalFile *file) { return _to RegisterFile(path, file); } \
  NS_SCRIPTABLE NS_IMETHOD RegisterPathHandler(const char *path, nsIHttpRequestHandler *handler) { return _to RegisterPathHandler(path, handler); } \
  NS_SCRIPTABLE NS_IMETHOD RegisterErrorHandler(PRUint32 code, nsIHttpRequestHandler *handler) { return _to RegisterErrorHandler(code, handler); } \
  NS_SCRIPTABLE NS_IMETHOD RegisterDirectory(const char *path, nsILocalFile *dir) { return _to RegisterDirectory(path, dir); } \
  NS_SCRIPTABLE NS_IMETHOD RegisterContentType(const char *extension, const char *type) { return _to RegisterContentType(extension, type); } \
  NS_SCRIPTABLE NS_IMETHOD SetIndexHandler(nsIHttpRequestHandler *handler) { return _to SetIndexHandler(handler); } \
  NS_SCRIPTABLE NS_IMETHOD GetIdentity(nsIHttpServerIdentity **aIdentity) { return _to GetIdentity(aIdentity); } \
  NS_SCRIPTABLE NS_IMETHOD GetState(const nsAString & path, const nsAString & key, nsAString & _retval NS_OUTPARAM) { return _to GetState(path, key, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD SetState(const nsAString & path, const nsAString & key, const nsAString & value) { return _to SetState(path, key, value); } \
  NS_SCRIPTABLE NS_IMETHOD GetSharedState(const nsAString & key, nsAString & _retval NS_OUTPARAM) { return _to GetSharedState(key, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD SetSharedState(const nsAString & key, const nsAString & value) { return _to SetSharedState(key, value); } \
  NS_SCRIPTABLE NS_IMETHOD GetObjectState(const nsAString & key, nsISupports **_retval NS_OUTPARAM) { return _to GetObjectState(key, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD SetObjectState(const nsAString & key, nsISupports *value) { return _to SetObjectState(key, value); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIHTTPSERVER(_to) \
  NS_SCRIPTABLE NS_IMETHOD Start(PRInt32 port) { return !_to ? NS_ERROR_NULL_POINTER : _to->Start(port); } \
  NS_SCRIPTABLE NS_IMETHOD Stop(nsIHttpServerStoppedCallback *callback) { return !_to ? NS_ERROR_NULL_POINTER : _to->Stop(callback); } \
  NS_SCRIPTABLE NS_IMETHOD RegisterFile(const char *path, nsILocalFile *file) { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisterFile(path, file); } \
  NS_SCRIPTABLE NS_IMETHOD RegisterPathHandler(const char *path, nsIHttpRequestHandler *handler) { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisterPathHandler(path, handler); } \
  NS_SCRIPTABLE NS_IMETHOD RegisterErrorHandler(PRUint32 code, nsIHttpRequestHandler *handler) { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisterErrorHandler(code, handler); } \
  NS_SCRIPTABLE NS_IMETHOD RegisterDirectory(const char *path, nsILocalFile *dir) { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisterDirectory(path, dir); } \
  NS_SCRIPTABLE NS_IMETHOD RegisterContentType(const char *extension, const char *type) { return !_to ? NS_ERROR_NULL_POINTER : _to->RegisterContentType(extension, type); } \
  NS_SCRIPTABLE NS_IMETHOD SetIndexHandler(nsIHttpRequestHandler *handler) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetIndexHandler(handler); } \
  NS_SCRIPTABLE NS_IMETHOD GetIdentity(nsIHttpServerIdentity **aIdentity) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIdentity(aIdentity); } \
  NS_SCRIPTABLE NS_IMETHOD GetState(const nsAString & path, const nsAString & key, nsAString & _retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetState(path, key, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD SetState(const nsAString & path, const nsAString & key, const nsAString & value) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetState(path, key, value); } \
  NS_SCRIPTABLE NS_IMETHOD GetSharedState(const nsAString & key, nsAString & _retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSharedState(key, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD SetSharedState(const nsAString & key, const nsAString & value) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetSharedState(key, value); } \
  NS_SCRIPTABLE NS_IMETHOD GetObjectState(const nsAString & key, nsISupports **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetObjectState(key, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD SetObjectState(const nsAString & key, nsISupports *value) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetObjectState(key, value); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsHttpServer : public nsIHttpServer
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIHTTPSERVER

  nsHttpServer();

private:
  ~nsHttpServer();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsHttpServer, nsIHttpServer)

nsHttpServer::nsHttpServer()
{
  /* member initializers and constructor code */
}

nsHttpServer::~nsHttpServer()
{
  /* destructor code */
}

/* void start (in long port); */
NS_IMETHODIMP nsHttpServer::Start(PRInt32 port)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void stop (in nsIHttpServerStoppedCallback callback); */
NS_IMETHODIMP nsHttpServer::Stop(nsIHttpServerStoppedCallback *callback)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void registerFile (in string path, in nsILocalFile file); */
NS_IMETHODIMP nsHttpServer::RegisterFile(const char *path, nsILocalFile *file)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void registerPathHandler (in string path, in nsIHttpRequestHandler handler); */
NS_IMETHODIMP nsHttpServer::RegisterPathHandler(const char *path, nsIHttpRequestHandler *handler)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void registerErrorHandler (in unsigned long code, in nsIHttpRequestHandler handler); */
NS_IMETHODIMP nsHttpServer::RegisterErrorHandler(PRUint32 code, nsIHttpRequestHandler *handler)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void registerDirectory (in string path, in nsILocalFile dir); */
NS_IMETHODIMP nsHttpServer::RegisterDirectory(const char *path, nsILocalFile *dir)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void registerContentType (in string extension, in string type); */
NS_IMETHODIMP nsHttpServer::RegisterContentType(const char *extension, const char *type)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setIndexHandler (in nsIHttpRequestHandler handler); */
NS_IMETHODIMP nsHttpServer::SetIndexHandler(nsIHttpRequestHandler *handler)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIHttpServerIdentity identity; */
NS_IMETHODIMP nsHttpServer::GetIdentity(nsIHttpServerIdentity **aIdentity)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString getState (in AString path, in AString key); */
NS_IMETHODIMP nsHttpServer::GetState(const nsAString & path, const nsAString & key, nsAString & _retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setState (in AString path, in AString key, in AString value); */
NS_IMETHODIMP nsHttpServer::SetState(const nsAString & path, const nsAString & key, const nsAString & value)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString getSharedState (in AString key); */
NS_IMETHODIMP nsHttpServer::GetSharedState(const nsAString & key, nsAString & _retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setSharedState (in AString key, in AString value); */
NS_IMETHODIMP nsHttpServer::SetSharedState(const nsAString & key, const nsAString & value)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISupports getObjectState (in AString key); */
NS_IMETHODIMP nsHttpServer::GetObjectState(const nsAString & key, nsISupports **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setObjectState (in AString key, in nsISupports value); */
NS_IMETHODIMP nsHttpServer::SetObjectState(const nsAString & key, nsISupports *value)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIHttpServerStoppedCallback */
#define NS_IHTTPSERVERSTOPPEDCALLBACK_IID_STR "925a6d33-9937-4c63-abe1-a1c56a986455"

#define NS_IHTTPSERVERSTOPPEDCALLBACK_IID \
  {0x925a6d33, 0x9937, 0x4c63, \
    { 0xab, 0xe1, 0xa1, 0xc5, 0x6a, 0x98, 0x64, 0x55 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIHttpServerStoppedCallback : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IHTTPSERVERSTOPPEDCALLBACK_IID)

  /**
 * An interface through which a notification of the complete stopping (socket
 * closure, in-flight requests all fully served and responded to) of an HTTP
 * server may be received.
 */
/** Called when the corresponding server has been fully stopped. */
  /* void onStopped (); */
  NS_SCRIPTABLE NS_IMETHOD OnStopped(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIHttpServerStoppedCallback, NS_IHTTPSERVERSTOPPEDCALLBACK_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIHTTPSERVERSTOPPEDCALLBACK \
  NS_SCRIPTABLE NS_IMETHOD OnStopped(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIHTTPSERVERSTOPPEDCALLBACK(_to) \
  NS_SCRIPTABLE NS_IMETHOD OnStopped(void) { return _to OnStopped(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIHTTPSERVERSTOPPEDCALLBACK(_to) \
  NS_SCRIPTABLE NS_IMETHOD OnStopped(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnStopped(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsHttpServerStoppedCallback : public nsIHttpServerStoppedCallback
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIHTTPSERVERSTOPPEDCALLBACK

  nsHttpServerStoppedCallback();

private:
  ~nsHttpServerStoppedCallback();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsHttpServerStoppedCallback, nsIHttpServerStoppedCallback)

nsHttpServerStoppedCallback::nsHttpServerStoppedCallback()
{
  /* member initializers and constructor code */
}

nsHttpServerStoppedCallback::~nsHttpServerStoppedCallback()
{
  /* destructor code */
}

/* void onStopped (); */
NS_IMETHODIMP nsHttpServerStoppedCallback::OnStopped()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIHttpServerIdentity */
#define NS_IHTTPSERVERIDENTITY_IID_STR "a89de175-ae8e-4c46-91a5-0dba99bbd284"

#define NS_IHTTPSERVERIDENTITY_IID \
  {0xa89de175, 0xae8e, 0x4c46, \
    { 0x91, 0xa5, 0x0d, 0xba, 0x99, 0xbb, 0xd2, 0x84 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIHttpServerIdentity : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IHTTPSERVERIDENTITY_IID)

  /**
 * Represents a set of names for a server, one of which is the primary name for
 * the server and the rest of which are secondary.  By default every server will
 * contain ("http", "localhost", port) and ("http", "127.0.0.1", port) as names,
 * where port is what was provided to the corresponding server when started;
 * however, except for their being removed when the corresponding server stops
 * they have no special importance.
 */
/**
   * The primary scheme at which the corresponding server is located, defaulting
   * to 'http'.  This name will be the value of nsIHttpRequest.scheme for
   * HTTP/1.0 requests.
   *
   * This value is always set when the corresponding server is running.  If the
   * server is not running, this value is set only if it has been set to a
   * non-default name using setPrimary.  In this case reading this value will
   * throw NS_ERROR_NOT_INITIALIZED.
   */
  /* readonly attribute string primaryScheme; */
  NS_SCRIPTABLE NS_IMETHOD GetPrimaryScheme(char **aPrimaryScheme) = 0;

  /**
   * The primary name by which the corresponding server is known, defaulting to
   * 'localhost'.  This name will be the value of nsIHttpRequest.host for
   * HTTP/1.0 requests.
   *
   * This value is always set when the corresponding server is running.  If the
   * server is not running, this value is set only if it has been set to a
   * non-default name using setPrimary.  In this case reading this value will
   * throw NS_ERROR_NOT_INITIALIZED.
   */
  /* readonly attribute string primaryHost; */
  NS_SCRIPTABLE NS_IMETHOD GetPrimaryHost(char **aPrimaryHost) = 0;

  /**
   * The primary port on which the corresponding server runs, defaulting to the
   * associated server's port.  This name will be the value of
   * nsIHttpRequest.port for HTTP/1.0 requests.
   *
   * This value is always set when the corresponding server is running.  If the
   * server is not running, this value is set only if it has been set to a
   * non-default name using setPrimary.  In this case reading this value will
   * throw NS_ERROR_NOT_INITIALIZED.
   */
  /* readonly attribute long primaryPort; */
  NS_SCRIPTABLE NS_IMETHOD GetPrimaryPort(PRInt32 *aPrimaryPort) = 0;

  /**
   * Adds a location at which this server may be accessed.
   *
   * @throws NS_ERROR_ILLEGAL_VALUE
   *   if scheme or host do not match the scheme or host productions imported
   *   into RFC 2616 from RFC 2396, or if port is not a valid port number
   */
  /* void add (in string scheme, in string host, in long port); */
  NS_SCRIPTABLE NS_IMETHOD Add(const char *scheme, const char *host, PRInt32 port) = 0;

  /**
   * Removes this name from the list of names by which the corresponding server
   * is known.  If name is also the primary name for the server, the primary
   * name reverts to 'http://127.0.0.1' with the associated server's port.
   *
   * @throws NS_ERROR_ILLEGAL_VALUE
   *   if scheme or host do not match the scheme or host productions imported
   *   into RFC 2616 from RFC 2396, or if port is not a valid port number
   * @returns
   *   true if the given name was a name for this server, false otherwise
   */
  /* PRBool remove (in string scheme, in string host, in long port); */
  NS_SCRIPTABLE NS_IMETHOD Remove(const char *scheme, const char *host, PRInt32 port, PRBool *_retval NS_OUTPARAM) = 0;

  /**
   * Returns true if the given name is in this, false otherwise.
   *
   * @throws NS_ERROR_ILLEGAL_VALUE
   *   if scheme or host do not match the scheme or host productions imported
   *   into RFC 2616 from RFC 2396, or if port is not a valid port number
   */
  /* PRBool has (in string scheme, in string host, in long port); */
  NS_SCRIPTABLE NS_IMETHOD Has(const char *scheme, const char *host, PRInt32 port, PRBool *_retval NS_OUTPARAM) = 0;

  /**
   * Returns the scheme for the name with the given host and port, if one is
   * present; otherwise returns the empty string.
   *
   * @throws NS_ERROR_ILLEGAL_VALUE
   *   if host does not match the host production imported into RFC 2616 from
   *   RFC 2396, or if port is not a valid port number
   */
  /* string getScheme (in string host, in long port); */
  NS_SCRIPTABLE NS_IMETHOD GetScheme(const char *host, PRInt32 port, char **_retval NS_OUTPARAM) = 0;

  /**
   * Designates the given name as the primary name in this and adds it to this
   * if it is not already present.
   *
   * @throws NS_ERROR_ILLEGAL_VALUE
   *   if scheme or host do not match the scheme or host productions imported
   *   into RFC 2616 from RFC 2396, or if port is not a valid port number
   */
  /* void setPrimary (in string scheme, in string host, in long port); */
  NS_SCRIPTABLE NS_IMETHOD SetPrimary(const char *scheme, const char *host, PRInt32 port) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIHttpServerIdentity, NS_IHTTPSERVERIDENTITY_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIHTTPSERVERIDENTITY \
  NS_SCRIPTABLE NS_IMETHOD GetPrimaryScheme(char **aPrimaryScheme); \
  NS_SCRIPTABLE NS_IMETHOD GetPrimaryHost(char **aPrimaryHost); \
  NS_SCRIPTABLE NS_IMETHOD GetPrimaryPort(PRInt32 *aPrimaryPort); \
  NS_SCRIPTABLE NS_IMETHOD Add(const char *scheme, const char *host, PRInt32 port); \
  NS_SCRIPTABLE NS_IMETHOD Remove(const char *scheme, const char *host, PRInt32 port, PRBool *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD Has(const char *scheme, const char *host, PRInt32 port, PRBool *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD GetScheme(const char *host, PRInt32 port, char **_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetPrimary(const char *scheme, const char *host, PRInt32 port); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIHTTPSERVERIDENTITY(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetPrimaryScheme(char **aPrimaryScheme) { return _to GetPrimaryScheme(aPrimaryScheme); } \
  NS_SCRIPTABLE NS_IMETHOD GetPrimaryHost(char **aPrimaryHost) { return _to GetPrimaryHost(aPrimaryHost); } \
  NS_SCRIPTABLE NS_IMETHOD GetPrimaryPort(PRInt32 *aPrimaryPort) { return _to GetPrimaryPort(aPrimaryPort); } \
  NS_SCRIPTABLE NS_IMETHOD Add(const char *scheme, const char *host, PRInt32 port) { return _to Add(scheme, host, port); } \
  NS_SCRIPTABLE NS_IMETHOD Remove(const char *scheme, const char *host, PRInt32 port, PRBool *_retval NS_OUTPARAM) { return _to Remove(scheme, host, port, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD Has(const char *scheme, const char *host, PRInt32 port, PRBool *_retval NS_OUTPARAM) { return _to Has(scheme, host, port, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD GetScheme(const char *host, PRInt32 port, char **_retval NS_OUTPARAM) { return _to GetScheme(host, port, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD SetPrimary(const char *scheme, const char *host, PRInt32 port) { return _to SetPrimary(scheme, host, port); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIHTTPSERVERIDENTITY(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetPrimaryScheme(char **aPrimaryScheme) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPrimaryScheme(aPrimaryScheme); } \
  NS_SCRIPTABLE NS_IMETHOD GetPrimaryHost(char **aPrimaryHost) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPrimaryHost(aPrimaryHost); } \
  NS_SCRIPTABLE NS_IMETHOD GetPrimaryPort(PRInt32 *aPrimaryPort) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPrimaryPort(aPrimaryPort); } \
  NS_SCRIPTABLE NS_IMETHOD Add(const char *scheme, const char *host, PRInt32 port) { return !_to ? NS_ERROR_NULL_POINTER : _to->Add(scheme, host, port); } \
  NS_SCRIPTABLE NS_IMETHOD Remove(const char *scheme, const char *host, PRInt32 port, PRBool *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->Remove(scheme, host, port, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD Has(const char *scheme, const char *host, PRInt32 port, PRBool *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->Has(scheme, host, port, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD GetScheme(const char *host, PRInt32 port, char **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetScheme(host, port, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD SetPrimary(const char *scheme, const char *host, PRInt32 port) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPrimary(scheme, host, port); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsHttpServerIdentity : public nsIHttpServerIdentity
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIHTTPSERVERIDENTITY

  nsHttpServerIdentity();

private:
  ~nsHttpServerIdentity();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsHttpServerIdentity, nsIHttpServerIdentity)

nsHttpServerIdentity::nsHttpServerIdentity()
{
  /* member initializers and constructor code */
}

nsHttpServerIdentity::~nsHttpServerIdentity()
{
  /* destructor code */
}

/* readonly attribute string primaryScheme; */
NS_IMETHODIMP nsHttpServerIdentity::GetPrimaryScheme(char **aPrimaryScheme)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute string primaryHost; */
NS_IMETHODIMP nsHttpServerIdentity::GetPrimaryHost(char **aPrimaryHost)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute long primaryPort; */
NS_IMETHODIMP nsHttpServerIdentity::GetPrimaryPort(PRInt32 *aPrimaryPort)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void add (in string scheme, in string host, in long port); */
NS_IMETHODIMP nsHttpServerIdentity::Add(const char *scheme, const char *host, PRInt32 port)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRBool remove (in string scheme, in string host, in long port); */
NS_IMETHODIMP nsHttpServerIdentity::Remove(const char *scheme, const char *host, PRInt32 port, PRBool *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRBool has (in string scheme, in string host, in long port); */
NS_IMETHODIMP nsHttpServerIdentity::Has(const char *scheme, const char *host, PRInt32 port, PRBool *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* string getScheme (in string host, in long port); */
NS_IMETHODIMP nsHttpServerIdentity::GetScheme(const char *host, PRInt32 port, char **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setPrimary (in string scheme, in string host, in long port); */
NS_IMETHODIMP nsHttpServerIdentity::SetPrimary(const char *scheme, const char *host, PRInt32 port)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIHttpRequestHandler */
#define NS_IHTTPREQUESTHANDLER_IID_STR "2bbb4db7-d285-42b3-a3ce-142b8cc7e139"

#define NS_IHTTPREQUESTHANDLER_IID \
  {0x2bbb4db7, 0xd285, 0x42b3, \
    { 0xa3, 0xce, 0x14, 0x2b, 0x8c, 0xc7, 0xe1, 0x39 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIHttpRequestHandler : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IHTTPREQUESTHANDLER_IID)

  /**
 * A representation of a handler for HTTP requests.  The handler is used by
 * calling its .handle method with data for an incoming request; it is the
 * handler's job to use that data as it sees fit to make the desired response.
 *
 * @note
 *   This interface uses the [function] attribute, so you can pass a
 *   script-defined function with the functionality of handle() to any
 *   method which has a nsIHttpRequestHandler parameter, instead of wrapping
 *   it in an otherwise empty object.
 */
/**
   * Processes an HTTP request and initializes the passed-in response to reflect
   * the correct HTTP response.
   *
   * If this method throws an exception, externally observable behavior depends
   * upon whether is being processed asynchronously.  If such is the case, the
   * output is some prefix (perhaps all, perhaps none, perhaps only some) of the
   * data which would have been sent if, instead, the response had been finished
   * at that point.  If no data has been written, the response has not had
   * seizePower() called on it, and it is not being asynchronously created, an
   * error handler will be invoked (usually 500 unless otherwise specified).
   *
   * Some uses of nsIHttpRequestHandler may require this method to never throw
   * an exception; in the general case, however, this method may throw an
   * exception (causing an HTTP 500 response to occur, if the above conditions
   * are met).
   *
   * @param request
   *   data representing an HTTP request
   * @param response
   *   an initially-empty response which must be modified to reflect the data
   *   which should be sent as the response to the request described by metadata
   */
  /* void handle (in nsIHttpRequest request, in nsIHttpResponse response); */
  NS_SCRIPTABLE NS_IMETHOD Handle(nsIHttpRequest *request, nsIHttpResponse *response) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIHttpRequestHandler, NS_IHTTPREQUESTHANDLER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIHTTPREQUESTHANDLER \
  NS_SCRIPTABLE NS_IMETHOD Handle(nsIHttpRequest *request, nsIHttpResponse *response); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIHTTPREQUESTHANDLER(_to) \
  NS_SCRIPTABLE NS_IMETHOD Handle(nsIHttpRequest *request, nsIHttpResponse *response) { return _to Handle(request, response); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIHTTPREQUESTHANDLER(_to) \
  NS_SCRIPTABLE NS_IMETHOD Handle(nsIHttpRequest *request, nsIHttpResponse *response) { return !_to ? NS_ERROR_NULL_POINTER : _to->Handle(request, response); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsHttpRequestHandler : public nsIHttpRequestHandler
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIHTTPREQUESTHANDLER

  nsHttpRequestHandler();

private:
  ~nsHttpRequestHandler();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsHttpRequestHandler, nsIHttpRequestHandler)

nsHttpRequestHandler::nsHttpRequestHandler()
{
  /* member initializers and constructor code */
}

nsHttpRequestHandler::~nsHttpRequestHandler()
{
  /* destructor code */
}

/* void handle (in nsIHttpRequest request, in nsIHttpResponse response); */
NS_IMETHODIMP nsHttpRequestHandler::Handle(nsIHttpRequest *request, nsIHttpResponse *response)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIHttpRequest */
#define NS_IHTTPREQUEST_IID_STR "978cf30e-ad73-42ee-8f22-fe0aaf1bf5d2"

#define NS_IHTTPREQUEST_IID \
  {0x978cf30e, 0xad73, 0x42ee, \
    { 0x8f, 0x22, 0xfe, 0x0a, 0xaf, 0x1b, 0xf5, 0xd2 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIHttpRequest : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IHTTPREQUEST_IID)

  /**
 * A representation of the data included in an HTTP request.
 */
/**
   * The request type for this request (see RFC 2616, section 5.1.1).
   */
  /* readonly attribute string method; */
  NS_SCRIPTABLE NS_IMETHOD GetMethod(char **aMethod) = 0;

  /**
   * The scheme of the requested path, usually 'http' but might possibly be
   * 'https' if some form of SSL tunneling is in use.  Note that this value
   * cannot be accurately determined unless the incoming request used the
   * absolute-path form of the request line; it defaults to 'http', so only
   * if it is something else can you be entirely certain it's correct.
   */
  /* readonly attribute string scheme; */
  NS_SCRIPTABLE NS_IMETHOD GetScheme(char **aScheme) = 0;

  /**
   * The host of the data being requested (e.g. "localhost" for the
   * http://localhost:8080/file resource).  Note that the relevant port on the
   * host is specified in this.port.  This value is in the ASCII character
   * encoding.
   */
  /* readonly attribute string host; */
  NS_SCRIPTABLE NS_IMETHOD GetHost(char **aHost) = 0;

  /**
   * The port on the server on which the request was received.
   */
  /* readonly attribute unsigned long port; */
  NS_SCRIPTABLE NS_IMETHOD GetPort(PRUint32 *aPort) = 0;

  /**
   * The requested path, without any query string (e.g. "/dir/file.txt").  It is
   * guaranteed to begin with a "/".  The individual components in this string
   * are URL-encoded.
   */
  /* readonly attribute string path; */
  NS_SCRIPTABLE NS_IMETHOD GetPath(char **aPath) = 0;

  /**
   * The URL-encoded query string associated with this request, not including
   * the initial "?", or "" if no query string was present.
   */
  /* readonly attribute string queryString; */
  NS_SCRIPTABLE NS_IMETHOD GetQueryString(char **aQueryString) = 0;

  /**
   * A string containing the HTTP version of the request (i.e., "1.1").  Leading
   * zeros for either component of the version will be omitted.  (In other
   * words, if the request contains the version "1.01", this attribute will be
   * "1.1"; see RFC 2616, section 3.1.)
   */
  /* readonly attribute string httpVersion; */
  NS_SCRIPTABLE NS_IMETHOD GetHttpVersion(char **aHttpVersion) = 0;

  /**
   * Returns the value for the header in this request specified by fieldName.
   *
   * @param fieldName
   *   the name of the field whose value is to be gotten; note that since HTTP
   *   header field names are case-insensitive, this method produces equivalent
   *   results for "HeAdER" and "hEADer" as fieldName
   * @returns
   *   The result is a string containing the individual values of the header,
   *   usually separated with a comma.  The headers WWW-Authenticate,
   *   Proxy-Authenticate, and Set-Cookie violate the HTTP specification,
   *   however, and for these headers only the separator string is '\n'.
   *
   * @throws NS_ERROR_INVALID_ARG
   *   if fieldName does not constitute a valid header field name
   * @throws NS_ERROR_NOT_AVAILABLE
   *   if the given header does not exist in this
   */
  /* string getHeader (in string fieldName); */
  NS_SCRIPTABLE NS_IMETHOD GetHeader(const char *fieldName, char **_retval NS_OUTPARAM) = 0;

  /**
   * Returns true if a header with the given field name exists in this, false
   * otherwise.
   *
   * @param fieldName
   *   the field name whose existence is to be determined in this; note that
   *   since HTTP header field names are case-insensitive, this method produces
   *   equivalent results for "HeAdER" and "hEADer" as fieldName
   * @throws NS_ERROR_INVALID_ARG
   *   if fieldName does not constitute a valid header field name
   */
  /* boolean hasHeader (in string fieldName); */
  NS_SCRIPTABLE NS_IMETHOD HasHeader(const char *fieldName, PRBool *_retval NS_OUTPARAM) = 0;

  /**
   * An nsISimpleEnumerator of nsISupportsStrings over the names of the headers
   * in this request.  The header field names in the enumerator may not
   * necessarily have the same case as they do in the request itself.
   */
  /* readonly attribute nsISimpleEnumerator headers; */
  NS_SCRIPTABLE NS_IMETHOD GetHeaders(nsISimpleEnumerator **aHeaders) = 0;

  /**
   * A stream from which data appearing in the body of this request can be read.
   */
  /* readonly attribute nsIInputStream bodyInputStream; */
  NS_SCRIPTABLE NS_IMETHOD GetBodyInputStream(nsIInputStream **aBodyInputStream) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIHttpRequest, NS_IHTTPREQUEST_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIHTTPREQUEST \
  NS_SCRIPTABLE NS_IMETHOD GetMethod(char **aMethod); \
  NS_SCRIPTABLE NS_IMETHOD GetScheme(char **aScheme); \
  NS_SCRIPTABLE NS_IMETHOD GetHost(char **aHost); \
  NS_SCRIPTABLE NS_IMETHOD GetPort(PRUint32 *aPort); \
  NS_SCRIPTABLE NS_IMETHOD GetPath(char **aPath); \
  NS_SCRIPTABLE NS_IMETHOD GetQueryString(char **aQueryString); \
  NS_SCRIPTABLE NS_IMETHOD GetHttpVersion(char **aHttpVersion); \
  NS_SCRIPTABLE NS_IMETHOD GetHeader(const char *fieldName, char **_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD HasHeader(const char *fieldName, PRBool *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD GetHeaders(nsISimpleEnumerator **aHeaders); \
  NS_SCRIPTABLE NS_IMETHOD GetBodyInputStream(nsIInputStream **aBodyInputStream); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIHTTPREQUEST(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetMethod(char **aMethod) { return _to GetMethod(aMethod); } \
  NS_SCRIPTABLE NS_IMETHOD GetScheme(char **aScheme) { return _to GetScheme(aScheme); } \
  NS_SCRIPTABLE NS_IMETHOD GetHost(char **aHost) { return _to GetHost(aHost); } \
  NS_SCRIPTABLE NS_IMETHOD GetPort(PRUint32 *aPort) { return _to GetPort(aPort); } \
  NS_SCRIPTABLE NS_IMETHOD GetPath(char **aPath) { return _to GetPath(aPath); } \
  NS_SCRIPTABLE NS_IMETHOD GetQueryString(char **aQueryString) { return _to GetQueryString(aQueryString); } \
  NS_SCRIPTABLE NS_IMETHOD GetHttpVersion(char **aHttpVersion) { return _to GetHttpVersion(aHttpVersion); } \
  NS_SCRIPTABLE NS_IMETHOD GetHeader(const char *fieldName, char **_retval NS_OUTPARAM) { return _to GetHeader(fieldName, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD HasHeader(const char *fieldName, PRBool *_retval NS_OUTPARAM) { return _to HasHeader(fieldName, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD GetHeaders(nsISimpleEnumerator **aHeaders) { return _to GetHeaders(aHeaders); } \
  NS_SCRIPTABLE NS_IMETHOD GetBodyInputStream(nsIInputStream **aBodyInputStream) { return _to GetBodyInputStream(aBodyInputStream); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIHTTPREQUEST(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetMethod(char **aMethod) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMethod(aMethod); } \
  NS_SCRIPTABLE NS_IMETHOD GetScheme(char **aScheme) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetScheme(aScheme); } \
  NS_SCRIPTABLE NS_IMETHOD GetHost(char **aHost) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetHost(aHost); } \
  NS_SCRIPTABLE NS_IMETHOD GetPort(PRUint32 *aPort) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPort(aPort); } \
  NS_SCRIPTABLE NS_IMETHOD GetPath(char **aPath) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPath(aPath); } \
  NS_SCRIPTABLE NS_IMETHOD GetQueryString(char **aQueryString) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetQueryString(aQueryString); } \
  NS_SCRIPTABLE NS_IMETHOD GetHttpVersion(char **aHttpVersion) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetHttpVersion(aHttpVersion); } \
  NS_SCRIPTABLE NS_IMETHOD GetHeader(const char *fieldName, char **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetHeader(fieldName, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD HasHeader(const char *fieldName, PRBool *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->HasHeader(fieldName, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD GetHeaders(nsISimpleEnumerator **aHeaders) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetHeaders(aHeaders); } \
  NS_SCRIPTABLE NS_IMETHOD GetBodyInputStream(nsIInputStream **aBodyInputStream) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBodyInputStream(aBodyInputStream); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsHttpRequest : public nsIHttpRequest
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIHTTPREQUEST

  nsHttpRequest();

private:
  ~nsHttpRequest();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsHttpRequest, nsIHttpRequest)

nsHttpRequest::nsHttpRequest()
{
  /* member initializers and constructor code */
}

nsHttpRequest::~nsHttpRequest()
{
  /* destructor code */
}

/* readonly attribute string method; */
NS_IMETHODIMP nsHttpRequest::GetMethod(char **aMethod)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute string scheme; */
NS_IMETHODIMP nsHttpRequest::GetScheme(char **aScheme)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute string host; */
NS_IMETHODIMP nsHttpRequest::GetHost(char **aHost)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned long port; */
NS_IMETHODIMP nsHttpRequest::GetPort(PRUint32 *aPort)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute string path; */
NS_IMETHODIMP nsHttpRequest::GetPath(char **aPath)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute string queryString; */
NS_IMETHODIMP nsHttpRequest::GetQueryString(char **aQueryString)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute string httpVersion; */
NS_IMETHODIMP nsHttpRequest::GetHttpVersion(char **aHttpVersion)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* string getHeader (in string fieldName); */
NS_IMETHODIMP nsHttpRequest::GetHeader(const char *fieldName, char **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean hasHeader (in string fieldName); */
NS_IMETHODIMP nsHttpRequest::HasHeader(const char *fieldName, PRBool *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsISimpleEnumerator headers; */
NS_IMETHODIMP nsHttpRequest::GetHeaders(nsISimpleEnumerator **aHeaders)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIInputStream bodyInputStream; */
NS_IMETHODIMP nsHttpRequest::GetBodyInputStream(nsIInputStream **aBodyInputStream)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIHttpResponse */
#define NS_IHTTPRESPONSE_IID_STR "1acd16c2-dc59-42fa-9160-4f26c43c1c21"

#define NS_IHTTPRESPONSE_IID \
  {0x1acd16c2, 0xdc59, 0x42fa, \
    { 0x91, 0x60, 0x4f, 0x26, 0xc4, 0x3c, 0x1c, 0x21 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIHttpResponse : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IHTTPRESPONSE_IID)

  /**
 * Represents an HTTP response, as described in RFC 2616, section 6.
 */
/**
   * Sets the status line for this.  If this method is never called on this, the
   * status line defaults to "HTTP/", followed by the server's default HTTP
   * version (e.g. "1.1"), followed by " 200 OK".
   *
   * @param httpVersion
   *   the HTTP version of this, as a string (e.g. "1.1"); if null, the server
   *   default is used
   * @param code
   *   the numeric HTTP status code for this
   * @param description
   *   a human-readable description of code; may be null if no description is
   *   desired
   * @throws NS_ERROR_INVALID_ARG
   *   if httpVersion is not a valid HTTP version string, statusCode is greater
   *   than 999, or description contains invalid characters
   * @throws NS_ERROR_NOT_AVAILABLE
   *   if this response is being processed asynchronously and data has been
   *   written to this response's body, or if seizePower() has been called on
   *   this
   */
  /* void setStatusLine (in string httpVersion, in unsigned short statusCode, in string description); */
  NS_SCRIPTABLE NS_IMETHOD SetStatusLine(const char *httpVersion, PRUint16 statusCode, const char *description) = 0;

  /**
   * Sets the specified header in this.
   *
   * @param name
   *   the name of the header; must match the field-name production per RFC 2616
   * @param value
   *   the value of the header; must match the field-value production per RFC
   *   2616
   * @param merge
   *   when true, if the given header already exists in this, the values passed
   *   to this function will be merged into the existing header, per RFC 2616
   *   header semantics (except for the Set-Cookie, WWW-Authenticate, and
   *   Proxy-Authenticate headers, which will treat each such merged header as
   *   an additional instance of the header, for real-world compatibility
   *   reasons); when false, replaces any existing header of the given name (if
   *   any exists) with a new header with the specified value
   * @throws NS_ERROR_INVALID_ARG
   *   if name or value is not a valid header component
   * @throws NS_ERROR_NOT_AVAILABLE
   *   if this response is being processed asynchronously and data has been
   *   written to this response's body, or if seizePower() has been called on
   *   this
   */
  /* void setHeader (in string name, in string value, in boolean merge); */
  NS_SCRIPTABLE NS_IMETHOD SetHeader(const char *name, const char *value, PRBool merge) = 0;

  /**
   * A stream to which data appearing in the body of this response (or in the
   * totality of the response if seizePower() is called) should be written.
   * After this response has been designated as being processed asynchronously,
   * or after seizePower() has been called on this, subsequent writes will no
   * longer be buffered and will be written to the underlying transport without
   * delaying until the entire response is constructed.  Write-through may or
   * may not be synchronous in the implementation, and in any case particular
   * behavior may not be observable to the HTTP client as intermediate buffers
   * both in the server socket and in the client may delay written data; be
   * prepared for delays at any time.
   *
   * @throws NS_ERROR_NOT_AVAILABLE
   *   if accessed after this response is fully constructed
   */
  /* readonly attribute nsIOutputStream bodyOutputStream; */
  NS_SCRIPTABLE NS_IMETHOD GetBodyOutputStream(nsIOutputStream **aBodyOutputStream) = 0;

  /**
   * Writes a string to the response's output stream.  This method is merely a
   * convenient shorthand for writing the same data to bodyOutputStream
   * directly.
   *
   * @note
   *   This method is only guaranteed to work with ASCII data.
   * @throws NS_ERROR_NOT_AVAILABLE
   *   if called after this response has been fully constructed
   */
  /* void write (in string data); */
  NS_SCRIPTABLE NS_IMETHOD Write(const char *data) = 0;

  /**
   * Signals that this response is being constructed asynchronously.  Requests
   * are typically completely constructed during nsIHttpRequestHandler.handle;
   * however, responses which require significant resources (time, memory,
   * processing) to construct can be created and sent incrementally by calling
   * this method during the call to nsIHttpRequestHandler.handle.  This method
   * only has this effect when called during nsIHttpRequestHandler.handle;
   * behavior is undefined if it is called at a later time.  It may be called
   * multiple times with no ill effect, so long as each call occurs before
   * finish() is called.
   *
   * @throws NS_ERROR_UNEXPECTED
   *   if not initially called within a nsIHttpRequestHandler.handle call or if
   *   called after this response has been finished
   * @throws NS_ERROR_NOT_AVAILABLE
   *   if seizePower() has been called on this
   */
  /* void processAsync (); */
  NS_SCRIPTABLE NS_IMETHOD ProcessAsync(void) = 0;

  /**
   * Seizes complete control of this response (and its connection) from the
   * server, allowing raw and unfettered access to data being sent in the HTTP
   * response.  Once this method has been called the only property which may be
   * accessed without an exception being thrown is bodyOutputStream, and the
   * only methods which may be accessed without an exception being thrown are
   * write(), finish(), and seizePower() (which may be called multiple times
   * without ill effect so long as all calls are otherwise allowed).
   *
   * After a successful call, all data subsequently written to the body of this
   * response is written directly to the corresponding connection.  (Previously-
   * written data is silently discarded.)  No status line or headers are sent
   * before doing so; if the response handler wishes to write such data, it must
   * do so manually.  Data generation completes only when finish() is called; it
   * is not enough to simply call close() on bodyOutputStream.
   *
   * @throws NS_ERROR_NOT_AVAILABLE
   *   if processAsync() has been called on this
   * @throws NS_ERROR_UNEXPECTED
   *   if finish() has been called on this
   */
  /* void seizePower (); */
  NS_SCRIPTABLE NS_IMETHOD SeizePower(void) = 0;

  /**
   * Signals that construction of this response is complete and that it may be
   * sent over the network to the client, or if seizePower() has been called
   * signals that all data has been written and that the underlying connection
   * may be closed.  This method may only be called after processAsync() or
   * seizePower() has been called.  This method is idempotent.
   *
   * @throws NS_ERROR_UNEXPECTED
   *   if processAsync() or seizePower() has not already been properly called
   */
  /* void finish (); */
  NS_SCRIPTABLE NS_IMETHOD Finish(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIHttpResponse, NS_IHTTPRESPONSE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIHTTPRESPONSE \
  NS_SCRIPTABLE NS_IMETHOD SetStatusLine(const char *httpVersion, PRUint16 statusCode, const char *description); \
  NS_SCRIPTABLE NS_IMETHOD SetHeader(const char *name, const char *value, PRBool merge); \
  NS_SCRIPTABLE NS_IMETHOD GetBodyOutputStream(nsIOutputStream **aBodyOutputStream); \
  NS_SCRIPTABLE NS_IMETHOD Write(const char *data); \
  NS_SCRIPTABLE NS_IMETHOD ProcessAsync(void); \
  NS_SCRIPTABLE NS_IMETHOD SeizePower(void); \
  NS_SCRIPTABLE NS_IMETHOD Finish(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIHTTPRESPONSE(_to) \
  NS_SCRIPTABLE NS_IMETHOD SetStatusLine(const char *httpVersion, PRUint16 statusCode, const char *description) { return _to SetStatusLine(httpVersion, statusCode, description); } \
  NS_SCRIPTABLE NS_IMETHOD SetHeader(const char *name, const char *value, PRBool merge) { return _to SetHeader(name, value, merge); } \
  NS_SCRIPTABLE NS_IMETHOD GetBodyOutputStream(nsIOutputStream **aBodyOutputStream) { return _to GetBodyOutputStream(aBodyOutputStream); } \
  NS_SCRIPTABLE NS_IMETHOD Write(const char *data) { return _to Write(data); } \
  NS_SCRIPTABLE NS_IMETHOD ProcessAsync(void) { return _to ProcessAsync(); } \
  NS_SCRIPTABLE NS_IMETHOD SeizePower(void) { return _to SeizePower(); } \
  NS_SCRIPTABLE NS_IMETHOD Finish(void) { return _to Finish(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIHTTPRESPONSE(_to) \
  NS_SCRIPTABLE NS_IMETHOD SetStatusLine(const char *httpVersion, PRUint16 statusCode, const char *description) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetStatusLine(httpVersion, statusCode, description); } \
  NS_SCRIPTABLE NS_IMETHOD SetHeader(const char *name, const char *value, PRBool merge) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetHeader(name, value, merge); } \
  NS_SCRIPTABLE NS_IMETHOD GetBodyOutputStream(nsIOutputStream **aBodyOutputStream) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBodyOutputStream(aBodyOutputStream); } \
  NS_SCRIPTABLE NS_IMETHOD Write(const char *data) { return !_to ? NS_ERROR_NULL_POINTER : _to->Write(data); } \
  NS_SCRIPTABLE NS_IMETHOD ProcessAsync(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->ProcessAsync(); } \
  NS_SCRIPTABLE NS_IMETHOD SeizePower(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->SeizePower(); } \
  NS_SCRIPTABLE NS_IMETHOD Finish(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Finish(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsHttpResponse : public nsIHttpResponse
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIHTTPRESPONSE

  nsHttpResponse();

private:
  ~nsHttpResponse();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsHttpResponse, nsIHttpResponse)

nsHttpResponse::nsHttpResponse()
{
  /* member initializers and constructor code */
}

nsHttpResponse::~nsHttpResponse()
{
  /* destructor code */
}

/* void setStatusLine (in string httpVersion, in unsigned short statusCode, in string description); */
NS_IMETHODIMP nsHttpResponse::SetStatusLine(const char *httpVersion, PRUint16 statusCode, const char *description)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setHeader (in string name, in string value, in boolean merge); */
NS_IMETHODIMP nsHttpResponse::SetHeader(const char *name, const char *value, PRBool merge)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIOutputStream bodyOutputStream; */
NS_IMETHODIMP nsHttpResponse::GetBodyOutputStream(nsIOutputStream **aBodyOutputStream)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void write (in string data); */
NS_IMETHODIMP nsHttpResponse::Write(const char *data)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void processAsync (); */
NS_IMETHODIMP nsHttpResponse::ProcessAsync()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void seizePower (); */
NS_IMETHODIMP nsHttpResponse::SeizePower()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void finish (); */
NS_IMETHODIMP nsHttpResponse::Finish()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIHttpServer_h__ */
