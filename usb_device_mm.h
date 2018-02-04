

#ifdef PIC32MM

void usbDevice_init();
int usbDevice_isConnected();
void usbDevice_txData(char ep, char* buffer, short int len);
int usbDevice_isTxProgress(char ep);
void usbDevice_rxData(char ep, char* buffer, short int len);
int usbDevice_isRxProgress(char ep);
void usbDevice_interrupt();

// <editor-fold defaultstate="collapsed" desc="local fce">
static void USB_reset();
static char* USB_getBDAddress(char ep, char dir, char ppbi);
static char* USB_getBuffer(char ep, char dir, char ppbi);
static void USB_setBD(char* bd, char b0, int size, char* buffer_addr);
static void USB_prepareBDForOut(char ep, char ppbi, char toggle, short int len);
static void USB_sendDeviceDescriptor(short int len);
static void USB_sendDeviceConfiguration(unsigned int len);
static void USB_sendDeviceStatus();
static void USB_sendSetupNull();
static void USB_sendString(const unsigned char index);

// </editor-fold>

#endif

