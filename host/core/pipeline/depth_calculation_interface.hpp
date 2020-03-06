#pragma once


class DepthCalculationInterface
{
public:
    const float  c_distance_undefined = -1.f;


    virtual ~DepthCalculationInterface() {}


    virtual bool canCalculateDistance() const = 0;

    virtual float getDistanceForRectangle(
        float rx1, float ry1,
        float rx2, float ry2,
        int width, int height
        ) = 0;
};
