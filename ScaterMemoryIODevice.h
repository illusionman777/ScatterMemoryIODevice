#pragma once

#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/positioning.hpp>
#include <vector>

class ScatterMemoryIODevice
{
public:
    using char_type = char;
    using category = boost::iostreams::seekable_device_tag;
    using stream_offset = boost::iostreams::stream_offset;
    using AreasVector = std::vector<std::pair<void*, size_t>>;

    ScatterMemoryIODevice(
            const AreasVector &memoryAreas);
    std::streamsize read(char_type *s, std::streamsize n);
    std::streamsize write(const char_type *s, std::streamsize n);
    stream_offset seek(stream_offset off, std::ios_base::seekdir way);
private:
    const AreasVector &_memoryAreas;
    size_t _memorySize;
    size_t _pos;
};
