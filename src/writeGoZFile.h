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
