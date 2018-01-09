/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/modules/plugin/base/public/nsIPluginStreamListener.idl
 */

#ifndef __gen_nsIPluginStreamListener_h__
#define __gen_nsIPluginStreamListener_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nspluginroot_h__
#include "nspluginroot.h"
#endif

#ifndef __gen_nsIPluginStreamInfo_h__
#include "nsIPluginStreamInfo.h"
#endif

#ifndef __gen_nsIInputStream_h__
#include "nsIInputStream.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
#include "npapi.h"

/* starting interface:    nsIPluginStreamListener */
#define NS_IPLUGINSTREAMLISTENER_IID_STR "02c9dad7-d9dd-4f36-bd5e-93ca404a31bc"

#define NS_IPLUGINSTREAMLISTENER_IID \
  {0x02c9dad7, 0xd9dd, 0x4f36, \
    { 0xbd, 0x5e, 0x93, 0xca, 0x40, 0x4a, 0x31, 0xbc }}

/**
 * The nsIPluginStreamListener interface defines the minimum set of functionality that
 * the browser will support if it allows plugins. Plugins can call QueryInterface
 * to determine if a plugin manager implements more specific APIs or other 
 * browser interfaces for the plugin to use (e.g. nsINetworkManager).
 */
class NS_NO_VTABLE nsIPluginStreamListener : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IPLUGINSTREAMLISTENER_IID)

  /**
     * Notify the observer that the URL has started to load.  This method is
     * called only once, at the beginning of a URL load.<BR><BR>
   *
   * @param aPluginInfo  - plugin stream info
   * @return             - the return value is currently ignored, in the future
   * it may be used to cancel the URL load..
     */
  /* void onStartBinding (in nsIPluginStreamInfo aPluginInfo); */
  NS_IMETHOD OnStartBinding(nsIPluginStreamInfo *aPluginInfo) = 0;

  /**
     * Notify the client that data is available in the input stream.  This
     * method is called whenver data is written into the input stream by the
     * networking library...<BR><BR>
     * 
   * @param aPluginInfo  - plugin stream info
   * @param aInputStream - the input stream containing the data. This stream can
     * be either a blocking or non-blocking stream.
   * @param aLength      - the amount of data that was just pushed into the stream.
   * @return             - the return value is currently ignored.
     */
  /* void onDataAvailable (in nsIPluginStreamInfo aPluginInfo, in nsIInputStream aInputStream, in unsigned long aLength); */
  NS_IMETHOD OnDataAvailable(nsIPluginStreamInfo *aPluginInfo, nsIInputStream *aInputStream, PRUint32 aLength) = 0;

  /**
   * Notify the client that data is available in the file.
   * 
   * @param aPluginInfo - plugin stream info
   * @param aFileName   - the name of the file containing the data
   * @return            - the return value is currently ignored.
   */
  /* void onFileAvailable (in nsIPluginStreamInfo aPluginInfo, in string aFileName); */
  NS_IMETHOD OnFileAvailable(nsIPluginStreamInfo *aPluginInfo, const char *aFileName) = 0;

  /**
     * Notify the observer that the URL has finished loading.  This method is 
     * called once when the networking library has finished processing the 
   * URL transaction initiatied via the nsINetService::Open(...) call.<BR><BR>
     * 
     * This method is called regardless of whether the URL loaded successfully.<BR><BR>
     * 
   * @param aPluginInfo - plugin stream info
   * @param aStatus     - reason why the stream has been terminated
   * @return            - the return value is currently ignored.
     */
  /* void onStopBinding (in nsIPluginStreamInfo aPluginInfo, in nsresult aStatus); */
  NS_IMETHOD OnStopBinding(nsIPluginStreamInfo *aPluginInfo, nsresult aStatus) = 0;

  /**
   * Gets the type of the stream
   *
   * @param aStreamType - the type of the stream
   */
  /* readonly attribute PRInt32 streamType; */
  NS_IMETHOD GetStreamType(PRInt32 *aStreamType) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIPluginStreamListener, NS_IPLUGINSTREAMLISTENER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIPLUGINSTREAMLISTENER \
  NS_IMETHOD OnStartBinding(nsIPluginStreamInfo *aPluginInfo); \
  NS_IMETHOD OnDataAvailable(nsIPluginStreamInfo *aPluginInfo, nsIInputStream *aInputStream, PRUint32 aLength); \
  NS_IMETHOD OnFileAvailable(nsIPluginStreamInfo *aPluginInfo, const char *aFileName); \
  NS_IMETHOD OnStopBinding(nsIPluginStreamInfo *aPluginInfo, nsresult aStatus); \
  NS_IMETHOD GetStreamType(PRInt32 *aStreamType); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIPLUGINSTREAMLISTENER(_to) \
  NS_IMETHOD OnStartBinding(nsIPluginStreamInfo *aPluginInfo) { return _to OnStartBinding(aPluginInfo); } \
  NS_IMETHOD OnDataAvailable(nsIPluginStreamInfo *aPluginInfo, nsIInputStream *aInputStream, PRUint32 aLength) { return _to OnDataAvailable(aPluginInfo, aInputStream, aLength); } \
  NS_IMETHOD OnFileAvailable(nsIPluginStreamInfo *aPluginInfo, const char *aFileName) { return _to OnFileAvailable(aPluginInfo, aFileName); } \
  NS_IMETHOD OnStopBinding(nsIPluginStreamInfo *aPluginInfo, nsresult aStatus) { return _to OnStopBinding(aPluginInfo, aStatus); } \
  NS_IMETHOD GetStreamType(PRInt32 *aStreamType) { return _to GetStreamType(aStreamType); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIPLUGINSTREAMLISTENER(_to) \
  NS_IMETHOD OnStartBinding(nsIPluginStreamInfo *aPluginInfo) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnStartBinding(aPluginInfo); } \
  NS_IMETHOD OnDataAvailable(nsIPluginStreamInfo *aPluginInfo, nsIInputStream *aInputStream, PRUint32 aLength) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnDataAvailable(aPluginInfo, aInputStream, aLength); } \
  NS_IMETHOD OnFileAvailable(nsIPluginStreamInfo *aPluginInfo, const char *aFileName) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnFileAvailable(aPluginInfo, aFileName); } \
  NS_IMETHOD OnStopBinding(nsIPluginStreamInfo *aPluginInfo, nsresult aStatus) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnStopBinding(aPluginInfo, aStatus); } \
  NS_IMETHOD GetStreamType(PRInt32 *aStreamType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStreamType(aStreamType); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsPluginStreamListener : public nsIPluginStreamListener
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIPLUGINSTREAMLISTENER

  nsPluginStreamListener();

private:
  ~nsPluginStreamListener();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsPluginStreamListener, nsIPluginStreamListener)

nsPluginStreamListener::nsPluginStreamListener()
{
  /* member initializers and constructor code */
}

nsPluginStreamListener::~nsPluginStreamListener()
{
  /* destructor code */
}

/* void onStartBinding (in nsIPluginStreamInfo aPluginInfo); */
NS_IMETHODIMP nsPluginStreamListener::OnStartBinding(nsIPluginStreamInfo *aPluginInfo)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onDataAvailable (in nsIPluginStreamInfo aPluginInfo, in nsIInputStream aInputStream, in unsigned long aLength); */
NS_IMETHODIMP nsPluginStreamListener::OnDataAvailable(nsIPluginStreamInfo *aPluginInfo, nsIInputStream *aInputStream, PRUint32 aLength)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onFileAvailable (in nsIPluginStreamInfo aPluginInfo, in string aFileName); */
NS_IMETHODIMP nsPluginStreamListener::OnFileAvailable(nsIPluginStreamInfo *aPluginInfo, const char *aFileName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onStopBinding (in nsIPluginStreamInfo aPluginInfo, in nsresult aStatus); */
NS_IMETHODIMP nsPluginStreamListener::OnStopBinding(nsIPluginStreamInfo *aPluginInfo, nsresult aStatus)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRInt32 streamType; */
NS_IMETHODIMP nsPluginStreamListener::GetStreamType(PRInt32 *aStreamType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIPluginStreamListener_h__ */
