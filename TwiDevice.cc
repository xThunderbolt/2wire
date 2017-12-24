//
// Created by tim on 07.01.17.
//

#include "TwiDevice.h"
#include <nan.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <errno.h>

#define I2C_SLAVE   0x0703

TwiDevice::TwiDevice(const char* deviceFile, uint8_t deviceId) {
	fileDescriptor = open(deviceFile, O_RDWR);
	if (fileDescriptor < 0) {
		Nan::ThrowError(("Unable to open I2C device: " + std::string(strerror(errno))).c_str());
		return;
	}

	if (ioctl(fileDescriptor, I2C_SLAVE, deviceId) < 0) {
		Nan::ThrowError(("Unable to select I2C device: " + std::string(strerror(errno))).c_str());
		return;
	}
}

void TwiDevice::writeRawByte(uint8_t byte) const {
	writeRawBytes(&byte, 1);
}

void TwiDevice::writeRawBytes(const void* buffer, size_t length) const {
	ssize_t size = write(fileDescriptor, buffer, length);

	if (size < 0) {
		return Nan::ThrowError(("Unable to write I2C device: " + std::string(strerror(errno))).c_str());
	}
}

uint8_t TwiDevice::readRawByte() const {
	uint8_t value;
	readRawBytes(&value, 1);
	return value;
}

size_t TwiDevice::readRawBytes(void* buffer, size_t length) const {
	ssize_t size = read(fileDescriptor, buffer, length);

	if (size < 0) {
		Nan::ThrowError(("Unable to read I2C device: " + std::string(strerror(errno))).c_str());
		return 0;
	}

	return static_cast<size_t>(size);
}

void TwiDevice::writeByte(uint8_t reg, uint8_t value) const {
	uint8_t buffer[] = {reg, value};
	writeRawBytes(&buffer, 2);
}

void TwiDevice::writeWord(uint8_t reg, uint16_t value) const {
	// I2C is big-endian, we have little-endian
	value = value >> 8 | value << 8;

	uint8_t buffer[] = {reg, static_cast<uint8_t>(value >> 8), static_cast<uint8_t>(value & 0xff)};
	writeRawBytes(&buffer, 3);
}

void TwiDevice::writeBytes(uint8_t reg, const void* buffer, size_t length) const {
	const uint8_t* dataBuffer = reinterpret_cast<const uint8_t*>(buffer);
	uint8_t writeBuffer[length + 1];

	writeBuffer[0] = reg;
	for (uint32_t i = 0; i < length; i++) {
		writeBuffer[i + 1] = dataBuffer[i];
	}

	writeRawBytes(writeBuffer, length + 1);
}

void TwiDevice::writeBit(uint8_t reg, uint8_t bit, uint8_t data) const {
	uint8_t regContent = readByte(reg);
	if (data) {
		regContent |= (1 << bit);
	}
	else {
		regContent &= ~(1 << bit);
	}

	writeByte(reg, regContent);
}

void TwiDevice::writeBits(uint8_t reg, uint8_t bitStart, uint8_t length, uint8_t data) const {
	uint8_t regContent = readByte(reg);

	// Build the bitmask. Example: length is 3
	// 1 << length gives 0b1000. Minus one gives 0b111
	// This is then shifted to the right position, like the data, too.
	uint8_t mask = static_cast<uint8_t>(((1 << length) - 1) << (bitStart - length + 1));
	data <<= (bitStart - length + 1);

	// Discard any "crap"
	data &= mask;

	// Modify bits as wanted
	regContent &= ~(mask);
	regContent |= data;

	writeByte(reg, regContent);
}

uint8_t TwiDevice::readByte(uint8_t reg) const {
	writeRawByte(reg);
	return readRawByte();
}

uint16_t TwiDevice::readWord(uint8_t reg) const {
	uint16_t buffer;
	writeRawByte(reg);
	readRawBytes(&buffer, sizeof(buffer));

	// I2C is big-endian, we need little-endian
	buffer = buffer >> 8 | buffer << 8;

	return buffer;
}

size_t TwiDevice::readBytes(uint8_t reg, void* buffer, size_t length) const {
	writeRawByte(reg);
	return readRawBytes(buffer, length);
}

bool TwiDevice::readBit(uint8_t reg, uint8_t bit) const {
	uint8_t data = readByte(reg);
	return static_cast<bool>(data & (1 << bit));
}

uint8_t TwiDevice::readBits(uint8_t reg, uint8_t bitStart, uint8_t length) const {
	uint8_t data = readByte(reg);

	// Build the bitmask. Example: length is 3
	// 1 << length gives 0b1000. Minus one gives 0b111
	// This is then shifted to the right position, like the data, too.
	uint8_t mask = static_cast<uint8_t>(((1 << length) - 1) << (bitStart - length + 1));

	// Remove unwanted bits
	data &= mask;

	// Shift to correct position, ie. bitStart becomes the first bit.
	data >>= (bitStart - length + 1);

	return data;
}
