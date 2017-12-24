# 2wire

Easy access to I2C devices.

It was designed for the Raspberry Pi, but may work on different boards, too.

TypeScript definitions are included.

## Example

Write register 0x58 of device addressed by 0x20 to value 42.
```javascript
const TwiDevice = require('2wire').TwiDevice;

const wire = new TwiDevice('/dev/i2c-1', 0x20);
wire.writeByte(0x58, 42);
```

