# People Tracking with DepthAI

This demonstration highlights the application of DepthAI technology in monitoring human movement. It captures and annotates the paths of individuals as they pass, creating a visual trail behind them.
## Features

- Real-time people detection and tracking
- Display annotation for tracked people
- Graceful exit with a 'q' key press

## Pipeline

The pipeline consists of the following nodes:

### [ColorCamera Node](https://docs.luxonis.com/projects/api/en/latest/components/nodes/color_camera/?highlight=ColorCamera)
- **Function**: Captures video frames directly from the camera.
- **Output**: Outputs the `preview` stream which consists of the raw video frames.

### [MobileNetDetectionNetwork Node](https://docs.luxonis.com/projects/api/en/latest/components/nodes/mobilenet_detection_network/?highlight=MobileNetDetectionNetwork)
- **Function**: Analyzes the incoming `preview` stream and performs people detection via a neural network.
- **Input**: Receives `preview` frames from the `ColorCamera` node.
- **Output**: Provides `passthrough` and `nn_detections` containing data about detected people, including bounding boxes and confidence scores.
- **Model**: Uses the `person-detection-retail-0013` model from the blobconvertor repository, optimized for fast and efficient people detection.

### [ObjectTracker Node](https://docs.luxonis.com/projects/api/en/latest/components/nodes/object_tracker/?highlight=ObjectTracker)
- **Function**: Analyzes the detection results and detection frames to track detected objects.
- **Input**:
  - `input_detection_frame`: Video frames on which detections were performed.
  - `input_tracker_frame`: Video frames on which tracking will be performed.
  - `input_detections`: Detection results from the `MobileNetDetectionNetwork` node.
- **Output**: [Tracklets](https://docs.luxonis.com/projects/api/en/latest/components/messages/tracklets/?highlight=Tracklets) that carry tracking results.

### PeopleTrackingNode (Custom Node)
- **Function**: Processes tracking results from `ObjectTracker` and annotates frames based on `Tracklets` detections. 
- **Input**: 
  - `image`: Video frames form the `ColorCamera` node.
  - `tracklets`: `Tracklets` from `ObjectTracker`.
- **Output**: Displays the video frames annotated with bounding boxes, a tracking dot, and a trailing tail on detected people.

## Running the Application
```bash
python testing/people-tracking/app.py
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