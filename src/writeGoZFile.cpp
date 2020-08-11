#ifndef writeGoZFile_CPP
#define writeGoZFile_CPP

#include "writeGoZFile.h"

#include <fstream>
#include <iostream>

template <typename Scalar, typename Index>
void FromZ::writeGoZFile(
    const std::string &GoZBinFilename,
    const std::string &meshName,
    const std::vector<std::vector<Scalar>> &V,
    const std::vector<std::vector<Index>> &F)
{
    std::vector<std::vector<std::pair<double, double>>> UV;
    std::vector<std::vector<double>> VC;
    std::vector<double> M;
    std::vector<int> G;

    FromZ::writeGoZFile(GoZBinFilenamem, meshName, V, F, UV, VC, M, G);
}

template <typename Scalar, typename Index>
void FromZ::writeGoZFile(
    const std::string &GoZBinFilename,
    const std::string &meshName,
    const std::vector<std::vector<Scalar>> &V,
    const std::vector<std::vector<Index>> &F,
    const std::vector<std::vector<std::pair<Scalar, Scalar>>> &UV,
    const std::vector<std::vector<Scalar>> &VC,
    const std::vector<Scalar> &M,
    const std::vector<Index> &G)
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

    // GoZ binary file uses littele endian
    bool isThisSystemLittleEndian = false;
    {
        uint32_char tmp ={ 0x01020304 };
        isThisSystemLittleEndian = (tmp.c[0] == 4);
    }

    std::ofstream GoZFile;
    GoZFile.open(GoZBinFilename, std::ios::out | std::ios::binary);

    // Header
    {
        std::string buf("GoZb 1.0 ZBrush GoZ Binary");
        GoZFile.write(buf.c_str(), buf.size());
        for (int i=0;i<6;++i)
        {
            GoZFile.put(static_cast<unsigned char>(0x2E));
        }
    }
    {
        uint32_char tmp;
        tmp.i = uint32_t(1);
        if (!isThisSystemLittleEndian)
        {
            std::swap(tmp.c[0], tmp.c[3]);
            std::swap(tmp.c[1], tmp.c[2]);
        }
        GoZFile.write(tmp.c, 4);

        tmp.i = meshName.size() + 24;
        if (!isThisSystemLittleEndian)
        {
            std::swap(tmp.c[0], tmp.c[3]);
            std::swap(tmp.c[1], tmp.c[2]);
        }
        GoZFile.write(tmp.c, 4);
    }
    {
        uint64_char tmp;
        tmp.i = uint64_t(1);
        if (!isThisSystemLittleEndian)
        {
            std::swap(tmp.c[0], tmp.c[7]);
            std::swap(tmp.c[1], tmp.c[6]);
            std::swap(tmp.c[2], tmp.c[5]);
            std::swap(tmp.c[3], tmp.c[4]);
        }
        GoZFile.write(tmp.c, 8);
    }

    // Mesh name
    {
        std::string buf("GoZMesh_");
        buf += meshName;
        GoZFile.write(buf.c_str(), buf.size());
        GoZFile.put(static_cast<unsigned char>(0x89));
        GoZFile.put(static_cast<unsigned char>(0x13));
        GoZFile.put(static_cast<unsigned char>(0x00));
        GoZFile.put(static_cast<unsigned char>(0x00));
    }
    {
        uint32_char tmp;
        tmp.i = uint32_t(20);
        if (!isThisSystemLittleEndian)
        {
            std::swap(tmp.c[0], tmp.c[3]);
            std::swap(tmp.c[1], tmp.c[2]);
        }
        GoZFile.write(tmp.c, 4);
    }
    {
        uint64_char tmp;
        tmp.i = uint64_t(1);
        if (!isThisSystemLittleEndian)
        {
            std::swap(tmp.c[0], tmp.c[7]);
            std::swap(tmp.c[1], tmp.c[6]);
            std::swap(tmp.c[2], tmp.c[5]);
            std::swap(tmp.c[3], tmp.c[4]);
        }
        GoZFile.write(tmp.c, 8);
    }
    {
        uint32_char tmp;
        tmp.i = uint32_t(0);
        if (!isThisSystemLittleEndian)
        {
            std::swap(tmp.c[0], tmp.c[3]);
            std::swap(tmp.c[1], tmp.c[2]);
        }
        GoZFile.write(tmp.c, 4);
    }

    // Vertices
    {
        GoZFile.put(static_cast<unsigned char>(0x11));
        GoZFile.put(static_cast<unsigned char>(0x27));
        GoZFile.put(static_cast<unsigned char>(0x00));
        GoZFile.put(static_cast<unsigned char>(0x00));
    }
    {
        uint32_char tmp;
        tmp.i = uint32_t(V.size() * 3 * 4 + 16);
        if (!isThisSystemLittleEndian)
        {
            std::swap(tmp.c[0], tmp.c[3]);
            std::swap(tmp.c[1], tmp.c[2]);
        }
        GoZFile.write(tmp.c, 4);
    }
    {
        uint64_char tmp;
        tmp.i = uint64_t(V.size());
        if (!isThisSystemLittleEndian)
        {
            std::swap(tmp.c[0], tmp.c[7]);
            std::swap(tmp.c[1], tmp.c[6]);
            std::swap(tmp.c[2], tmp.c[5]);
            std::swap(tmp.c[3], tmp.c[4]);
        }
        GoZFile.write(tmp.c, 8);
    }
    for (const auto& vertex : V)
    {
        for (const auto& xyz : vertex)
        {
            float_char tmp;
            tmp.f = static_cast<float>(xyz);
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[3]);
                std::swap(tmp.c[1], tmp.c[2]);
            }
            GoZFile.write(tmp.c, 4);
        }
    }

    // Faces
    {
        GoZFile.put(static_cast<unsigned char>(0x21));
        GoZFile.put(static_cast<unsigned char>(0x4E));
        GoZFile.put(static_cast<unsigned char>(0x00));
        GoZFile.put(static_cast<unsigned char>(0x00));
    }
    {
        uint32_char tmp;
        tmp.i = uint32_t(F.size() * 4 * 4 + 16);
        if (!isThisSystemLittleEndian)
        {
            std::swap(tmp.c[0], tmp.c[3]);
            std::swap(tmp.c[1], tmp.c[2]);
        }
        GoZFile.write(tmp.c, 4);
    }
    {
        uint64_char tmp;
        tmp.i = uint64_t(F.size());
        if (!isThisSystemLittleEndian)
        {
            std::swap(tmp.c[0], tmp.c[7]);
            std::swap(tmp.c[1], tmp.c[6]);
            std::swap(tmp.c[2], tmp.c[5]);
            std::swap(tmp.c[3], tmp.c[4]);
        }
        GoZFile.write(tmp.c, 8);
    }
    for (const auto& face : F)
    {
        for (const auto& fv : face)
        {
            uint32_char tmp;
            tmp.i = static_cast<uint32_t>(fv);
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[3]);
                std::swap(tmp.c[1], tmp.c[2]);
            }
            GoZFile.write(tmp.c, 4);
        }
        for (int i=face.size();i<4;++i)
        {
            GoZFile.put(static_cast<unsigned char>(0xff));
            GoZFile.put(static_cast<unsigned char>(0xff));
            GoZFile.put(static_cast<unsigned char>(0xff));
            GoZFile.put(static_cast<unsigned char>(0xff));
        }
    }

    // UVs
    if (UV.size() == F.size())
    {
        {
            GoZFile.put(static_cast<unsigned char>(0xA9));
            GoZFile.put(static_cast<unsigned char>(0x61));
            GoZFile.put(static_cast<unsigned char>(0x00));
            GoZFile.put(static_cast<unsigned char>(0x00));
        }
        {
            uint32_char tmp;
            tmp.i = uint32_t(UV.size() * 4 * 2 * 4 + 16);
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[3]);
                std::swap(tmp.c[1], tmp.c[2]);
            }
            GoZFile.write(tmp.c, 4);
        }
        {
            uint64_char tmp;
            tmp.i = uint64_t(UV.size());
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[7]);
                std::swap(tmp.c[1], tmp.c[6]);
                std::swap(tmp.c[2], tmp.c[5]);
                std::swap(tmp.c[3], tmp.c[4]);
            }
            GoZFile.write(tmp.c, 8);
        }
        for (const auto& faceUV : UV)
        {
            for (const auto& uv : faceUV)
            {
                float_char tmp;
                tmp.f = static_cast<float>(uv.first);
                if (!isThisSystemLittleEndian)
                {
                    std::swap(tmp.c[0], tmp.c[3]);
                    std::swap(tmp.c[1], tmp.c[2]);
                }
                GoZFile.write(tmp.c, 4);
                tmp.f = static_cast<float>(uv.second);
                if (!isThisSystemLittleEndian)
                {
                    std::swap(tmp.c[0], tmp.c[3]);
                    std::swap(tmp.c[1], tmp.c[2]);
                }
                GoZFile.write(tmp.c, 4);
            }
            for (int i=faceUV.size();i<4;++i)
            {
                float_char tmp;
                tmp.f = 0.0f;
                if (!isThisSystemLittleEndian)
                {
                    std::swap(tmp.c[0], tmp.c[3]);
                    std::swap(tmp.c[1], tmp.c[2]);
                }
                GoZFile.write(tmp.c, 4);
            }
        }
    }

    // Polypaints
    if (VC.size() == V.size())
    {
        {
            GoZFile.put(static_cast<unsigned char>(0xB9));
            GoZFile.put(static_cast<unsigned char>(0x88));
            GoZFile.put(static_cast<unsigned char>(0x00));
            GoZFile.put(static_cast<unsigned char>(0x00));
        }
        {
            uint32_char tmp;
            tmp.i = uint32_t(V.size() * 4 + 16);
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[3]);
                std::swap(tmp.c[1], tmp.c[2]);
            }
            GoZFile.write(tmp.c, 4);
        }
        {
            uint64_char tmp;
            tmp.i = uint64_t(V.size());
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[7]);
                std::swap(tmp.c[1], tmp.c[6]);
                std::swap(tmp.c[2], tmp.c[5]);
                std::swap(tmp.c[3], tmp.c[4]);
            }
            GoZFile.write(tmp.c, 8);
        }
        for (const auto& vc : VC)
        {
            uint32_char tmp;
            tmp.uc[0] = static_cast<unsigned char>(std::round(vc.at(2) * 255.0));
            tmp.uc[1] = static_cast<unsigned char>(std::round(vc.at(1) * 255.0));
            tmp.uc[2] = static_cast<unsigned char>(std::round(vc.at(0) * 255.0));
            tmp.uc[3] = static_cast<unsigned char>(std::round(vc.at(3) * 255.0));

            GoZFile.write(tmp.c, 4);
        }
    }

    // Mask
    if (M.size() == V.size())
    {
        {
            GoZFile.put(static_cast<unsigned char>(0x32));
            GoZFile.put(static_cast<unsigned char>(0x75));
            GoZFile.put(static_cast<unsigned char>(0x00));
            GoZFile.put(static_cast<unsigned char>(0x00));
        }
        {
            uint32_char tmp;
            tmp.i = uint32_t(V.size() * 2 + 16);
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[3]);
                std::swap(tmp.c[1], tmp.c[2]);
            }
            GoZFile.write(tmp.c, 4);
        }
        {
            uint64_char tmp;
            tmp.i = uint64_t(V.size());
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[7]);
                std::swap(tmp.c[1], tmp.c[6]);
                std::swap(tmp.c[2], tmp.c[5]);
                std::swap(tmp.c[3], tmp.c[4]);
            }
            GoZFile.write(tmp.c, 8);
        }
        for (const auto& m : M)
        {
            uint32_char tmp;
            tmp.i = static_cast<uint32_t>(std::round(m * std::numeric_limits<uint16_t>::max()));
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[3]);
                std::swap(tmp.c[1], tmp.c[2]);
            }
            GoZFile.write(tmp.c, 2);
        }
    }

    // Polygroups
    if (G.size() == F.size())
    {
        {
            GoZFile.put(static_cast<unsigned char>(0x41));
            GoZFile.put(static_cast<unsigned char>(0x9C));
            GoZFile.put(static_cast<unsigned char>(0x00));
            GoZFile.put(static_cast<unsigned char>(0x00));
        }
        {
            uint32_char tmp;
            tmp.i = uint32_t(G.size() * 2 + 16);
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[3]);
                std::swap(tmp.c[1], tmp.c[2]);
            }
            GoZFile.write(tmp.c, 4);
        }
        {
            uint64_char tmp;
            tmp.i = uint64_t(G.size());
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[7]);
                std::swap(tmp.c[1], tmp.c[6]);
                std::swap(tmp.c[2], tmp.c[5]);
                std::swap(tmp.c[3], tmp.c[4]);
            }
            GoZFile.write(tmp.c, 8);
        }
        for (const auto& g : G)
        {
            uint32_char tmp;
            tmp.i = static_cast<uint32_t>(g);
            if (!isThisSystemLittleEndian)
            {
                std::swap(tmp.c[0], tmp.c[3]);
                std::swap(tmp.c[1], tmp.c[2]);
            }
            GoZFile.write(tmp.c, 2);
        }
    }

    // End
    {
        GoZFile.put(static_cast<unsigned char>(0x00));
        GoZFile.put(static_cast<unsigned char>(0x00));
        GoZFile.put(static_cast<unsigned char>(0x00));
        GoZFile.put(static_cast<unsigned char>(0x00));
    }

    GoZFile.close();
}


#endif
