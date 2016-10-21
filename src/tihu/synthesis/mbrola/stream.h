/*******************************************************************************
* This file is part of Tihu.
*
* Tihu is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Tihu is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Tihu.  If not, see <http://www.gnu.org/licenses/>.
*
* Contributor(s):
*    Mostafa Sedaghat Joo (mostafa.sedaghat@gmail.com)
*
*******************************************************************************/
#ifndef __TIHU__MBROLA__STREAM_H
#define __TIHU__MBROLA__STREAM_H

#include <malloc.h>

//const static long dyn_buffer_step = 1240; // initial size of array memory (elements)
//const static long dyn_buffer_mult = 2; // multiplier (enlarge array memory

/**
\brief CStream template based class

A Template base class to create and manipulate the a dynamic buffer.

*/
template <class T>
class CStream
{
public:
    /// <summary>
    /// Default constructor. Construct the CStream object.
    /// </summary>
    CStream();

    /// <summary>
    /// Copy constructor
    /// </summary>
    CStream(const CStream &stream);

    /// <summary>
    /// Default destructor. Destroy the CStream object
    /// </summary>
    virtual ~CStream();

    CStream<T> &operator = (const CStream &stream);

    T &operator [](unsigned long pos);


    /**
    *\brief Put data in buffer
    *
    * Put the inData pointer to buffer and increase the buffer length same as dataSize
    */
    void Push(T* inData, unsigned long dataSize);

    /// <summary>
    /// Add item to the end of array
    /// </summary>
    ///
    ////////
    void Add(const T &el);

    /**
    *\brief Get data from buffer
    *
    * Fill the outData pointer from begining of buffer and
    * decrease the buffer length same as dataSize
    */
    long Pop(T* outData, unsigned long dataSize);

    /// <summary>
    /// Clear the buffer and set buffer size to zero
    /// </summary>
    void Clear();

    void Delete(unsigned long pos);

    /// <summary>
    /// Get the number of stored elements
    /// </summary>
    /// <return>
    /// The buffer length
    /// </return>
    long GetSize()
    {
        return m_size;
    }

    void SetSize(unsigned long newSize);

    /// <summary>
    /// Get the size of buffer in BYTEs
    /// </summary>
    /// <return>
    /// The number of BYTEs stored in stream buffer
    /// </return>
    long GetSizeInBYTE()
    {
        sizeof(T)*(m_size);
    }

    /// <summary>
    /// Get a const pointer to first block of buffer
    /// </summary>
    /// <return>
    /// The buffer pointer
    /// </return>
    const T* GetBuffer()
    {
        return (const T*)uffer;
    }

    /**
    *\brief Get the buffer pointer
    *
    * Get a pointer to first block of buffer
    */
    //T* GetBuffer() { return uffer; }


    /// <summary>
    ///
    /// </summary>
    enum exception { MEMFAIL }; // exception enum

private:
    /// <summary>
    /// stores pointer to the stream buffer
    /// </summary>
    T* uffer;

    /// <summary>
    /// number of elements in stream buffer
    /// </summary>
    unsigned long m_size;

    /// <summary>
    /// actual size of allocated memory
    /// </summary>
    unsigned long m_realSize;
};

//-------------------------------------------------------------------
// Construction/Destruction
//-------------------------------------------------------------------

template <class T>
CStream<T>::CStream()
{
    m_realSize = dyn_buffer_step; // First, allocate step
    // for dyn_array_step items
    m_size = 0;
    uffer = (T*)malloc(m_realSize*sizeof(T));

    if(uffer == NULL) {
        throw MEMFAIL;
    }
}

template <class T>
CStream<T>::~CStream()
{
    if(uffer) {
        free(uffer);
        uffer = NULL;
    }
}

template <class T>
CStream<T>::CStream(const CStream &stream)
{
    uffer = (T*)malloc(sizeof(T)*stream.m_realSize);

    if(uffer == NULL) {
        throw MEMFAIL;
    }

    memcpy(uffer, stream.uffer, sizeof(T)*stream.m_realSize);

    m_realSize = stream.m_realSize;
    m_size = stream.m_size;
}


template <class T>
CStream<T> &CStream<T>::operator = (const CStream &stream)
{
    if(this == &stream) { // in case somebody tries assign array to itself
        return *this;
    }

    if(stream.m_size == 0) { // is other array is empty -- clear this array
        Clear();
    }

    SetSize(stream.m_size); // set size

    memcpy(uffer, stream.uffer , sizeof(T)*stream.m_size);

    return *this;
}


template <class T>
void CStream<T>::SetSize(unsigned long newSize)
{
    m_size = newSize;

    if(m_size != 0) {
        // change array memory size
        // if new size is larger than current
        // or new size is less then half of the current
        if((m_size > m_realSize) || (m_size < m_realSize/2)) {
            m_realSize = m_size;

            uffer = (T*)realloc(uffer, sizeof(T)*m_size);

            if(uffer == NULL) {
                throw MEMFAIL;
            }
        }
    } else {
        Clear();
    }
}

template <class T>
void CStream<T>::Delete(unsigned long pos)
{
    if(m_size == 1) { // If array has only one element
        Clear();    // than we clear it, since it will be deleted
    } else {
        // otherwise, shift array elements
        for(unsigned long i=pos; i<m_size-1; i++) {
            uffer[i] = uffer[i+1];
        }

        // decrease array size
        m_size--;
    }
}

template <class T>
T &CStream<T>::operator [](unsigned long pos)
{
    return uffer[pos]; // return array element
}

//-------------------------------------------------------------------
//Name : put
//Desc : puts number of items into stream buffer
//       dataSize must be number of inData types
//-------------------------------------------------------------------
template <class T>
void CStream<T>::Push(T* inData, unsigned long dataSize)
{
    if(dataSize == 0 || inData == NULL) {
        return;
    }

    // incease the size of allocated memore
    if(m_size + dataSize > m_realSize) {
        m_realSize = (dataSize + m_size) * dyn_buffer_mult;

        uffer = (T*)realloc(uffer, sizeof(T)*m_realSize);

        if(uffer == NULL) {
            throw MEMFAIL;
        }
    }

#ifndef USE_MEMCPY
    for(UINT ii =0 ; ii< dataSize; ii++) {
        *(uffer + m_size + ii) = inData[ii];
    }
#else
    memcpy((T*)(uffer + m_size), inData, dataSize * sizeof(T));
#endif

    m_size += dataSize;

    /*
    if(m_size == 0) //create new memory block
    {
        uffer = (T *)malloc(dataSize * sizeof(T));
        if(uffer != NULL)
        {
            m_size = dataSize;
            memcpy((T *)uffer,(T *)inData,dataSize * sizeof(T));    //simply copy the block into stream
        }
    }
    else  // expand previously created block
    {
        T* backupBuf = (T *)malloc(m_size * sizeof(T));
        if(backupBuf != NULL)
        {
            memcpy((T *)backupBuf,(T *)uffer, m_size * sizeof(T));
            uffer = (T *)realloc(uffer,(dataSize + m_size) * sizeof(T));
            if(uffer != NULL)
            {
                memcpy((T *)uffer,(T *)backupBuf , m_size * sizeof(T)); // put back previous data
                memcpy((T *)(uffer + m_size),(T *)inData,dataSize * sizeof(T)); // append new data at the end
                m_size += dataSize;
            }
            free(backupBuf);
        }
    }*/
}

template <class T>
void CStream<T>::Add(const T &el)
{
    if(m_size + 1> m_realSize) {
        m_realSize *= dyn_buffer_mult;

        uffer = (T*)realloc(uffer, sizeof(T)*m_realSize);

        if(uffer == NULL) {
            throw MEMFAIL;
        }
    }

    m_size++; // ensure that the block is re-alloced, then increase the buffer size
    uffer[m_size-1] = el;
}

//---------------------------------------------------------------------
//Name : get
//Desc : receives number of items from the begining of the stream
//       and deletes items that has been read
//       dataSize must be number of outData types
//       returns number of items actually read , if 0,there is no more
//---------------------------------------------------------------------
template <class T>
long CStream<T>::Pop(T* outData, unsigned long dataSize)
{
    if(m_size == 0 || dataSize == 0 || outData == NULL) {
        return 0;
    }

    long itemsGet;

    // control and fix dataSize parameter and check if it's more than stream size
    if(dataSize > m_size) {
        itemsGet = m_size;
    } else {
        itemsGet = dataSize;
    }

    //memcpy((T *)outData,(T *)uffer,itemsGet);
    memcpy((T*)outData,(T*)uffer, itemsGet * sizeof(T));

    // now delete from the begining of the buffer
    long remain = m_size - itemsGet;
    if(remain <= 0) { // there is no remaining buffer , free it
        Clear();
        return itemsGet;
    }


    //memmove()

    T* newBuffer = (T*)malloc(m_realSize * sizeof(T));

    if(uffer == NULL) {
        throw MEMFAIL;
    }

    memcpy((T*)newBuffer, (T*)(uffer + itemsGet), remain * sizeof(T));

    m_size = remain;
    free(uffer);
    uffer = newBuffer;
    /*

    if(newBuffer != NULL)
    {
        memcpy((T *)backupBuf,(T *)(uffer + itemsGet),remain * sizeof(T));

        uffer = (T *)realloc(uffer,remain * sizeof(T));
        memcpy((T *)uffer,(T *)backupBuf,remain * sizeof(T));
        m_size -= itemsGet;

        free(backupBuf);
    }
    */

    return itemsGet;

}

//-------------------------------------------------------------------
//Name : release
//Desc : frees stream from memory
//-------------------------------------------------------------------
template <class T>
void CStream<T>::Clear()
{
    m_size = 0;
    // set initial memory size again
    m_realSize = dyn_buffer_step;

    uffer = (T*)realloc(uffer, sizeof(T)*m_realSize);

    if(uffer == NULL) {
        throw MEMFAIL;
    }
}

#endif
