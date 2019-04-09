#include "ScaterMemoryIODevice.h"
#include <array>
#include <boost/iostreams/stream.hpp>
#include <iostream>

int main()
{
    std::vector<std::pair<void*, size_t>> memAreas;
    char outArea0[5] = {'0', '1', '2', '3', '\0'};
    int outArea1[4] = {5, 6, 7, 8};
    std::array<int64_t, 2> outArea2{100000000000, 200000000000};
    double outArea3 = 10.01;
    std::array<char16_t, 5> outArea4{'H', 'e', 'l', 'l', 'o'};
    memAreas.resize(5);
    memAreas[0].first = static_cast<void*>(&outArea0);
    memAreas[0].second = sizeof(char) * 5;
    memAreas[1].first = static_cast<void*>(&outArea1);
    memAreas[1].second = sizeof(int) * 4;
    memAreas[2].first = static_cast<void*>(&outArea2);
    memAreas[2].second = sizeof (outArea2);
    memAreas[3].first = static_cast<void*>(&outArea3);
    memAreas[3].second = sizeof (outArea3);
    memAreas[4].first = static_cast<void*>(&outArea4);
    memAreas[4].second = sizeof (outArea4);

    ScatterMemoryIODevice testDevice = ScatterMemoryIODevice(memAreas);
    boost::iostreams::stream<ScatterMemoryIODevice> io(testDevice);

    std::string strBuffer;
    io >> strBuffer;
    io.flush();
    io.seekg(0, BOOST_IOS::beg);

    std::vector<std::pair<void*, size_t>> memAreasIn;
    char inArea0[5];
    int inArea1[4];
    int *inArea1ptr = &inArea1[0];
    std::array<int64_t, 2> inArea2;
    double inArea3;
    std::array<char16_t, 5> inArea4;

    memAreasIn.resize(5);
    memAreasIn[0].first = static_cast<void*>(&inArea0);
    memAreasIn[0].second = sizeof(char) * 5;
    memAreasIn[1].first = static_cast<void*>(&inArea1);
    memAreasIn[1].second = sizeof(int) * 4;
    memAreasIn[2].first = static_cast<void*>(&inArea2);
    memAreasIn[2].second = sizeof (inArea2);
    memAreasIn[3].first = static_cast<void*>(&inArea3);
    memAreasIn[3].second = sizeof (inArea3);
    memAreasIn[4].first = static_cast<void*>(&inArea4);
    memAreasIn[4].second = sizeof (inArea4);
    ScatterMemoryIODevice testDeviceIn = ScatterMemoryIODevice(memAreasIn);
    boost::iostreams::stream<ScatterMemoryIODevice> ioIn(testDeviceIn);
    std::string inStr0, inStr1;

    ioIn << strBuffer;
    ioIn.flush();

    ioIn.seekg(0, BOOST_IOS::beg);
    inStr0.append(strBuffer, 0, 10);
    inStr1.append(strBuffer, 10);
    ioIn << inStr0;
    ioIn.flush();
    ioIn << inStr1;
    ioIn.flush();

    std::cout << inArea0 << std::endl;
    for (int i = 0; i < 4; i++) {
        std::cout << *inArea1ptr;
        inArea1ptr++;
    }
    std::cout << std::endl;
    for (auto value : inArea2) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << inArea3 << std::endl;
    for (auto value : inArea4) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}
