#Example for Object Tracking
import blobconverter
import cv2
import depthai
from depthai import node, message

class ObjectTrackingNode(depthai.Node):
    def __run__(self,img_frame: message.ImgFrame,tracklets_data: message.Tracklets):
        #Convert formats
        frame = img_frame.getCvFrame()
        tracklets_d = tracklets_data.tracklets
        
        #Iterating over all of the tracked objects
        for t in tracklets_d:
            roi = t.roi.denormalize(frame.shape[1], frame.shape[0])
            x1, y1, x2, y2 = int(roi.topLeft().x), int(roi.topLeft().y), int(roi.bottomRight().x), int(roi.bottomRight().y)

            cv2.putText(frame, f"ID: {t.id}", (x1 + 10, y1 + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5, (255, 0, 0))
            cv2.putText(frame, f"Status: {t.status.name}", (x1 + 10, y1 + 35), cv2.FONT_HERSHEY_TRIPLEX, 0.5, (255, 0, 0))
            cv2.rectangle(frame, (x1, y1), (x2, y2), (255, 0, 0), 2)
        
        cv2.imshow("tracker", frame)

        if cv2.waitKey(1) == ord('q'): raise KeyboardInterrupt()

#Creat pipeline
pipeline = depthai.Pipeline()
with pipeline:
    image = node.ColorCamera()
    detections, passtrought = node.MobileNetDetectionNetwork(
        image.preview,blob_path=blobconverter.from_zoo(name='mobilenet-ssd', shaves=6))
    tracklets = node.ObjectTracker(
        inputTrackerFrame=image.preview,inputDetectionFrame=passtrought,inputDetections = detections)
    ObjectTrackingNode(img_frame = image.preview, tracklets_data = tracklets)
depthai.run(pipeline)