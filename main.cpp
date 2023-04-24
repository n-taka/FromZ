//
// Copyright (C) 2022-2023 Kazutaka Nakashima (kazutaka.nakashima@n-taka.info)
// 
// GPLv3
//
// This file is part of FromZ.
// 
// FromZ is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// 
// FromZ is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with FromZ. If not, see <https://www.gnu.org/licenses/>.
//

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