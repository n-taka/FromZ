#pragma once

#include <string>
#include <vector>

namespace FromZ
{
	// [Input]
	// GoZBinFilename
	//     GoZ binary file to be read.
	//     I recommend to use "GoZ Complete Binary" because standard one doesn't store several attributes such as polypaints.
	// [Output]
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

	template <typename Scalar, typename Index>
	void readGoZFile(
		const std::string &GoZBinFilename,
		std::string &meshName,
		std::vector<std::vector<Scalar>> &V,
		std::vector<std::vector<Index>> &F,
		std::vector<std::vector<std::pair<Scalar, Scalar>>> &UV,
		std::vector<std::vector<Scalar>> &VC,
		std::vector<Scalar> &M,
		std::vector<Index> &G);

	template <typename Scalar, typename Index>
	void readGoZFile(
		const std::string &GoZBinFilename,
		std::string &meshName,
		std::vector<std::vector<Scalar>> &V,
		std::vector<std::vector<Index>> &F);

}

#include "readGoZFile.cpp"
