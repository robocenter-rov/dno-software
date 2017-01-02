reg_except(0, NONE, "Everything is ok")

//UDPConnectionProvider

reg_except(100, CP_CANT_TAKE_ADRESS_FROM_DHCP, "Can't take adress from DHCP")
reg_except(101, CP_MESSAGE_TOO_LARGE, "Message too large to fit in buffer")

//StdCommunicator

reg_except(200, SC_NOT_CONNECTED, "Communicator doesn't have connection")
reg_except(201, SC_MESSAGE_WRONG_SIZE, "Message size does not match with message type")

reg_except(202, SC_MESSAGE_WRONG_TYPE_ID, "Unknown message type id")
reg_except(203, SC_RECEIVER_NOT_SETTED, "Receiver function is not setted")

//TaskPool

reg_except(300, TP_FULL, "Can't add task to pool: pool is full")
reg_except(301, TP_RESOURCE_LOCKED, "Can't add task to pool: resource locked")
