/******************************************************************************\
Copyright (c) 2005-2020, Intel Corporation
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

\**********************************************************************************/


// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"

#include <string>
#include <vector>

#include <inference_engine.hpp>
#include <opencv2/core/core.hpp>
#include <gpu/gpu_context_api_va.hpp>

#include "sample_utils.h"
#include "network_factory.hpp"

//#define USE_MOBILENET_SSD

#define INFER_FD_INPUT_W 300
#define INFER_FD_INPUT_H 300
struct ObjectDetectResult {
    int label;
    float confidence;
    cv::Rect location;
    std::string color;
    std::string type;
};

class ObjectDetect {
public:

    ObjectDetect(bool mEnablePerformanceReport = false);
    int Init(const std::string& detectorModelPath,
            const std::string& targetDeviceName = "GPU",
            bool remoteBlob = false, VADisplay vaDpy = NULL);
    void Detect(const cv::Mat& image, std::vector<ObjectDetectResult>& results);
    void Detect(VASurfaceID surface, std::vector<ObjectDetectResult>& results);
    void SetSrcImageSize(int width, int height);
    void GetInputSize(int &width, int &height);
    void RenderResults(std::vector<ObjectDetectResult>& results, cv::Mat& image, bool isGrey = false);
    int Detect(mfxFrameData *pData, mfxFrameData *pData_dec, std::vector<ObjectDetectResult>& results);
    ~ObjectDetect();

private:
    //Nonassinable
    ObjectDetect(ObjectDetect const&);
    ObjectDetect& operator=(ObjectDetect const&);

    float mDetectThreshold;
    bool mRemoteBlob;
    std::string mInputName;

    void CopyDetectResults(const float *detections, std::vector<ObjectDetectResult>& results);
    int CopyImageData(unsigned char *dst, char unsigned *src, unsigned int width, unsigned int height, unsigned int pitch);

    NetworkInfo *mNetworkInfo;

    InferenceEngine::InferRequest mDetectorRequest;
    int mDetectorMaxProposalCount;
    int mDetectorObjectSize;
    std::string mDetectorRoiBlobName;
    std::string mDetectorOutputName;
    bool mEnablePerformanceReport;
    cv::Size mSrcImageSize;

    unsigned int mInputW;
    unsigned int mInputH;
    unsigned char *mImgData;
};

