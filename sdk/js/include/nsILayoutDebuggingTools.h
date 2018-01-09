/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/layout/tools/layout-debug/src/nsILayoutDebuggingTools.idl
 */

#ifndef __gen_nsILayoutDebuggingTools_h__
#define __gen_nsILayoutDebuggingTools_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMWindow; /* forward declaration */


/* starting interface:    nsILayoutDebuggingTools */
#define NS_ILAYOUTDEBUGGINGTOOLS_IID_STR "4b968d4b-9c08-4635-a7e0-55084843f0fd"

#define NS_ILAYOUTDEBUGGINGTOOLS_IID \
  {0x4b968d4b, 0x9c08, 0x4635, \
    { 0xa7, 0xe0, 0x55, 0x08, 0x48, 0x43, 0xf0, 0xfd }}

/**
 * A series of hooks into non-IDL-ized layout code to allow all the
 * layout debugging functions to be used from chrome.
 */
class NS_NO_VTABLE NS_SCRIPTABLE nsILayoutDebuggingTools : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ILAYOUTDEBUGGINGTOOLS_IID)

  /* void init (in nsIDOMWindow win); */
  NS_SCRIPTABLE NS_IMETHOD Init(nsIDOMWindow *win) = 0;

  /* void newURILoaded (); */
  NS_SCRIPTABLE NS_IMETHOD NewURILoaded(void) = 0;

  /* attribute boolean visualDebugging; */
  NS_SCRIPTABLE NS_IMETHOD GetVisualDebugging(PRBool *aVisualDebugging) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetVisualDebugging(PRBool aVisualDebugging) = 0;

  /* attribute boolean visualEventDebugging; */
  NS_SCRIPTABLE NS_IMETHOD GetVisualEventDebugging(PRBool *aVisualEventDebugging) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetVisualEventDebugging(PRBool aVisualEventDebugging) = 0;

  /* attribute boolean paintFlashing; */
  NS_SCRIPTABLE NS_IMETHOD GetPaintFlashing(PRBool *aPaintFlashing) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetPaintFlashing(PRBool aPaintFlashing) = 0;

  /* attribute boolean paintDumping; */
  NS_SCRIPTABLE NS_IMETHOD GetPaintDumping(PRBool *aPaintDumping) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetPaintDumping(PRBool aPaintDumping) = 0;

  /* attribute boolean invalidateDumping; */
  NS_SCRIPTABLE NS_IMETHOD GetInvalidateDumping(PRBool *aInvalidateDumping) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetInvalidateDumping(PRBool aInvalidateDumping) = 0;

  /* attribute boolean eventDumping; */
  NS_SCRIPTABLE NS_IMETHOD GetEventDumping(PRBool *aEventDumping) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetEventDumping(PRBool aEventDumping) = 0;

  /* attribute boolean motionEventDumping; */
  NS_SCRIPTABLE NS_IMETHOD GetMotionEventDumping(PRBool *aMotionEventDumping) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetMotionEventDumping(PRBool aMotionEventDumping) = 0;

  /* attribute boolean crossingEventDumping; */
  NS_SCRIPTABLE NS_IMETHOD GetCrossingEventDumping(PRBool *aCrossingEventDumping) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetCrossingEventDumping(PRBool aCrossingEventDumping) = 0;

  /* attribute boolean reflowCounts; */
  NS_SCRIPTABLE NS_IMETHOD GetReflowCounts(PRBool *aReflowCounts) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetReflowCounts(PRBool aReflowCounts) = 0;

  /* void dumpWebShells (); */
  NS_SCRIPTABLE NS_IMETHOD DumpWebShells(void) = 0;

  /* void dumpContent (); */
  NS_SCRIPTABLE NS_IMETHOD DumpContent(void) = 0;

  /* void dumpFrames (); */
  NS_SCRIPTABLE NS_IMETHOD DumpFrames(void) = 0;

  /* void dumpViews (); */
  NS_SCRIPTABLE NS_IMETHOD DumpViews(void) = 0;

  /* void dumpStyleSheets (); */
  NS_SCRIPTABLE NS_IMETHOD DumpStyleSheets(void) = 0;

  /* void dumpStyleContexts (); */
  NS_SCRIPTABLE NS_IMETHOD DumpStyleContexts(void) = 0;

  /* void dumpReflowStats (); */
  NS_SCRIPTABLE NS_IMETHOD DumpReflowStats(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsILayoutDebuggingTools, NS_ILAYOUTDEBUGGINGTOOLS_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSILAYOUTDEBUGGINGTOOLS \
  NS_SCRIPTABLE NS_IMETHOD Init(nsIDOMWindow *win); \
  NS_SCRIPTABLE NS_IMETHOD NewURILoaded(void); \
  NS_SCRIPTABLE NS_IMETHOD GetVisualDebugging(PRBool *aVisualDebugging); \
  NS_SCRIPTABLE NS_IMETHOD SetVisualDebugging(PRBool aVisualDebugging); \
  NS_SCRIPTABLE NS_IMETHOD GetVisualEventDebugging(PRBool *aVisualEventDebugging); \
  NS_SCRIPTABLE NS_IMETHOD SetVisualEventDebugging(PRBool aVisualEventDebugging); \
  NS_SCRIPTABLE NS_IMETHOD GetPaintFlashing(PRBool *aPaintFlashing); \
  NS_SCRIPTABLE NS_IMETHOD SetPaintFlashing(PRBool aPaintFlashing); \
  NS_SCRIPTABLE NS_IMETHOD GetPaintDumping(PRBool *aPaintDumping); \
  NS_SCRIPTABLE NS_IMETHOD SetPaintDumping(PRBool aPaintDumping); \
  NS_SCRIPTABLE NS_IMETHOD GetInvalidateDumping(PRBool *aInvalidateDumping); \
  NS_SCRIPTABLE NS_IMETHOD SetInvalidateDumping(PRBool aInvalidateDumping); \
  NS_SCRIPTABLE NS_IMETHOD GetEventDumping(PRBool *aEventDumping); \
  NS_SCRIPTABLE NS_IMETHOD SetEventDumping(PRBool aEventDumping); \
  NS_SCRIPTABLE NS_IMETHOD GetMotionEventDumping(PRBool *aMotionEventDumping); \
  NS_SCRIPTABLE NS_IMETHOD SetMotionEventDumping(PRBool aMotionEventDumping); \
  NS_SCRIPTABLE NS_IMETHOD GetCrossingEventDumping(PRBool *aCrossingEventDumping); \
  NS_SCRIPTABLE NS_IMETHOD SetCrossingEventDumping(PRBool aCrossingEventDumping); \
  NS_SCRIPTABLE NS_IMETHOD GetReflowCounts(PRBool *aReflowCounts); \
  NS_SCRIPTABLE NS_IMETHOD SetReflowCounts(PRBool aReflowCounts); \
  NS_SCRIPTABLE NS_IMETHOD DumpWebShells(void); \
  NS_SCRIPTABLE NS_IMETHOD DumpContent(void); \
  NS_SCRIPTABLE NS_IMETHOD DumpFrames(void); \
  NS_SCRIPTABLE NS_IMETHOD DumpViews(void); \
  NS_SCRIPTABLE NS_IMETHOD DumpStyleSheets(void); \
  NS_SCRIPTABLE NS_IMETHOD DumpStyleContexts(void); \
  NS_SCRIPTABLE NS_IMETHOD DumpReflowStats(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSILAYOUTDEBUGGINGTOOLS(_to) \
  NS_SCRIPTABLE NS_IMETHOD Init(nsIDOMWindow *win) { return _to Init(win); } \
  NS_SCRIPTABLE NS_IMETHOD NewURILoaded(void) { return _to NewURILoaded(); } \
  NS_SCRIPTABLE NS_IMETHOD GetVisualDebugging(PRBool *aVisualDebugging) { return _to GetVisualDebugging(aVisualDebugging); } \
  NS_SCRIPTABLE NS_IMETHOD SetVisualDebugging(PRBool aVisualDebugging) { return _to SetVisualDebugging(aVisualDebugging); } \
  NS_SCRIPTABLE NS_IMETHOD GetVisualEventDebugging(PRBool *aVisualEventDebugging) { return _to GetVisualEventDebugging(aVisualEventDebugging); } \
  NS_SCRIPTABLE NS_IMETHOD SetVisualEventDebugging(PRBool aVisualEventDebugging) { return _to SetVisualEventDebugging(aVisualEventDebugging); } \
  NS_SCRIPTABLE NS_IMETHOD GetPaintFlashing(PRBool *aPaintFlashing) { return _to GetPaintFlashing(aPaintFlashing); } \
  NS_SCRIPTABLE NS_IMETHOD SetPaintFlashing(PRBool aPaintFlashing) { return _to SetPaintFlashing(aPaintFlashing); } \
  NS_SCRIPTABLE NS_IMETHOD GetPaintDumping(PRBool *aPaintDumping) { return _to GetPaintDumping(aPaintDumping); } \
  NS_SCRIPTABLE NS_IMETHOD SetPaintDumping(PRBool aPaintDumping) { return _to SetPaintDumping(aPaintDumping); } \
  NS_SCRIPTABLE NS_IMETHOD GetInvalidateDumping(PRBool *aInvalidateDumping) { return _to GetInvalidateDumping(aInvalidateDumping); } \
  NS_SCRIPTABLE NS_IMETHOD SetInvalidateDumping(PRBool aInvalidateDumping) { return _to SetInvalidateDumping(aInvalidateDumping); } \
  NS_SCRIPTABLE NS_IMETHOD GetEventDumping(PRBool *aEventDumping) { return _to GetEventDumping(aEventDumping); } \
  NS_SCRIPTABLE NS_IMETHOD SetEventDumping(PRBool aEventDumping) { return _to SetEventDumping(aEventDumping); } \
  NS_SCRIPTABLE NS_IMETHOD GetMotionEventDumping(PRBool *aMotionEventDumping) { return _to GetMotionEventDumping(aMotionEventDumping); } \
  NS_SCRIPTABLE NS_IMETHOD SetMotionEventDumping(PRBool aMotionEventDumping) { return _to SetMotionEventDumping(aMotionEventDumping); } \
  NS_SCRIPTABLE NS_IMETHOD GetCrossingEventDumping(PRBool *aCrossingEventDumping) { return _to GetCrossingEventDumping(aCrossingEventDumping); } \
  NS_SCRIPTABLE NS_IMETHOD SetCrossingEventDumping(PRBool aCrossingEventDumping) { return _to SetCrossingEventDumping(aCrossingEventDumping); } \
  NS_SCRIPTABLE NS_IMETHOD GetReflowCounts(PRBool *aReflowCounts) { return _to GetReflowCounts(aReflowCounts); } \
  NS_SCRIPTABLE NS_IMETHOD SetReflowCounts(PRBool aReflowCounts) { return _to SetReflowCounts(aReflowCounts); } \
  NS_SCRIPTABLE NS_IMETHOD DumpWebShells(void) { return _to DumpWebShells(); } \
  NS_SCRIPTABLE NS_IMETHOD DumpContent(void) { return _to DumpContent(); } \
  NS_SCRIPTABLE NS_IMETHOD DumpFrames(void) { return _to DumpFrames(); } \
  NS_SCRIPTABLE NS_IMETHOD DumpViews(void) { return _to DumpViews(); } \
  NS_SCRIPTABLE NS_IMETHOD DumpStyleSheets(void) { return _to DumpStyleSheets(); } \
  NS_SCRIPTABLE NS_IMETHOD DumpStyleContexts(void) { return _to DumpStyleContexts(); } \
  NS_SCRIPTABLE NS_IMETHOD DumpReflowStats(void) { return _to DumpReflowStats(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSILAYOUTDEBUGGINGTOOLS(_to) \
  NS_SCRIPTABLE NS_IMETHOD Init(nsIDOMWindow *win) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(win); } \
  NS_SCRIPTABLE NS_IMETHOD NewURILoaded(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->NewURILoaded(); } \
  NS_SCRIPTABLE NS_IMETHOD GetVisualDebugging(PRBool *aVisualDebugging) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetVisualDebugging(aVisualDebugging); } \
  NS_SCRIPTABLE NS_IMETHOD SetVisualDebugging(PRBool aVisualDebugging) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetVisualDebugging(aVisualDebugging); } \
  NS_SCRIPTABLE NS_IMETHOD GetVisualEventDebugging(PRBool *aVisualEventDebugging) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetVisualEventDebugging(aVisualEventDebugging); } \
  NS_SCRIPTABLE NS_IMETHOD SetVisualEventDebugging(PRBool aVisualEventDebugging) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetVisualEventDebugging(aVisualEventDebugging); } \
  NS_SCRIPTABLE NS_IMETHOD GetPaintFlashing(PRBool *aPaintFlashing) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPaintFlashing(aPaintFlashing); } \
  NS_SCRIPTABLE NS_IMETHOD SetPaintFlashing(PRBool aPaintFlashing) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPaintFlashing(aPaintFlashing); } \
  NS_SCRIPTABLE NS_IMETHOD GetPaintDumping(PRBool *aPaintDumping) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPaintDumping(aPaintDumping); } \
  NS_SCRIPTABLE NS_IMETHOD SetPaintDumping(PRBool aPaintDumping) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPaintDumping(aPaintDumping); } \
  NS_SCRIPTABLE NS_IMETHOD GetInvalidateDumping(PRBool *aInvalidateDumping) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetInvalidateDumping(aInvalidateDumping); } \
  NS_SCRIPTABLE NS_IMETHOD SetInvalidateDumping(PRBool aInvalidateDumping) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetInvalidateDumping(aInvalidateDumping); } \
  NS_SCRIPTABLE NS_IMETHOD GetEventDumping(PRBool *aEventDumping) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetEventDumping(aEventDumping); } \
  NS_SCRIPTABLE NS_IMETHOD SetEventDumping(PRBool aEventDumping) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetEventDumping(aEventDumping); } \
  NS_SCRIPTABLE NS_IMETHOD GetMotionEventDumping(PRBool *aMotionEventDumping) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMotionEventDumping(aMotionEventDumping); } \
  NS_SCRIPTABLE NS_IMETHOD SetMotionEventDumping(PRBool aMotionEventDumping) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetMotionEventDumping(aMotionEventDumping); } \
  NS_SCRIPTABLE NS_IMETHOD GetCrossingEventDumping(PRBool *aCrossingEventDumping) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCrossingEventDumping(aCrossingEventDumping); } \
  NS_SCRIPTABLE NS_IMETHOD SetCrossingEventDumping(PRBool aCrossingEventDumping) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetCrossingEventDumping(aCrossingEventDumping); } \
  NS_SCRIPTABLE NS_IMETHOD GetReflowCounts(PRBool *aReflowCounts) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetReflowCounts(aReflowCounts); } \
  NS_SCRIPTABLE NS_IMETHOD SetReflowCounts(PRBool aReflowCounts) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetReflowCounts(aReflowCounts); } \
  NS_SCRIPTABLE NS_IMETHOD DumpWebShells(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->DumpWebShells(); } \
  NS_SCRIPTABLE NS_IMETHOD DumpContent(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->DumpContent(); } \
  NS_SCRIPTABLE NS_IMETHOD DumpFrames(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->DumpFrames(); } \
  NS_SCRIPTABLE NS_IMETHOD DumpViews(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->DumpViews(); } \
  NS_SCRIPTABLE NS_IMETHOD DumpStyleSheets(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->DumpStyleSheets(); } \
  NS_SCRIPTABLE NS_IMETHOD DumpStyleContexts(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->DumpStyleContexts(); } \
  NS_SCRIPTABLE NS_IMETHOD DumpReflowStats(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->DumpReflowStats(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsLayoutDebuggingTools : public nsILayoutDebuggingTools
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSILAYOUTDEBUGGINGTOOLS

  nsLayoutDebuggingTools();

private:
  ~nsLayoutDebuggingTools();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsLayoutDebuggingTools, nsILayoutDebuggingTools)

nsLayoutDebuggingTools::nsLayoutDebuggingTools()
{
  /* member initializers and constructor code */
}

nsLayoutDebuggingTools::~nsLayoutDebuggingTools()
{
  /* destructor code */
}

/* void init (in nsIDOMWindow win); */
NS_IMETHODIMP nsLayoutDebuggingTools::Init(nsIDOMWindow *win)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void newURILoaded (); */
NS_IMETHODIMP nsLayoutDebuggingTools::NewURILoaded()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean visualDebugging; */
NS_IMETHODIMP nsLayoutDebuggingTools::GetVisualDebugging(PRBool *aVisualDebugging)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsLayoutDebuggingTools::SetVisualDebugging(PRBool aVisualDebugging)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean visualEventDebugging; */
NS_IMETHODIMP nsLayoutDebuggingTools::GetVisualEventDebugging(PRBool *aVisualEventDebugging)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsLayoutDebuggingTools::SetVisualEventDebugging(PRBool aVisualEventDebugging)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean paintFlashing; */
NS_IMETHODIMP nsLayoutDebuggingTools::GetPaintFlashing(PRBool *aPaintFlashing)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsLayoutDebuggingTools::SetPaintFlashing(PRBool aPaintFlashing)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean paintDumping; */
NS_IMETHODIMP nsLayoutDebuggingTools::GetPaintDumping(PRBool *aPaintDumping)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsLayoutDebuggingTools::SetPaintDumping(PRBool aPaintDumping)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean invalidateDumping; */
NS_IMETHODIMP nsLayoutDebuggingTools::GetInvalidateDumping(PRBool *aInvalidateDumping)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsLayoutDebuggingTools::SetInvalidateDumping(PRBool aInvalidateDumping)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean eventDumping; */
NS_IMETHODIMP nsLayoutDebuggingTools::GetEventDumping(PRBool *aEventDumping)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsLayoutDebuggingTools::SetEventDumping(PRBool aEventDumping)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean motionEventDumping; */
NS_IMETHODIMP nsLayoutDebuggingTools::GetMotionEventDumping(PRBool *aMotionEventDumping)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsLayoutDebuggingTools::SetMotionEventDumping(PRBool aMotionEventDumping)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean crossingEventDumping; */
NS_IMETHODIMP nsLayoutDebuggingTools::GetCrossingEventDumping(PRBool *aCrossingEventDumping)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsLayoutDebuggingTools::SetCrossingEventDumping(PRBool aCrossingEventDumping)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean reflowCounts; */
NS_IMETHODIMP nsLayoutDebuggingTools::GetReflowCounts(PRBool *aReflowCounts)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsLayoutDebuggingTools::SetReflowCounts(PRBool aReflowCounts)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void dumpWebShells (); */
NS_IMETHODIMP nsLayoutDebuggingTools::DumpWebShells()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void dumpContent (); */
NS_IMETHODIMP nsLayoutDebuggingTools::DumpContent()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void dumpFrames (); */
NS_IMETHODIMP nsLayoutDebuggingTools::DumpFrames()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void dumpViews (); */
NS_IMETHODIMP nsLayoutDebuggingTools::DumpViews()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void dumpStyleSheets (); */
NS_IMETHODIMP nsLayoutDebuggingTools::DumpStyleSheets()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void dumpStyleContexts (); */
NS_IMETHODIMP nsLayoutDebuggingTools::DumpStyleContexts()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void dumpReflowStats (); */
NS_IMETHODIMP nsLayoutDebuggingTools::DumpReflowStats()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsILayoutDebuggingTools_h__ */
