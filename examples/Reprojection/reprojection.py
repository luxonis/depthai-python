import numpy as np
from numba import jit, prange

@jit(nopython=True, parallel=True)
def reprojection(depth_image, depth_camera_intrinsics, camera_extrinsics, color_camera_intrinsics, depth_image_show = None):
    height = len(depth_image)
    width = len(depth_image[0])
    if depth_image_show is not None:
        image = np.zeros((height, width), np.uint8)
    else:
        image = np.zeros((height, width), np.uint16)
    for i in prange(0, height):
        for j in prange(0, width):
            d = depth_image[i][j]
            if(d == 0):
                continue
            # Convert pixel to 3d point
            x = (j - depth_camera_intrinsics[0][2]) * d / depth_camera_intrinsics[0][0]
            y = (i - depth_camera_intrinsics[1][2]) * d / depth_camera_intrinsics[1][1]
            z = d

            # Move the point to the camera frame
            x1 = camera_extrinsics[0][0] * x + camera_extrinsics[0][1] * y + camera_extrinsics[0][2] * z + camera_extrinsics[0][3]
            y1 = camera_extrinsics[1][0] * x + camera_extrinsics[1][1] * y + camera_extrinsics[1][2] * z + camera_extrinsics[1][3]
            z1 = camera_extrinsics[2][0] * x + camera_extrinsics[2][1] * y + camera_extrinsics[2][2] * z + camera_extrinsics[2][3]

            u = color_camera_intrinsics[0][0] * (x1  / z1) + color_camera_intrinsics[0][2]
            v = color_camera_intrinsics[1][1] * (y1  / z1) + color_camera_intrinsics[1][2]
            int_u = round(u)
            int_v = round(v)
            if(int_v != i):
                print(f'v -> {v} and i -> {i}') # This should never be printed
            if int_u >= 0 and int_u < len(image[0]) and int_v >= 0 and int_v < len(image):
                if depth_image_show is not None:
                    image[int_v][int_u] = depth_image_show[i][j][0]
                    # image[int_v][int_u - 1] = depth_image_show[i][j][0]
                    image[int_v][int_u + 1] = depth_image_show[i][j][0]
                    # if int_u < len(image[0]) - 2:
                    #     image[int_v][int_u + 1] = depth_image_show[i][j][0] # Do the same for the next pixel due to the rounding
                    #     image[int_v][int_u + 2] = depth_image_show[i][j][0] # Do the same for the next pixel due to the rounding
                else:
                    image[int_v][int_u] = z1
    return image