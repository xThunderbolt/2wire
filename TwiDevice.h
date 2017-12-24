#ifndef TWI_TWIDEVICE_H
#define TWI_TWIDEVICE_H

#include <cstdint>
#include <cstddef>

class TwiDevice {
public:
	TwiDevice(const char* deviceFile, uint8_t deviceId);

	void writeRawByte(uint8_t byte) const;
	void writeRawBytes(const void* buffer, size_t length) const;

	uint8_t readRawByte() const;
	size_t readRawBytes(void* buffer, size_t length) const;

	void writeByte(uint8_t reg, uint8_t value) const;
	void writeWord(uint8_t reg, uint16_t value) const;
	void writeBytes(uint8_t reg, const void* buffer, size_t length) const;
	void writeBit(uint8_t reg, uint8_t bit, uint8_t data) const;
	void writeBits(uint8_t reg, uint8_t bitStart, uint8_t length, uint8_t data) const;

	uint8_t readByte(uint8_t reg) const;
	uint16_t readWord(uint8_t reg) const;
	size_t readBytes(uint8_t reg, void* buffer, size_t length) const;
	bool readBit(uint8_t reg, uint8_t bit) const;
	uint8_t readBits(uint8_t reg, uint8_t bitStart, uint8_t length) const;

private:
	int fileDescriptor;
};

#endif // TWI_TWIDEVICE_H
