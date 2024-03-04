from typing import List
from PyQt5 import QtCore, QtGui, QtWidgets
import depthai as dai
import sys
import signal
import os
import psutil


class CamTestGui:

    CAM_SOCKET_OPTIONS = ["rgb", "left", "right", "camd"]
    CAM_TYPE_OPTIONS = ["color", "mono"]
    MONO_RESOLUTION_OPTIONS = ["400", "480",  "720", "800"]
    COLOR_RESOLUTION_OPTIONS = ["720", "800",
                                "1080", "1200", "4k", "5mp", "12mp", "48mp"]
    ROTATE_OPTIONS = ["disabled", "all", "rgb", "mono"]
    DEPTHAI_CONNECT_TIMEOUT_DEFAULT = 30000
    DEPTHAI_BOOTUP_TIMEOUT_DEFAULT = 30000

    def remove_camera(self, layout: QtWidgets.QHBoxLayout):
        for i in reversed(range(layout.count())):
            layout.itemAt(i).widget().setParent(None)
        self.cameras_list.removeItem(layout)

    def add_camera(self, camera: str = None, camera_type: str = None):
        layout = QtWidgets.QHBoxLayout()
        cam_combo = QtWidgets.QComboBox()
        cam_combo.addItems(self.CAM_SOCKET_OPTIONS)
        cam_combo.setCurrentIndex(
            self.CAM_SOCKET_OPTIONS.index(camera) if camera else 0)
        layout.addWidget(cam_combo)
        cam_type_combo = QtWidgets.QComboBox()
        cam_type_combo.addItems(self.CAM_TYPE_OPTIONS)
        cam_type_combo.setCurrentIndex(
            self.CAM_TYPE_OPTIONS.index(camera_type) if camera_type else 0)
        layout.addWidget(cam_type_combo)
        self.cameras_list.addLayout(layout)
        remove_button = QtWidgets.QPushButton("Remove")
        remove_button.clicked.connect(lambda: self.remove_camera(layout))
        layout.addWidget(remove_button)

    def set_default_cameras(self):
        self.add_camera("rgb", "color")
        self.add_camera("left", "mono")
        self.add_camera("right", "mono")
        self.add_camera("camd", "color")

    def __init__(self, app: "Application"):
        self.app = app
        self.app.setWindowTitle("Camera Test")
        self.main_widget = QtWidgets.QWidget()
        self.scroll_widget = QtWidgets.QScrollArea()
        self.scroll_widget.setWidget(self.main_widget)
        self.scroll_widget.setWidgetResizable(True)
        self.scroll_widget.setMinimumHeight(500)
        self.main_layout = QtWidgets.QVBoxLayout()
        self.main_widget.setLayout(self.main_layout)
        self.app.setCentralWidget(self.scroll_widget)
        self.label_cameras = QtWidgets.QLabel("Cameras")
        self.main_layout.addWidget(self.label_cameras)

        self.cameras_list = QtWidgets.QVBoxLayout()
        self.main_layout.addLayout(self.cameras_list)
        self.set_default_cameras()

        self.add_cam_button = QtWidgets.QPushButton("Add Camera")
        self.add_cam_button.clicked.connect(self.add_camera)
        self.main_layout.addWidget(self.add_cam_button)

        self.mono_resolution_label = QtWidgets.QLabel("Mono Resolution")
        self.main_layout.addWidget(self.mono_resolution_label)
        self.mono_resolution_combo = QtWidgets.QComboBox()
        self.mono_resolution_combo.addItems(self.MONO_RESOLUTION_OPTIONS)
        self.main_layout.addWidget(self.mono_resolution_combo)
        self.mono_resolution_combo.setCurrentIndex(3)

        self.label_color_resolution = QtWidgets.QLabel("Color Resolution")
        self.main_layout.addWidget(self.label_color_resolution)
        self.combo_color_resolution = QtWidgets.QComboBox()
        self.combo_color_resolution.addItems(self.COLOR_RESOLUTION_OPTIONS)
        self.main_layout.addWidget(self.combo_color_resolution)
        self.combo_color_resolution.setCurrentIndex(2)

        self.label_rotate = QtWidgets.QLabel("Rotate")
        self.main_layout.addWidget(self.label_rotate)
        self.combo_rotate = QtWidgets.QComboBox()
        self.combo_rotate.addItems(self.ROTATE_OPTIONS)
        self.main_layout.addWidget(self.combo_rotate)

        self.label_fps = QtWidgets.QLabel("FPS")
        self.main_layout.addWidget(self.label_fps)
        self.spin_fps = QtWidgets.QSpinBox()
        self.spin_fps.setMinimum(1)
        self.spin_fps.setMaximum(120)
        self.spin_fps.setValue(30)
        self.main_layout.addWidget(self.spin_fps)

        self.label_isp3afps = QtWidgets.QLabel("ISP3 AFPS")
        self.main_layout.addWidget(self.label_isp3afps)
        self.spin_isp3afps = QtWidgets.QSpinBox()
        self.spin_isp3afps.setMinimum(0)
        self.spin_isp3afps.setMaximum(120)
        self.spin_isp3afps.setValue(0)
        self.main_layout.addWidget(self.spin_isp3afps)

        self.label_isp_downscale = QtWidgets.QLabel("ISP Downscale")
        self.main_layout.addWidget(self.label_isp_downscale)
        self.spin_isp_downscale = QtWidgets.QSpinBox()
        self.spin_isp_downscale.setMinimum(1)
        self.spin_isp_downscale.setMaximum(4)
        self.spin_isp_downscale.setValue(1)
        self.main_layout.addWidget(self.spin_isp_downscale)

        self.label_resizable_windows = QtWidgets.QLabel("Resizable Windows")
        self.main_layout.addWidget(self.label_resizable_windows)
        self.check_resizable_windows = QtWidgets.QCheckBox()
        self.main_layout.addWidget(self.check_resizable_windows)

        self.label_camera_tuning = QtWidgets.QLabel("Camera Tuning")
        self.main_layout.addWidget(self.label_camera_tuning)
        self.camera_tuning_path = QtWidgets.QLineEdit()
        self.main_layout.addWidget(self.camera_tuning_path)

        self.label_connect_timeout = QtWidgets.QLabel("Connect Timeout (ms)")
        self.main_layout.addWidget(self.label_connect_timeout)
        self.spin_connect_timeout = QtWidgets.QSpinBox()
        self.spin_connect_timeout.setMinimum(1)
        self.spin_connect_timeout.setMaximum(60000)
        self.spin_connect_timeout.setValue(
            self.DEPTHAI_CONNECT_TIMEOUT_DEFAULT)
        self.main_layout.addWidget(self.spin_connect_timeout)

        self.label_boot_timeout = QtWidgets.QLabel("Bootup Timeout (ms)")
        self.main_layout.addWidget(self.label_boot_timeout)
        self.spin_boot_timeout = QtWidgets.QSpinBox()
        self.spin_boot_timeout.setMinimum(1)
        self.spin_boot_timeout.setMaximum(60000)
        self.spin_boot_timeout.setValue(self.DEPTHAI_BOOTUP_TIMEOUT_DEFAULT)
        self.main_layout.addWidget(self.spin_boot_timeout)

        self.label_available_devices = QtWidgets.QLabel("Available Devices")
        self.main_layout.addWidget(self.label_available_devices)
        self.available_devices_combo = QtWidgets.QComboBox()
        self.main_layout.addWidget(self.available_devices_combo)

        self.connect_layout = QtWidgets.QHBoxLayout()
        self.connect_button = QtWidgets.QPushButton("Connect")
        self.connect_button.clicked.connect(self.app.connect)
        self.connect_layout.addWidget(self.connect_button)
        self.check_auto_mode = QtWidgets.QCheckBox("Auto Mode")
        self.check_auto_mode.setToolTip(
            "Whenever a device is available, connect to it automatically")
        self.connect_layout.addWidget(self.check_auto_mode)
        self.check_auto_mode.setChecked(False)
        self.main_layout.addLayout(self.connect_layout)

        self.disconnect_button = QtWidgets.QPushButton("Disconnect")
        self.disconnect_button.clicked.connect(self.app.disconnect)
        self.main_layout.addWidget(self.disconnect_button)
        self.disconnect_button.setHidden(True)

    def handle_automode_changed(self, state):
        self.disconnect_button.setHidden(bool(state))
        self.connect_button.setHidden(bool(state))

    def handle_disconnect(self):
        self.available_devices_combo.clear()
        if not self.check_auto_mode.isChecked():
            self.connect_button.setDisabled(True)
            self.disconnect_button.setDisabled(True)
            self.disconnect_button.setHidden(True)
            self.connect_button.setHidden(False)
        self.add_cam_button.setDisabled(False)
        self.mono_resolution_combo.setDisabled(False)
        self.combo_color_resolution.setDisabled(False)
        self.combo_rotate.setDisabled(False)
        self.spin_fps.setDisabled(False)
        self.spin_isp3afps.setDisabled(False)
        self.spin_isp_downscale.setDisabled(False)
        self.check_resizable_windows.setDisabled(False)
        self.camera_tuning_path.setDisabled(False)
        self.available_devices_combo.setDisabled(False)
        for i in range(self.cameras_list.count()):
            self.cameras_list.itemAt(i).itemAt(0).widget().setDisabled(False)
            self.cameras_list.itemAt(i).itemAt(1).widget().setDisabled(False)
            self.cameras_list.itemAt(i).itemAt(2).widget().setDisabled(False)
        self.spin_connect_timeout.setDisabled(False)
        self.spin_boot_timeout.setDisabled(False)

    def handle_connect(self):
        self.spin_boot_timeout.setDisabled(True)
        self.spin_connect_timeout.setDisabled(True)
        if not self.check_auto_mode.isChecked():
            self.connect_button.setDisabled(True)
            self.disconnect_button.setDisabled(False)
            self.disconnect_button.setHidden(False)
            self.connect_button.setHidden(True)
        self.add_cam_button.setDisabled(True)
        self.mono_resolution_combo.setDisabled(True)
        self.combo_color_resolution.setDisabled(True)
        self.combo_rotate.setDisabled(True)
        self.spin_fps.setDisabled(True)
        self.spin_isp3afps.setDisabled(True)
        self.spin_isp_downscale.setDisabled(True)
        self.check_resizable_windows.setDisabled(True)
        self.camera_tuning_path.setDisabled(True)
        self.available_devices_combo.setDisabled(True)
        for i in range(self.cameras_list.count()):
            self.cameras_list.itemAt(i).itemAt(0).widget().setDisabled(True)
            self.cameras_list.itemAt(i).itemAt(1).widget().setDisabled(True)
            self.cameras_list.itemAt(i).itemAt(2).widget().setDisabled(True)


class WorkerSignals(QtCore.QObject):
    finished = QtCore.pyqtSignal(list)


class Worker(QtCore.QRunnable):

    def __init__(self, fn, *args, **kwargs):
        super().__init__()
        self.fn = fn
        self.args = args
        self.kwargs = kwargs
        self.signals = WorkerSignals()

    @QtCore.pyqtSlot()
    def run(self):
        result = self.fn(*self.args, **self.kwargs)
        self.signals.finished.emit(result)


class Application(QtWidgets.QMainWindow):

    def __init__(self):
        super().__init__()
        self.available_devices: List[dai.DeviceInfo] = []
        self.ui = CamTestGui(self)
        self.query_devices_timer = QtCore.QTimer()
        self.query_devices_timer.timeout.connect(self.query_devices)
        self.query_devices_timer.start(2000)
        self.query_devices()

        self.test_process_pid = None

        # Once the test process is started, periodically check if it's still running (catches eg. camera unplugged)
        self.check_test_process_timer = QtCore.QTimer()
        self.check_test_process_timer.timeout.connect(self.check_test_process)

        self.ui.check_auto_mode.stateChanged.connect(self.automode_changed)

    def closeEvent(self, a0: QtGui.QCloseEvent) -> None:
        if self.test_process_pid:
            os.kill(self.test_process_pid, signal.SIGINT)
        return super().closeEvent(a0)

    def automode_changed(self, state):
        self.ui.handle_automode_changed(state)
        if not state:
            self.disconnect()

    def construct_args_from_gui(self) -> List[str]:
        if not self.available_devices:
            return []
        self.device = self.available_devices[self.ui.available_devices_combo.currentIndex(
        )].mxid
        cmd = []
        cmd.append("--cameras")
        for i in range(self.ui.cameras_list.count()):
            hbox = self.ui.cameras_list.itemAt(i)
            cam_combo = hbox.itemAt(0)
            cam_type_combo = hbox.itemAt(1)
            cam = cam_combo.widget().currentText()
            cam_type = cam_type_combo.widget().currentText()
            cmd.append(f"{cam},{cam_type[0]}")
        cmd.append("-mres")
        cmd.append(self.ui.mono_resolution_combo.currentText())
        cmd.append("-cres")
        cmd.append(self.ui.combo_color_resolution.currentText())
        if self.ui.combo_rotate.currentText() != "disabled":
            cmd.append("-rot")
            cmd.append(self.ui.combo_rotate.currentText())
        cmd.append("-fps")
        cmd.append(str(self.ui.spin_fps.value()))
        cmd.append("-isp3afps")
        cmd.append(str(self.ui.spin_isp3afps.value()))
        cmd.append("-ds")
        cmd.append(str(self.ui.spin_isp_downscale.value()))
        if self.ui.check_resizable_windows.isChecked():
            cmd.append("-rs")
        if self.ui.camera_tuning_path.text():
            cmd.append("-tun")
            cmd.append(self.ui.camera_tuning_path.text())
        cmd.append("--device")
        cmd.append(self.device)
        cmd.append("--connection-timeout")
        cmd.append(str(self.ui.spin_connect_timeout.value()))
        cmd.append("--boot-timeout")
        cmd.append(str(self.ui.spin_boot_timeout.value()))
        return cmd

    def check_test_process(self):
        if self.test_process_pid and psutil.pid_exists(self.test_process_pid):
            return
        self.test_process_pid = None
        self.disconnect()
        self.check_test_process_timer.stop()

    def connect(self):
        args = self.construct_args_from_gui()
        if not args:
            return
        
        started_successfully = False
        self.test_process = QtCore.QProcess()
        # Start detached process with the function that also returns the PID
        if getattr(sys, 'frozen', False):
            started_successfully, self.test_process_pid = self.test_process.startDetached(
                sys.executable, args, "")
        else:
            if "-gui" in sys.argv:
                sys.argv.remove("-gui")
            started_successfully, self.test_process_pid = self.test_process.startDetached(
                sys.executable, sys.argv + args, "")
        if not started_successfully:
            self.test_process_pid = None
            self.disconnect()
            return
        self.query_devices_timer.stop()
        self.check_test_process_timer.start(1000)
        self.ui.handle_connect()

    def disconnect(self):
        if self.test_process_pid:
            try:
                os.kill(self.test_process_pid, signal.SIGINT)
            except OSError:
                self.test_process_pid = None
        self.test_process_pid = None
        self.query_devices_timer.start()
        self.ui.handle_disconnect()

    def query_devices(self):
        self.query_devices_timer.stop()
        pool = QtCore.QThreadPool.globalInstance()
        query_devices_worker = Worker(dai.Device.getAllAvailableDevices)
        query_devices_worker.signals.finished.connect(
            self.on_finish_query_devices)
        pool.start(query_devices_worker)

    def on_finish_query_devices(self, result):
        current_device = self.ui.available_devices_combo.currentText()
        self.ui.available_devices_combo.clear()
        self.available_devices = result
        self.ui.available_devices_combo.addItems(
            list(map(lambda d: f"{d.name} ({d.getMxId()})", self.available_devices)))
        self.query_devices_timer.start()
        if self.available_devices:
            if current_device:
                index = self.ui.available_devices_combo.findText(
                    current_device)
                if index != -1:
                    self.ui.available_devices_combo.setCurrentIndex(index)
            if self.ui.check_auto_mode.isChecked():
                self.connect()
            self.ui.connect_button.setDisabled(False)
        else:
            self.ui.connect_button.setDisabled(True)


def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    app = QtWidgets.QApplication(sys.argv)
    application = Application()
    application.show()
    sys.exit(app.exec_())
