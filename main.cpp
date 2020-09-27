
#include <vector>
#include <string>
#include "readGoZFile.h"
#include "writeGoZFile.h"

int main()
{
    std::string meshName;
    std::vector<std::vector<double>> V, VC;
    std::vector<double> M;
    std::vector<int> G;
    std::vector<std::vector<int>> F;
    std::vector<std::vector<std::pair<double, double>>> UV;

    FromZ::readGoZFile("./example/Dog.GoZ", meshName, V, F, UV, VC, M, G);
    FromZ::writeGoZFile("./example/DogOut.GoZ", meshName, V, F, UV, VC, M, G);

    FromZ::readGoZFile("./example/DemoHead.GoZ", meshName, V, F, UV, VC, M, G);
    FromZ::writeGoZFile("./example/DemoHeadOut.GoZ", meshName, V, F, UV, VC, M, G);

    return 0;
}