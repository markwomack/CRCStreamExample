//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef CRCSTREAM_H
#define CRCSTREAM_H

// Arduino includes
#include <Stream.h>

// Local includes
extern "C" {
  #include "crc32.h"
}

class CRCStream : public Stream {
  public:
    CRCStream(Stream* stream, uint32_t expectedSize, uint32_t expectedCRC) {
      _stream = stream;
      _expectedSize = expectedSize;
      _expectedCRC = expectedCRC;
      _currentSize = 0;
      _currentCRC = 0;
    };

    int available() {
      return _stream->available();
    };
    
    int read() {
      int data = _stream->read();
      
      if (data != -1) {
        crc32((uint8_t*)&data, 1, &_currentCRC);
        _currentSize++;
      }
      
      return data;
    };
    
    int peek() {
      return _stream->peek();
    };

    size_t write(uint8_t b) {
      return _stream->write(b);
    };
    
    bool sizeAndCRCMatch() {
      return (_expectedSize == _currentSize) && (_expectedCRC == _currentCRC);
    };

    uint32_t getExpectedSize() {
      return _expectedSize;
    };

    uint32_t getCurrentSize() {
      return _currentSize;
    };

    uint32_t getExpectedCRC() {
      return _expectedCRC;
    };

    uint32_t getCurrentCRC() {
      return _currentCRC;
    };
    
  private:
    Stream* _stream;
    uint32_t _expectedSize;
    uint32_t _currentSize;
    uint32_t _expectedCRC;
    uint32_t _currentCRC;
};
#endif // CRCSTREAM_H
