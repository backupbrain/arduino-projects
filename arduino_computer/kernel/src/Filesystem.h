

// principles taken from: http://pages.cs.wisc.edu/~remzi/OSTEP/file-implementation.pdf

/**
 * CPU considerations:
 * We don't have a lot of RAM on this computer
 * If we use a typical Arduino, we may have 1024kb or 2048kb of RAM for the entire OS, programs, and files
 * 
 * therefore, we need to be very careful about file size and stuff.
 * can't create a very complex filesystem, since we probably won't have room for addresses and stuff
 */

/**
 * Filesystem considerations:
 *
 * - Filesystem is likely 1024kb on our flash chip
 * - typical filesystem has 4kb chunks
 * - need an inode table
 * - likely going to used linkedlist that links chunks in multi-chunk files
 * - 
 * -  I want to be able to look up filenames, figure out where the file points to
 * 
 * - if we add a superblock, we can count how big the filesystem is
 * - where do we add read/write/executable bits to the filesystem?
 */
// proposed filesystem structure:
/**
 * - filename <-> block map
 * [ 0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1 2 3 4 5 6 7 8 9 A B C D E F ]
 * [ ALPHANUM  ALPMANUM  ALPHANUM    | ALPHANUM  ALPHANUM  ALPHANUM    | BLOCK DATA                      | BLOCK DATA                      ]
 *
 * if block entry is 0x00 0x00 0x00 0x00, then the block is defereferenced 
 * files that take more than one block will have a duplicate reference in the block table
 * when a file is cleared, all relevant filename blocks will be cleared
 * 
 * - chunk formatted as:
 * [ BYTES 0-509  |  BYTE 510-511              ]
 * [ file data    |  next block data reference ]
 *
 * when a file is cleared, nothing needs to be done to the data chunks, since the inode table is cleared
 *
 */

 /**
  * Process for finding a file: 
  * - convert ascii filename to filesystem-specific alphanum string
  * - determine filesystem/block map (likely 4 bytes * number of 4k blocks in 1024kyte flash)
  * - move cursor to filesystem/block map (likely block 0)
  * - loop through filesystem/block map, moving cursor 4 bytes at a time, 
  * 	- byte by byte, up to 6 bytes, read into memory and look for matching filename
  * 	- no match? move on
  *  	- match? read the next 2 bytes and return as block data reference
  * - no match? return 0
  */

  /**
   * filename considerations:
   * 
   * - in order to get 6 characters in 4 bytes, we will have to use a custom, 64 character filename
   * - disallow ? as first character
   * CHARACTER	0-9 		A-Z				.		-		?
   * ORDINANCE 	0x00-0x09 	0x0A-0x34		0x35	0x36	0xFF
   */

 /**
  * Process for reading a file:
  * 
  * - program requests file
  * - move cursor to requested block
  * - read byte and send back
  * - when no more data, return -1
  */ 

#include "Adafruit_TinyFlash.h"

#ifndef Filesystem_h
#define Filesystem_h

class Filesystem {
public:
	Filesystem(byte pin);
	~Filesystem();

	bool format(unsigned int byteLength);

	// return the filename/block reference position
	unsigned long createFile(char* filename);
	unsigned long openFile(char* filename, byte mode);
	byte* readFile(unsigned long blockReference);
	bool writeFile(unsigned int blockReference, byte* data);
	bool deleteFile(unsigned int blockReference);
	bool closeFile(unsigned int blockReference);
	bool format();


private:
	static const unsigned int _blockLength = 4096;
	static const unsigned long _blockMapLength = 131072; //2 * 16 addresses * 4096 block length;
	unsigned long _diskLength_bytes;

	unsigned long _readHead;
	unsigned long _writeHead;

	unsigned int _openFiles; // references to open files

	unsigned long _moveCursorTo(unsigned int position);
	unsigned long _getBlockLocationFromReference(unsigned int blockReference);

	unsigned long findEmptyBlock();
	bool _attachBlockReference(unsigned long blockReference, unsigned long block); // when a file is created or extended
	bool _clearBlockReference(unsigned long block);	// clear an individual block, for instance if the file shortened
	bool _clearBlockReferences(unsigned long block); // clear all matching blocks

	unsigned long _getDataFromBlock(unsigned long currentBlock);
	unsigned long _writeDataFromBlock(unsigned long currentBlock, unsigned long data);
	unsigned long _getNextBlock(unsigned long data);
	bool _isEndOfFile(unsigned long data);

	// convert ascii filename to custom filename
	unsigned long _convertFilenameFromAscii(String filename);
	String _convertFilenameToAscii(unsigned long encodedFilename);

	Adafruit_TinyFlash* _disk;


};


#endif