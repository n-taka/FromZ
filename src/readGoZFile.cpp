#ifndef readGoZFile_CPP
#define readGoZFile_CPP

#include "readGoZFile.h"

#include <fstream>
#include <iostream>

template <typename Scalar, typename Index>
void FromZ::readGoZFile(
    const std::string &GoZBinFilename,
    std::string &meshName,
    std::vector<std::vector<Scalar>> &V,
    std::vector<std::vector<Index>> &F)
{
    std::vector<std::vector<std::pair<double, double>>> UV;
    std::vector<std::vector<double>> VC;
    std::vector<double> M;
    std::vector<int> G;

    FromZ::readGoZFile(GoZBinFilename, meshName, V, F, UV, VC, M, G);
}

template <typename Scalar, typename Index>
void FromZ::readGoZFile(
    const std::string &GoZBinFilename,
    std::string &meshName,
    std::vector<std::vector<Scalar>> &V,
    std::vector<std::vector<Index>> &F,
    std::vector<std::vector<std::pair<Scalar, Scalar>>> &UV,
    std::vector<std::vector<Scalar>> &VC,
    std::vector<Scalar> &M,
    std::vector<Index> &G)
{
    // union data structures for conversion
    union uint32_char
    {
        uint32_t i;
        char c[4];
        unsigned char uc[4];
    };
    union uint64_char
    {
        uint64_t i;
        char c[8];
    };
    union float_char
    {
        float f;
        char c[4];
    };

    // GoZ binary file uses little endian
    bool isThisSystemLittleEndian = false;
    {
        uint32_char tmp ={ 0x01020304 };
        isThisSystemLittleEndian = (tmp.c[0] == 4);
    }

    V.clear();
    F.clear();
    UV.clear();
    VC.clear();

    std::ifstream GoZFile;
    GoZFile.open(GoZBinFilename, std::ios::in | std::ios::binary);
    GoZFile.seekg(36, std::ios::beg);


    int lenObjName;
    {
        uint32_char tmp;
        GoZFile.read(tmp.c, 4);
        if (!isThisSystemLittleEndian)
        {
            std::swap(tmp.c[0], tmp.c[3]);
            std::swap(tmp.c[1], tmp.c[2]);
        }
        lenObjName = tmp.i - 16;
        GoZFile.seekg(8, std::ios::cur);
    }
    {
        std::vector<char> buf(lenObjName);
        GoZFile.read(buf.data(), lenObjName);
        meshName = std::string(buf.begin(), buf.end());
        // remove prefix "GoZMesh_"
        if (meshName.substr(0, 8) == "GoZMesh_")
        {
            meshName = meshName.substr(8);
        }
        // erase white space and non-printable
        const auto isInValid = [](const int c)
        {
            return (!isprint(c) || isspace(c));
        };
        meshName.erase(std::remove_if(meshName.begin(), meshName.end(), isInValid), meshName.end());
    }

    //////
    // remote non-ascii chars (if needed)
    // not yet implemented!
    //////

    // std::cout << "Importing: " << meshName << " from " << GoZBinFilename << std::endl;

    uint32_char tag;
    GoZFile.read(tag.c, 4);

    // set of tags for parse
    uint32_char name, vertices, faces, uvs, polypaints, mask, polygroups, end;
    name.c[0] = 0x89;
    name.c[1] = 0x13;
    name.c[2] = 0x00;
    name.c[3] = 0x00;

    vertices.c[0] = 0x11;
    vertices.c[1] = 0x27;
    vertices.c[2] = 0x00;
    vertices.c[3] = 0x00;

    faces.c[0] = 0x21;
    faces.c[1] = 0x4e;
    faces.c[2] = 0x00;
    faces.c[3] = 0x00;

    uvs.c[0] = 0xa9;
    uvs.c[1] = 0x61;
    uvs.c[2] = 0x00;
    uvs.c[3] = 0x00;

    polypaints.c[0] = 0xb9;
    polypaints.c[1] = 0x88;
    polypaints.c[2] = 0x00;
    polypaints.c[3] = 0x00;

    mask.c[0] = 0x32;
    mask.c[1] = 0x75;
    mask.c[2] = 0x00;
    mask.c[3] = 0x00;

    polygroups.c[0] = 0x41;
    polygroups.c[1] = 0x9c;
    polygroups.c[2] = 0x00;
    polygroups.c[3] = 0x00;

    end.c[0] = 0x00;
    end.c[1] = 0x00;
    end.c[2] = 0x00;
    end.c[3] = 0x00;

    while (!GoZFile.eof())
    {
        if (tag.i == name.i)
        {
            // Name
            // std::cout << "name: " << tag.i << std::endl;

            uint32_char tmp;
            GoZFile.read(tmp.c, 4);
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[3]);
                std::swap(tmp.c[1], tmp.c[2]);
            }
            const uint32_t cnt = tmp.i - 8;
            GoZFile.seekg(cnt, std::ios::cur);
        }
        else if (tag.i == vertices.i)
        {
            // Vertices
            // std::cout << "vertices: " << tag.i << std::endl;

            GoZFile.seekg(4, std::ios::cur);
            uint64_char tmp;
            GoZFile.read(tmp.c, 8);
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[7]);
                std::swap(tmp.c[1], tmp.c[6]);
                std::swap(tmp.c[2], tmp.c[5]);
                std::swap(tmp.c[3], tmp.c[4]);
            }
            const uint64_t cnt = tmp.i;
            V.resize(cnt);
            for (uint64_t v = 0; v < cnt; ++v)
            {
                std::vector<Scalar> vertex(3);
                for (int xyz = 0; xyz < 3; ++xyz)
                {
                    float_char tmp;
                    GoZFile.read(tmp.c, 4);
                    if (!isThisSystemLittleEndian)
                    {
                        std::swap(tmp.c[0], tmp.c[3]);
                        std::swap(tmp.c[1], tmp.c[2]);
                    }
                    vertex.at(xyz) = static_cast<Scalar>(tmp.f);
                }
                // [Prevent flip]
                // As long as read/write with FromZ, this solves flipping problem
                // In ZBrush, +Y goes downward and +Z goes into the screen
                vertex.at(1) *= -1;
                vertex.at(2) *= -1;
                V.at(v) = vertex;
            }
        }
        else if (tag.i == faces.i)
        {
            // Faces
            // std::cout << "Faces: " << tag.i << std::endl;

            GoZFile.seekg(4, std::ios::cur);
            uint64_char tmp;
            GoZFile.read(tmp.c, 8);
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[7]);
                std::swap(tmp.c[1], tmp.c[6]);
                std::swap(tmp.c[2], tmp.c[5]);
                std::swap(tmp.c[3], tmp.c[4]);
            }
            const uint64_t cnt = tmp.i;
            F.resize(cnt);
            for (uint64_t f = 0; f < cnt; ++f)
            {
                std::vector<Index> tmpFace(4);
                for (int xyzw = 0; xyzw < 4; ++xyzw)
                {
                    uint32_char tmp;
                    GoZFile.read(tmp.c, 4);
                    if (!isThisSystemLittleEndian)
                    {
                        std::swap(tmp.c[0], tmp.c[3]);
                        std::swap(tmp.c[1], tmp.c[2]);
                    }
                    tmpFace.at(xyzw) = static_cast<Index>(tmp.i);
                }
                std::vector<Index> face;
                if (tmpFace.at(3) == 0xffffffff)
                {
                    face.push_back(tmpFace.at(0));
                    face.push_back(tmpFace.at(1));
                    face.push_back(tmpFace.at(2));
                }
                else if (tmpFace.at(3) == 0)
                {
                    // Kazutaka: I don't fully understand
                    // the meaning of this "else if" block
                    face.push_back(tmpFace.at(3));
                    face.push_back(tmpFace.at(0));
                    face.push_back(tmpFace.at(1));
                    face.push_back(tmpFace.at(2));
                }
                else
                {
                    face.push_back(tmpFace.at(0));
                    face.push_back(tmpFace.at(1));
                    face.push_back(tmpFace.at(2));
                    face.push_back(tmpFace.at(3));
                }
                F.at(f) = face;
            }
        }
        else if (tag.i == uvs.i)
        {
            // UVs
            // std::cout << "UVs: " << tag.i << std::endl;

            GoZFile.seekg(4, std::ios::cur);
            uint64_char tmp;
            GoZFile.read(tmp.c, 8);
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[7]);
                std::swap(tmp.c[1], tmp.c[6]);
                std::swap(tmp.c[2], tmp.c[5]);
                std::swap(tmp.c[3], tmp.c[4]);
            }
            const uint64_t cnt = tmp.i;

            UV.resize(cnt);
            for (uint64_t f = 0; f < cnt; ++f)
            {
                std::vector<std::pair<Scalar, Scalar>> tmpFaceUV(4);
                for (int xyzw = 0; xyzw < 4; ++xyzw)
                {
                    float_char tmp;
                    GoZFile.read(tmp.c, 4);
                    if (!isThisSystemLittleEndian)
                    {
                        std::swap(tmp.c[0], tmp.c[3]);
                        std::swap(tmp.c[1], tmp.c[2]);
                    }
                    tmpFaceUV.at(xyzw).first = static_cast<Scalar>(tmp.f);
                    GoZFile.read(tmp.c, 4);
                    if (!isThisSystemLittleEndian)
                    {
                        std::swap(tmp.c[0], tmp.c[3]);
                        std::swap(tmp.c[1], tmp.c[2]);
                    }
                    tmpFaceUV.at(xyzw).second = static_cast<Scalar>(tmp.f);
                }
                std::vector<std::pair<Scalar, Scalar>> faceUV;
                for (int f = 0; f < F.at(f).size(); ++f)
                {
                    faceUV.push_back(tmpFaceUV.at(f));
                }
                UV.at(f) = faceUV;
            }
        }
        else if (tag.i == polypaints.i)
        {
            // Polypaints
            // std::cout << "Polypaints: " << tag.i << std::endl;

            GoZFile.seekg(4, std::ios::cur);
            uint64_char tmp;
            GoZFile.read(tmp.c, 8);
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[7]);
                std::swap(tmp.c[1], tmp.c[6]);
                std::swap(tmp.c[2], tmp.c[5]);
                std::swap(tmp.c[3], tmp.c[4]);
            }
            const uint64_t cnt = tmp.i;
            VC.resize(cnt);
            for (uint64_t v = 0; v < cnt; ++v)
            {
                std::vector<Scalar> vertexColor(4);
                uint32_char tmp;
                GoZFile.read(tmp.c, 4);
                for (int rgba = 0; rgba < 4; ++rgba)
                {
                    vertexColor.at(rgba) = static_cast<Scalar>(tmp.uc[rgba] / 255.0);
                }
                // in GoZ file format, color is stored as BGRA
                std::swap(vertexColor.at(0), vertexColor.at(2));
                VC.at(v) = vertexColor;
            }
        }
        else if (tag.i == mask.i)
        {
            // Mask
            // std::cout << "Mask: " << tag.i << std::endl;

            GoZFile.seekg(4, std::ios::cur);
            uint64_char tmp;
            GoZFile.read(tmp.c, 8);
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[7]);
                std::swap(tmp.c[1], tmp.c[6]);
                std::swap(tmp.c[2], tmp.c[5]);
                std::swap(tmp.c[3], tmp.c[4]);
            }
            const uint64_t cnt = tmp.i;
            M.resize(cnt);
            for (uint64_t v = 0; v < cnt; ++v)
            {
                uint32_char tmp;
                GoZFile.read(tmp.c, 2);
                tmp.c[2] = 0;
                tmp.c[3] = 0;
                if (!isThisSystemLittleEndian)
                {
                    std::swap(tmp.c[0], tmp.c[3]);
                    std::swap(tmp.c[1], tmp.c[2]);
                }
                M.at(v) = static_cast<Scalar>(tmp.i) / static_cast<Scalar>(std::numeric_limits<uint16_t>::max());
            }
        }
        else if (tag.i == polygroups.i)
        {
            // Polygroups
            // std::cout << "Polygroups: " << tag.i << std::endl;

            GoZFile.seekg(4, std::ios::cur);
            uint64_char tmp;
            GoZFile.read(tmp.c, 8);
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[7]);
                std::swap(tmp.c[1], tmp.c[6]);
                std::swap(tmp.c[2], tmp.c[5]);
                std::swap(tmp.c[3], tmp.c[4]);
            }
            const uint64_t cnt = tmp.i;
            G.resize(cnt);
            for (uint64_t f = 0; f < cnt; ++f)
            {
                std::vector<Scalar> vertexMask(4);
                uint32_char tmp;
                GoZFile.read(tmp.c, 2);
                tmp.c[2] = 0;
                tmp.c[3] = 0;
                if (!isThisSystemLittleEndian)
                {
                    std::swap(tmp.c[0], tmp.c[3]);
                    std::swap(tmp.c[1], tmp.c[2]);
                }
                G.at(f) = static_cast<Index>(tmp.i);
            }
        }
        else if (tag.i == end.i)
        {
            // End
            // std::cout << "end: " << tag.i << std::endl;
            break;
        }
        else
        {
            // Unknown
            // std::cout << "Unknown tag: " << tag.i << std::endl;
            uint32_char tmp;
            GoZFile.read(tmp.c, 4);
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[3]);
                std::swap(tmp.c[1], tmp.c[2]);
            }
            const uint32_t cnt = tmp.i - 8;
            GoZFile.seekg(cnt, std::ios::cur);
        }

        GoZFile.read(tag.c, 4);
    }

    GoZFile.close();
}

#endif
