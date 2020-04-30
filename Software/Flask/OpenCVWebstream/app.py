from OpenCVWebstream import OpenCVWebstream

IsActive = True

def onStop():
    global IsActive

    IsActive = False

if(__name__ == "__main__"):
    app = OpenCVWebstream(onStop = onStop)
    app.Run()

    while(IsActive):
        pass

    print("[INFO] Exit application...")

    exit(0)