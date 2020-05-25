#ifndef rtc_mem_h
	#define rtc_mem_h
	#define ENTRIES 200
	#include "Arduino.h"
	struct RtcData{
		uint32_t crc32;	//checksum value of current data
		uint8_t count;	
		bool get_ready_to_send;	//a value that indicates if we are going to manual mode
		bool sent;		//a bool value that indicates if the data has been sent, only applicable to
		uint16_t data[ENTRIES]; //remaining 508 bytes of data
	};
			
	class RTC_MEM
	{
		public:
			RTC_MEM();//setup whatever we want with RTC_MEM
				/*
					*function will create a blank _rtcData struct
				*/
				
			bool write_to_RTC_MEM(uint16_t data);//write to the RTC_MEM, returns true if it's time to broadcast
			void read_from_RTC_MEM();//write to the RTC_MEM
			void print_RTC_MEM();//prints the RTC MEM helper function
			bool validate_check_sum();//return true/false based on checksum
			const uint8_t countLimit=ENTRIES;
			void write_to_RTC_MEM_no_data();//just updates count
			RtcData rtcData;
			bool one_away;	//goes true if the next wakeup will be a transmit
			
		private:
			//whatever is only accessible internal to the class

			uint32_t _crcOfData;
			uint32_t _gen_check_sum(const uint8_t *data,size_t length);	

				/*
					returns a uint32_t utilizing the CRC32 checksum method
				*/
			
	};

#endif