import can
import struct

def Can_RxMsg():
	flag = False
	can0 = can.interface.Bus(channel = 'can0', bustype = 'socketcan',bitrate=500000) #socketcan_native
	msg = can0.recv(10.0)
	if msg is not None:
		temp_field= msg.data[0:4]
		temp=struct.unpack('<f',temp_field)
		temp_formated="{:.2f}".format(float(temp[0]))
		print("Temperature " + str(temp_formated) + " from Msg ID " + str(hex(msg.arbitration_id)))
		if float(temp[0]) > 30:
			flag = True
	return flag


def Can_TxMsg(status_flag):
	bus = can.interface.Bus(channel = 'can0', bustype = 'socketcan',bitrate=500000) #socketcan_native
	Txmsg = can.Message(arbitration_id=0x13,
	data=[255, 255, 255, 255, 255, 255, 255, 255],
	is_extended_id=False)

	if status_flag:
		Txmsg.data[0] = 1
		print("Activation message sent with ID 0x13")
	else:
		Txmsg.data[0] = 0
	
	try:
		bus.send(Txmsg)
		print("Message sent")
	except can.CanError:
		print("Message NOT sent")

if __name__ == "__main__":
	while True:
		Temp_flag = Can_RxMsg()
		Can_TxMsg(Temp_flag)
		

