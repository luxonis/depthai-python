# Stereo Preview

This example shows how to display WLS filtered disparity map using OpenCV.

## Features

- Real-time processing of stereo images to generate disparity maps.
- Display the disparity map in a colorized form.
- Graceful exit from the application with the 'q' key press.

### Pipeline Structure

![Stereo Preview Pipeline](https://docs.luxonis.com/projects/sdk/en/latest/_images/stereo.png)
The pipeline involves the following nodes:

### [MonoCamera Node](https://docs.luxonis.com/projects/api/en/latest/components/nodes/mono_camera/)
- **Function**: Captures monochrome video frames from two cameras (left and right).
- **Output**: Outputs `left` and `right` streams, which are monochrome video frames from the respective cameras.

### [StereoDepth Node](https://docs.luxonis.com/projects/api/en/latest/components/nodes/stereo_depth/)
- **Function**: Processes pairs of monochrome images to compute depth information.
- **Input**: Receives `left` and `right` frames from two MonoCamera nodes.
- **Output**: Produces a disparity map.

### StereoPreviewNode (Custom Node)
- **Function**: Processes the disparity map to display the depth information in a colorized form.
- **Input**: 
  - `disparity`: The disparity map from the StereoDepth node.
  - `config`: The configuration for the StereoDepth node to get the maximum disparity value.
- **Output**: Displays the depth information in a colorized form on the screen.

## Running the Application
```bash
python testing/stereo-preview/app.py
```

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
