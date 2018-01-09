/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/netwerk/protocol/http/nsIHttpChannelInternal.idl
 */

#ifndef __gen_nsIHttpChannelInternal_h__
#define __gen_nsIHttpChannelInternal_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
#include "nsTArray.h"
class nsCString;
class nsIURI; /* forward declaration */

class nsIProxyInfo; /* forward declaration */


/* starting interface:    nsIHttpChannelInternal */
#define NS_IHTTPCHANNELINTERNAL_IID_STR "12eb906a-71fe-4b79-b33a-6fe9ab57ea38"

#define NS_IHTTPCHANNELINTERNAL_IID \
  {0x12eb906a, 0x71fe, 0x4b79, \
    { 0xb3, 0x3a, 0x6f, 0xe9, 0xab, 0x57, 0xea, 0x38 }}

/**
 * Dumping ground for http.  This interface will never be frozen.  If you are
 * using any feature exposed by this interface, be aware that this interface
 * will change and you will be broken.  You have been warned.
 */
class NS_NO_VTABLE NS_SCRIPTABLE nsIHttpChannelInternal : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IHTTPCHANNELINTERNAL_IID)

  /**
     * An http channel can own a reference to the document URI
     */
  /* attribute nsIURI documentURI; */
  NS_SCRIPTABLE NS_IMETHOD GetDocumentURI(nsIURI **aDocumentURI) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetDocumentURI(nsIURI *aDocumentURI) = 0;

  /**
     * Get the major/minor version numbers for the request
     */
  /* void getRequestVersion (out unsigned long major, out unsigned long minor); */
  NS_SCRIPTABLE NS_IMETHOD GetRequestVersion(PRUint32 *major NS_OUTPARAM, PRUint32 *minor NS_OUTPARAM) = 0;

  /**
     * Get the major/minor version numbers for the response
     */
  /* void getResponseVersion (out unsigned long major, out unsigned long minor); */
  NS_SCRIPTABLE NS_IMETHOD GetResponseVersion(PRUint32 *major NS_OUTPARAM, PRUint32 *minor NS_OUTPARAM) = 0;

  /**
     * Helper method to set a cookie with a consumer-provided
     * cookie header, _but_ using the channel's other information
     * (URI's, prompters, date headers etc).
     *
     * @param aCookieHeader
     *        The cookie header to be parsed.
     */
  /* void setCookie (in string aCookieHeader); */
  NS_SCRIPTABLE NS_IMETHOD SetCookie(const char *aCookieHeader) = 0;

  /**
     * Setup this channel as an application cache fallback channel.
     */
  /* void setupFallbackChannel (in string aFallbackKey); */
  NS_SCRIPTABLE NS_IMETHOD SetupFallbackChannel(const char *aFallbackKey) = 0;

  /**
     * Force relevant cookies to be sent with this load even if normally they
     * wouldn't be.
     */
  /* attribute boolean forceAllowThirdPartyCookie; */
  NS_SCRIPTABLE NS_IMETHOD GetForceAllowThirdPartyCookie(PRBool *aForceAllowThirdPartyCookie) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetForceAllowThirdPartyCookie(PRBool aForceAllowThirdPartyCookie) = 0;

  /**
     * True iff the channel has been canceled.
     */
  /* readonly attribute boolean canceled; */
  NS_SCRIPTABLE NS_IMETHOD GetCanceled(PRBool *aCanceled) = 0;

  /**
     * External handlers may set this to true to notify the channel
     * that it is open on behalf of a download.
     */
  /* attribute boolean channelIsForDownload; */
  NS_SCRIPTABLE NS_IMETHOD GetChannelIsForDownload(PRBool *aChannelIsForDownload) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetChannelIsForDownload(PRBool aChannelIsForDownload) = 0;

  /**
     * The local IP address to which this channel is bound, in the
     * format produced by PR_NetAddrToString. May be IPv4 or IPv6.
     * Note: in the presence of NAT, this may not be the same as the
     * address that the remote host thinks it's talking to.
     *
     * May throw NS_ERROR_NOT_AVAILABLE if accessed when the channel's
     * endpoints are not yet determined, or in any case when
     * nsIHttpActivityObserver.isActive is false. See bugs 534698 and 526207.
     */
  /* readonly attribute AUTF8String localAddress; */
  NS_SCRIPTABLE NS_IMETHOD GetLocalAddress(nsACString & aLocalAddress) = 0;

  /**
     * The local port number to which this channel is bound.
     *
     * May throw NS_ERROR_NOT_AVAILABLE if accessed when the channel's
     * endpoints are not yet determined, or in any case when
     * nsIHttpActivityObserver.isActive is false. See bugs 534698 and 526207.
     */
  /* readonly attribute PRInt32 localPort; */
  NS_SCRIPTABLE NS_IMETHOD GetLocalPort(PRInt32 *aLocalPort) = 0;

  /**
     * The IP address of the remote host that this channel is
     * connected to, in the format produced by PR_NetAddrToString.
     *
     * May throw NS_ERROR_NOT_AVAILABLE if accessed when the channel's
     * endpoints are not yet determined, or in any case when
     * nsIHttpActivityObserver.isActive is false. See bugs 534698 and 526207.
     */
  /* readonly attribute AUTF8String remoteAddress; */
  NS_SCRIPTABLE NS_IMETHOD GetRemoteAddress(nsACString & aRemoteAddress) = 0;

  /**
     * The remote port number that this channel is connected to.
     *
     * May throw NS_ERROR_NOT_AVAILABLE if accessed when the channel's
     * endpoints are not yet determined, or in any case when
     * nsIHttpActivityObserver.isActive is false. See bugs 534698 and 526207.
     */
  /* readonly attribute PRInt32 remotePort; */
  NS_SCRIPTABLE NS_IMETHOD GetRemotePort(PRInt32 *aRemotePort) = 0;

  /**
     * Transfer chain of redirected cache-keys.
     */
  /* [noscript] void setCacheKeysRedirectChain (in StringArray cacheKeys); */
  NS_IMETHOD SetCacheKeysRedirectChain(nsTArray<nsCString> *cacheKeys) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIHttpChannelInternal, NS_IHTTPCHANNELINTERNAL_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIHTTPCHANNELINTERNAL \
  NS_SCRIPTABLE NS_IMETHOD GetDocumentURI(nsIURI **aDocumentURI); \
  NS_SCRIPTABLE NS_IMETHOD SetDocumentURI(nsIURI *aDocumentURI); \
  NS_SCRIPTABLE NS_IMETHOD GetRequestVersion(PRUint32 *major NS_OUTPARAM, PRUint32 *minor NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD GetResponseVersion(PRUint32 *major NS_OUTPARAM, PRUint32 *minor NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetCookie(const char *aCookieHeader); \
  NS_SCRIPTABLE NS_IMETHOD SetupFallbackChannel(const char *aFallbackKey); \
  NS_SCRIPTABLE NS_IMETHOD GetForceAllowThirdPartyCookie(PRBool *aForceAllowThirdPartyCookie); \
  NS_SCRIPTABLE NS_IMETHOD SetForceAllowThirdPartyCookie(PRBool aForceAllowThirdPartyCookie); \
  NS_SCRIPTABLE NS_IMETHOD GetCanceled(PRBool *aCanceled); \
  NS_SCRIPTABLE NS_IMETHOD GetChannelIsForDownload(PRBool *aChannelIsForDownload); \
  NS_SCRIPTABLE NS_IMETHOD SetChannelIsForDownload(PRBool aChannelIsForDownload); \
  NS_SCRIPTABLE NS_IMETHOD GetLocalAddress(nsACString & aLocalAddress); \
  NS_SCRIPTABLE NS_IMETHOD GetLocalPort(PRInt32 *aLocalPort); \
  NS_SCRIPTABLE NS_IMETHOD GetRemoteAddress(nsACString & aRemoteAddress); \
  NS_SCRIPTABLE NS_IMETHOD GetRemotePort(PRInt32 *aRemotePort); \
  NS_IMETHOD SetCacheKeysRedirectChain(nsTArray<nsCString> *cacheKeys); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIHTTPCHANNELINTERNAL(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetDocumentURI(nsIURI **aDocumentURI) { return _to GetDocumentURI(aDocumentURI); } \
  NS_SCRIPTABLE NS_IMETHOD SetDocumentURI(nsIURI *aDocumentURI) { return _to SetDocumentURI(aDocumentURI); } \
  NS_SCRIPTABLE NS_IMETHOD GetRequestVersion(PRUint32 *major NS_OUTPARAM, PRUint32 *minor NS_OUTPARAM) { return _to GetRequestVersion(major, minor); } \
  NS_SCRIPTABLE NS_IMETHOD GetResponseVersion(PRUint32 *major NS_OUTPARAM, PRUint32 *minor NS_OUTPARAM) { return _to GetResponseVersion(major, minor); } \
  NS_SCRIPTABLE NS_IMETHOD SetCookie(const char *aCookieHeader) { return _to SetCookie(aCookieHeader); } \
  NS_SCRIPTABLE NS_IMETHOD SetupFallbackChannel(const char *aFallbackKey) { return _to SetupFallbackChannel(aFallbackKey); } \
  NS_SCRIPTABLE NS_IMETHOD GetForceAllowThirdPartyCookie(PRBool *aForceAllowThirdPartyCookie) { return _to GetForceAllowThirdPartyCookie(aForceAllowThirdPartyCookie); } \
  NS_SCRIPTABLE NS_IMETHOD SetForceAllowThirdPartyCookie(PRBool aForceAllowThirdPartyCookie) { return _to SetForceAllowThirdPartyCookie(aForceAllowThirdPartyCookie); } \
  NS_SCRIPTABLE NS_IMETHOD GetCanceled(PRBool *aCanceled) { return _to GetCanceled(aCanceled); } \
  NS_SCRIPTABLE NS_IMETHOD GetChannelIsForDownload(PRBool *aChannelIsForDownload) { return _to GetChannelIsForDownload(aChannelIsForDownload); } \
  NS_SCRIPTABLE NS_IMETHOD SetChannelIsForDownload(PRBool aChannelIsForDownload) { return _to SetChannelIsForDownload(aChannelIsForDownload); } \
  NS_SCRIPTABLE NS_IMETHOD GetLocalAddress(nsACString & aLocalAddress) { return _to GetLocalAddress(aLocalAddress); } \
  NS_SCRIPTABLE NS_IMETHOD GetLocalPort(PRInt32 *aLocalPort) { return _to GetLocalPort(aLocalPort); } \
  NS_SCRIPTABLE NS_IMETHOD GetRemoteAddress(nsACString & aRemoteAddress) { return _to GetRemoteAddress(aRemoteAddress); } \
  NS_SCRIPTABLE NS_IMETHOD GetRemotePort(PRInt32 *aRemotePort) { return _to GetRemotePort(aRemotePort); } \
  NS_IMETHOD SetCacheKeysRedirectChain(nsTArray<nsCString> *cacheKeys) { return _to SetCacheKeysRedirectChain(cacheKeys); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIHTTPCHANNELINTERNAL(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetDocumentURI(nsIURI **aDocumentURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDocumentURI(aDocumentURI); } \
  NS_SCRIPTABLE NS_IMETHOD SetDocumentURI(nsIURI *aDocumentURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDocumentURI(aDocumentURI); } \
  NS_SCRIPTABLE NS_IMETHOD GetRequestVersion(PRUint32 *major NS_OUTPARAM, PRUint32 *minor NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetRequestVersion(major, minor); } \
  NS_SCRIPTABLE NS_IMETHOD GetResponseVersion(PRUint32 *major NS_OUTPARAM, PRUint32 *minor NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetResponseVersion(major, minor); } \
  NS_SCRIPTABLE NS_IMETHOD SetCookie(const char *aCookieHeader) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetCookie(aCookieHeader); } \
  NS_SCRIPTABLE NS_IMETHOD SetupFallbackChannel(const char *aFallbackKey) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetupFallbackChannel(aFallbackKey); } \
  NS_SCRIPTABLE NS_IMETHOD GetForceAllowThirdPartyCookie(PRBool *aForceAllowThirdPartyCookie) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetForceAllowThirdPartyCookie(aForceAllowThirdPartyCookie); } \
  NS_SCRIPTABLE NS_IMETHOD SetForceAllowThirdPartyCookie(PRBool aForceAllowThirdPartyCookie) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetForceAllowThirdPartyCookie(aForceAllowThirdPartyCookie); } \
  NS_SCRIPTABLE NS_IMETHOD GetCanceled(PRBool *aCanceled) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCanceled(aCanceled); } \
  NS_SCRIPTABLE NS_IMETHOD GetChannelIsForDownload(PRBool *aChannelIsForDownload) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetChannelIsForDownload(aChannelIsForDownload); } \
  NS_SCRIPTABLE NS_IMETHOD SetChannelIsForDownload(PRBool aChannelIsForDownload) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetChannelIsForDownload(aChannelIsForDownload); } \
  NS_SCRIPTABLE NS_IMETHOD GetLocalAddress(nsACString & aLocalAddress) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLocalAddress(aLocalAddress); } \
  NS_SCRIPTABLE NS_IMETHOD GetLocalPort(PRInt32 *aLocalPort) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLocalPort(aLocalPort); } \
  NS_SCRIPTABLE NS_IMETHOD GetRemoteAddress(nsACString & aRemoteAddress) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetRemoteAddress(aRemoteAddress); } \
  NS_SCRIPTABLE NS_IMETHOD GetRemotePort(PRInt32 *aRemotePort) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetRemotePort(aRemotePort); } \
  NS_IMETHOD SetCacheKeysRedirectChain(nsTArray<nsCString> *cacheKeys) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetCacheKeysRedirectChain(cacheKeys); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsHttpChannelInternal : public nsIHttpChannelInternal
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIHTTPCHANNELINTERNAL

  nsHttpChannelInternal();

private:
  ~nsHttpChannelInternal();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsHttpChannelInternal, nsIHttpChannelInternal)

nsHttpChannelInternal::nsHttpChannelInternal()
{
  /* member initializers and constructor code */
}

nsHttpChannelInternal::~nsHttpChannelInternal()
{
  /* destructor code */
}

/* attribute nsIURI documentURI; */
NS_IMETHODIMP nsHttpChannelInternal::GetDocumentURI(nsIURI **aDocumentURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsHttpChannelInternal::SetDocumentURI(nsIURI *aDocumentURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void getRequestVersion (out unsigned long major, out unsigned long minor); */
NS_IMETHODIMP nsHttpChannelInternal::GetRequestVersion(PRUint32 *major NS_OUTPARAM, PRUint32 *minor NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void getResponseVersion (out unsigned long major, out unsigned long minor); */
NS_IMETHODIMP nsHttpChannelInternal::GetResponseVersion(PRUint32 *major NS_OUTPARAM, PRUint32 *minor NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setCookie (in string aCookieHeader); */
NS_IMETHODIMP nsHttpChannelInternal::SetCookie(const char *aCookieHeader)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setupFallbackChannel (in string aFallbackKey); */
NS_IMETHODIMP nsHttpChannelInternal::SetupFallbackChannel(const char *aFallbackKey)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean forceAllowThirdPartyCookie; */
NS_IMETHODIMP nsHttpChannelInternal::GetForceAllowThirdPartyCookie(PRBool *aForceAllowThirdPartyCookie)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsHttpChannelInternal::SetForceAllowThirdPartyCookie(PRBool aForceAllowThirdPartyCookie)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean canceled; */
NS_IMETHODIMP nsHttpChannelInternal::GetCanceled(PRBool *aCanceled)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean channelIsForDownload; */
NS_IMETHODIMP nsHttpChannelInternal::GetChannelIsForDownload(PRBool *aChannelIsForDownload)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsHttpChannelInternal::SetChannelIsForDownload(PRBool aChannelIsForDownload)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AUTF8String localAddress; */
NS_IMETHODIMP nsHttpChannelInternal::GetLocalAddress(nsACString & aLocalAddress)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRInt32 localPort; */
NS_IMETHODIMP nsHttpChannelInternal::GetLocalPort(PRInt32 *aLocalPort)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AUTF8String remoteAddress; */
NS_IMETHODIMP nsHttpChannelInternal::GetRemoteAddress(nsACString & aRemoteAddress)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRInt32 remotePort; */
NS_IMETHODIMP nsHttpChannelInternal::GetRemotePort(PRInt32 *aRemotePort)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void setCacheKeysRedirectChain (in StringArray cacheKeys); */
NS_IMETHODIMP nsHttpChannelInternal::SetCacheKeysRedirectChain(nsTArray<nsCString> *cacheKeys)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIHttpChannelInternal_h__ */
