#pragma once

#include <cstdint>

#ifdef HOST_PYTHON_MODULE
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;
#endif

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

struct Tracklet {
    ImgRoi roi;
    int64_t id;
    int32_t label;
    int32_t status;

private:

public:

    int64_t getId(void){
        return id;
    }

    int32_t getLabel(void){
        return label;
    }

    std::string getStatus(void){
        std::vector<std::string> status_str = {"NEW", "TRACKED", "LOST"};
        if(status < 0 || status > 3) assert(0);
        return status_str.at(status);
    }

    int32_t getLeftCoord(void){
        return roi.left;
    }

    int32_t getTopCoord(void){
        return roi.top;
    }

    int32_t getRightCoord(void){
        return roi.right;
    }

    int32_t getBottomCoord(void){
        return roi.bottom;
    }

};

struct ObjectTracker {

    int nr_tracklets;
    Tracklet tracklet[MAX_OBJECTS];

private:

public:

    int getNrTracklets(){
        assert(nr_tracklets < MAX_OBJECTS);
        return nr_tracklets;
    }
#ifdef HOST_PYTHON_MODULE
    py::object getTracklet(int tracklet_nr)
    {
        assert(tracklet_nr < nr_tracklets);
        return py::cast<Tracklet>(tracklet[tracklet_nr]);
    }
#else
    Tracklet getTracklet(int tracklet_nr)
    {
        assert(tracklet_nr < nr_tracklets);
        return tracklet[tracklet_nr];
    }
#endif


};