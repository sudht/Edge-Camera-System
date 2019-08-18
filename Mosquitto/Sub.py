import paho.mqtt.client as mqtt
import time
import base64

def on_message(client, userdata, message):
        m_decode = str(message.payload.decode("utf-8"))
        m_bytes = bytes(m_decode, encoding='utf8')
#       print("message received " ,m_decode)
        print("message topic=",message.topic)
        print("message qos=",message.qos)
        print("message retain flag=",message.retain)
        imgdata = base64.b64decode(m_bytes)
        filename = 'Sub.jpg'
        with open(filename, 'wb') as f:
                f.write(imgdata)
        print(type(m_bytes))

def on_log(client, userdata, level, buf):
        print("log: ",buf)


broker_address="localhost"

client = mqtt.Client("Sub1")
client.on_message=on_message
client.on_log=on_log

client.connect(broker_address)

client.loop_start()
client.subscribe("image")
client.publish("image","OFF")
time.sleep(10)
client.loop_stop()
