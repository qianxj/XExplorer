/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/layout/tools/layout-debug/src/nsILayoutRegressionTester.idl
 */

#ifndef __gen_nsILayoutRegressionTester_h__
#define __gen_nsILayoutRegressionTester_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMWindow; /* forward declaration */

class nsILocalFile; /* forward declaration */


/* starting interface:    nsILayoutRegressionTester */
#define NS_ILAYOUTREGRESSIONTESTER_IID_STR "b249b2c0-ee11-11da-8ad9-0800200c9a66"

#define NS_ILAYOUTREGRESSIONTESTER_IID \
  {0xb249b2c0, 0xee11, 0x11da, \
    { 0x8a, 0xd9, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsILayoutRegressionTester : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ILAYOUTREGRESSIONTESTER_IID)

  /**
   * Dumps the content of a window
   * @param aWindowToDump       the window to dump (may be an iframe etc)
   * @param aFile               the file to dump to. It will be created if necessary, otherwise
                                truncated. If nil, write to stdout.
   * @param aFlagsMask          some flags that determine what to dump
   * @param aFlagsMask          some flags that determine what to dump
   * @param aResult             a status value indicating whether the dump happened, 
                                whether the page was still loading, or whether some other error happened.
   */
  enum { DUMP_FLAGS_MASK_DEFAULT = 0 };

  enum { DUMP_FLAGS_MASK_PRINT_MODE = 1 };

  enum { DUMP_RESULT_COMPLETED = 0 };

  enum { DUMP_RESULT_LOADING = 1 };

  enum { DUMP_RESULT_ERROR = 2 };

  /* long dumpFrameModel (in nsIDOMWindow aWindowToDump, in nsILocalFile aFile, in unsigned long aFlagsMask); */
  NS_SCRIPTABLE NS_IMETHOD DumpFrameModel(nsIDOMWindow *aWindowToDump, nsILocalFile *aFile, PRUint32 aFlagsMask, PRInt32 *_retval NS_OUTPARAM) = 0;

  /**
   * Compares the contents of frame model files
   * @param aBaseFile           the baseline file, opened with read permissions
   * @param aVerFile            file containing the results to verify, opened with read permissions
   * @param aFlags              flags specifying output verbosity
   * @param aResult             result of the comparison: zero if the files are same, non-zero if different
   */
  enum { COMPARE_FLAGS_VERBOSE = 0 };

  enum { COMPARE_FLAGS_BRIEF = 1 };

  /* long compareFrameModels (in nsILocalFile aBaseFile, in nsILocalFile aVerFile, in unsigned long aFlags); */
  NS_SCRIPTABLE NS_IMETHOD CompareFrameModels(nsILocalFile *aBaseFile, nsILocalFile *aVerFile, PRUint32 aFlags, PRInt32 *_retval NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsILayoutRegressionTester, NS_ILAYOUTREGRESSIONTESTER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSILAYOUTREGRESSIONTESTER \
  NS_SCRIPTABLE NS_IMETHOD DumpFrameModel(nsIDOMWindow *aWindowToDump, nsILocalFile *aFile, PRUint32 aFlagsMask, PRInt32 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD CompareFrameModels(nsILocalFile *aBaseFile, nsILocalFile *aVerFile, PRUint32 aFlags, PRInt32 *_retval NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSILAYOUTREGRESSIONTESTER(_to) \
  NS_SCRIPTABLE NS_IMETHOD DumpFrameModel(nsIDOMWindow *aWindowToDump, nsILocalFile *aFile, PRUint32 aFlagsMask, PRInt32 *_retval NS_OUTPARAM) { return _to DumpFrameModel(aWindowToDump, aFile, aFlagsMask, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD CompareFrameModels(nsILocalFile *aBaseFile, nsILocalFile *aVerFile, PRUint32 aFlags, PRInt32 *_retval NS_OUTPARAM) { return _to CompareFrameModels(aBaseFile, aVerFile, aFlags, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSILAYOUTREGRESSIONTESTER(_to) \
  NS_SCRIPTABLE NS_IMETHOD DumpFrameModel(nsIDOMWindow *aWindowToDump, nsILocalFile *aFile, PRUint32 aFlagsMask, PRInt32 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->DumpFrameModel(aWindowToDump, aFile, aFlagsMask, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD CompareFrameModels(nsILocalFile *aBaseFile, nsILocalFile *aVerFile, PRUint32 aFlags, PRInt32 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->CompareFrameModels(aBaseFile, aVerFile, aFlags, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsLayoutRegressionTester : public nsILayoutRegressionTester
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSILAYOUTREGRESSIONTESTER

  nsLayoutRegressionTester();

private:
  ~nsLayoutRegressionTester();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsLayoutRegressionTester, nsILayoutRegressionTester)

nsLayoutRegressionTester::nsLayoutRegressionTester()
{
  /* member initializers and constructor code */
}

nsLayoutRegressionTester::~nsLayoutRegressionTester()
{
  /* destructor code */
}

/* long dumpFrameModel (in nsIDOMWindow aWindowToDump, in nsILocalFile aFile, in unsigned long aFlagsMask); */
NS_IMETHODIMP nsLayoutRegressionTester::DumpFrameModel(nsIDOMWindow *aWindowToDump, nsILocalFile *aFile, PRUint32 aFlagsMask, PRInt32 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* long compareFrameModels (in nsILocalFile aBaseFile, in nsILocalFile aVerFile, in unsigned long aFlags); */
NS_IMETHODIMP nsLayoutRegressionTester::CompareFrameModels(nsILocalFile *aBaseFile, nsILocalFile *aVerFile, PRUint32 aFlags, PRInt32 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsILayoutRegressionTester_h__ */
