//
// Created by gratt on 2020-04-30.
//

#include "GPS.h"
GPS::GPS(PinName tx, PinName rx, uint32_t baud_rate) : m_uart(tx, rx, DEFAULT_BAUD_RATE) {
	rollingChecksumA = 0;
	rollingChecksumB = 0;
	ubxFrameCounter = 0;
	ignoreThisPayload = false;
	ack_flag = false;
	nack_flag = false;
	//TODO: verify that ACKs get read no matter what target class/id is selected
	target_class = UBX_CLASS_NAV; //default class target
	target_msgID = UBX_NAV_PVT; //default id target
	setSerialRate(baud_rate, COM_PORT_UART1);
	m_uart.baud(baud_rate);
	wait_ms(50); //ACK gets lost during baud rate change, so no way to verify here
	m_uart.attach(callback(this, &GPS::onByteReceive));
}

void GPS::configure() {
	//TODO: Determine GNSS constellation configuration (necessary? best config?)
	//allow uart1 to output UBX messages
	setPortOutput(COM_PORT_UART1, COM_TYPE_UBX);
	if (!waitForAck()) {
		//TODO: handle NACK response
	}
	setNavigationFrequency(NAV_FREQUENCY);
	if (!waitForAck()) {
		//TODO: handle NACK response
	}
	//configure UBX_NAV_PVT to output from uart 1 every navigation cycle
	configureMessage(UBX_CLASS_NAV, UBX_NAV_PVT, COM_PORT_UART1, 1);
	if (!waitForAck()) {
		//TODO: handle NACK response
	}
	//turn default NEMA messages off
	for (const uint8_t message : DEFAULT_ENABLED_NMEA) {
		configureMessage(UBX_CLASS_NMEA, message, COM_PORT_UART1, 0); //0 sendRate message off
		if (!waitForAck()) {
			//TODO: handle NACK response
		}
	}

}

bool GPS::waitForAck() {
	//TODO: need error checking for failed checksums, etc
	while (!ack_flag) { //wait for ack
		if (nack_flag) {
			nack_flag = false;
			return false;
		}
	}
	ack_flag = false;
	return true;
}

void GPS::onByteReceive() {
	process(m_uart.getc(), (activePacketBuffer == UBLOX_PACKET_PACKETCFG ? &packetCfg :
					   activePacketBuffer == UBLOX_PACKET_PACKETACK ? &packetAck : &packetBuf),
			target_class, target_msgID);
}

void GPS::sendSerialCommand(ubxPacket *outgoingUBX) {
	calcChecksum(outgoingUBX);
	//Write header bytes
	m_uart.putc(UBX_SYNCH_1); //μ
	m_uart.putc(UBX_SYNCH_2); //b
	m_uart.putc(outgoingUBX->cls);
	m_uart.putc(outgoingUBX->id);
	m_uart.putc(outgoingUBX->len & 0xFF); //LSB
	m_uart.putc(outgoingUBX->len >> 8);   //MSB

	//Write payload
	m_uart.write(outgoingUBX->payload, outgoingUBX->len, NULL);

	//Write checksum
	m_uart.putc(outgoingUBX->checksumA);
	m_uart.putc(outgoingUBX->checksumB);
}

//this whole processing chain is a mess, I did my best to port it from the sparkfun library
//ideally a total refactor can be done at some point
//lots of stuff in here we wont use, and lots that seems VERY fragile
//I have my doubts that this works at all honestly, but I can't say for sure until I test it
void GPS::process(uint8_t incoming, ubxPacket *incomingUBX, uint8_t requestedClass, uint8_t requestedID) {
	if (currentSentence == NONE) {
		if (incoming == 0xB5) //UBX binary frames start with 0xB5, aka μ
		{
			//This is the start of a binary sentence. Reset flags.
			//We still don't know the response class
			ubxFrameCounter = 0;
			currentSentence = UBX;
			packetBuf.counter = 0;
			ignoreThisPayload = false; //We should not ignore this payload - yet
			//Store data in packetBuf until we know if we have a requested class and ID match
			//Or we see that the data is an ACK
			activePacketBuffer = UBLOX_PACKET_PACKETBUF;
		} else {
			//TODO: handle failure case (missed sentence start)
		}
	}

	//non-payload bytes if/else chain
	if (((ubxFrameCounter == 0) && (incoming != 0xB5)) ||     //'μ'
			((ubxFrameCounter == 1) && (incoming != 0x62)))  //'b'
		currentSentence = NONE;        //Something went wrong. Reset.
	else if (ubxFrameCounter == 2) //Class
	{
		rollingChecksumA = 0; //Reset our rolling checksums
		rollingChecksumB = 0;
		packetBuf.counter = 0; //Reset the packetBuf.counter
		packetBuf.valid = UBLOX_PACKET_VALIDITY_NOT_DEFINED; // Reset the packet validity
		packetBuf.startingSpot = incomingUBX->startingSpot; //Copy the startingSpot
	} else if (ubxFrameCounter == 3) { //ID
		//We can now identify the type of response
		//If the packet we are receiving is not an ACK then check for a class and ID match
		if (packetBuf.cls != UBX_CLASS_ACK) {
			if ((packetBuf.cls == requestedClass) && (packetBuf.id == requestedID)) {
				//This is not an ACK and we have a class and ID match
				//So start diverting data into incomingUBX
				activePacketBuffer = UBLOX_PACKET_PACKETCFG;
				incomingUBX->cls = packetBuf.cls; //Copy fields into incomingUBX
				incomingUBX->id = packetBuf.id;
				incomingUBX->counter = packetBuf.counter;
			} else {
				//This is not an ACK and we do not have a class and ID match
				//so we do not care about the message contents and
				ignoreThisPayload = true;
			}
		}
	} else if (ubxFrameCounter == 6 && (activePacketBuffer == UBLOX_PACKET_PACKETBUF)) {
		//This should be the first byte of the payload unless .len is zero
		//Only execute this if we are not already processing a data packet
		if (packetBuf.len == 0) { // Check if length is zero (hopefully this is impossible!)
			//If length is zero (!) this will be the first byte of the checksum so record it
			packetBuf.checksumA = incoming;
		} else {
			//The length is not zero so record this byte in the payload
			packetBuf.payload[0] = incoming;
		}
	} else if (ubxFrameCounter == 7 && (activePacketBuffer == UBLOX_PACKET_PACKETBUF)) {
		//This should be the second byte of the payload unless .len is zero or one
		//Only execute this if we are not already processing a data packet
		if (packetBuf.len == 0) // Check if length is zero (hopefully this is impossible!)
		{
			//If length is zero (!) this will be the second byte of the checksum so record it
			packetBuf.checksumB = incoming;
		} else if (packetBuf.len == 1) {// Check if length is one
			//The length is one so this is the first byte of the checksum
			packetBuf.checksumA = incoming;
		} else { // Length is >= 2 so this must be a payload byte
			packetBuf.payload[1] = incoming;
		}
		// Now that we have received two payload bytes, we can check for a matching ACK/NACK
		if ((packetBuf.cls == UBX_CLASS_ACK)                  // if this is an ACK/NACK
				&& (packetBuf.payload[0] == requestedClass)  // and if the class matches
				&& (packetBuf.payload[1] == requestedID)) { // and if the ID matches
			if (packetBuf.len == 2) {
				// Then this is a matching ACK so copy it into packetAck
				activePacketBuffer = UBLOX_PACKET_PACKETACK;
				packetAck.cls = packetBuf.cls;
				packetAck.id = packetBuf.id;
				packetAck.len = packetBuf.len;
				packetAck.counter = packetBuf.counter;
				packetAck.payload[0] = packetBuf.payload[0];
				packetAck.payload[1] = packetBuf.payload[1];
			} else {
				//TODO: cover error if length is not 2 (may impact waitForAck)
			}
		}
	}

	//Process incoming data using the proper buffer
	if (activePacketBuffer == UBLOX_PACKET_PACKETACK) {                    //ACK/NACK
		processUBX(incoming, &packetAck, requestedClass, requestedID);
	} else if (activePacketBuffer == UBLOX_PACKET_PACKETCFG) {               //data packet
		processUBX(incoming, incomingUBX, requestedClass, requestedID);
	} else {                                                              //not yet known
		processUBX(incoming, &packetBuf, requestedClass, requestedID);
	}
	//increment the frame counter
	ubxFrameCounter++;
}

//Given a character, file it away into the uxb packet structure
//Set valid to VALID or NOT_VALID once sentence is completely received and passes or fails CRC
//The payload portion of the packet can be 100s of bytes but the max array
//size is MAX_PAYLOAD_SIZE bytes. startingSpot can be set so we only record
//a subset of bytes within a larger packet.
void GPS::processUBX(uint8_t incoming, ubxPacket *incomingUBX, uint8_t requestedClass, uint8_t requestedID) {
	//Note that incomingUBX->counter == (ubxFrameCounter - 2)
	//Add all incoming bytes to the rolling checksum
	//Stop at len+4, as these are the checksum bytes
	if (incomingUBX->counter < incomingUBX->len + 4) {
		addToChecksum(incoming);
	}
	if (incomingUBX->counter == 0) {
		incomingUBX->cls = incoming;
	} else if (incomingUBX->counter == 1) {
		incomingUBX->id = incoming;
	} else if (incomingUBX->counter == 2) {//Len LSB
		incomingUBX->len = incoming;
	} else if (incomingUBX->counter == 3) { //Len MSB
		incomingUBX->len |= incoming << 8;
	} else if (incomingUBX->counter == incomingUBX->len + 4) { //ChecksumA
		incomingUBX->checksumA = incoming;
	} else if (incomingUBX->counter == incomingUBX->len + 5) { //ChecksumB, last byte of sentence trasmission
		incomingUBX->checksumB = incoming;

		currentSentence = NONE; //reset the current sentence, next byte will start the process again

		//Validate this sentence
		if ((incomingUBX->checksumA == rollingChecksumA) && (incomingUBX->checksumB == rollingChecksumB)) {
			incomingUBX->valid = UBLOX_PACKET_VALIDITY_VALID; // Flag the packet as valid

			//Verifying class and ID match (for data or ACK packet)
			if (((incomingUBX->cls == requestedClass) && (incomingUBX->id == requestedID)) ||
					((incomingUBX->cls == UBX_CLASS_ACK) && (incomingUBX->id == UBX_ACK_ACK)
							&& (incomingUBX->payload[0] == requestedClass)
							&& (incomingUBX->payload[1] == requestedID))) {
				//If we have a match, set the classAndIDmatch flag to valid
				incomingUBX->classAndIDmatch = UBLOX_PACKET_VALIDITY_VALID;
			}

				// If this is a NACK then let's check if the class and ID match the requestedClass and requestedID
			else if ((incomingUBX->cls == UBX_CLASS_ACK) && (incomingUBX->id == UBX_ACK_NACK)
					&& (incomingUBX->payload[0] == requestedClass) && (incomingUBX->payload[1] == requestedID)) {
				//If we have a match, set the classAndIDmatch flag to NOTACKNOWLEDGED
				incomingUBX->classAndIDmatch = UBLOX_PACKET_NOTACKNOWLEDGED;
			}

			//The packet is valid, now process it (unless we are ignoring it)
			if (!ignoreThisPayload) {
				processUBXpacket(incomingUBX);
			}
		} else {  // Checksum failure
			incomingUBX->valid = UBLOX_PACKET_VALIDITY_NOT_VALID;

			//Not sure if I like the idea behind this..., will likely remove once I verify that I can
			// Let's check if the class and ID match the requestedClass and requestedID.
			// This is potentially risky as we are saying that we saw the requested Class and ID
			// but that the packet checksum failed. Potentially it could be the class or ID bytes
			// that caused the checksum error!
			//checking for data packet or ACK packet
			//TODO: Determine if this can be removed without breaking things
			if (((incomingUBX->cls == requestedClass) && (incomingUBX->id == requestedID)) ||
					((incomingUBX->cls == UBX_CLASS_ACK)
							&& (incomingUBX->payload[0] == requestedClass)
							&& (incomingUBX->payload[1] == requestedID))) {
				// If we have a match, set the classAndIDmatch flag to not valid
				incomingUBX->classAndIDmatch = UBLOX_PACKET_VALIDITY_NOT_VALID;
			}
		}
	} else { //payload byte
		if (!ignoreThisPayload) {
			//If a UBX_NAV_PVT packet comes in asynchronously, we need to fudge the startingSpot
			uint16_t tmp_startingSpot = incomingUBX->startingSpot;
			if (incomingUBX->cls == UBX_CLASS_NAV && incomingUBX->id == UBX_NAV_PVT)
				tmp_startingSpot = 0;
			//Begin recording if counter goes past startingSpot
			if ((incomingUBX->counter - 4) >= tmp_startingSpot) {
				//Check to see if we have room for this byte
				if (((incomingUBX->counter - 4) - tmp_startingSpot) < MAX_PAYLOAD_SIZE) {
					//Store this byte into payload array
					incomingUBX->payload[incomingUBX->counter - 4 - tmp_startingSpot] = incoming;
				}
			}
		}
	}
	//Increment the counter
	incomingUBX->counter++;

	if (incomingUBX->counter == MAX_PAYLOAD_SIZE) {
		//Something has gone very wrong
		//TODO: look into error handling here
		currentSentence = NONE; //Reset the sentence to being looking for a new start char
	}
}

void GPS::processUBXpacket(ubxPacket *msg) {
	if (msg->id == UBX_NAV_PVT && msg->len == 92) {
		//Parse various byte fields
		nav_data.year = extractInt(4);
		nav_data.month = extractByte(6);
		nav_data.day = extractByte(7);
		nav_data.hour = extractByte(8);
		nav_data.minute = extractByte(9);
		nav_data.second = extractByte(10);
		nav_data.nanosecond = extractLong(16); //Includes milliseconds

		nav_data.longitude = extractLong(24);
		nav_data.latitude = extractLong(28);
		nav_data.altitude = extractLong(32);
		nav_data.groundSpeed = extractLong(60);
		nav_data.headingOfMotion = extractLong(64);
	} else if (msg->cls == UBX_CLASS_ACK) {
		(msg->id == UBX_ACK_ACK ? ack_flag : nack_flag) = true;
	}
	//TODO: Cover Failure Case
	//note that we don't care about any other message types for our localization
}

//checksum helpers
void GPS::calcChecksum(ubxPacket *msg) {
	msg->checksumA = 0;
	msg->checksumB = 0;

	msg->checksumA += msg->cls;
	msg->checksumB += msg->checksumA;

	msg->checksumA += msg->id;
	msg->checksumB += msg->checksumA;

	msg->checksumA += (msg->len & 0xFF);
	msg->checksumB += msg->checksumA;

	msg->checksumA += (msg->len >> 8);
	msg->checksumB += msg->checksumA;

	for (uint16_t i = 0; i < msg->len; i++) {
		msg->checksumA += msg->payload[i];
		msg->checksumB += msg->checksumA;
	}
}
void GPS::addToChecksum(uint8_t incoming) {
	rollingChecksumA += incoming;
	rollingChecksumB += rollingChecksumA;
}

void GPS::setSerialRate(uint32_t baudrate, uint8_t uartPort) {
	//Get the current config values for the UART port
	getPortSettings(uartPort); //This will load the payloadCfg array with current port settings
	packetCfg.cls = UBX_CLASS_CFG;
	packetCfg.id = UBX_CFG_PRT;
	packetCfg.len = 20;
	packetCfg.startingSpot = 0;

	//payloadCfg is now loaded with current bytes. Change only the ones we need to
	payloadCfg[8] = baudrate;
	payloadCfg[9] = baudrate >> 8;
	payloadCfg[10] = baudrate >> 16;
	payloadCfg[11] = baudrate >> 24;

	sendSerialCommand(&packetCfg);
}

//Configure a given port to output UBX, NMEA, RTCM3 or a combination thereof
//Port:     	use COM_PORT_ constants
//Settings: 	use COM_TYPE_ constants
void GPS::setPortOutput(uint8_t portID, uint8_t outStreamSettings) {
	//Get the current config values for this port ID, loads payloadCfg array
	getPortSettings(portID);
	packetCfg.cls = UBX_CLASS_CFG;
	packetCfg.id = UBX_CFG_PRT;
	packetCfg.len = 20;
	packetCfg.startingSpot = 0;
	//payloadCfg is now loaded with current bytes. Change only the ones we need to
	payloadCfg[14] = outStreamSettings;
	sendSerialCommand(&packetCfg);
}

//Configure a given port to accept UBX, NMEA, RTCM3 or a combination thereof
//Port:     	use COM_PORT_ constants
//Settings: 	use COM_TYPE_ constants
void GPS::setPortInput(uint8_t portID, uint8_t inStreamSettings) {
	//Get the current config values for this port ID, loads payloadCfg array
	packetCfg.cls = UBX_CLASS_CFG;
	packetCfg.id = UBX_CFG_PRT;
	packetCfg.len = 20;
	packetCfg.startingSpot = 0;
	//payloadCfg is now loaded with current bytes. Change only the ones we need to
	payloadCfg[12] = inStreamSettings;
	sendSerialCommand(&packetCfg);
}

void GPS::setNavigationFrequency(uint8_t navFreq) {
	packetCfg.cls = UBX_CLASS_CFG;
	packetCfg.id = UBX_CFG_RATE;
	packetCfg.len = 0;
	packetCfg.startingSpot = 0;

	//This will load the payloadCfg array with current settings of the given register
	sendSerialCommand(&packetCfg);

	uint16_t measurementRate = 1000 / navFreq;

	//payloadCfg is now loaded with current bytes. Change only the ones we need to
	payloadCfg[0] = measurementRate & 0xFF; //measRate LSB
	payloadCfg[1] = measurementRate >> 8;   //measRate MSB

	sendSerialCommand(&packetCfg);
}

void GPS::configureMessage(uint8_t msgClass, uint8_t msgID, uint8_t portID, uint8_t sendRate) {
	packetCfg.cls = UBX_CLASS_CFG;
	packetCfg.id = UBX_CFG_MSG;
	packetCfg.len = 8;
	packetCfg.startingSpot = 0;

	//Clear packet payload
	for (int x = 0; x < packetCfg.len; x++) {
		packetCfg.payload[x] = 0;
	}
	packetCfg.payload[0] = msgClass;
	packetCfg.payload[1] = msgID;
	//Send rate is relative to the event a message is registered on.
	// For example, if the rate of a navigation message is set to 2, the message is sent every 2nd navigation solution.
	packetCfg.payload[2 + portID] = sendRate;

	sendSerialCommand(&packetCfg);
}

void GPS::saveConfiguration()
// saved config is automatically loaded on startup
// will allow configuration to persist through module resets
// where initial configuration would not necessarily run again
{
	packetCfg.cls = UBX_CLASS_CFG;
	packetCfg.id = UBX_CFG_CFG;
	packetCfg.len = 12;
	packetCfg.startingSpot = 0;

	//Clear packet payload
	//TODO: make sure replacing uint8_t with int didn't break anything here
	for (int x = 0; x < packetCfg.len; x++) {
		packetCfg.payload[x] = 0;
	}

	packetCfg.payload[4] = 0xFF; //Setting saveMask bits
	packetCfg.payload[5] = 0xFF;

	sendSerialCommand(&packetCfg);
}

//Current port settings helper function
void GPS::getPortSettings(uint8_t portID) {
	packetCfg.cls = UBX_CLASS_CFG;
	packetCfg.id = UBX_CFG_PRT;
	packetCfg.len = 1;
	packetCfg.startingSpot = 0;
	payloadCfg[0] = portID;
}

//Helper functions for extracting byte data
uint32_t GPS::extractLong(uint8_t spotToStart) {
	uint32_t val = 0;
	val |= (uint32_t) payloadCfg[spotToStart + 0] << 8 * 0;
	val |= (uint32_t) payloadCfg[spotToStart + 1] << 8 * 1;
	val |= (uint32_t) payloadCfg[spotToStart + 2] << 8 * 2;
	val |= (uint32_t) payloadCfg[spotToStart + 3] << 8 * 3;
	return (val);
}
//Given a spot in the payload array, extract two bytes and build an int
uint16_t GPS::extractInt(uint8_t spotToStart) {
	uint16_t val = 0;
	val |= (uint16_t) payloadCfg[spotToStart + 0] << 8 * 0;
	val |= (uint16_t) payloadCfg[spotToStart + 1] << 8 * 1;
	return (val);
}
//Given a spot, extract a byte from the payload
uint8_t GPS::extractByte(uint8_t spotToStart) {
	return (payloadCfg[spotToStart]);
}




