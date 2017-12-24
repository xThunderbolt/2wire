#include "TwiDevice_Wrap.h"

Nan::Persistent<v8::Function> TwiDevice_Wrap::constructor;

NAN_MODULE_INIT(TwiDevice_Wrap::Init) {
	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	tpl->SetClassName(Nan::New("TwiDevice").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	Nan::SetPrototypeMethod(tpl, "writeRawByte", writeRawByte);
	Nan::SetPrototypeMethod(tpl, "writeRawBytes", writeRawBytes);

	Nan::SetPrototypeMethod(tpl, "readRawByte", readRawByte);
	Nan::SetPrototypeMethod(tpl, "readRawBytes", readRawBytes);

	Nan::SetPrototypeMethod(tpl, "writeByte", writeByte);
	Nan::SetPrototypeMethod(tpl, "writeWord", writeWord);
	Nan::SetPrototypeMethod(tpl, "writeBytes", writeBytes);
	Nan::SetPrototypeMethod(tpl, "writeBit", writeBit);
	Nan::SetPrototypeMethod(tpl, "writeBits", writeBits);

	Nan::SetPrototypeMethod(tpl, "readByte", readByte);
	Nan::SetPrototypeMethod(tpl, "readWord", readWord);
	Nan::SetPrototypeMethod(tpl, "readBytes", readBytes);
	Nan::SetPrototypeMethod(tpl, "readBit", readBit);
	Nan::SetPrototypeMethod(tpl, "readBits", readBits);

	constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
	Nan::Set(target, Nan::New("TwiDevice").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

TwiDevice_Wrap::TwiDevice_Wrap(char* deviceFile, uint8_t deviceId)
	: deviceFile(deviceFile),
	  device(TwiDevice(deviceFile, deviceId)) {
}

TwiDevice_Wrap::~TwiDevice_Wrap() {
	delete[] deviceFile;
}

NAN_METHOD(TwiDevice_Wrap::New) {
	v8::Local<v8::String> deviceFileString;
	uint32_t deviceId;

	try {
		std::string error;
		if (!Nan::Check(info).ArgumentsCount(2)
			.Argument(0).IsString().Bind(deviceFileString)
			.Argument(1).Bind(deviceId)
			.Error(&error))
		{
			return Nan::ThrowSyntaxError(error.c_str());
		}
	}
	catch (Nan::CheckException& ex) {
		return Nan::ThrowTypeError(ex.what());
	}

	if (deviceId > 127) {
		return Nan::ThrowRangeError("deviceId: Must be in interval [0; 127].");
	}

	if (info.IsConstructCall()) {
		auto deviceFile = new char[deviceFileString->Utf8Length()];
		deviceFileString->WriteUtf8(deviceFile);

		auto obj = new TwiDevice_Wrap(deviceFile, deviceId);
		obj->Wrap(info.This());
		info.GetReturnValue().Set(info.This());
	}
	else {
		const int argc = 2;
		v8::Local<v8::Value> argv[argc] = { info[0], info[1] };
		info.GetReturnValue().Set(Nan::NewInstance(Nan::New(constructor), argc, argv).ToLocalChecked());
	}
}

#define OBJ() auto obj = Nan::ObjectWrap::Unwrap<TwiDevice_Wrap>(info.This())

NAN_METHOD(TwiDevice_Wrap::writeRawByte) {
	OBJ();

	uint32_t byte;

	try {
		std::string error;
		if (!Nan::Check(info).ArgumentsCount(1)
			.Argument(0).Bind(byte)
			.Error(&error))
		{
			return Nan::ThrowSyntaxError(error.c_str());
		}
	}
	catch (Nan::CheckException& ex) {
		return Nan::ThrowTypeError(ex.what());
	}

	obj->device.writeRawByte(byte);
}

NAN_METHOD(TwiDevice_Wrap::writeRawBytes) {
	OBJ();

	v8::Local<v8::Object> bufferObject;

	try {
		std::string error;
		if (!Nan::Check(info).ArgumentsCount(1)
			.Argument(0).IsBuffer().Bind(bufferObject)
			.Error(&error))
		{
			return Nan::ThrowSyntaxError(error.c_str());
		}
	}
	catch (Nan::CheckException& ex) {
		return Nan::ThrowTypeError(ex.what());
	}

	auto length = node::Buffer::Length(bufferObject);
	auto buffer = node::Buffer::Data(bufferObject);
	obj->device.writeRawBytes(buffer, length);
}

NAN_METHOD(TwiDevice_Wrap::readRawByte) {
	OBJ();
	info.GetReturnValue().Set(obj->device.readRawByte());
}

NAN_METHOD(TwiDevice_Wrap::readRawBytes) {
	OBJ();

	uint32_t length;

	try {
		std::string error;
		if (!Nan::Check(info).ArgumentsCount(1)
			.Argument(0).Bind(length)
			.Error(&error))
		{
			return Nan::ThrowSyntaxError(error.c_str());
		}
	}
	catch (Nan::CheckException& ex) {
		return Nan::ThrowTypeError(ex.what());
	}

	char* buffer = new char[length];
	length = obj->device.readRawBytes(buffer, length);

	info.GetReturnValue().Set(Nan::NewBuffer(buffer, length).ToLocalChecked());
}

NAN_METHOD(TwiDevice_Wrap::writeByte) {
	OBJ();

	uint32_t reg, value;

	try {
		std::string error;
		if (!Nan::Check(info).ArgumentsCount(2)
			.Argument(0).Bind(reg)
			.Argument(1).Bind(value)
			.Error(&error))
		{
			return Nan::ThrowSyntaxError(error.c_str());
		}
	}
	catch (Nan::CheckException& ex) {
		return Nan::ThrowTypeError(ex.what());
	}

	obj->device.writeByte(reg, value);
}

NAN_METHOD(TwiDevice_Wrap::writeWord) {
	OBJ();

	uint32_t reg;
	uint32_t value;

	try {
		std::string error;
		if (!Nan::Check(info).ArgumentsCount(2)
			.Argument(0).Bind(reg)
			.Argument(1).Bind(value)
			.Error(&error))
		{
			return Nan::ThrowSyntaxError(error.c_str());
		}
	}
	catch (Nan::CheckException& ex) {
		return Nan::ThrowTypeError(ex.what());
	}

	obj->device.writeWord(reg, value);
}

NAN_METHOD(TwiDevice_Wrap::writeBytes) {
	OBJ();

	uint32_t reg;
	v8::Local<v8::Object> bufferObject;

	try {
		std::string error;
		if (!Nan::Check(info).ArgumentsCount(2)
			.Argument(0).Bind(reg)
			.Argument(1).IsBuffer().Bind(bufferObject)
			.Error(&error))
		{
			return Nan::ThrowSyntaxError(error.c_str());
		}
	}
	catch (Nan::CheckException& ex) {
		return Nan::ThrowTypeError(ex.what());
	}

	auto length = node::Buffer::Length(bufferObject);
	auto buffer = node::Buffer::Data(bufferObject);
	obj->device.writeBytes(reg, buffer, length);
}

NAN_METHOD(TwiDevice_Wrap::writeBit) {
	OBJ();

	uint32_t reg, bit, data;

	try {
		std::string error;
		if (!Nan::Check(info).ArgumentsCount(3)
			.Argument(0).Bind(reg)
			.Argument(1).Bind(bit)
			.Argument(2).Bind(data)
			.Error(&error))
		{
			return Nan::ThrowSyntaxError(error.c_str());
		}
	}
	catch (Nan::CheckException& ex) {
		return Nan::ThrowTypeError(ex.what());
	}

	obj->device.writeBit(reg, bit, data);
}

NAN_METHOD(TwiDevice_Wrap::writeBits) {
	OBJ();

	uint32_t reg, bitStart, length, data;

	try {
		std::string error;
		if (!Nan::Check(info).ArgumentsCount(4)
			.Argument(0).Bind(reg)
			.Argument(1).Bind(bitStart)
			.Argument(2).Bind(length)
			.Argument(3).Bind(data)
			.Error(&error))
		{
			return Nan::ThrowSyntaxError(error.c_str());
		}
	}
	catch (Nan::CheckException& ex) {
		return Nan::ThrowTypeError(ex.what());
	}

	obj->device.writeBits(reg, bitStart, length, data);
}

NAN_METHOD(TwiDevice_Wrap::readByte) {
	OBJ();

	uint32_t reg;

	try {
		std::string error;
		if (!Nan::Check(info).ArgumentsCount(1)
			.Argument(0).Bind(reg)
			.Error(&error))
		{
			return Nan::ThrowSyntaxError(error.c_str());
		}
	}
	catch (Nan::CheckException& ex) {
		return Nan::ThrowTypeError(ex.what());
	}

	info.GetReturnValue().Set(obj->device.readByte(reg));
}

NAN_METHOD(TwiDevice_Wrap::readWord) {
	OBJ();

	uint32_t reg;

	try {
		std::string error;
		if (!Nan::Check(info).ArgumentsCount(1)
			.Argument(0).Bind(reg)
			.Error(&error))
		{
			return Nan::ThrowSyntaxError(error.c_str());
		}
	}
	catch (Nan::CheckException& ex) {
		return Nan::ThrowTypeError(ex.what());
	}

	info.GetReturnValue().Set(obj->device.readWord(reg));
}

NAN_METHOD(TwiDevice_Wrap::readBytes) {
	OBJ();

	uint32_t reg, length;

	try {
		std::string error;
		if (!Nan::Check(info).ArgumentsCount(2)
			.Argument(0).Bind(reg)
			.Argument(1).Bind(length)
			.Error(&error))
		{
			return Nan::ThrowSyntaxError(error.c_str());
		}
	}
	catch (Nan::CheckException& ex) {
		return Nan::ThrowTypeError(ex.what());
	}

	char* buffer = new char[length];
	length = obj->device.readBytes(reg, buffer, length);

	info.GetReturnValue().Set(Nan::NewBuffer(buffer, length).ToLocalChecked());
}

NAN_METHOD(TwiDevice_Wrap::readBit) {
	OBJ();

	uint32_t reg, bit;

	try {
		std::string error;
		if (!Nan::Check(info).ArgumentsCount(2)
			.Argument(0).Bind(reg)
			.Argument(1).Bind(bit)
			.Error(&error))
		{
			return Nan::ThrowSyntaxError(error.c_str());
		}
	}
	catch (Nan::CheckException& ex) {
		return Nan::ThrowTypeError(ex.what());
	}

	info.GetReturnValue().Set(obj->device.readBit(reg, bit));
}

NAN_METHOD(TwiDevice_Wrap::readBits) {
	OBJ();

	uint32_t reg, bitStart, length;

	try {
		std::string error;
		if (!Nan::Check(info).ArgumentsCount(3)
			.Argument(0).Bind(reg)
			.Argument(1).Bind(bitStart)
			.Argument(1).Bind(length)
			.Error(&error))
		{
			return Nan::ThrowSyntaxError(error.c_str());
		}
	}
	catch (Nan::CheckException& ex) {
		return Nan::ThrowTypeError(ex.what());
	}

	info.GetReturnValue().Set(obj->device.readBits(reg, bitStart, length));
}
