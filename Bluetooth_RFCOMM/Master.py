
from bluetooth import *

#######################################################
# Scan
#######################################################

target_name = "iot_test"   # target device name
target_address = "B8:27:EB:66:87:00"
port = 1         # RFCOMM port

nearby_devices = discover_devices()

# scanning for target device
#for bdaddr in nearby_devices:
#    print(lookup_name( bdaddr ))
#    if target_name == lookup_name( bdaddr ):
#        target_address = bdaddr
#        break

if target_address is not None:
    print('device found. target address %s' % target_address)
else:
    print('could not find target bluetooth device nearby')

#######################################################
# Connect
#######################################################

# establishing a bluetooth connection
try:
    sock=BluetoothSocket( RFCOMM )
    sock.connect((target_address, port))
#    print("input angle 1.5 ~ 12.5")

#    a = input()
#    sock.send(a)


#    print("send input")
#    sock.settimeout(3)
#    data_transferred = 0
    while True:
        print("input angle 1.5 ~ 12.5")
        angle = input()
        sock.send(angle)

        print("send input")
        sock.settimeout(3)
        data_transferred = 0
        try:

            data = sock.recv(1024)
            filename = "Sub.jpg"
            with open(filename, 'wb') as f:
                try:
                    while data:
                        f.write(data)
                        data_transferred += len(data)
                        data = sock.recv(1024)
                except Exception as e:
                    print(e)

            print('file [%s] end. KB [%d]' %(filename, data_transferred))
            sock.settimeout(None)

            # recv_data = sock.recv(1024)
            # print(recv_data)
            # sock.send("asda")

        except KeyboardInterrupt:
            print("disconnected")
            sock.close()
            print("all done")
except btcommon.BluetoothError as err:
    print('An error occurred : %s ' % err)
    pass

