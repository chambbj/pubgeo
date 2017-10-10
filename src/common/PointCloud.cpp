// Copyright 2017 The Johns Hopkins University Applied Physics Laboratory.
// Licensed under the MIT License. See LICENSE.txt in the project root for full
// license information.

#include "PointCloud.h"

namespace pubgeo {
    PointCloud::PointCloud() : zone(0), xOff(0), yOff(0), zOff(0), numPoints(0)
    {
        bounds = MinMaxXYZ{0, 0, 0, 0, 0, 0};
    }


    bool PointCloud::Read(pdal::PointViewPtr view) {
        numPoints = view->size();
        if (numPoints < 1) {
            std::cerr << "[PUBGEO::PointCloud::READ] No points found in file."
                      << std::endl;
            return false;
        }

        pdal::BOX3D box;
        view->calculateBounds(box);
        pdal::SpatialReference sr = view->spatialReference();
        zone = sr.computeUTMZone(box);

        // used later to return points
        xOff = (int) floor(box.minx);
        yOff = (int) floor(box.miny);
        zOff = (int) floor(box.minz);

        bounds = {box.minx, box.maxx, box.miny, box.maxy, box.minz, box.maxz};
        return true;
    }

    bool PointCloud::TransformPointCloud(const char* inputFileName,
                                         const char* outputFileName,
                                         float translateX = 0,
                                         float translateY = 0,
                                         float translateZ = 0) {
        using namespace pdal;

        std::ostringstream oss;
        oss << "1 0 0 " << translateX << " 0 1 0 " << translateY << " 0 0 1 "
            << translateZ << " 0 0 0 1";

        PipelineManager m;
        StageFactory f;

        std::string inputDriver = f.inferReaderDriver(inputFileName);
        Stage& r = m.addReader(inputDriver);
        Option optsR("filename", inputFileName);
        r.setOptions(optsR);

        Stage& t = m.addFilter("filters.transformation");
        Option optsF("matrix", oss.str());
        t.setOptions(optsF);
        t.setInput(r);

        std::string outputDriver = f.inferWriterDriver(outputFileName);
        Stage& w = m.addWriter(outputDriver);
        Option optsW("filename", outputFileName);
        w.setOptions(optsW);
        w.setInput(t);

        point_count_t np = m.execute();

        return true;
    }
};
