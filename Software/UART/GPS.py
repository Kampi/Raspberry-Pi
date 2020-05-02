import time
import serial

if(__name__ == "__main__"):
    UART = serial.Serial("COM3", 9600)

    try:
        if(not(UART.isOpen())):
            UART.open()
    except:
        print("[ERROR] Can not open serial port!")
        exit()

    while(True):
        Datasets = dict()
        Message = UART.readline().decode("ascii").strip().split(",")
        if("$" in Message[0]):
            Datasets[Message[0]] = Message[1:]

        for Key in Datasets.keys():
            if("GGA" in Key):
                print("Use {} satellites".format(Datasets[Key][6]))
                if(Datasets[Key][5] == "1"):
                    print("Time: {}".format(Datasets[Key][0]))
                    print("Latitude: {} - {}".format(Datasets[Key][1], Datasets[Key][2]))
                    print("Longitude: {} - {}".format(Datasets[Key][3], Datasets[Key][4]))
                else:
                    print("GPS not fix!")
                    print("GPS quality: {}".format(Datasets[Key][5]))
            elif("GSV" in Key):
                print("Number: {}".format(Datasets[Key][1]))
                print("ID: {}".format(Datasets[Key][3]))
                print("Visible satellites: {}".format(Datasets[Key][2]))
                print("Elevation: {} degree".format(Datasets[Key][4]))
                print("Azimuth: {} degree".format(Datasets[Key][5]))
                print("SNR: {} dB".format(Datasets[Key][6]))