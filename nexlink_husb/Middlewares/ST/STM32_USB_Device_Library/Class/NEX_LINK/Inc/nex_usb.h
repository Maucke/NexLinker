/*

The MIT License (MIT)

Copyright (c) 2016 Hubert Denkmair

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#pragma once

#include <stdint.h>

#define u32 uint32_t
#define u8 uint8_t

#define GSUSB_ENDPOINT_IN          0x81
#define GSUSB_ENDPOINT_OUT         0x02

enum nex_usb_breq {
	NEX_BREQ_HOST_FORMAT = 0,
	NEX_TIMESTAMP_SET,
	NEX_TIMESTAMP_GET,
	NEX_BRIGHTNESS_SET,
	NEX_BRIGHTNESS_GET,
	NEX_SCREEN_SET,
	NEX_SCREEN_GET,
	NEX_COMMAND_LEN,
};

typedef struct {
	uint16_t brightness;
	uint16_t damp;
}nex_brightness_des;

typedef struct {
	uint8_t direction:2;
	uint8_t reserved:6;
}nex_screen_des;

typedef struct {
	uint64_t timestamp_s;
	nex_brightness_des brides;
	nex_screen_des scrdes;
}nex_usb_des;
