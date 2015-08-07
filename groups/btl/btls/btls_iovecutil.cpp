// btls_iovecutil.cpp                                                 -*-C++-*-
#include <btls_iovecutil.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(btls_iovecutil_cpp,"$Id$ $CSID$")

#include <bdlmca_blob.h>
#include <bdlmca_pooledblobbufferfactory.h>
#include <bslma_allocator.h>
#include <bslma_default.h>
#include <bsls_assert.h>
#include <bsls_types.h>

#include <bsl_algorithm.h>
#include <bsl_cstring.h>

namespace BloombergLP {

namespace btls {

namespace {

                         // ------------------------
                         // local function templates
                         // ------------------------

template <class IOVEC>
void genericAppendToBlob(bdlmca::Blob *blob,
                         const IOVEC  *vecs,
                         int           numVecs,
                         int           offset)
{
    BSLS_ASSERT(0 <= offset);
    BSLS_ASSERT(0 <  numVecs);

    // Set up loop invariant stated below.  Note that if blobLength is 0,
    // or if last data buffer is complete, the call 'blob->setLength(...)'
    // below will create additional buffers as needed, so that the call to
    // 'blob->buffer(currentBufIndex)' will always be legal.

    int currentBufIndex  = blob->numDataBuffers() - 1;
    int currentBufOffset = blob->lastDataBufferLength();
    if (currentBufIndex < 0 ||
        currentBufOffset == blob->buffer(currentBufIndex).size()) {

        // Blob is empty or last data buffer is complete: skip to next buffer.

        ++currentBufIndex;
        currentBufOffset = 0;
    }

    // Skip to 'offset' in 'vecs'.

    int currentVecIndex = 0;
    int currentVecAvailable = vecs[currentVecIndex].length();
    BSLS_ASSERT(0 < currentVecAvailable);

    int prefixLength = 0;
    while (prefixLength + currentVecAvailable <= offset) {
        prefixLength += currentVecAvailable;
        ++currentVecIndex;
        currentVecAvailable = vecs[currentVecIndex].length();
        BSLS_ASSERT(0 < currentVecAvailable);
    }
    int currentVecOffset = offset - prefixLength;
    currentVecAvailable -= currentVecOffset;
    BSLS_ASSERT(0 <= currentVecOffset);
    BSLS_ASSERT(0 < currentVecAvailable);

    // For simplicity,  finish computing the iovec lengths, and reserve blob's
    // length in a single setLength call.  Since prefixLength isn't used, we
    // reset it and use it for that computation.

    prefixLength = currentVecAvailable;
    for (int i = currentVecIndex+1; i < numVecs; ++i) {
        prefixLength += vecs[i].length();
    }
    blob->setLength(blob->length() + prefixLength);

    // Compute number of bytes available to read or write in current vec or
    // buffer.  This must be done *after* setting the blob length.

    int currentBufAvailable = blob->buffer(currentBufIndex).size();
    currentBufAvailable -= currentBufOffset;
    BSLS_ASSERT(0 < currentBufAvailable);

    // Append the iovecs as individual blob buffers, re-segmented to take
    // advantage of the factory's buffer size: this is a classic merge.

    while (1) {
        // Invariants:
        // 1. 0 <= currentVec < numVecs
        // 2. 0 <= currentVecOffset < vecs[currentVec].length()
        // 3. 0 <  currentVecAvailable
        // 4. 0 <= currentBuf < blobs.numDataBuffers()
        // 5. 0 <= currentBufOffset < blob->buffer(currentBuf).size()
        // 6. 0 <  currentBufAvailable

        int numBytesCopied = bsl::min(currentVecAvailable,
                                      currentBufAvailable);

        bsl::memcpy(blob->buffer(currentBufIndex).data() + currentBufOffset,
                    static_cast<const char *>(vecs[currentVecIndex].buffer())
                             + currentVecOffset,
                    numBytesCopied);

        currentBufOffset += numBytesCopied;
        currentVecOffset += numBytesCopied;

        if (currentVecAvailable == numBytesCopied) {
            currentVecOffset = 0;
            if (++currentVecIndex == numVecs) {
                return;                                               // RETURN
            }
            currentVecAvailable = vecs[currentVecIndex].length();
        } else {
            currentVecAvailable -= numBytesCopied;
        }

        if (currentBufAvailable == numBytesCopied) {
            currentBufOffset = 0;
            ++currentBufIndex;
            currentBufAvailable = blob->buffer(currentBufIndex).size();
        } else {
            currentBufAvailable -= numBytesCopied;
        }
    }
}

template <class IOVEC>
int genericGather(char        *buffer,
                  int          length,
                  const IOVEC *buffers,
                  int          numBuffers)
{
    BSLS_ASSERT(buffer);
    BSLS_ASSERT(0 <= length);
    BSLS_ASSERT(buffers);
    BSLS_ASSERT(0 <= numBuffers);

    int numCopied = 0;
    while(numBuffers && 0 < length) {
        if (buffers->length() < length) {
            bsl::memcpy(buffer,
                        static_cast<const char *>(buffers->buffer()),
                        buffers->length());
            numCopied += buffers->length();
            length -= buffers->length();
            buffer += buffers->length();
            ++buffers;
        }
        else {
            bsl::memcpy(buffer,
                        static_cast<const char *>(buffers->buffer()),
                        length);
            numCopied += length;
            length = 0;
        }
        --numBuffers;
    }
    return numCopied;
}

template <class IOVEC>
int genericLength(const IOVEC *buffers, int numBuffers)
{
    BSLS_ASSERT(buffers);
    BSLS_ASSERT(0 <= numBuffers);

    int length = 0;
    while (numBuffers) {
        length += buffers->length();
        ++buffers;
        --numBuffers;
    }
    return length;
}

template <class IOVEC>
void genericPivot(int         *bufferIdx,
                  int         *offset,
                  const IOVEC *buffers,
                  int          numBuffers,
                  int          position)
{
    BSLS_ASSERT(bufferIdx);
    BSLS_ASSERT(offset);
    BSLS_ASSERT(buffers);
    BSLS_ASSERT(0 <= numBuffers);
    BSLS_ASSERT(0 <= position);

    for (int i = 0; i < numBuffers; ++i) {
        if (position < buffers[i].length()) {
            *bufferIdx = i;
            *offset    = position;
            return;                                                   // RETURN
        }
        position -= buffers[i].length();
    }

    *bufferIdx = numBuffers;
    return;
}

template <class IOVEC>
int genericScatter(const IOVEC *buffers,
                   int          numBuffers,
                   const char  *buffer,
                   int          length)
{
    BSLS_ASSERT(buffers);
    BSLS_ASSERT(0 <= numBuffers);
    BSLS_ASSERT(buffer);
    BSLS_ASSERT(0 <= length);

    int numCopied = 0;
    while(numBuffers && 0 < length) {
        if (buffers->length() < length) {
            bsl::memcpy(static_cast<char *>(
                                        const_cast<void *>(buffers->buffer())),
                        buffer,
                        buffers->length());
            numCopied += buffers->length();
            length    -= buffers->length();
            buffer    += buffers->length();
            ++buffers;
        }
        else {
            bsl::memcpy(static_cast<char *>(
                                        const_cast<void *>(buffers->buffer())),
                        buffer,
                        length);
            numCopied += length;
            length = 0;
        }
        --numBuffers;
    }
    return numCopied;
}

}  // close unnamed namespace

                         // ---------------
                         // class IovecUtil
                         // ---------------

void IovecUtil::appendToBlob(bdlmca::Blob *blob,
                             const Iovec  *vecs,
                             int           numVecs,
                             int           offset)
{
    genericAppendToBlob(blob, vecs, numVecs, offset);
}

void IovecUtil::appendToBlob(bdlmca::Blob *blob,
                             const Ovec   *vecs,
                             int           numVecs,
                             int           offset)
{
    genericAppendToBlob(blob, vecs, numVecs, offset);
}

bdlmca::Blob *IovecUtil::blob(const Iovec               *vecs,
                              int                        numVecs,
                              int                        offset,
                              bdlmca::BlobBufferFactory *factory,
                              bslma::Allocator          *basicAllocator)
{
    bslma::Allocator *allocator = bslma::Default::allocator(basicAllocator);
    bdlmca::Blob     *blob = new (*allocator) bdlmca::Blob(factory, allocator);

    appendToBlob(blob, vecs, numVecs, offset);
    return blob;
}

int IovecUtil::gather(char        *buffer,
                      int          length,
                      const Iovec *buffers,
                      int          numBuffers)
{
    return genericGather(buffer, length, buffers, numBuffers);
}

int IovecUtil::gather(char       *buffer,
                      int         length,
                      const Ovec *buffers,
                      int         numBuffers)
{
    return genericGather(buffer, length, buffers, numBuffers);
}

int IovecUtil::length(const Iovec *buffers, int numBuffers)
{
    return genericLength(buffers, numBuffers);
}

int IovecUtil::length(const Ovec *buffers, int numBuffers)
{
    return genericLength(buffers, numBuffers);
}

void IovecUtil::pivot(int         *bufferIdx,
                      int         *offset,
                      const Iovec *buffers,
                      int          numBuffers,
                      int          length)
{
    genericPivot(bufferIdx, offset, buffers, numBuffers, length);
}

void IovecUtil::pivot(int        *bufferIdx,
                      int        *offset,
                      const Ovec *buffers,
                      int         numBuffers,
                      int         length)
{
    genericPivot(bufferIdx, offset, buffers, numBuffers, length);
}

int IovecUtil::scatter(const Iovec *buffers,
                       int          numBuffers,
                       const char  *buffer,
                       int          length)
{
    return genericScatter(buffers, numBuffers, buffer, length);
}

int IovecUtil::scatter(const Ovec *buffers,
                       int         numBuffers,
                       const char *buffer,
                       int         length)
{
    return genericScatter(buffers, numBuffers, buffer, length);
}

}  // close package namespace

}  // close namespace BloombergLP

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2015
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
