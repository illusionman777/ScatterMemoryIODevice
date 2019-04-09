#include "ScaterMemoryIODevice.h"

using namespace std;
using stream_offset = boost::iostreams::stream_offset;

ScatterMemoryIODevice::ScatterMemoryIODevice(const AreasVector &memoryAreas) :
    _memoryAreas(memoryAreas), _pos(0)
{
    _memorySize = 0;
    for (auto pair : _memoryAreas) {
        _memorySize += pair.second;
    }
}

streamsize ScatterMemoryIODevice::read(char_type *s, const streamsize n)
{
    streamsize amt = static_cast<streamsize>(_memorySize - _pos);
    streamsize result = min(n, amt);
    size_t endPos = _pos + static_cast<size_t>(n);
    if (result != 0) {
        for (auto pair : _memoryAreas) {
            size_t outBytes = min(endPos - _pos, pair.second);
            memcpy(s, pair.first, outBytes);
            s += pair.second;
            _pos += outBytes;
            if (_pos == endPos) {
                break;
            }
        }
        return result;
    } else {
        return -1; // EOF
    }
}

streamsize ScatterMemoryIODevice::write(const char_type *s, const streamsize n)
{
    streamsize result = 0;
    if (_pos != _memorySize) {
        size_t comparePos = 0;
        size_t endPos = min(_memorySize, _pos + static_cast<size_t>(n));
        result = static_cast<streamsize>(endPos - _pos);
        uint32_t sIndent = 0;
        for (auto iter = _memoryAreas.begin();
             iter < _memoryAreas.end(); iter++) {
            if (comparePos < _pos) {
                size_t bytesToPos = _pos - comparePos;
                if (bytesToPos < iter->second) {
                    void *memoryPtr = static_cast<char*>(iter->first) +
                                      bytesToPos;
                    size_t inBytes = iter->second - bytesToPos;
                    memcpy(memoryPtr, s + sIndent, inBytes);
                    sIndent += inBytes;
                }
            } else {
                size_t inBytes = min(endPos - _pos, iter->second);
                memcpy(iter->first, s + sIndent, inBytes);
                sIndent += iter->second;
                _pos += inBytes;
                if (_pos == endPos) {
                    break;
                }
            }
            comparePos += iter->second;
        }
    }
    // if result < n, write() method will be called until EOS reached
    return result;
}

stream_offset ScatterMemoryIODevice::seek(const stream_offset off,
                                          const std::ios_base::seekdir way)
{
    stream_offset next;
    if (way == ios_base::beg) {
        next = off;
    } else if (way == ios_base::cur) {
        next = static_cast<stream_offset>(_pos) + off;
    } else if (way == ios_base::end) {
        next = static_cast<stream_offset>(_memorySize) + off - 1;
    } else {
        throw ios_base::failure("bad seek direction");
    }

    if (next < 0 || next >= static_cast<stream_offset>(_memorySize))
        throw ios_base::failure("bad seek offset");

    _pos = static_cast<size_t>(next);
    return next;
}
