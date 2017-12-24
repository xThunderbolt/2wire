#ifndef TWI_TWIDEVICE_WRAP_H
#define TWI_TWIDEVICE_WRAP_H

#include "TwiDevice.h"
#include <nan.h>
#include <nan-check.h>

class TwiDevice_Wrap : public Nan::ObjectWrap {
	public:
		static NAN_MODULE_INIT(Init);

	private:
		TwiDevice_Wrap(char* deviceFile, uint8_t deviceId);
		~TwiDevice_Wrap();

		char* deviceFile;
		TwiDevice device;

		static NAN_METHOD(writeRawByte);
		static NAN_METHOD(writeRawBytes);

		static NAN_METHOD(readRawByte);
		static NAN_METHOD(readRawBytes);

		static NAN_METHOD(writeByte);
		static NAN_METHOD(writeWord);
		static NAN_METHOD(writeBytes);
		static NAN_METHOD(writeBit);
		static NAN_METHOD(writeBits);

		static NAN_METHOD(readByte);
		static NAN_METHOD(readWord);
		static NAN_METHOD(readBytes);
		static NAN_METHOD(readBit);
		static NAN_METHOD(readBits);

		static NAN_METHOD(New);
		
		static Nan::Persistent<v8::Function> constructor;
};

#endif // TWI_TWIDEVICE_WRAP_H
