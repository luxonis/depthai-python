import PySimpleGUI as sg
import threading
import depthai as dai

class IrHandler:
    def __init__(self):
        self.running = False
        self.thread = None

        self.strobe_opts = {'level': 0, 'edge': 1}
        self.strobe_type = 0  # default 0
        self.mode_opts = {'standby': 0, 'irdrive': 1, 'torch': 2, 'flash': 3}
        self.mode = 1     # default 0
        self.enable = {
            'Led1': 0,
            'Led2': 0,
            'TxPin': 0,
            'Strobe':  0,
            'TorchTempPin': 0,
        }

        # IVFM Register (0x02)
        self.uvlo_en = 0  # default 0
        self.ivfm_level = 0  # 0..7 = 2.9..3.6V, default 0
        self.ivfm_hyst_en = 0  # 1 = 50mV, default 0
        self.ivfm_mode = 1  # 0-disabled, 1-stop_and_hold, 2-down_mode, 3-up_and_down, default 1

        # LED1/LED2 Flash/Torch brightness Register (0x03,0x04,0x05,0x06)
        # Note: LED2 override (set together with LED1) not enabled
        # Set raw values (0..127)
        self.brightness = {
            'FlashLed1': 0,
            'FlashLed2': 0,
            'TorchLed1': 0,
            'TorchLed2': 0,
        }

        # Boost Configuration Register (0x07)
        self.led_short_fault_detect_en = 1  # default 1
        self.boost_mode_opts = {'normal': 0, 'pass': 1}
        self.boost_mode = 0  # default 0
        self.boost_freq_opts = {'2MHz': 0, '4MHz': 1}
        self.boost_freq = 0  # default 0
        self.boost_limit_opts = {'1.9A': 0, '2.8A': 1}
        self.boost_limit = 1  # default 1

        # Timing Configuration Register (0x08)
        self.torch_ramp_time = 1  # 0-no_ramp, 1=1ms, 2=32ms, 3=64ms,... 7=1024ms, default 1
        # Note a flash timeout event requires reconfig - not handled yet
        self.flash_timeout = 10  # 0..9 = 10..100ms, 10..15 = 150..400ms, default 10

        # TEMP Register (0x09)
        self.torch_polarity = 0  # 0-high, 1-low, default 0
        self.ntc_open_fault_en = 0  # default 0
        self.ntc_short_fault_en = 0  # default 0
        self.temp_volt_thresh = 4  # 0..7 = 0.2..0.9V, default 4 = 0.6V
        self.torch_temp_sel = 0  # 0-torch, 1-temp, default 0

    def set_device(self, device: dai.Device):
        self.dev = device
        self.idx = 1
        self.running = True
        self.thread = threading.Thread(target=self._run_gui, args=(), daemon=True)
        self.thread.start()

    def is_flash_en(self) -> bool:
        return self.mode in (1, 3)

    def is_torch_en(self) -> bool:
        return self.mode == 2

    def brightness_text(self, modeLed) -> str:
        v = self.brightness[modeLed]
        if modeLed.startswith('Flash'):
            return '{:6.1f} mA'.format(v * 11.725 + 10.9) \
                + ('' if self.is_flash_en() else ' - Disabled')
        else:
            return '{:5.1f} mA'.format(v * 1.4 + 0.977) \
                + ('' if self.is_torch_en() else ' - Disabled')
    def flash_timeout_text(self) -> str:
        return str((10*(self.flash_timeout+1)) if (self.flash_timeout<10) else (50*(self.flash_timeout-7))) + ' ms'

    def set_enable(self) -> None:
        v = (self.enable['TxPin'] << 7 | self.strobe_type << 6 | self.enable['Strobe'] << 5
            | self.enable['TorchTempPin'] << 4 | self.mode << 2 | self.enable['Led2'] << 1 | self.enable['Led1'])
        self.irWriteReg(0x01, v)
    def set_ivfm(self) -> None:
        v = self.uvlo_en << 6 | self.ivfm_level << 3 | self.ivfm_hyst_en << 2 | self.ivfm_mode
        self.irWriteReg(0x02, v)
    def set_brightness(self, modeLed) -> None:
        regs = {
            'FlashLed1': 0x03,
            'FlashLed2': 0x04,
            'TorchLed1': 0x05,
            'TorchLed2': 0x06,
        }
        self.irWriteReg(regs[modeLed], self.brightness[modeLed])
    def set_boost(self) -> None:
        v = self.led_short_fault_detect_en << 3 | self.boost_mode << 2 | self.boost_freq << 1 | self.boost_limit
        self.irWriteReg(0x07, v)
    def set_timing(self) -> None:
        v = self.torch_ramp_time << 4 | self.flash_timeout
        self.irWriteReg(0x08, v)
    def set_temp(self) -> None:
        v = (self.torch_polarity << 6 | self.ntc_open_fault_en << 5 | self.ntc_short_fault_en << 4
            | self.temp_volt_thresh << 1 | self.torch_temp_sel)
        self.irWriteReg(0x09, v)

    def _run_gui(self) -> None:
        # Enable Register (0x01)
        self.set_ivfm()
        self.set_brightness('FlashLed1')
        self.set_brightness('FlashLed2')
        self.set_brightness('TorchLed1')
        self.set_brightness('TorchLed2')
        self.set_boost()
        self.set_timing()
        self.set_temp()
        self.set_enable()

        # Create GUI
        sg.theme('Default1')

        layoutEnable = [
            [sg.Frame('Mode', [
                [sg.Radio('Standby',  'Rmode', key='standby', default=(self.mode==0), enable_events=True)],
                [sg.Radio('IR Drive', 'Rmode', key='irdrive', default=(self.mode==1), enable_events=True)],
                [sg.Radio('Torch',    'Rmode', key='torch',   default=(self.mode==2), enable_events=True)],
                [sg.Radio('Flash',    'Rmode', key='flash',   default=(self.mode==3), enable_events=True)],
                ]),
            sg.Column([
                [sg.Checkbox('TX Pin Enable', key='TxPin', default=self.enable['TxPin'], enable_events=True)],
                [sg.Checkbox('TORCH/TEMP Pin Enable', key='TorchTempPin', default=self.enable['TorchTempPin'], enable_events=True)],
                [sg.Checkbox('STROBE Enable:', key='Strobe', default=self.enable['Strobe'], enable_events=True),
                sg.Radio('Level', 'Rstr', key='level', default=(self.strobe_type==0), enable_events=True),
                sg.Radio('Edge',  'Rstr', key='edge',  default=(self.strobe_type==1), enable_events=True),
                ],
            ]),
            ],
        ]

        layoutBoostTiming = [
            [sg.Frame('Boost', [
                [sg.Text('Mode:'),
                sg.Radio('Normal',    'RBm', key='normal', default=(self.boost_mode==0), enable_events=True),
                sg.Radio('Pass only', 'RBm', key='pass',   default=(self.boost_mode==1), enable_events=True)],
                [sg.Text('Frequency:'),
                sg.Radio('2 MHz', 'RBfreq', key='2MHz', default=(self.boost_freq==0), enable_events=True),
                sg.Radio('4 MHz', 'RBfreq', key='4MHz', default=(self.boost_freq==1), enable_events=True)],
                [sg.Text('Current limit:'),
                sg.Radio('1.9 A', 'RBlimit', key='1.9A', default=(self.boost_limit==0), enable_events=True),
                sg.Radio('2.8 A', 'RBlimit', key='2.8A', default=(self.boost_limit==1), enable_events=True)],
                ]),
            sg.Frame('Flash timeout duration', [
                [sg.Slider(range=(0, 15), default_value=self.flash_timeout, orientation='h',
                        enable_events=True, key='FlashTimeout')],
                [sg.Text(self.flash_timeout_text(), key='TextFlashTimeout')],
                ]),
            ],
        ]

        layoutLed1 = [
            [sg.Checkbox('Enable', default=self.enable['Led1'], enable_events=True, key='Led1')],
            [sg.Text('Flash:'),
            sg.Slider(range=(0, 127), default_value=self.brightness['FlashLed1'], orientation='h',
                    size=(25, 18), enable_events=True, key='FlashLed1'),
            sg.Text(self.brightness_text('FlashLed1'), key='TextFlashLed1')
            ],
            [sg.Text('Torch:'),
            sg.Slider(range=(0, 127), default_value=self.brightness['TorchLed1'], orientation='h',
                    size=(25, 18), enable_events=True, key='TorchLed1'),
            sg.Text(self.brightness_text('TorchLed1'), key='TextTorchLed1')
            ],
        ]

        layoutLed2 = [
            [sg.Checkbox('Enable', default=self.enable['Led2'], enable_events=True, key='Led2')],
            [sg.Text('Flash:'),
            sg.Slider(range=(0, 127), default_value=self.brightness['FlashLed2'], orientation='h',
                    size=(25, 18), enable_events=True, key='FlashLed2'),
            sg.Text(self.brightness_text('FlashLed2'), key='TextFlashLed2')
            ],
            [sg.Text('Torch:'),
            sg.Slider(range=(0, 127), default_value=self.brightness['TorchLed2'], orientation='h',
                    size=(25, 18), enable_events=True, key='TorchLed2'),
            sg.Text(self.brightness_text('TorchLed2'), key='TextTorchLed2')
            ],
        ]

        layoutRegCtrl = [
            [sg.Text('Reg:'),   sg.InputText('0x', size=(10,1), key='InRegW'),
            sg.Text('Value:'), sg.InputText('0x', size=(10,1), key='InValW'),
            sg.Button('Write')],
            [sg.Text('Reg:'),   sg.InputText('0x', size=(10,1), key='InRegR'),
            sg.Button('Read'), sg.Text('...', key='TextValR')],
        ]

        layoutMain = [
            layoutEnable,
            layoutBoostTiming,
            [sg.Frame('LED1 / Flood IR', layoutLed1)],
            [sg.Frame('LED2 / Laser Pattern Projector', layoutLed2)],
            [sg.Frame('LM3644 Register Control', layoutRegCtrl)],
        ]

        window = sg.Window('IR Control', layoutMain)

        while self.running:
            event, values = window.read(timeout=100)
            #print('==EV', event, values)
            if event == sg.WIN_CLOSED:
                break
            if event == 'Write':
                reg = int(values['InRegW'], 0)
                val = int(values['InValW'], 0)
                self.irWriteReg(reg, val)
            if event == 'Read':
                reg = int(values['InRegR'], 0)
                val = hex(self.irReadReg(reg))
                window['TextValR'].update(val)
            if event in ('FlashLed1', 'FlashLed2', 'TorchLed1', 'TorchLed2'):
                self.brightness[event] = int(values[event])
                window['Text' + event].update(self.brightness_text(event))
                self.set_brightness(event)
            if event == 'FlashTimeout':
                self.flash_timeout = int(values['FlashTimeout'])
                window['TextFlashTimeout'].update(self.flash_timeout_text())
                self.set_timing()
            if event in ('Led1', 'Led2', 'Strobe', 'TxPin', 'TorchTempPin'):
                self.enable[event] = values[event]
                self.set_enable()
            if event in ('standby', 'irdrive', 'torch', 'flash'):
                self.mode = self.mode_opts[event]
                self.set_enable()
                for e in ('FlashLed1', 'FlashLed2', 'TorchLed1', 'TorchLed2'):
                    window['Text' + e].update(self.brightness_text(e))
            if event in ('level', 'edge'):  self.strobe_type = self.strobe_opts[event]; self.set_enable()
            if event in ('normal', 'pass'): self.boost_mode  = self.boost_mode_opts[event];  self.set_boost()
            if event in ('2MHz', '4MHz'):   self.boost_freq  = self.boost_freq_opts[event];  self.set_boost()
            if event in ('1.9A', '2.8A'):   self.boost_limit = self.boost_limit_opts[event]; self.set_boost()

        window.close()

    def irWriteReg(self, reg, value):
        print(f'===== Op{self.idx:3}: dev.irWriteReg({hex(reg)}, {hex(value)})')
        self.dev.customRPC('irWriteReg', reg, value)
        self.idx += 1

    def irReadReg(self, reg):
        value = self.dev.customRPC('irReadReg', reg)
        print(f'===== Op{self.idx:3}: dev.irReadReg({hex(reg)}) -> {hex(value)}')
        self.idx += 1
        return value

    def getID(self) -> str:
        return self.dev.getMxId()

    def close(self):
        self.running = False
        if self.thread:
            self.thread.join()

