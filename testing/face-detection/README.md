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
- **Function**: Captures video frames from the onboard camera.
- **Input**: None.
- **Output**: `preview` stream.
- **Details**: This node is set to output a preview stream that is used for both display purposes and as input for the object detection network.

### [MobileNetDetectionNetwork Node](https://docs.luxonis.com/projects/api/en/latest/components/nodes/mobilenet_detection_network/?highlight=MobileNetDetectionNetwork)
- **Function**: Performs object detection on the input frames.
- **Input**: `preview` stream from the `ColorCamera` node.
- **Output**: `nn_detections` which include bounding boxes and object classifications.
- **Details**: Utilizes a pre-trained MobileNet SSD model optimized for the OAK device to detect objects in real-time.
- **Model**: MobileNet SSD, specifically `face-detection-retail-0004` automatically downloaded from the [blobconverter repository](https://artifacts.luxonis.com/artifactory/blobconverter-backup/blobs/).

### FaceDetectionNode (Custom Node)
- **Function**: Annotates the video frames with detection results.
- **Input**:
  - `image`: Frames from the `ColorCamera` node.
  - `detections`: Detection results from the `MobileNetDetectionNetwork` node.
- **Output**: Annotated video frames displayed on the screen.
- **Details**: This is a custom node that integrates the `FaceDetector` class to overlay bounding boxes on detected faces and displays the processed frames. The `FaceDetector` class encapsulates the logic for drawing bounding boxes and text annotations on the frame based on the detection results. The node exits when the 'q' key is pressed. This design modularizes the face detection logic, making the code more maintainable and scalable.

## How It Works

1. The `ColorCamera` node captures video frames from the device's camera.
2. Captured frames are sent to the `MobileNetDetectionNetwork` node which detects objects within the frame.
3. Detection results are then passed to the `FaceDetectionNode`.
4. The `FaceDetectionNode` processes the frame and the detection results, drawing bounding boxes around detected faces.
5. The annotated frames are displayed in real-time.
6. The application runs continuously until the user exits by pressing the 'q' key.

## Running the Application

To start the face detection, execute the pipeline with the following command:

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