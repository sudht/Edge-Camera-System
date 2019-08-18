import paho.mqtt.client as mqtt
import base64
import math
import random, string
import simplejson as json
import picamera
from time import sleep

#packet_size=3000

def on_publish(mosq, userdata, mid):
    mosq.disconnect()

def convertImageToBase64():
    with open("Pub.jpg", "rb") as image_file:
        encoded = base64.b64encode(image_file.read())
#    print(type(encoded))
    return encoded

# def randomword(length):
#         return ''.join(random.choice(string.ascii_lowercase) for i in range(length))

# def publishEncodedImage():
#         with open("b.jpg", "rb") as image_file:
#                 encoded = base64.b64encode(image_file.read())

#         end = packet_size
#         start = 0
#         length = len(encoded)
#         picId = randomword(8)
#         pos = 0
#         no_of_packets = math.ceil(length/packet_size)

#         while start <= len(encoded):
#                 data = {"data": encoded[start:end], "pic_id":picId, "pos": pos, "size": no_of_packets}
#                 client.publish("image",json.JSONEncoder().encode(data),0)
#                 end += packet_size
#                 start += packet_size
#                 pos = pos +1

camera = picamera.PiCamera()
camera.rotation = 90

try:
        camera.start_preview()
        sleep(1)
        camera.capture('Pub.jpg', resize=(500,281))
        camera.stop_preview()
        pass
finally:
        camera.close()
        
        client = mqtt.Client()
client.connect("192.168.101.78", 1883, 60)
client.on_publish = on_publish

f = convertImageToBase64()
client.publish("image",f,0)

client.loop_forever()
