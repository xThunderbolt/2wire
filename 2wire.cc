#include "TwiDevice_Wrap.h"

NAN_MODULE_INIT(InitAll) {
  TwiDevice_Wrap::Init(target);
}

NODE_MODULE(2wire, InitAll)
