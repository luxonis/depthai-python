import cv2
import numpy as np
from sympy import symbols, Eq, solve
from math import sqrt
import depthai as dai

def get_rhs(r):
    # r = sqrt(x**2 + y**2)
    num = 1 + 5.8138 * (r**2) + 8.4603 * (r**4) + (-26.8367 * (r**6))
    din = 1 + 5.5135 * (r**2) + 9.481 * (r**4) + (-27.6777 * (r**6))
    return (num/din - 1)/(r**2)


k = np.array(
    [
        [807.60571289, 0.0, 666.73193359],
        [0.0, 807.60571289, 358.409729],
        [0.0, 0.0, 1.0],
    ])

print(k)

dist_8_coeffs = np.array(
    [
        5.813818454742432,
        8.46031665802002,
        0.0009447094053030014,
        4.222730058245361e-05,
        -26.836753845214844,
        5.513574123382568,
        9.481003761291504,
        -27.6777286529541,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
    ])


distorted_frame = cv2.imread('./../MonoCamera/mono_data/1645472947815.png', 0)



# {k1: -0.000198482595555556, k2: 2.47884555555556e-7, k3: -5.90078888888889e-11}

x, y, z = symbols('x,y,z')
r = 10*sqrt(2)/2
eq1 = Eq((x + (r**2)*y + (r**4)*z),get_rhs(r))
r = 20*sqrt(2)/2
eq2 = Eq((x + (r**2)*y + (r**4)*z),get_rhs(r))
r = 40*sqrt(2)/2
eq3 = Eq((x + (r**2)*y + (r**4)*z),get_rhs(r))

radial_coeff = solve((eq1, eq2, eq3), (x, y, z))
print(radial_coeff)


dist_5Coeff =  np.array(
    [
        float(radial_coeff[x]),
        float(radial_coeff[y]),
        0.0009447094053030014,
        4.222730058245361e-05,
        float(radial_coeff[z]),
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
    ])
print(type(radial_coeff[x]))
undistorted_frame = cv2.undistort(
            distorted_frame, k, dist_8_coeffs, None, k)



undistorted_frame_5f = cv2.undistort(
            distorted_frame, k, dist_5Coeff, None, k)

cv2.imshow("distorted_frame", distorted_frame)

cv2.imshow("distorted_frame_8_coeff", undistorted_frame)
cv2.imshow("distorted_frame_5_coeff", undistorted_frame_5f)

cv2.waitKey(0)
# dist_5Coeff =  np.array(
#     [
#         radial_coeff.x,
#         radial_coeff.y,
#         0.0009447094053030014,
#         4.222730058245361e-05,
#         radial_coeff.z,
#         0.0,
#         0.0,
#         0.0,
#         0.0,
#         0.0,
#         0.0,
#         0.0,
#         0.0,
#         0.0,
#     ],
#     dtype=np.float16)

#------------------------------- Least Squares Method --------------------------------

def pixel_coord_np(width, height):
    """
    Pixel in homogenous coordinate
    Returns:
        Pixel coordinate:       [3, width * height]
    """
    x = np.linspace(0, width - 1, width)
    y = np.linspace(0, height - 1, height)
    [x, y] = np.meshgrid(x, y)
    return np.vstack((x.flatten(), y.flatten(), np.ones_like(x.flatten())))


with dai.Device() as device:
    calibData = device.readCalibration()
    M_right = np.array(calibData.getCameraIntrinsics(dai.CameraBoardSocket.RIGHT, 1280, 720))

    # get cam coordinates
    width = 1280
    height = 720
    pixel_coords = pixel_coord_np(width, height) 
    cam_coord = np.matmul(np.linalg.inv(M_right), pixel_coords)

    # get r from cam_coord
    r = np.sqrt(cam_coord[0, :] **2 + cam_coord[1, :]**2)

    print(radial_coeff)
    print(r[10])
    # r = r.reshape(-1, 1)
    rhs = np.apply_along_axis(get_rhs, 0, r)
    print(r.shape)

    r_2 = np.square(r)
    r_4 = np.square(r_2)
    print('r: {}, r^2: {}, r^4: {} '.format(r[10], r_2[10], r_4[10]))

    matrix = np.array([np.ones(r.shape, like = r), r_2, r_4])
    # print(np.linalg.matrix_rank(matrix))
    matrix = matrix.T
    print(matrix.shape)
    print(matrix[10])
    print(rhs.shape)

    res = np.linalg.lstsq(matrix, rhs)
    print(res)