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

#pragma once

#include <string>
#include <vector>

namespace FromZ
{
	// [Input]
	// GoZBinFilename
	//     GoZ binary file to be written
	// meshName
	//     mesh name extracted from GoZ file
	// V
	//     (#V x 3) XYZ coordinate for the mesh vertices
	// F
	//     (#F x {3 or 4}) vertex indices into V ([0, #V-1])
	// UV
	//     (#F x {3 or 4}) UV coordinates of vertices of each face
	// VC
	//     (#V x 4) normalized polypaints (a.k.a vertex color) stored as RGBA ([0.0, 1.0])
	//     It seems that alpha values are always 0.0
	// M
	//     (#V x 1) normalized mask ([0.0 (masked), 1.0 (not masked)])
	// G
	//     (#F x 1) face groups stored as a set of ramdom unique ids
	// [Output]
	// None

	template <typename Scalar, typename Index>
	void writeGoZFile(
		const std::string &GoZBinFilename,
		const std::string &meshName,
		const std::vector<std::vector<Scalar>> &V,
		const std::vector<std::vector<Index>> &F,
		const std::vector<std::vector<std::pair<Scalar, Scalar>>> &UV,
		const std::vector<std::vector<Scalar>> &VC,
		const std::vector<Scalar> &M,
		const std::vector<Index> &G);

	template <typename Scalar, typename Index>
	void writeGoZFile(
		const std::string &GoZBinFilename,
		const std::string &meshName,
		const std::vector<std::vector<Scalar>> &V,
		const std::vector<std::vector<Index>> &F);

}

#include "writeGoZFile.cpp"
