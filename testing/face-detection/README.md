# Face Detection with DepthAI

This project integrates DepthAI technology to process live camera feeds and detect faces in real-time. Detected faces are highlighted with rectangles in the output display.

## Features

- Real-time face detection
- Display annotations for detected faces
- Graceful exit with a 'q' key press

### Pipeline Structure

![Face Detection Pipeline](https://docs.luxonis.com/projects/sdk/en/latest/_images/face_detection_color.png)

The pipeline consists of the following nodes:

### [ColorCamera Node](https://docs.luxonis.com/projects/api/en/latest/components/nodes/color_camera/?highlight=ColorCamera)
- **Function**: Captures video frames directly from the camera.
- **Output**: Outputs the `preview` stream which consists of the raw video frames.

### [MobileNetDetectionNetwork Node](https://docs.luxonis.com/projects/api/en/latest/components/nodes/mobilenet_detection_network/?highlight=MobileNetDetectionNetwork)
- **Function**: Analyzes the incoming `preview` stream and performs face detection via a neural network.
- **Input**: Receives `preview` frames from the `ColorCamera` node.
- **Output**: Provides `nn_detections` containing data about detected faces, including bounding boxes and confidence scores.
- **Model**: Uses the `face-detection-retail-0004` model from the blobconverter repository, optimized for fast and efficient face detection.

### FaceDetectionNode (Custom Node)
- **Function**: Enhances the frames with visual annotations based on detection results.
- **Input**: 
  - `image`: Video frames from the `ColorCamera` node.
  - `detections`: Detection results from the `MobileNetDetectionNetwork` node.
- **Output**: Displays the video frames with annotated bounding boxes around detected faces.

## Running the Application
```bash
python testing/face_detection/app.py
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