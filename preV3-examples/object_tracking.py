import blobconverter
import cv2
import depthai as dai

class Viewer(dai.Node):
    def __run__(self,
                img_frame: dai.message.ImgFrame,
                tracklets: dai.message.Tracklets):
        if cv2.waitKey(1) == ord('q'): raise KeyboardInterrupt()

        frame = img_frame.getCvFrame()
        for t in tracklets.tracklets:
            roi = t.roi.denormalize(frame.shape[1], frame.shape[0])
            x1, y1, x2, y2 = map(int, (
                roi.topLeft().x, 
                roi.topLeft().y, 
                roi.bottomRight().x, 
                roi.bottomRight().y))
            cv2.rectangle(frame, (x1, y1), (x2, y2), (255, 0, 0), 2)

            def text(text, pos):
                cv2.putText(frame, text, pos, 
                           cv2.FONT_HERSHEY_TRIPLEX, 
                           0.5, 
                           (255, 0, 0))
            text(f"ID: {t.id}", (x1 + 10, y1 + 20))
            text(f"Status: {t.status.name}", (x1 + 10, y1 + 35))
        
        cv2.imshow("Object tracking", frame)

pipeline = dai.Pipeline()
with pipeline:
    image = dai.node.ColorCamera().preview
    detections, passtrough = dai.node.MobileNetDetectionNetwork(image,
        blob_path = blobconverter.from_zoo(name='mobilenet-ssd', shaves=6))
    tracklets = dai.node.ObjectTracker(
        input_tracker_frame = image,
        input_detection_frame = passtrough,
        input_detections = detections)
    Viewer(image, tracklets)
dai.run(pipeline)
