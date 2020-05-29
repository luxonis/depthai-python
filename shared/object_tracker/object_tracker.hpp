#pragma once

#include <cstdint>
#include <unordered_map>

#define MAX_OBJECTS (20)

enum TrackingStatus
{
    NEW = 0,         /**< The object is newly added. */
    TRACKED,     /**< The object is being tracked. */
    LOST         /**< The object gets lost now. The object can be tracked again automatically(long term tracking) or by specifying detected object manually(short term and zero term tracking). */
};

typedef struct ImgRoi
{
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
}ImgRoi;

typedef struct RoiObject
{
    ImgRoi roi;
    int64_t id;
    int32_t label;
    int32_t status;
}RoiObject;


struct ObjectTracker {

    int nr_tracklet;
    RoiObject tracklet[MAX_OBJECTS];

private:

public:

    int getNrTracklets(){
        assert(nr_tracklet < MAX_OBJECTS);
        return nr_tracklet;
    }

    int64_t getId(int tracklet_no){
        return tracklet[tracklet_no].id;
    }

    int32_t getLabel(int tracklet_no){
        return tracklet[tracklet_no].label;
    }

    std::string getStatus(int tracklet_no){
        TrackingStatus status = (TrackingStatus)tracklet[tracklet_no].status;
        std::unordered_map<enum TrackingStatus, std::string> trackletStatusMap =
        {
            {NEW,     "NEW"},
            {TRACKED, "TRACKED"},
            {LOST,    "LOST"}
        };
        std::string trackletStatusStr = trackletStatusMap.at(status);
        return trackletStatusStr;
    }

    int32_t getLeftCoord(int tracklet_no){
        return tracklet[tracklet_no].roi.left;
    }

    int32_t getTopCoord(int tracklet_no){
        return tracklet[tracklet_no].roi.top;
    }

    int32_t getRightCoord(int tracklet_no){
        return tracklet[tracklet_no].roi.right;
    }

    int32_t getBottomCoord(int tracklet_no){
        return tracklet[tracklet_no].roi.bottom;
    }

};