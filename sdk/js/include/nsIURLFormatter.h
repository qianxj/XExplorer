/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/toolkit/components/urlformatter/nsIURLFormatter.idl
 */

#ifndef __gen_nsIURLFormatter_h__
#define __gen_nsIURLFormatter_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIURLFormatter */
#define NS_IURLFORMATTER_IID_STR "4ab31d30-372d-11db-a98b-0800200c9a66"

#define NS_IURLFORMATTER_IID \
  {0x4ab31d30, 0x372d, 0x11db, \
    { 0xa9, 0x8b, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIURLFormatter : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IURLFORMATTER_IID)

  /** 
   * formatURL - Formats a string URL
   *
   * The set of known variables is predefined.
   * If a variable is unknown, it is left unchanged and a non-fatal error is reported.
   *
   * @param aFormat string Unformatted URL.
   *
   * @return The formatted URL.
   */
  /* AString formatURL (in AString aFormat); */
  NS_SCRIPTABLE NS_IMETHOD FormatURL(const nsAString & aFormat, nsAString & _retval NS_OUTPARAM) = 0;

  /** 
   * formatURLPref - Formats a string URL stored in a preference
   *
   * If the preference value cannot be retrieved, a fatal error is reported
   * and the "about:blank" URL is returned.
   *
   * @param aPref string Preference name.
   *
   * @return The formatted URL returned by formatURL(), or "about:blank".
   */
  /* AString formatURLPref (in AString aPref); */
  NS_SCRIPTABLE NS_IMETHOD FormatURLPref(const nsAString & aPref, nsAString & _retval NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIURLFormatter, NS_IURLFORMATTER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIURLFORMATTER \
  NS_SCRIPTABLE NS_IMETHOD FormatURL(const nsAString & aFormat, nsAString & _retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD FormatURLPref(const nsAString & aPref, nsAString & _retval NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIURLFORMATTER(_to) \
  NS_SCRIPTABLE NS_IMETHOD FormatURL(const nsAString & aFormat, nsAString & _retval NS_OUTPARAM) { return _to FormatURL(aFormat, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD FormatURLPref(const nsAString & aPref, nsAString & _retval NS_OUTPARAM) { return _to FormatURLPref(aPref, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIURLFORMATTER(_to) \
  NS_SCRIPTABLE NS_IMETHOD FormatURL(const nsAString & aFormat, nsAString & _retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->FormatURL(aFormat, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD FormatURLPref(const nsAString & aPref, nsAString & _retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->FormatURLPref(aPref, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsURLFormatter : public nsIURLFormatter
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIURLFORMATTER

  nsURLFormatter();

private:
  ~nsURLFormatter();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsURLFormatter, nsIURLFormatter)

nsURLFormatter::nsURLFormatter()
{
  /* member initializers and constructor code */
}

nsURLFormatter::~nsURLFormatter()
{
  /* destructor code */
}

/* AString formatURL (in AString aFormat); */
NS_IMETHODIMP nsURLFormatter::FormatURL(const nsAString & aFormat, nsAString & _retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString formatURLPref (in AString aPref); */
NS_IMETHODIMP nsURLFormatter::FormatURLPref(const nsAString & aPref, nsAString & _retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIURLFormatter_h__ */
