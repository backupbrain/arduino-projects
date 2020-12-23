#include "Filesystem.h"


Filesystem::Filesystem(Adafruit_TinyFlash* disk) {
	this->_disk = disk;
}

Filesystem::~Filesystem() {
	this->_disk = NULL;
}

unsigned long Filesystem::_convertFilenameFromAscii(String filename) {
  /*
   * - in order to get 6 characters in 4 bytes, we will have to use a custom, 64 character filename
   * - disallow ? as first character
   * CHARACTER	0-9 		A-Z				.		-		?
   * ORDINANCE 	0x00-0x09 	0x0A-0x34		0x35	0x36	0xFF


   30 - 39: 0-9
   61 - 7a: a-z
   41 - 5a: A-Z
   3f: ?
   2e: .
   2d: -

   */
   unsigned int length = filename.length();
   if (length > maxFilenameLength) {

   }
   unsigned long encodedFilename;
   for (int cursor = 0; cursor < length; cursor++) {

   }

}

Filesystem::format() {
	this->_disk->eraseChip();

	unsigned long blockAddress = 0;

	// now go through and write 0xFF for entire block map
	// might need to break this up into chunks of 256 or something
	byte[] data = new byte[this->_blockMapLength];
	for (int cursor = 0; cursor < this->_blockMapLength; cursor++) {
		data[cursor] = 0xFF;
	}

	for (long cursor=blockAddress; cursor < endAddress; cursor += blockIncrement) {
		this->_disk->writePage(blockAddress, *data);
	}
}


Filesystem::writeFile(unsigned int blockReference, byte* data) {
	// 
}
