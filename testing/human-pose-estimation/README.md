# Human Pose Detection with DepthAI

Powered by DepthAI tech, this setup processes live camera feeds to instantly detect human poses. It then renders them as on-body line illustrations in real-time.

## Features

- Real-time human pose detection
- Display annotation for detected pose
- Graceful exit with a 'q' key press

### Pipeline Structure

![Human Pose Detection Pipeline](https://docs.luxonis.com/projects/sdk/en/latest/_images/human_pose.png)

The pipeline consists of the following nodes:

### [ColorCamera Node](https://docs.luxonis.com/projects/api/en/latest/components/nodes/color_camera/?highlight=ColorCamera)
- **Function**: Captures video frames directly from the camera.
- **Output**: Outputs the `preview` stream which consists of the raw video frames.

### [NeuralNetwork Node](https://docs.luxonis.com/projects/api/en/latest/components/nodes/neural_network/?highlight=NeuralNetwork)
- **Function**: Analyzes the incoming `preview` stream and performs human pose detection.
- **Input**: Receives `preview` frames from the `ColorCamera` node.
- **Output**: Provides tensors and its data in the form of [NNData](https://docs.luxonis.com/projects/api/en/latest/components/messages/nn_data/?highlight=NNData) to the `HumanPoseEstimationNode`.
- **Model**: Uses the `human-pose-estimation-0001` model from the blobconvertor repository, optimized for fast and efficient human pose detection.

### HumanPoseEstimationNode (Custom Node)
- **Function**: Processes the `NNData` from `NeuralNetwork` and annotates the frame based on the resulting detections. 
- **Input**: 
  - `image`: Video frames form the `ColorCamera` node.
  - `detections`: `NNData` from the `NeuralNetwork` node.
- **Output**: Displays the video frames annotated with lines on the detected pose.

## Running the Application
```bash
python testing/human-pose-estimation/app.py
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
