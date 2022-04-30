#include <fstream>

std::string readFile(const std::string& pFileName) {

    std::ifstream lFile(pFileName, std::ios::binary | std::ios::ate);
    if(!lFile.good())
        throw std::runtime_error("File does not exist");

    std::streampos lFileSize = lFile.tellg();
    lFile.seekg(0);

    std::string lFileData(lFileSize, ' ');
    
    // If you get an incompatible type error on this line set the C++ standard to c++20 or higher
    lFile.read(lFileData.data(), lFileSize);

    lFile.close();

    return lFileData;
}