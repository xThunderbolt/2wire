declare module '2wire' {
	declare class TwiDevice {
		constructor(deviceFile: string, address: number);

		writeRawByte(byte: number);
		writeRawBytes(buffer: Buffer);

		writeByte(register: number, value: number);
		writeWord(register: number, value: number);
		writeBytes(register: number, value: Buffer);
		writeBit(register: number, bit: number, value: number);
		writeBits(register: number, bitStart: number, length: number, value: number);

		readByte(register: number): number;
		readWord(register: number): number;
		readBytes(register: number, length: number): Buffer;
		readBit(register: number, bit: number): boolean;
		readBits(register: number, bitStart: number, length: number): number;
	}
}
