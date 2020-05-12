#pragma once

#include <memory>
#include <string>
#include <tuple>

#include "host_data_packet.hpp"
#include "depthai-shared/disparity_luts.hpp"


class DepthPacket
{
public:
    // data_packet may point to nullptr
    DepthPacket(std::shared_ptr<HostDataPacket> &data_packet)
        : _data_packet(data_packet)
    {}

    bool canCalculateDistance()
    {
        return _data_packet.get() != nullptr;
    }

    std::tuple<int, int, int, int> innerBoundingBox(
        float left,
        float top,
        float right,
        float bottom,
        int width,
        int height,
        float fraction
    )
    {
        // bounding box
        int x1 = std::max(std::min(int(left   * width ), width ), 0);
        int y1 = std::max(std::min(int(top    * height), height), 0);
        int x2 = std::max(std::min(int(right  * width ), width ), 0);
        int y2 = std::max(std::min(int(bottom * height), height), 0);

        int bb_w = x2 - x1;
        int bb_h = y2 - y1;

        // center-cut (inner bounding box)
        int dx = bb_w * fraction;
        int dy = bb_h * fraction;

        int ix1 = x1 + dx;
        int iy1 = y1 + dy;
        int ix2 = x2 - dx;
        int iy2 = y2 - dy;

        return std::make_tuple(ix1, iy1, ix2, iy2);
    }

    float getAverageDistanceForRectangle(
        float left,
        float top,
        float right,
        float bottom,
        float padding_factor
    )
    {
        assert(_data_packet->stream_name == c_stream_name_depth);

        // TODO: better way ?
        int h = _data_packet->dimensions[0];
        int w = _data_packet->dimensions[1];

        int x1, y1, x2, y2;
        std::tie(x1, y1, x2, y2) = innerBoundingBox(left, top, right, bottom, w, h, padding_factor);

        int iw = x2 - x1;
        int ih = y2 - y1;

        // average calc
        const uint16_t* data = reinterpret_cast<const uint16_t*>(_data_packet->data.data());

        long sum = 0;
        long n = 0;

        for (int y = y1; y < y2; ++y)
        {
            const auto* data_y = data + y * w;
            for (int x = x1; x < x2; ++x)
            {
                if (data_y[x] != 0 && data_y[x] < 10000)
                {
                    sum += data_y[x];
                    ++n;
                }
            }
        }

        if (n == 0)
        {
            return -1.f;
        }
        else
        {
            return sum / float(n) / 1000.f;
        }
    }

    float getMedianDistanceForRectangle(
        float left,
        float top,
        float right,
        float bottom,
        float padding_factor,
        float h_fov_deg,
        float baseline_m
    )
    {
        assert(_data_packet->stream_name == c_stream_name_disparity);

        // TODO: better way ?
        int h = _data_packet->dimensions[0];
        int w = _data_packet->dimensions[1];

        int x1, y1, x2, y2;
        std::tie(x1, y1, x2, y2) = innerBoundingBox(left, top, right, bottom, w, h, padding_factor);

        int iw = x2 - x1;
        int ih = y2 - y1;

        // average calc
        const unsigned char* data = reinterpret_cast<const unsigned char*>(_data_packet->data.data());

        long n = 0;
        int counters[256] = {0};

        for (int y = y1; y < y2; ++y)
        {
            const auto* data_y = data + y * w;
            for (int x = x1; x < x2; ++x)
            {
                if (data_y[x] != 0)
                {
                    ++counters[ data_y[x] ];
                    ++n;
                }
            }
        }


        long mid_n = n / 2;
        long pos = 0;
        int disparity = 0;
        while (pos < mid_n) { pos += counters[disparity]; ++disparity; };

        return calculateDisparityToDepthMM(disparity, w, h_fov_deg, baseline_m) / 1000.f;
    }


private:
    const std::string c_stream_name_depth = "depth";
    const std::string c_stream_name_disparity = "disparity";

    std::shared_ptr<HostDataPacket> _data_packet;
};
