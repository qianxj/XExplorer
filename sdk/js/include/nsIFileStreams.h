/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/netwerk/base/public/nsIFileStreams.idl
 */

#ifndef __gen_nsIFileStreams_h__
#define __gen_nsIFileStreams_h__


#ifndef __gen_nsIInputStream_h__
#include "nsIInputStream.h"
#endif

#ifndef __gen_nsIOutputStream_h__
#include "nsIOutputStream.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIFile; /* forward declaration */


/* starting interface:    nsIFileInputStream */
#define NS_IFILEINPUTSTREAM_IID_STR "e3d56a20-c7ec-11d3-8cda-0060b0fc14a3"

#define NS_IFILEINPUTSTREAM_IID \
  {0xe3d56a20, 0xc7ec, 0x11d3, \
    { 0x8c, 0xda, 0x00, 0x60, 0xb0, 0xfc, 0x14, 0xa3 }}

/**
 * An input stream that allows you to read from a file.
 */
class NS_NO_VTABLE NS_SCRIPTABLE nsIFileInputStream : public nsIInputStream {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IFILEINPUTSTREAM_IID)

  /**
     * @param file          file to read from (must QI to nsILocalFile)
     * @param ioFlags       file open flags listed in prio.h (see
     *                      PR_Open documentation) or -1 to open the
     *                      file in default mode (PR_RDONLY).
     * @param perm          file mode bits listed in prio.h or -1 to
     *                      use the default value (0)
     * @param behaviorFlags flags specifying various behaviors of the class
     *        (see enumerations in the class)
     */
  /* void init (in nsIFile file, in long ioFlags, in long perm, in long behaviorFlags); */
  NS_SCRIPTABLE NS_IMETHOD Init(nsIFile *file, PRInt32 ioFlags, PRInt32 perm, PRInt32 behaviorFlags) = 0;

  /**
     * If this is set, the file will be deleted by the time the stream is
     * closed.  It may be removed before the stream is closed if it is possible
     * to delete it and still read from it.
     *
     * If OPEN_ON_READ is defined, and the file was recreated after the first
     * delete, the file will be deleted again when it is closed again.
     */
  enum { DELETE_ON_CLOSE = 2 };

  /**
     * If this is set, the file will close automatically when the end of the
     * file is reached.
     */
  enum { CLOSE_ON_EOF = 4 };

  /**
     * If this is set, the file will be reopened whenever Seek(0) occurs.  If
     * the file is already open and the seek occurs, it will happen naturally.
     * (The file will only be reopened if it is closed for some reason.)
     */
  enum { REOPEN_ON_REWIND = 8 };

  /**
     * If this is set, the file will be opened (i.e., a call to
     * PR_Open done) only when we do an actual operation on the stream,
     * or more specifically, when one of the following is called:
     *   - Seek
     *   - Tell
     *   - Available
     *   - Read
     *   - ReadLine
     *
     * DEFER_OPEN is useful if we use the stream on a background
     * thread, so that the opening and possible |stat|ing of the file
     * happens there as well.
     *
     * @note Using this flag results in the file not being opened
     *       during the call to Init.  This means that any errors that might
     *       happen when this flag is not set would happen during the
     *       first read.  Also, the file is not locked when Init is called,
     *       so it might be deleted before we try to read from it.
     */
  enum { DEFER_OPEN = 16 };

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIFileInputStream, NS_IFILEINPUTSTREAM_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIFILEINPUTSTREAM \
  NS_SCRIPTABLE NS_IMETHOD Init(nsIFile *file, PRInt32 ioFlags, PRInt32 perm, PRInt32 behaviorFlags); \

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIFILEINPUTSTREAM(_to) \
  NS_SCRIPTABLE NS_IMETHOD Init(nsIFile *file, PRInt32 ioFlags, PRInt32 perm, PRInt32 behaviorFlags) { return _to Init(file, ioFlags, perm, behaviorFlags); } \

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIFILEINPUTSTREAM(_to) \
  NS_SCRIPTABLE NS_IMETHOD Init(nsIFile *file, PRInt32 ioFlags, PRInt32 perm, PRInt32 behaviorFlags) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(file, ioFlags, perm, behaviorFlags); } \

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsFileInputStream : public nsIFileInputStream
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIFILEINPUTSTREAM

  nsFileInputStream();

private:
  ~nsFileInputStream();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsFileInputStream, nsIFileInputStream)

nsFileInputStream::nsFileInputStream()
{
  /* member initializers and constructor code */
}

nsFileInputStream::~nsFileInputStream()
{
  /* destructor code */
}

/* void init (in nsIFile file, in long ioFlags, in long perm, in long behaviorFlags); */
NS_IMETHODIMP nsFileInputStream::Init(nsIFile *file, PRInt32 ioFlags, PRInt32 perm, PRInt32 behaviorFlags)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIFileOutputStream */
#define NS_IFILEOUTPUTSTREAM_IID_STR "e6f68040-c7ec-11d3-8cda-0060b0fc14a3"

#define NS_IFILEOUTPUTSTREAM_IID \
  {0xe6f68040, 0xc7ec, 0x11d3, \
    { 0x8c, 0xda, 0x00, 0x60, 0xb0, 0xfc, 0x14, 0xa3 }}

/**
 * An output stream that lets you stream to a file.
 */
class NS_NO_VTABLE NS_SCRIPTABLE nsIFileOutputStream : public nsIOutputStream {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IFILEOUTPUTSTREAM_IID)

  /**
     * @param file          file to write to (must QI to nsILocalFile)
     * @param ioFlags       file open flags listed in prio.h (see
     *                      PR_Open documentation) or -1 to open the
     *                      file in default mode (PR_WRONLY |
     *                      PR_CREATE_FILE | PR_TRUNCATE)
     * @param perm          file mode bits listed in prio.h or -1 to
     *                      use the default permissions (0664)
     * @param behaviorFlags flags specifying various behaviors of the class
     *        (currently none supported)
     */
  /* void init (in nsIFile file, in long ioFlags, in long perm, in long behaviorFlags); */
  NS_SCRIPTABLE NS_IMETHOD Init(nsIFile *file, PRInt32 ioFlags, PRInt32 perm, PRInt32 behaviorFlags) = 0;

  /**
     * See the same constant in nsIFileInputStream. The deferred open will
     * be performed when one of the following is called:
     *   - Seek
     *   - Tell
     *   - Write
     *   - Flush
     *
     * @note Using this flag results in the file not being opened
     *       during the call to Init.  This means that any errors that might
     *       happen when this flag is not set would happen during the
     *       first write, and if the file is to be created, then it will not
     *       appear on the disk until the first write.
     */
  enum { DEFER_OPEN = 1 };

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIFileOutputStream, NS_IFILEOUTPUTSTREAM_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIFILEOUTPUTSTREAM \
  NS_SCRIPTABLE NS_IMETHOD Init(nsIFile *file, PRInt32 ioFlags, PRInt32 perm, PRInt32 behaviorFlags); \

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIFILEOUTPUTSTREAM(_to) \
  NS_SCRIPTABLE NS_IMETHOD Init(nsIFile *file, PRInt32 ioFlags, PRInt32 perm, PRInt32 behaviorFlags) { return _to Init(file, ioFlags, perm, behaviorFlags); } \

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIFILEOUTPUTSTREAM(_to) \
  NS_SCRIPTABLE NS_IMETHOD Init(nsIFile *file, PRInt32 ioFlags, PRInt32 perm, PRInt32 behaviorFlags) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(file, ioFlags, perm, behaviorFlags); } \

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsFileOutputStream : public nsIFileOutputStream
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIFILEOUTPUTSTREAM

  nsFileOutputStream();

private:
  ~nsFileOutputStream();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsFileOutputStream, nsIFileOutputStream)

nsFileOutputStream::nsFileOutputStream()
{
  /* member initializers and constructor code */
}

nsFileOutputStream::~nsFileOutputStream()
{
  /* destructor code */
}

/* void init (in nsIFile file, in long ioFlags, in long perm, in long behaviorFlags); */
NS_IMETHODIMP nsFileOutputStream::Init(nsIFile *file, PRInt32 ioFlags, PRInt32 perm, PRInt32 behaviorFlags)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIPartialFileInputStream */
#define NS_IPARTIALFILEINPUTSTREAM_IID_STR "3ce03a2f-97f7-4375-b6bb-1788a60cad3b"

#define NS_IPARTIALFILEINPUTSTREAM_IID \
  {0x3ce03a2f, 0x97f7, 0x4375, \
    { 0xb6, 0xbb, 0x17, 0x88, 0xa6, 0x0c, 0xad, 0x3b }}

/**
 * An input stream that allows you to read from a slice of a file.
 */
class NS_NO_VTABLE NS_SCRIPTABLE nsIPartialFileInputStream : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IPARTIALFILEINPUTSTREAM_IID)

  /**
     * Initialize with a file and new start/end positions. Both start and
     * start+length must be smaller than the size of the file. Not doing so
     * will lead to undefined behavior.
     * You must initialize the stream, and only initialize it once, before it
     * can be used.
     * 
     * @param file          file to read from (must QI to nsILocalFile)
     * @param start         start offset of slice to read. Must be smaller
     *                      than the size of the file.
     * @param length        length of slice to read. Must be small enough that
     *                      start+length is smaller than the size of the file.
     * @param ioFlags       file open flags listed in prio.h (see
     *                      PR_Open documentation) or -1 to open the
     *                      file in default mode (PR_RDONLY).
     * @param perm          file mode bits listed in prio.h or -1 to
     *                      use the default value (0)
     * @param behaviorFlags flags specifying various behaviors of the class
     *        (see enumerations in nsIFileInputStream)
     */
  /* void init (in nsIFile file, in unsigned long long start, in unsigned long long length, in long ioFlags, in long perm, in long behaviorFlags); */
  NS_SCRIPTABLE NS_IMETHOD Init(nsIFile *file, PRUint64 start, PRUint64 length, PRInt32 ioFlags, PRInt32 perm, PRInt32 behaviorFlags) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIPartialFileInputStream, NS_IPARTIALFILEINPUTSTREAM_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIPARTIALFILEINPUTSTREAM \
  NS_SCRIPTABLE NS_IMETHOD Init(nsIFile *file, PRUint64 start, PRUint64 length, PRInt32 ioFlags, PRInt32 perm, PRInt32 behaviorFlags); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIPARTIALFILEINPUTSTREAM(_to) \
  NS_SCRIPTABLE NS_IMETHOD Init(nsIFile *file, PRUint64 start, PRUint64 length, PRInt32 ioFlags, PRInt32 perm, PRInt32 behaviorFlags) { return _to Init(file, start, length, ioFlags, perm, behaviorFlags); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIPARTIALFILEINPUTSTREAM(_to) \
  NS_SCRIPTABLE NS_IMETHOD Init(nsIFile *file, PRUint64 start, PRUint64 length, PRInt32 ioFlags, PRInt32 perm, PRInt32 behaviorFlags) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(file, start, length, ioFlags, perm, behaviorFlags); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsPartialFileInputStream : public nsIPartialFileInputStream
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIPARTIALFILEINPUTSTREAM

  nsPartialFileInputStream();

private:
  ~nsPartialFileInputStream();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsPartialFileInputStream, nsIPartialFileInputStream)

nsPartialFileInputStream::nsPartialFileInputStream()
{
  /* member initializers and constructor code */
}

nsPartialFileInputStream::~nsPartialFileInputStream()
{
  /* destructor code */
}

/* void init (in nsIFile file, in unsigned long long start, in unsigned long long length, in long ioFlags, in long perm, in long behaviorFlags); */
NS_IMETHODIMP nsPartialFileInputStream::Init(nsIFile *file, PRUint64 start, PRUint64 length, PRInt32 ioFlags, PRInt32 perm, PRInt32 behaviorFlags)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIFileStreams_h__ */
