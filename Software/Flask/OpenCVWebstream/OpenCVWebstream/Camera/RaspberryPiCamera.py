import io
import time
import threading

from picamera import PiCamera
from picamera.array import PiRGBArray

class Camera():
    def __init__(self, Delay = 1):
        self.__Worker = None
        self.__Frame = None
        self.__ImageReady = False
        self.__Delay = Delay
        self.__LastFrame = time.time()

    def Stop(self):
        if(self.__Worker is not None):
            self.__Worker.join()

    def GetFrame(self, Resolution):
        if((self.__Worker is None) and ((time.time() - self.__LastFrame) > self.__Delay)):
            self.__ImageReady = False
            self.__Worker = threading.Thread(target = self.__thread, args = (Resolution,))
            self.__Worker.start()

            # Wait until the frame is ready
            while(self.__ImageReady == False):
                pass

        return self.__Frame

    def __thread(self, Resolution):
        with PiCamera() as Device:

            RawCapture = PiRGBArray(Device)

            Device.resolution = Resolution
            Device.hflip = True
            Device.vflip = True

            Device.capture(RawCapture, format = "bgr")
            self.__Frame = RawCapture.array

            self.__ImageReady = True

            self.__LastFrame = time.time()

        self.__Worker = None