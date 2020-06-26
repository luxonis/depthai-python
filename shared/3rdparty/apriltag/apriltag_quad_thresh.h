#include "apriltag.h"

#ifdef __cplusplus
extern "C" {
#endif

image_u8_t *threshold(apriltag_detector_t *td, image_u8_t *im);
unionfind_t* connected_components(apriltag_detector_t *td, image_u8_t* threshim, int w, int h, int ts);
zarray_t* gradient_clusters(apriltag_detector_t *td, image_u8_t* threshim, int w, int h, int ts, unionfind_t* uf);
zarray_t* fit_quads(apriltag_detector_t *td, int w, int h, zarray_t* clusters, image_u8_t* im);

#ifdef __cplusplus
}
#endif

