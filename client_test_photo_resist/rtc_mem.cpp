#include "Arduino.h"
#include "rtc_mem.h"

RTC_MEM::RTC_MEM(){
//this doesn't do anything yet.

//	Serial.print("pre Count: ");
//	Serial.println(rtcData.count);
	one_away=false;
	read_from_RTC_MEM();
	Serial.print("Count: ");
	Serial.println(rtcData.count);
}

void RTC_MEM::read_from_RTC_MEM(){
	if (ESP.rtcUserMemoryRead(0, (uint32_t*) &rtcData, sizeof(rtcData))) {
		Serial.println("Read complete");

	}
	validate_check_sum();//after teh read, validate teh checksum
}

bool RTC_MEM::write_to_RTC_MEM(uint16_t data_to_write){
	bool broadcast=false;
	
	/*for (size_t i = 0; i < sizeof(rtcData.data); i++) {	//write some random values to the RTC data space
		rtcData.data[i] = random(0, 128);
	}*/
	rtcData.data[rtcData.count]=data_to_write;
	
	if (rtcData.count>=countLimit){
		//need to do a memory dump to the server
		//do we initiate mem dump here or somewhere else?
		rtcData.count=0;	//reset the counter and proceed as usual
		broadcast=true;
	}else{
		rtcData.count+=1;
		if (rtcData.count==countLimit){
			//check to see if the next time we run this we need to wakeup
			one_away=true;
		}
	}
	
	// Update CRC32 of data
	rtcData.crc32 = _gen_check_sum((uint8_t*) &rtcData.data[0], sizeof(rtcData.data));
	
	// Write struct to RTC memory
	if (ESP.rtcUserMemoryWrite(0, (uint32_t*) &rtcData, sizeof(rtcData))) {
		Serial.println("Write: ");
		//print_RTC_MEM();
		Serial.println();
	}
	
	return broadcast;
}

void RTC_MEM::print_RTC_MEM(){
	/*char buf[3];
	uint8_t *ptr = (uint8_t *)&rtcData;
	for (size_t i = 0; i < sizeof(rtcData); i++) {
		sprintf(buf, "%02X", ptr[i]);
		Serial.print(buf);
		if ((i + 1) % 32 == 0) {
			Serial.println();
		} else {
			Serial.print(" ");
			}
		}
	Serial.println();*/
	

	for (int i=0;i<countLimit;i++){
		Serial.print(rtcData.data[i]);
		Serial.print(" ");
	}
	Serial.println();
}

bool RTC_MEM::validate_check_sum(){
	_crcOfData=_gen_check_sum((uint8_t*) &rtcData.data[0], sizeof(rtcData.data));	//passing the location of our data and the size of the data we get our crc32 value
	Serial.print("CRC32 of data: ");
	Serial.println(_crcOfData, HEX);
	Serial.print("CRC32 read from RTC: ");
	Serial.println(rtcData.crc32, HEX);
	
	if (_crcOfData != rtcData.crc32) {
		Serial.println("CRC32 in RTC memory doesn't match CRC32 of data. Data is probably invalid!");
	} else {
		Serial.println("CRC32 check ok, data is probably valid.");
	}
}

uint32_t RTC_MEM::_gen_check_sum(const uint8_t *data,size_t length){
	uint32_t crc = 0xffffffff;	//32 1's
	//while there is still stuff to decrement
	while (length--) {
	//get the hex stored in the current data segment
	uint8_t c = *data++;
	for (uint32_t i = 0x80; i > 0; i >>= 1) {
		bool bit = crc & 0x80000000;
		if (c & i) {
			bit = !bit;
		}
		crc <<= 1;
		if (bit) {
			crc ^= 0x04c11db7;
		}
	}
  }
  return crc;
}