#include "ebc/BitcodeArchive.h"
#include "ebc/BitcodeMetadata.h"
#include "ebc/EmbeddedBitcode.h"
#include "ebc/util/Bitcode.h"
#include "ebc/util/Xar.h"

#include "catch.hpp"

#include <string>
#include <vector>

using namespace ebc;

static constexpr int size = 4033;
static constexpr auto data =
    "\x78\x61\x72\x21\x00\x1c\x00\x01\x00\x00\x00\x00\x00\x00\x02\x7c\x00\x00\x00\x00\x00\x00\x07\x4b\x00\x00\x00\x01"
    "\x78\xda\xdc\x55\x4d\x8f\xda\x30\x10\xbd\xef\xaf\x88\x72\x37\x89\x13\xf2\xb5\x32\x59\x75\x55\x55\x95\xda\xaa\x95"
    "\xa0\x52\x6f\xc8\x71\x1c\x70\xb1\x13\x14\x1b\x14\xb6\xea\x7f\xaf\xed\x04\x02\x2c\x2b\xf5\x63\x4f\x3d\x20\x4f\xc6"
    "\xcf\x6f\x66\x9e\x5f\x02\x7a\xe8\x04\x77\xf6\xb4\x95\xac\xa9\x67\x2e\x9c\xf8\xae\x43\x6b\xd2\x94\xac\x5e\xcd\xdc"
    "\xaf\x8b\x77\x20\x75\x1f\xf2\x3b\xd4\xe1\x36\xbf\x73\x90\xdc\x15\x65\x43\x9c\x7e\x59\xd6\x58\xd0\x99\xfb\xb1\x74"
    "\xf5\x96\x83\x38\xab\x37\xa0\xd9\x2a\x4d\x24\x4d\xc2\x41\xfd\x43\x5e\xec\x18\x2f\xbd\x2e\x8d\x97\xf1\x74\xd2\x20"
    "\x6f\x48\x9f\x43\x00\xed\x28\xd9\x29\x5c\x70\xba\xdc\x62\xb5\xbe\x09\x5a\x0a\xcc\xea\x17\x8e\xdf\x4c\x43\x7f\x02"
    "\xf5\x44\xb7\x8f\x08\x4c\x1a\xd9\x2d\x87\xd1\x97\xe2\x45\x6a\xdb\xd9\x45\x01\xe4\x5d\xcf\x8a\xca\x03\x67\xc5\x30"
    "\xb6\x8e\xf2\x1f\xf3\xb7\x1f\xbe\xbc\x59\xbc\xff\xe9\xed\x64\xab\xe1\x85\xf9\xcd\x0f\x52\x51\x31\x79\x9c\x58\xb4"
    "\x61\x29\x7a\xba\xf1\x34\x92\xe5\x66\x68\x69\x6c\xff\x2c\x67\x20\x5b\x8e\x55\xd5\xb4\x22\xff\x84\xc9\xe7\xf9\x37"
    "\xe4\x9d\x12\x66\x17\xb7\x64\xcd\x14\x25\x6a\xd7\xd2\xbc\x17\x1d\x79\x17\x49\x83\x3a\xd5\x30\xfc\x23\xb9\xae\x65"
    "\xaf\xd6\x84\xca\x2e\x0e\x22\x2d\xc5\x66\x50\xa0\x98\xa0\x79\xe0\xc3\x18\xf8\x29\x80\x70\x11\xf8\xf7\x51\x72\x0f"
    "\x35\xfd\x25\xc4\x1e\x5a\x53\xb2\x91\x3b\xe1\x48\x75\xe0\xda\x24\x72\x8d\xa1\x3b\x08\x5b\x55\x92\xaa\xdc\x5c\x4b"
    "\x1f\xd9\xac\x64\x4f\x86\x5c\x37\x60\x02\xab\xca\x91\xc3\x3e\x55\x8c\x53\x87\x95\x33\x37\x18\x68\x08\xc7\xf5\xca"
    "\x86\x3a\x16\x65\x0e\x4a\x26\x8d\x7f\x00\xe7\x7b\x61\xee\xe6\xa9\x96\x9a\x43\xef\x9c\x61\xa8\x60\x0a\x34\xc5\xf7"
    "\xeb\x8d\x5e\x27\x80\xb7\x5b\x4d\xd0\x3b\xe3\xa4\xfe\x15\x85\x6a\x99\x06\x8d\x69\x1d\x9d\x3a\xb1\x5d\x02\x75\xd8"
    "\xd2\xfc\x91\x29\xfd\x16\x69\xdc\x98\xb2\x88\x12\x2b\x3c\xd0\x71\x5a\xaf\xd4\x3a\x87\x61\x16\x68\x2f\xf4\x0f\xfd"
    "\xce\xf1\x0d\x3c\xaa\x67\xfa\x62\xc4\x4a\xec\x35\x44\x51\x05\xa4\xd2\x92\x0b\xd7\x1b\x0e\x0c\x4a\xc2\x2c\x09\x2e"
    "\x64\x1d\x74\xed\x4b\x1c\x95\x35\x05\x3a\xd5\x62\x4d\x54\x82\xdb\x17\x15\x07\x18\x47\x45\x9a\x54\x34\x86\x71\x90"
    "\x14\x29\x21\x41\x16\x56\x51\x96\x91\x14\x16\xba\x4a\x4a\xb0\x5f\xd0\x0a\x79\xcf\x99\x86\x12\xd6\x72\xfb\x57\xa8"
    "\xf0\x8c\x68\x90\xfd\xa4\x24\xb2\xea\x1a\x9d\x91\x37\x0a\x6d\x3e\x4e\xb9\x9e\xda\xae\xd6\x4f\x06\x71\xe9\x25\xf8"
    "\xdf\x79\x29\x8b\x5e\xcb\x4b\x81\x7f\xd3\x49\xb6\xc0\x1f\x38\x29\x8b\xc3\x24\x24\x85\x5f\x4d\xb3\x90\xe0\x2c\xc2"
    "\xd3\x28\xc5\x55\x18\x55\xfa\x9a\x33\x1a\x11\x7f\x9a\x84\xd3\x8a\x96\x7f\xef\xa4\xdf\xaf\xf0\x2f\x4e\x82\x37\x9c"
    "\xa4\x41\xe6\x1b\x89\x3c\xfb\xef\xf8\x0b\x00\x00\xff\xff\x03\x00\x6a\x4c\x44\x6a\x31\xdd\xdd\xf7\xe4\x49\x39\xb9"
    "\x6d\xa2\xea\xc9\x78\xcc\x1e\xe5\xe2\x1f\x16\xe6\xde\xc0\x17\x0b\x00\x00\x00\x00\x14\x00\x00\x00\x88\x07\x00\x00"
    "\x07\x00\x00\x01\x42\x43\xc0\xde\x35\x14\x00\x00\x06\x00\x00\x00\x62\x0c\x30\x24\x92\x96\xa6\xa5\xf7\xd7\xff\x4e"
    "\xdf\x3e\xed\xdf\xf5\x4f\x0b\x51\x80\x4c\x01\x00\x21\x0c\x00\x00\xd7\x01\x00\x00\x0b\x82\x20\x00\x02\x00\x00\x00"
    "\x13\x00\x00\x00\x07\x81\x23\x91\x41\xc8\x04\x49\x06\x10\x32\x39\x92\x01\x84\x0c\x25\x05\x08\x19\x1e\x04\x8b\x62"
    "\x80\x10\x45\x02\x42\x92\x0b\x42\x84\x10\x32\x14\x38\x08\x18\x4b\x0a\x32\x42\x88\x48\x90\x14\x20\x43\x46\x88\xa5"
    "\x00\x19\x32\x42\xe4\x48\x0e\x90\x11\x22\xc4\x50\x41\x51\x81\x8c\xe1\x83\xe5\x8a\x04\x21\x46\x06\x51\x18\x00\x00"
    "\xe2\x00\x00\x00\x1b\xfa\x24\xf8\xff\xff\xff\xff\x01\x90\x00\x0d\x08\x03\x82\x1c\xd2\x61\x1e\xc2\x41\x1c\xd8\xa1"
    "\x1c\xda\x80\x1e\xc2\x21\x1d\xd8\xa1\x0d\xc6\x21\x1c\xd8\x81\x1d\xe6\x01\x30\x87\x70\x60\x87\x79\x28\x07\x80\x60"
    "\x87\x72\x98\x87\x79\x68\x03\x78\x90\x87\x72\x18\x87\x74\x98\x87\x72\x68\x03\x73\x80\x87\x76\x08\x07\x72\x00\xcc"
    "\x21\x1c\xd8\x61\x1e\xca\x01\x20\xdc\xe1\x1d\xda\xc0\x1c\xe4\x21\x1c\xda\xa1\x1c\xda\x00\x1e\xde\x21\x1d\xdc\x81"
    "\x1e\xca\x41\x1e\xda\xa0\x1c\xd8\x21\x1d\xda\x01\xa0\x07\x79\xa8\x87\x72\x00\x06\x77\x78\x87\x36\x30\x07\x79\x08"
    "\x87\x76\x28\x87\x36\x80\x87\x77\x48\x07\x77\xa0\x87\x72\x90\x87\x36\x28\x07\x76\x48\x87\x76\x68\x03\x77\x78\x07"
    "\x77\x68\x03\x76\x28\x87\x70\x30\x07\x80\x70\x87\x77\x68\x83\x74\x70\x07\x73\x98\x87\x36\x30\x07\x78\x68\x83\x76"
    "\x08\x07\x7a\x40\x07\xc0\x1c\xc2\x81\x1d\xe6\xa1\x1c\x00\xc2\x1d\xde\xa1\x0d\xdc\x21\x1c\xdc\x61\x1e\xda\xc0\x1c"
    "\xe0\xa1\x0d\xda\x21\x1c\xe8\x01\x1d\x00\x73\x08\x07\x76\x98\x87\x72\x00\x88\x79\xa0\x87\x70\x18\x87\x75\x68\x03"
    "\x78\x90\x87\x77\xa0\x87\x72\x18\x07\x7a\x78\x07\x79\x68\x03\x71\xa8\x07\x73\x30\x87\x72\x90\x87\x36\x98\x87\x74"
    "\xd0\x87\x72\x00\xf0\x00\x20\xe8\x21\x1c\xe4\xe1\x1c\xca\x81\x1e\xda\x60\x1c\xe0\xa1\x1e\x80\x71\x78\x07\x79\x28"
    "\x07\x39\x00\x08\x7a\x08\x07\x79\x38\x87\x72\xa0\x87\x36\x30\x87\x72\x08\x07\x7a\xa8\x07\x79\x28\x87\x79\x00\xd6"
    "\x60\x1c\xf0\x21\x0e\xec\x80\x0d\xd6\xc0\x1c\xf0\x61\x1e\xe4\x81\x0d\xd6\xa0\x1d\xda\x01\x1f\xd8\x60\x0d\xe6\x61"
    "\x1e\xca\x81\x0d\xd6\x60\x1e\xe6\xa1\x1c\xe4\x80\x0d\xd6\x60\x1e\xe6\xa1\x1c\xe6\x80\x0d\xd6\x60\x1e\xe6\x61\x1e"
    "\xca\x61\x0e\x00\xa2\x1e\xdc\x61\x1e\xc2\xc1\x1c\xca\xa1\x0d\xcc\x01\x1e\xda\xa0\x1d\xc2\x81\x1e\xd0\x01\x30\x87"
    "\x70\x60\x87\x79\x28\x07\x80\xa8\x87\x79\x28\x87\x36\x98\x87\x77\x30\x07\x7a\x68\x03\x73\x60\x87\x77\x08\x07\x7a"
    "\x00\xcc\x21\x1c\xd8\x61\x1e\xca\x01\xd8\x70\x27\xc2\xff\xff\xff\xff\x0f\x04\x39\xa4\xc3\x3c\x84\x83\x38\xb0\x43"
    "\x39\xb4\x01\x3d\x84\x43\x3a\xb0\x43\x1b\x8c\x43\x38\xb0\x03\x3b\xcc\x03\x60\x0e\xe1\xc0\x0e\xf3\x50\x0e\x00\xc1"
    "\x0e\xe5\x30\x0f\xf3\xd0\x06\xf0\x20\x0f\xe5\x30\x0e\xe9\x30\x0f\xe5\xd0\x06\xe6\x00\x0f\xed\x10\x0e\xe4\x00\x98"
    "\x43\x38\xb0\xc3\x3c\x94\x03\x40\xb8\xc3\x3b\xb4\x81\x39\xc8\x43\x38\xb4\x43\x39\xb4\x01\x3c\xbc\x43\x3a\xb8\x03"
    "\x3d\x94\x83\x3c\xb4\x41\x39\xb0\x43\x3a\xb4\x03\x40\x0f\xf2\x50\x0f\xe5\x00\x0c\xee\xf0\x0e\x6d\x60\x0e\xf2\x10"
    "\x0e\xed\x50\x0e\x6d\x00\x0f\xef\x90\x0e\xee\x40\x0f\xe5\x20\x0f\x6d\x50\x0e\xec\x90\x0e\xed\xd0\x06\xee\xf0\x0e"
    "\xee\xd0\x06\xec\x50\x0e\xe1\x60\x0e\x00\xe1\x0e\xef\xd0\x06\xe9\xe0\x0e\xe6\x30\x0f\x6d\x60\x0e\xf0\xd0\x06\xed"
    "\x10\x0e\xf4\x80\x0e\x80\x39\x84\x03\x3b\xcc\x43\x39\x00\x84\x3b\xbc\x43\x1b\xb8\x43\x38\xb8\xc3\x3c\xb4\x81\x39"
    "\xc0\x43\x1b\xb4\x43\x38\xd0\x03\x3a\x00\xe6\x10\x0e\xec\x30\x0f\xe5\x00\x10\xf3\x40\x0f\xe1\x30\x0e\xeb\xd0\x06"
    "\xf0\x20\x0f\xef\x40\x0f\xe5\x30\x0e\xf4\xf0\x0e\xf2\xd0\x06\xe2\x50\x0f\xe6\x60\x0e\xe5\x20\x0f\x6d\x30\x0f\xe9"
    "\xa0\x0f\xe5\x00\xe0\x01\x40\xd0\x43\x38\xc8\xc3\x39\x94\x03\x3d\xb4\xc1\x38\xc0\x43\x3d\x00\xe3\xf0\x0e\xf2\x50"
    "\x0e\x72\x00\x10\xf4\x10\x0e\xf2\x70\x0e\xe5\x40\x0f\x6d\x60\x0e\xe5\x10\x0e\xf4\x50\x0f\xf2\x50\x0e\xf3\x00\xac"
    "\xc1\x38\xe0\x43\x1c\xd8\x01\x1b\xac\x81\x39\xe0\xc3\x3c\xc8\x03\x1b\xac\x41\x3b\xb4\x03\x3e\xb0\xc1\x1a\xcc\xc3"
    "\x3c\x94\x03\x1b\xac\xc1\x3c\xcc\x43\x39\xc8\x01\x1b\xac\xc1\x3c\xcc\x43\x39\xcc\x01\x1b\xac\xc1\x3c\xcc\xc3\x3c"
    "\x94\xc3\x1c\x00\x44\x3d\xb8\xc3\x3c\x84\x83\x39\x94\x43\x1b\x98\x03\x3c\xb4\x41\x3b\x84\x03\x3d\xa0\x03\x60\x0e"
    "\xe1\xc0\x0e\xf3\x50\x0e\x00\x51\x0f\xf3\x50\x0e\x6d\x30\x0f\xef\x60\x0e\xf4\xd0\x06\xe6\xc0\x0e\xef\x10\x0e\xf4"
    "\x00\x98\x43\x38\xb0\xc3\x3c\x94\x03\x00\x00\x00\x49\x18\x00\x00\x02\x00\x00\x00\x13\x82\x60\x42\x20\x00\x00\x00"
    "\x89\x20\x00\x00\x0e\x00\x00\x00\x32\x22\x08\x09\x20\x64\x85\x04\x13\x22\xa4\x84\x04\x13\x22\xe3\x84\xa1\x90\x14"
    "\x12\x4c\x88\x8c\x0b\x84\x84\x4c\x10\x38\x73\x04\x48\x12\x00\xc5\x1c\x01\x18\x94\x60\x88\x08\xaa\x30\x46\x57\x84"
    "\x19\xca\x81\x00\x9a\x11\x00\x00\x13\x34\x7c\xc0\x03\x3b\xf8\x05\x3b\xa0\x83\x36\x08\x07\x78\x80\x07\x76\x28\x87"
    "\x36\x68\x87\x70\x18\x87\x77\x98\x07\x7c\x88\x03\x38\x70\x83\x38\x88\x03\x37\x80\x83\x0d\x65\x50\x0e\x6d\xd0\x0e"
    "\x7a\xf0\x0e\x6d\x90\x0e\x76\x40\x07\x7a\x60\x07\x74\xd0\x06\xe6\x80\x07\x70\xa0\x07\x71\x20\x07\x78\xd0\x06\xee"
    "\x80\x07\x7a\x10\x07\x76\xa0\x07\x73\x20\x07\x7a\x60\x07\x74\xd0\x06\xb3\x10\x07\x72\x80\x07\x3a\x0f\x24\x90\x21"
    "\x23\x45\x42\x80\x1d\x4c\x30\x1c\x49\x00\x00\x40\x00\x00\xc0\x90\x87\x00\x00\x20\x00\x00\x00\x00\x00\x00\x00\x80"
    "\x21\xcf\x01\x04\x80\x00\x00\x00\x00\x00\x00\x00\x00\x43\x9e\x04\x08\x00\x01\x00\x00\x00\x00\x00\x00\x00\x24\x36"
    "\x08\x14\x9d\x0e\x00\x00\xc8\x02\x01\x00\x00\x00\x07\x00\x00\x00\x32\x1e\x98\x0c\x19\x11\x4c\x90\x8c\x09\x26\x47"
    "\xc6\x04\x43\x9a\x22\x28\x01\x0a\x1d\x4a\x64\x05\x00\x00\x00\x00\x79\x18\x00\x00\x1c\x00\x00\x00\x1a\x03\x4c\x90"
    "\x46\x02\x13\x34\x41\x18\x0a\x26\x50\x49\x43\x20\x4c\x65\x76\x65\x6c\x13\x84\x81\xd8\x30\x04\xc2\xc0\x34\x08\x82"
    "\x83\x63\x2b\x03\x61\x62\xb2\x6a\x02\xb1\x2b\x93\x9b\x4b\x7b\x73\x03\xb9\x71\x99\x71\x81\x01\x41\x19\x63\x0b\x73"
    "\x3b\x6b\xb9\x81\x99\x71\x81\x71\x99\x89\x49\xd9\x10\x14\x8d\xd8\xd8\xec\xda\x5c\xda\xde\xc8\xea\xd8\xca\x5c\xcc"
    "\xd8\xc2\xce\xe6\xa6\x04\x43\x15\x36\x36\xbb\x36\x97\x34\xb2\x32\x37\xba\x29\x41\x01\x00\x00\x00\x79\x18\x00\x00"
    "\x56\x00\x00\x00\x33\x08\x80\x1c\xc4\xe1\x1c\x66\x14\x01\x3d\x88\x43\x38\x84\xc3\x8c\x42\x80\x07\x79\x78\x07\x73"
    "\x98\x71\x0c\xe6\x00\x0f\xed\x10\x0e\xf4\x80\x0e\x33\x0c\x42\x1e\xc2\xc1\x1d\xce\xa1\x1c\x66\x30\x05\x3d\x88\x43"
    "\x38\x84\x83\x1b\xcc\x03\x3d\xc8\x43\x3d\x8c\x03\x3d\xcc\x78\x8c\x74\x70\x07\x7b\x08\x07\x79\x48\x87\x70\x70\x07"
    "\x7a\x70\x03\x76\x78\x87\x70\x20\x87\x19\xcc\x11\x0e\xec\x90\x0e\xe1\x30\x0f\x6e\x30\x0f\xe3\xf0\x0e\xf0\x50\x0e"
    "\x33\x10\xc4\x1d\xde\x21\x1c\xd8\x21\x1d\xc2\x61\x1e\x66\x30\x89\x3b\xbc\x83\x3b\xd0\x43\x39\xb4\x03\x3c\xbc\x83"
    "\x3c\x84\x03\x3b\xcc\xf0\x14\x76\x60\x07\x7b\x68\x07\x37\x68\x87\x72\x68\x07\x37\x80\x87\x70\x90\x87\x70\x60\x07"
    "\x76\x28\x07\x76\xf8\x05\x76\x78\x87\x77\x80\x87\x5f\x08\x87\x71\x18\x87\x72\x98\x87\x79\x98\x81\x2c\xee\xf0\x0e"
    "\xee\xe0\x0e\xf5\xc0\x0e\xec\x30\x03\x62\xc8\xa1\x1c\xe4\xa1\x1c\xcc\xa1\x1c\xe4\xa1\x1c\xdc\x61\x1c\xca\x21\x1c"
    "\xc4\x81\x1d\xca\x61\x06\xd6\x90\x43\x39\xc8\x43\x39\x98\x43\x39\xc8\x43\x39\xb8\xc3\x38\x94\x43\x38\x88\x03\x3b"
    "\x94\xc3\x2f\xbc\x83\x3c\xfc\x82\x3b\xd4\x03\x3b\xb0\xc3\x0c\xc7\x69\x87\x70\x58\x87\x72\x70\x83\x74\x68\x07\x78"
    "\x60\x87\x74\x18\x87\x74\xa0\x87\x19\xce\x53\x0f\xee\x00\x0f\xf2\x50\x0e\xe4\x90\x0e\xe3\x40\x0f\xe1\x20\x0e\xec"
    "\x50\x0e\x33\x20\x28\x1d\xdc\xc1\x1e\xc2\x41\x1e\xd2\x21\x1c\xdc\x81\x1e\xdc\xe0\x1c\xe4\xe1\x1d\xea\x01\x1e\x66"
    "\x18\x51\x38\xb0\x43\x3a\x9c\x83\x3b\x00\x00\x00\x61\x20\x00\x00\x0f\x00\x00\x00\x13\x04\x41\x2c\x10\x00\x00\x00"
    "\x04\x00\x00\x00\x34\x23\x00\x85\x40\x36\xd4\x11\x08\xc0\x70\x0c\x07\x00\x00\x00\x33\x11\xc3\x50\x8c\xc2\xb0\x01"
    "\x11\x10\x03\x30\x62\x50\x00\x20\x08\x06\x09\x32\x8c\x18\x18\x00\x08\x82\xc1\xa1\x0c\x41\x1a\x00\x71\x20\x00\x00"
    "\x0c\x00\x00\x00\x52\x0e\x10\x22\x64\x82\xa4\x1c\x20\x44\xc8\xe4\x48\x39\x40\x88\x90\xa1\x09\x18\x4f\x84\x34\x53"
    "\xa1\x01\xc2\x03\x04\x03\x20\x8d\x01\x10\xbe\x34\x45\x36\x60\x3c\x8e\x45\x44\x36\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\xde\xc0\x17\x0b\x00\x00\x00\x00\x14\x00\x00\x00\x58\x05\x00\x00\x07\x00\x00\x01\x42\x43\xc0\xde"
    "\x35\x14\x00\x00\x06\x00\x00\x00\x62\x0c\x30\x24\x92\x96\xa6\xa5\xf7\xd7\xff\x4e\xdf\x3e\xed\xdf\xf5\x4f\x0b\x51"
    "\x80\x4c\x01\x00\x21\x0c\x00\x00\x4b\x01\x00\x00\x0b\x82\x20\x00\x02\x00\x00\x00\x13\x00\x00\x00\x07\x81\x23\x91"
    "\x41\xc8\x04\x49\x06\x10\x32\x39\x92\x01\x84\x0c\x25\x05\x08\x19\x1e\x04\x8b\x62\x80\x0c\x45\x02\x42\x92\x0b\x42"
    "\x64\x10\x32\x14\x38\x08\x18\x4b\x0a\x32\x32\x88\x48\x90\x14\x20\x43\x46\x88\xa5\x00\x19\x32\x42\xe4\x48\x0e\x90"
    "\x91\x21\xc4\x50\x41\x51\x81\x8c\xe1\x83\xe5\x8a\x04\x19\x46\x06\x51\x18\x00\x00\x72\x00\x00\x00\x1b\xfa\x24\xf8"
    "\xff\xff\xff\xff\x01\x90\x00\x0d\x08\x03\x82\x1c\xd2\x61\x1e\xc2\x41\x1c\xd8\xa1\x1c\xda\x80\x1e\xc2\x21\x1d\xd8"
    "\xa1\x0d\xc6\x21\x1c\xd8\x81\x1d\xe6\x01\x30\x87\x70\x60\x87\x79\x28\x07\x80\x60\x87\x72\x98\x87\x79\x68\x03\x78"
    "\x90\x87\x72\x18\x87\x74\x98\x87\x72\x68\x03\x73\x80\x87\x76\x08\x07\x72\x00\xcc\x21\x1c\xd8\x61\x1e\xca\x01\x20"
    "\xdc\xe1\x1d\xda\xc0\x1c\xe4\x21\x1c\xda\xa1\x1c\xda\x00\x1e\xde\x21\x1d\xdc\x81\x1e\xca\x41\x1e\xda\xa0\x1c\xd8"
    "\x21\x1d\xda\x01\xa0\x07\x79\xa8\x87\x72\x00\x06\x77\x78\x87\x36\x30\x07\x79\x08\x87\x76\x28\x87\x36\x80\x87\x77"
    "\x48\x07\x77\xa0\x87\x72\x90\x87\x36\x28\x07\x76\x48\x87\x76\x68\x03\x77\x78\x07\x77\x68\x03\x76\x28\x87\x70\x30"
    "\x07\x80\x70\x87\x77\x68\x83\x74\x70\x07\x73\x98\x87\x36\x30\x07\x78\x68\x83\x76\x08\x07\x7a\x40\x07\xc0\x1c\xc2"
    "\x81\x1d\xe6\xa1\x1c\x00\xc2\x1d\xde\xa1\x0d\xdc\x21\x1c\xdc\x61\x1e\xda\xc0\x1c\xe0\xa1\x0d\xda\x21\x1c\xe8\x01"
    "\x1d\x00\x73\x08\x07\x76\x98\x87\x72\x00\x88\x79\xa0\x87\x70\x18\x87\x75\x68\x03\x78\x90\x87\x77\xa0\x87\x72\x18"
    "\x07\x7a\x78\x07\x79\x68\x03\x71\xa8\x07\x73\x30\x87\x72\x90\x87\x36\x98\x87\x74\xd0\x87\x72\x00\xf0\x00\x20\xe8"
    "\x21\x1c\xe4\xe1\x1c\xca\x81\x1e\xda\x60\x1c\xe0\xa1\x1e\x80\x71\x78\x07\x79\x28\x07\x39\x00\x08\x7a\x08\x07\x79"
    "\x38\x87\x72\xa0\x87\x36\x30\x87\x72\x08\x07\x7a\xa8\x07\x79\x28\x87\x79\x00\xd6\x60\x1c\xf0\x21\x0e\xec\x80\x0d"
    "\xd6\xc0\x1c\xf0\x61\x1e\xe4\x81\x0d\xd6\xa0\x1d\xda\x01\x1f\xd8\x60\x0d\xe6\x61\x1e\xca\x81\x0d\xd6\x60\x1e\xe6"
    "\xa1\x1c\xe4\x80\x0d\xd6\x60\x1e\xe6\xa1\x1c\xe6\x80\x0d\xd6\x60\x1e\xe6\x61\x1e\xca\x61\x0e\x00\xa2\x1e\xdc\x61"
    "\x1e\xc2\xc1\x1c\xca\xa1\x0d\xcc\x01\x1e\xda\xa0\x1d\xc2\x81\x1e\xd0\x01\x30\x87\x70\x60\x87\x79\x28\x07\x80\xa8"
    "\x87\x79\x28\x87\x36\x98\x87\x77\x30\x07\x7a\x68\x03\x73\x60\x87\x77\x08\x07\x7a\x00\xcc\x21\x1c\xd8\x61\x1e\xca"
    "\x01\x00\x00\x00\x49\x18\x00\x00\x01\x00\x00\x00\x13\x82\x00\x00\x89\x20\x00\x00\x0b\x00\x00\x00\x32\x22\xc8\x08"
    "\x20\x64\x85\x04\x93\x21\xa4\x84\x04\x93\x21\xe3\x84\xa1\x90\x14\x12\x4c\x86\x8c\x0b\x84\x64\x4c\x10\x18\x73\x04"
    "\x60\x50\x04\x80\x32\x10\x40\x18\x01\x00\x00\x00\x13\x34\x7c\xc0\x03\x3b\xf8\x05\x3b\xa0\x83\x36\x08\x07\x78\x80"
    "\x07\x76\x28\x87\x36\x68\x87\x70\x18\x87\x77\x98\x07\x7c\x88\x03\x38\x70\x83\x38\x88\x03\x37\x80\x83\x0d\x65\x50"
    "\x0e\x6d\xd0\x0e\x7a\xf0\x0e\x6d\x90\x0e\x76\x40\x07\x7a\x60\x07\x74\xd0\x06\xe6\x80\x07\x70\xa0\x07\x71\x20\x07"
    "\x78\xd0\x06\xee\x80\x07\x7a\x10\x07\x76\xa0\x07\x73\x20\x07\x7a\x60\x07\x74\xd0\x06\xb3\x10\x07\x72\x80\x07\x43"
    "\x9e\x00\x00\x80\x00\x00\x00\x00\x00\x00\x00\x00\x24\x36\x08\x14\x64\x0a\x00\x00\xc8\x02\x01\x00\x05\x00\x00\x00"
    "\x32\x1e\x98\x08\x19\x11\x4c\x90\x8c\x09\x26\x47\xc6\x04\x43\x42\x09\x14\x01\x00\x79\x18\x00\x00\x1c\x00\x00\x00"
    "\x1a\x03\x4c\x90\x46\x02\x13\x34\x41\x00\x02\x26\x50\x49\x43\x20\x4c\x65\x76\x65\x6c\x13\x04\x40\xd8\x30\x04\xc2"
    "\xc0\x34\x08\x82\x83\x63\x2b\x03\x61\x62\xb2\x6a\x02\xb1\x2b\x93\x9b\x4b\x7b\x73\x03\xb9\x71\x99\x71\x81\x01\x41"
    "\x19\x63\x0b\x73\x3b\x6b\xb9\x81\x99\x71\x81\x71\x99\x89\x49\xd9\x10\x14\x8d\xd8\xd8\xec\xda\x5c\xda\xde\xc8\xea"
    "\xd8\xca\x5c\xcc\xd8\xc2\xce\xe6\xa6\x04\x43\x15\x36\x36\xbb\x36\x97\x34\xb2\x32\x37\xba\x29\x41\x01\x00\x00\x00"
    "\x79\x18\x00\x00\x56\x00\x00\x00\x33\x08\x80\x1c\xc4\xe1\x1c\x66\x14\x01\x3d\x88\x43\x38\x84\xc3\x8c\x42\x80\x07"
    "\x79\x78\x07\x73\x98\x71\x0c\xe6\x00\x0f\xed\x10\x0e\xf4\x80\x0e\x33\x0c\x42\x1e\xc2\xc1\x1d\xce\xa1\x1c\x66\x30"
    "\x05\x3d\x88\x43\x38\x84\x83\x1b\xcc\x03\x3d\xc8\x43\x3d\x8c\x03\x3d\xcc\x78\x8c\x74\x70\x07\x7b\x08\x07\x79\x48"
    "\x87\x70\x70\x07\x7a\x70\x03\x76\x78\x87\x70\x20\x87\x19\xcc\x11\x0e\xec\x90\x0e\xe1\x30\x0f\x6e\x30\x0f\xe3\xf0"
    "\x0e\xf0\x50\x0e\x33\x10\xc4\x1d\xde\x21\x1c\xd8\x21\x1d\xc2\x61\x1e\x66\x30\x89\x3b\xbc\x83\x3b\xd0\x43\x39\xb4"
    "\x03\x3c\xbc\x83\x3c\x84\x03\x3b\xcc\xf0\x14\x76\x60\x07\x7b\x68\x07\x37\x68\x87\x72\x68\x07\x37\x80\x87\x70\x90"
    "\x87\x70\x60\x07\x76\x28\x07\x76\xf8\x05\x76\x78\x87\x77\x80\x87\x5f\x08\x87\x71\x18\x87\x72\x98\x87\x79\x98\x81"
    "\x2c\xee\xf0\x0e\xee\xe0\x0e\xf5\xc0\x0e\xec\x30\x03\x62\xc8\xa1\x1c\xe4\xa1\x1c\xcc\xa1\x1c\xe4\xa1\x1c\xdc\x61"
    "\x1c\xca\x21\x1c\xc4\x81\x1d\xca\x61\x06\xd6\x90\x43\x39\xc8\x43\x39\x98\x43\x39\xc8\x43\x39\xb8\xc3\x38\x94\x43"
    "\x38\x88\x03\x3b\x94\xc3\x2f\xbc\x83\x3c\xfc\x82\x3b\xd4\x03\x3b\xb0\xc3\x0c\xc7\x69\x87\x70\x58\x87\x72\x70\x83"
    "\x74\x68\x07\x78\x60\x87\x74\x18\x87\x74\xa0\x87\x19\xce\x53\x0f\xee\x00\x0f\xf2\x50\x0e\xe4\x90\x0e\xe3\x40\x0f"
    "\xe1\x20\x0e\xec\x50\x0e\x33\x20\x28\x1d\xdc\xc1\x1e\xc2\x41\x1e\xd2\x21\x1c\xdc\x81\x1e\xdc\xe0\x1c\xe4\xe1\x1d"
    "\xea\x01\x1e\x66\x18\x51\x38\xb0\x43\x3a\x9c\x83\x3b\x00\x00\x00\x61\x20\x00\x00\x0a\x00\x00\x00\x13\x04\xc1\x4c"
    "\x04\x00\x04\xa3\x30\x6c\x40\x04\xc2\x00\x50\x80\x41\x02\x86\x09\x81\x08\x32\x08\x07\x02\x00\x00\x01\x00\x00\x00"
    "\x36\x10\x5c\x00\x00\x00\x00\x00\x71\x20\x00\x00\x07\x00\x00\x00\x52\x0e\x10\x22\x64\x82\xa4\x1c\x20\x44\xc8\xe4"
    "\x48\x39\x40\x88\x90\xa1\x02\x00\x0b\x18\x8f\x63\x11\x91\x0d\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00";

TEST_CASE("Bitcode Archive Files", "[BitcodeArchive]") {
  auto bitcodeArchive = BitcodeArchive(data, size);
  bitcodeArchive.GetBinaryMetadata().SetFileFormatName("Mach-O 64-bit x86-64");

  const std::vector<EmbeddedBitcode> expectedFiles = {
      EmbeddedBitcode("Mach-O 64-bit x86-64.0.bc"), EmbeddedBitcode("Mach-O 64-bit x86-64.1.bc"),
  };

  auto actualFiles = bitcodeArchive.GetEmbeddedFiles();
  if (util::xar::HasXar()) {
    REQUIRE(expectedFiles.size() == 2);
  } else {
    REQUIRE(actualFiles.empty());
  }
}

TEST_CASE("Bitcode Archive Linker Flags", "[BitcodeArchive]") {
  auto bitcodeArchive = BitcodeArchive(data, size);
  auto& bitcodeMetadata = bitcodeArchive.GetMetadata();

  const std::vector<std::string> expectedLinkOpts = {"-execute", "-macosx_version_min", "10.11.0",       "-e",
                                                     "_main",    "-executable_path",    "build/x86_64.o"};

  if (util::xar::HasXar()) {
    REQUIRE(bitcodeMetadata.GetLinkOptions() == expectedLinkOpts);
  } else {
    REQUIRE(bitcodeMetadata.GetLinkOptions().empty());
  }
}

TEST_CASE("Bitcode Archive Dylibs", "[BitcodeArchive]") {
  auto bitcodeArchive = BitcodeArchive(data, size);
  auto& bitcodeMetadata = bitcodeArchive.GetMetadata();

  const std::vector<std::string> expectedDylibs = {"{SDKPATH}/usr/lib/libSystem.B.dylib"};

  if (util::xar::HasXar()) {
    REQUIRE(bitcodeMetadata.GetDylibs() == expectedDylibs);
  } else {
    REQUIRE(bitcodeMetadata.GetDylibs().empty());
  }
}
