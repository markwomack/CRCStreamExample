//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

// Arduino includes
#include <stdlib.h>

// My includes
#include <DebugMsgs.h>

// This class is not currently used in the code, but is
// included for historical purposes.

const uint32_t INITIAL_SIZE(1024);

class DynamicArray {
  public:
    DynamicArray() {
      _dataPtr = 0;
      reset();
    };

    void append(uint8_t inData) {
      if (_used == _size) {
        if (!reallocate()) {
          return;
        }
      }
      
      _dataPtr[_used++] = inData;
    };

    void append(uint8_t* inData, uint32_t size) {      
      while (_size < _used + size) {
        if (!reallocate()) {
          return;
        }
      }

      memcpy(_dataPtr + _used, inData, size);
      _used += size;
    }

    void reset() {
      if (_dataPtr) {
        free(_dataPtr);
        _dataPtr = 0;
      }
      reallocate();
      _used = 0;
    }

    const uint8_t* getDataPtr() {
      return _dataPtr;
    };

    uint32_t getSize() {
      return _used;
    };
    
  private:
    uint8_t* _dataPtr = 0;
    uint32_t _size;
    uint32_t _used;

    bool reallocate() {
      uint32_t newSize;
      uint8_t* newPtr;
      
      if (!_dataPtr) {
        newSize = INITIAL_SIZE;
        newPtr = (uint8_t*)malloc(newSize);
      } else {
        newSize = _size * 2;
        newPtr = (uint8_t*)realloc(_dataPtr, newSize);
      }
      
      if (!newPtr) {
        DebugMsgs.error().printfln("Error allocating new ptr of size %d", newSize);
        return false;
      }

      _size = newSize;
      _dataPtr = newPtr;
      return true;
    }
};
