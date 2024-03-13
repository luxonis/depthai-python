# QR Code Detection with DepthAI

This illustration highlights the QR detection capabilities enabled by DepthAI technology. Identified codes are marked with bounding boxes, along with the corresponding website name they direct to.

## Features

- Real-time qr code detection and decoding
- Display annotation for detected qr codes
- Graceful exit with a 'q' key press

## Pipeline

The pipeline consists of the following nodes:

### [ColorCamera Node](https://docs.luxonis.com/projects/api/en/latest/components/nodes/color_camera/?highlight=ColorCamera)
- **Function**: Captures video frames directly from the camera.
- **Output**: Outputs the `preview` stream which consists of the raw video frames.

### [MobileNetDetectionNetwork Node](https://docs.luxonis.com/projects/api/en/latest/components/nodes/mobilenet_detection_network/?highlight=MobileNetDetectionNetwork)
- **Function**: Analyzes the incoming `preview` stream and performs qr code detection via a neural network.
- **Input**: Receives `preview` frames from the `ColorCamera` node.
- **Output**: Provides `nn_detections` containing data about detected qr codes, including bounding boxes and confidence scores.
- **Model**: Uses the `qr_code_detection_384x384` model from the DepthAI Model Zoo, optimized for fast and efficient qr code detection.

### QrDetectionNode (Custom Node)
- **Function**: Annotates the frame based on the detection results and qr code decoding.
- **Input**: 
  - `image`: Video frames from the `ColorCamera` node.
  - `detections`: Detection results from the `MobileNetDetectionNetwork` node.
- **Output**: Displays the video frames annotated with bounding boxes and website names decoded from the qr code.

## Running the Application
```bash
python testing/qr-code-reader/app.py
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
