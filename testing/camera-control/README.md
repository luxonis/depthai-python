# Camera Control with DepthAI

This example demonstrates controlling color camera parameters, enabling dynamic adjustments of exposure time and ISO sensitivity in real-time.

## Features

- Real-time adjustment of camera exposure time and ISO sensitivity
- Interactive control through keyboard inputs
- Live display of camera output with current settings

## Pipeline Structure

![CameraControl Pipeline](https://docs.luxonis.com/projects/sdk/en/latest/_images/camera_control.png)

The pipeline integrates the following nodes:

### [ColorCamera Node](https://docs.luxonis.com/projects/api/en/latest/components/nodes/color_camera/?highlight=ColorCamera)
- **Function**: Captures video frames directly from the camera.
- **Output**: Outputs the `preview` stream consisting of the raw video frames.

### CameraParametersControlNode (Custom Node)
- **Function**: Allows real-time control of camera parameters (exposure time and ISO sensitivity) based on keyboard inputs.
- **Input**: 
  - `key`: Keyboard inputs for adjusting camera settings.
- **Output**: Adjusted camera settings applied to the live camera feed.

### Setup
#### TODO: Update this section with the new installation instructions once the new release is out.
- Clone the depthai-python repository.
```bash
git clone --recurse-submodules -b preV3 https://github.com/luxonis/depthai-python.git
```
- Navigate to the `depthai-python` directory.
```bash
cd depthai-python
```
- Create a virtual environment.
```bash
python3 -m venv .venv
```
- Install the requirements for the project.
```bash
python3 -m pip install . --verbose
```
- Connect the OAK device to your computer.
- Run the application.

## Running the Application
```bash
python testing/camera-control/app.py
```
