// Copyright 2017 The Johns Hopkins University Applied Physics Laboratory.
// Licensed under the MIT License. See LICENSE.txt in the project root for full
// license information.

//
// Created by almess1 on 4/20/17.
// This file was created in order to adapt the SHR3D and ALIGN3D to open source
// IO.
//

#ifndef PUBGEO_NOT_POINT_SETS_H
#define PUBGEO_NOT_POINT_SETS_H

#include <pdal/PointView.hpp>
#include <pdal/PipelineExecutor.hpp>

namespace pubgeo {
    struct MinMaxXYZ {
        double xMin;
        double xMax;
        double yMin;
        double yMax;
        double zMin;
        double zMax;
    };


    class PointCloud {
    public:
        PointCloud();

        ~PointCloud()
        {}

        static bool TransformPointCloud(const char* inputFileName,
                                        const char* outputFileName,
                                        float translateX, float translateY,
                                        float translateZ);

        bool Read(pdal::PointViewPtr view);

        MinMaxXYZ bounds;
        int zone;
        int xOff;
        int yOff;
        int zOff;
        unsigned long numPoints;
    };
}
#endif //PUBGEO_NOT_POINT_SETS_H
