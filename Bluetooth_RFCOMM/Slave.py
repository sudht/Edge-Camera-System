from bluetooth import *
import picamera
import struct
from time import sleep
import RPi.GPIO as GPIO

# input pin Number
PanPin = panN
TiltPin = tiltN

GPIO.setmode(GPIO.BCM)
GPIO.setup(PanPin, GPIO.OUT)
GPIO.setup(TiltPin, GPIO.OUT)

PanPWM = GPIO.PWM(PanPin, 50)
TiltPWM = GPIO.PWM(TiltPin, 50)
PanPWM.start(7.5)
TiltPWM.start(7.5)

GPIO.setup(PanPin, GPIO.IN)
GPIO.setup(TiltPin, GPIO.IN)

def servoCtl(data):
    angle = data.split()
    PanAngle = float(angle[0])
    TiltAngle = float(angle[1])
#    GPIO.cleanup()
    PanPWM.ChangeDutyCycle(PanAngle)
    TiltPWM.ChangeDutyCycle(TiltAngle)
    sleep(0.2)
    GPIO.setup(PanPin, GPIO.IN)
    GPIO.setup(TiltPin, GPIO.IN)

def captureImg():
    camera = picamera.PiCamera()
    camera.rotation = 180

    try:
        camera.start_preview()
        sleep(1)
        camera.capture('Pub.jpg', resize=(500,281))
        camera.stop_preview()
        pass
    finally:
        camera.close()

#     with open("Pub.jpg", "rb") as image_file:
#         encoded = base64.b64encode(image_file.read())
# #    print(type(encoded))
#     return encoded


def receiveMsg():
    uuid = "94f39d29-7d6d-437d-973b-fba39e49d4ee"

    server_sock=BluetoothSocket( RFCOMM )
    server_sock.bind(('',PORT_ANY))
    server_sock.listen(1)

    port = server_sock.getsockname()[1]

    advertise_service( server_sock, "BtChat",
            service_id = uuid,
            service_classes = [ uuid, SERIAL_PORT_CLASS ],
            profiles = [ SERIAL_PORT_PROFILE ] )

    print("Waiting for connection : channel %d" % port)
    client_sock, client_info = server_sock.accept()
    print('accepted')
    while True:
        print("Accepted connection from ", client_info)
        try:
            data = client_sock.recv(1024)
            if len(data) == 0: break
            print("received [%s]" % data)

            print(data)

#            if 'img' in data:
#                convertImageToBase64()

            servoCtl(data)
            captureImg()
            data_transferred = 0

            filename = "Pub.jpg"
            print('file [%s] start...' %filename)
            with open(filename, "rb") as image_file:
                try:
                    data = image_file.read(1024)
                    while data:
                        data_transferred += client_sock.send(data)
                        data = image_file.read(1024)
                except Exception as e:
                    print(e)

            print('end [%s], KB [%d]' %(filename,data_transferred))

        except IOError:
            print("disconnected")
            client_sock.close()
            server_sock.close()
            print("all done")
            break

        except KeyboardInterrupt:
            print("disconnected")
            client_sock.close()
            server_sock.close()
            print("all done")
            break

receiveMsg()
