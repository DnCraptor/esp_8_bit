
/* Copyright (c) 2020, Peter Barrett
**
** Permission to use, copy, modify, and/or distribute this software for
** any purpose with or without fee is hereby granted, provided that the
** above copyright notice and this permission notice appear in all copies.
**
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
** WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
** BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
** OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
** WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
** ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
** SOFTWARE.
*/

#include "emu.h"
extern "C" {
    #include "ff.h"
}

using namespace std;

string get_ext(const string& s)
{
    string ext;
    auto i = s.find_last_of(".");
    if (i > 0) {
        ext = s.substr(i+1);
        for (i = 0; i < ext.length(); i++)
            ext[i] = tolower(ext[i]);
    }
    return ext;
}

// missing in arduino?
string to_string(int i)
{
    char buf[32];
    sprintf(buf,"%d",i);
    return buf;
}

const uint8_t _font[2048] = {
    // atari 256 char inverted set
    0x00,0x18,0x03,0x18,0x18,0x00,0x03,0xc0,
    0x01,0x00,0x80,0x0f,0xf0,0xff,0x00,0x00,
    0x24,0x28,0x08,0x10,0x10,0x10,0x08,0x08,
    0x08,0x08,0x08,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x18,0x00,0x1c,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x06,0x00,0x60,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x18,0x00,0x08,0x10,
    0xff,0xe7,0xfc,0xe7,0xe7,0xff,0xfc,0x3f,
    0xfe,0xff,0x7f,0xf0,0x0f,0x00,0xff,0xff,
    0xff,0xff,0xff,0xe7,0xff,0xff,0x3f,0xff,
    0xe7,0x0f,0xe7,0x87,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xe7,0xff,0xe3,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xf9,0xff,0x9f,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xe7,0xff,0xf7,0xef,

    0x00,0x18,0x03,0x18,0x18,0x00,0x07,0xe0,
    0x03,0x00,0xc0,0x0f,0xf0,0xff,0x00,0x00,
    0x00,0x00,0x10,0x08,0x08,0x08,0x14,0x14,
    0x14,0x14,0x14,0x00,0x18,0x18,0x18,0x18,
    0x00,0x18,0x66,0x66,0x3e,0x66,0x36,0x18,
    0x0e,0x70,0x66,0x18,0x00,0x00,0x00,0x06,
    0x3c,0x18,0x3c,0x7e,0x0c,0x7e,0x3c,0x7e,
    0x3c,0x3c,0x00,0x00,0x0c,0x00,0x30,0x3c,
    0x3c,0x18,0x7c,0x3c,0x78,0x7e,0x7e,0x3e,
    0x66,0x7e,0x06,0x66,0x60,0x63,0x66,0x3c,
    0x7c,0x3c,0x7c,0x3c,0x7e,0x66,0x66,0x63,
    0x66,0x66,0x7e,0x1e,0x40,0x78,0x08,0x00,
    0x18,0x00,0x60,0x00,0x06,0x00,0x0e,0x00,
    0x60,0x18,0x06,0x60,0x38,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,
    0x00,0x00,0x00,0x18,0x18,0x7e,0x18,0x18,
    0xff,0xe7,0xfc,0xe7,0xe7,0xff,0xf8,0x1f,
    0xfc,0xff,0x3f,0xf0,0x0f,0x00,0xff,0xff,
    0xe3,0xff,0xff,0xe7,0xff,0xff,0x3f,0xff,
    0xe7,0x0f,0xe7,0x9f,0xe7,0xe7,0xe7,0xe7,
    0xff,0xe7,0x99,0x99,0xc1,0x99,0xc9,0xe7,
    0xf1,0x8f,0x99,0xe7,0xff,0xff,0xff,0xf9,
    0xc3,0xe7,0xc3,0x81,0xf3,0x81,0xc3,0x81,
    0xc3,0xc3,0xff,0xff,0xf3,0xff,0xcf,0xc3,
    0xc3,0xe7,0x83,0xc3,0x87,0x81,0x81,0xc1,
    0x99,0x81,0xf9,0x99,0x9f,0x9c,0x99,0xc3,
    0x83,0xc3,0x83,0xc3,0x81,0x99,0x99,0x9c,
    0x99,0x99,0x81,0xe1,0xbf,0x87,0xf7,0xff,
    0xe7,0xff,0x9f,0xff,0xf9,0xff,0xf1,0xff,
    0x9f,0xe7,0xf9,0x9f,0xc7,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xe7,0xff,0xff,0xff,
    0xff,0xff,0xff,0xe7,0xe7,0x81,0xe7,0xe7,

    0x00,0x18,0x03,0x18,0x18,0x00,0x0e,0x70,
    0x07,0x00,0xe0,0x0f,0xf0,0x00,0x00,0x00,
    0x3c,0x38,0x3c,0x3c,0x3c,0x66,0x3c,0x3c,
    0x00,0x3c,0x62,0x3c,0x3c,0x18,0x30,0x0c,
    0x00,0x18,0x66,0xff,0x60,0x6c,0x1c,0x18,
    0x1c,0x38,0x3c,0x18,0x00,0x00,0x00,0x0c,
    0x66,0x38,0x66,0x0c,0x1c,0x60,0x60,0x06,
    0x66,0x66,0x18,0x18,0x18,0x7e,0x18,0x66,
    0x66,0x3c,0x66,0x66,0x6c,0x60,0x60,0x60,
    0x66,0x18,0x06,0x6c,0x60,0x77,0x76,0x66,
    0x66,0x66,0x66,0x60,0x18,0x66,0x66,0x63,
    0x66,0x66,0x0c,0x18,0x60,0x18,0x1c,0x00,
    0x3c,0x3c,0x60,0x3c,0x06,0x3c,0x18,0x3e,
    0x60,0x00,0x00,0x60,0x18,0x66,0x7c,0x3c,
    0x7c,0x3e,0x7c,0x3e,0x7e,0x66,0x66,0x63,
    0x66,0x66,0x7e,0x3c,0x18,0x78,0x38,0x1c,
    0xff,0xe7,0xfc,0xe7,0xe7,0xff,0xf1,0x8f,
    0xf8,0xff,0x1f,0xf0,0x0f,0xff,0xff,0xff,
    0xe3,0xff,0xff,0xe7,0xc3,0xff,0x3f,0xff,
    0xe7,0x0f,0xe7,0x87,0xc3,0xe7,0xcf,0xf3,
    0xff,0xe7,0x99,0x00,0x9f,0x93,0xe3,0xe7,
    0xe3,0xc7,0xc3,0xe7,0xff,0xff,0xff,0xf3,
    0x99,0xc7,0x99,0xf3,0xe3,0x9f,0x9f,0xf9,
    0x99,0x99,0xe7,0xe7,0xe7,0x81,0xe7,0x99,
    0x99,0xc3,0x99,0x99,0x93,0x9f,0x9f,0x9f,
    0x99,0xe7,0xf9,0x93,0x9f,0x88,0x89,0x99,
    0x99,0x99,0x99,0x9f,0xe7,0x99,0x99,0x9c,
    0x99,0x99,0xf3,0xe7,0x9f,0xe7,0xe3,0xff,
    0xc3,0xc3,0x9f,0xc3,0xf9,0xc3,0xe7,0xc1,
    0x9f,0xff,0xff,0x9f,0xe7,0x99,0x83,0xc3,
    0x83,0xc1,0x83,0xc1,0x81,0x99,0x99,0x9c,
    0x99,0x99,0x81,0xc3,0xe7,0x87,0xc7,0xe3,

    0x00,0x1f,0x03,0xf8,0xf8,0xf8,0x1c,0x38,
    0x0f,0x00,0xf0,0x0f,0xf0,0x00,0x00,0x00,
    0x66,0x18,0x66,0x06,0x66,0x66,0x06,0x66,
    0x38,0x66,0x66,0x60,0x7e,0x18,0x7e,0x7e,
    0x00,0x18,0x66,0x66,0x3c,0x18,0x38,0x18,
    0x18,0x18,0xff,0x7e,0x00,0x7e,0x00,0x18,
    0x6e,0x18,0x0c,0x18,0x3c,0x7c,0x7c,0x0c,
    0x3c,0x3e,0x18,0x18,0x30,0x00,0x0c,0x0c,
    0x6e,0x66,0x7c,0x60,0x66,0x7c,0x7c,0x60,
    0x7e,0x18,0x06,0x78,0x60,0x7f,0x7e,0x66,
    0x66,0x66,0x66,0x3c,0x18,0x66,0x66,0x6b,
    0x3c,0x3c,0x18,0x18,0x30,0x18,0x36,0x00,
    0x7e,0x06,0x7c,0x60,0x3e,0x66,0x3e,0x66,
    0x7c,0x38,0x06,0x6c,0x18,0x7f,0x66,0x66,
    0x66,0x66,0x66,0x60,0x18,0x66,0x66,0x6b,
    0x3c,0x66,0x0c,0x7e,0x18,0x7c,0x78,0x1e,
    0xff,0xe0,0xfc,0x07,0x07,0x07,0xe3,0xc7,
    0xf0,0xff,0x0f,0xf0,0x0f,0xff,0xff,0xff,
    0x88,0xe0,0x00,0x00,0x81,0xff,0x3f,0x00,
    0x00,0x0f,0xe0,0x9f,0x81,0xe7,0x81,0x81,
    0xff,0xe7,0x99,0x99,0xc3,0xe7,0xc7,0xe7,
    0xe7,0xe7,0x00,0x81,0xff,0x81,0xff,0xe7,
    0x91,0xe7,0xf3,0xe7,0xc3,0x83,0x83,0xf3,
    0xc3,0xc1,0xe7,0xe7,0xcf,0xff,0xf3,0xf3,
    0x91,0x99,0x83,0x9f,0x99,0x83,0x83,0x9f,
    0x81,0xe7,0xf9,0x87,0x9f,0x80,0x81,0x99,
    0x99,0x99,0x99,0xc3,0xe7,0x99,0x99,0x94,
    0xc3,0xc3,0xe7,0xe7,0xcf,0xe7,0xc9,0xff,
    0x81,0xf9,0x83,0x9f,0xc1,0x99,0xc1,0x99,
    0x83,0xc7,0xf9,0x93,0xe7,0x80,0x99,0x99,
    0x99,0x99,0x99,0x9f,0xe7,0x99,0x99,0x94,
    0xc3,0x99,0xf3,0x81,0xe7,0x83,0x87,0xe1,

    0x00,0x1f,0x03,0xf8,0xf8,0xf8,0x38,0x1c,
    0x1f,0x0f,0xf8,0x00,0x00,0x00,0x00,0xf0,
    0x7e,0x18,0x7e,0x3e,0x7e,0x66,0x3e,0x7e,
    0x18,0x66,0x66,0x60,0x18,0x7e,0x30,0x0c,
    0x00,0x18,0x00,0x66,0x06,0x30,0x6f,0x00,
    0x18,0x18,0x3c,0x18,0x00,0x00,0x00,0x30,
    0x76,0x18,0x18,0x0c,0x6c,0x06,0x66,0x18,
    0x66,0x06,0x00,0x00,0x18,0x00,0x18,0x18,
    0x6e,0x66,0x66,0x60,0x66,0x60,0x60,0x6e,
    0x66,0x18,0x06,0x78,0x60,0x6b,0x7e,0x66,
    0x7c,0x66,0x7c,0x06,0x18,0x66,0x66,0x7f,
    0x3c,0x18,0x30,0x18,0x18,0x18,0x63,0x00,
    0x7e,0x3e,0x66,0x60,0x66,0x7e,0x18,0x66,
    0x66,0x18,0x06,0x78,0x18,0x7f,0x66,0x66,
    0x66,0x66,0x60,0x3c,0x18,0x66,0x66,0x7f,
    0x18,0x66,0x18,0x7e,0x18,0x6e,0x38,0x1c,
    0xff,0xe0,0xfc,0x07,0x07,0x07,0xc7,0xe3,
    0xe0,0xf0,0x07,0xff,0xff,0xff,0xff,0x0f,
    0x88,0xe0,0x00,0x00,0x81,0x00,0x3f,0x00,
    0x00,0x0f,0xe0,0x81,0xe7,0x81,0xcf,0xf3,
    0xff,0xe7,0xff,0x99,0xf9,0xcf,0x90,0xff,
    0xe7,0xe7,0xc3,0xe7,0xff,0xff,0xff,0xcf,
    0x89,0xe7,0xe7,0xf3,0x93,0xf9,0x99,0xe7,
    0x99,0xf9,0xff,0xff,0xe7,0xff,0xe7,0xe7,
    0x91,0x99,0x99,0x9f,0x99,0x9f,0x9f,0x91,
    0x99,0xe7,0xf9,0x87,0x9f,0x94,0x81,0x99,
    0x83,0x99,0x83,0xf9,0xe7,0x99,0x99,0x80,
    0xc3,0xe7,0xcf,0xe7,0xe7,0xe7,0x9c,0xff,
    0x81,0xc1,0x99,0x9f,0x99,0x81,0xe7,0x99,
    0x99,0xe7,0xf9,0x87,0xe7,0x80,0x99,0x99,
    0x99,0x99,0x9f,0xc3,0xe7,0x99,0x99,0x80,
    0xe7,0x99,0xe7,0x81,0xe7,0x91,0xc7,0xe3,

    0x00,0x18,0x03,0x00,0x18,0x18,0x70,0x0e,
    0x3f,0x0f,0xfc,0x00,0x00,0x00,0x00,0xf0,
    0x60,0x18,0x60,0x66,0x60,0x66,0x66,0x60,
    0x18,0x66,0x66,0x60,0x18,0x3c,0x18,0x18,
    0x00,0x00,0x00,0xff,0x7c,0x66,0x66,0x00,
    0x1c,0x38,0x66,0x18,0x18,0x00,0x18,0x60,
    0x66,0x18,0x30,0x66,0x7e,0x66,0x66,0x30,
    0x66,0x0c,0x18,0x18,0x0c,0x7e,0x30,0x00,
    0x60,0x7e,0x66,0x66,0x6c,0x60,0x60,0x66,
    0x66,0x18,0x66,0x6c,0x60,0x63,0x6e,0x66,
    0x60,0x6c,0x6c,0x06,0x18,0x66,0x3c,0x77,
    0x66,0x18,0x60,0x18,0x0c,0x18,0x00,0x00,
    0x3c,0x66,0x66,0x60,0x66,0x60,0x18,0x3e,
    0x66,0x18,0x06,0x6c,0x18,0x6b,0x66,0x66,
    0x7c,0x3e,0x60,0x06,0x18,0x66,0x3c,0x3e,
    0x3c,0x3e,0x30,0x18,0x18,0x66,0x18,0x18,
    0xff,0xe7,0xfc,0xff,0xe7,0xe7,0x8f,0xf1,
    0xc0,0xf0,0x03,0xff,0xff,0xff,0xff,0x0f,
    0xf7,0xe7,0xff,0xe7,0x81,0x00,0x3f,0xe7,
    0xff,0x0f,0xff,0xe7,0xe7,0xc3,0xe7,0xe7,
    0xff,0xff,0xff,0x00,0x83,0x99,0x99,0xff,
    0xe3,0xc7,0x99,0xe7,0xe7,0xff,0xe7,0x9f,
    0x99,0xe7,0xcf,0x99,0x81,0x99,0x99,0xcf,
    0x99,0xf3,0xe7,0xe7,0xf3,0x81,0xcf,0xff,
    0x9f,0x81,0x99,0x99,0x93,0x9f,0x9f,0x99,
    0x99,0xe7,0x99,0x93,0x9f,0x9c,0x91,0x99,
    0x9f,0x93,0x93,0xf9,0xe7,0x99,0xc3,0x88,
    0x99,0xe7,0x9f,0xe7,0xf3,0xe7,0xff,0xff,
    0xc3,0x99,0x99,0x9f,0x99,0x9f,0xe7,0xc1,
    0x99,0xe7,0xf9,0x93,0xe7,0x94,0x99,0x99,
    0x83,0xc1,0x9f,0xf9,0xe7,0x99,0xc3,0xc1,
    0xc3,0xc1,0xcf,0xe7,0xe7,0x99,0xe7,0xe7,

    0x00,0x18,0x03,0x00,0x18,0x18,0xe0,0x07,
    0x7f,0x0f,0xfe,0x00,0x00,0x00,0xff,0xf0,
    0x3c,0x3c,0x3c,0x3e,0x3c,0x3e,0x3e,0x3c,
    0x3c,0x3c,0x3e,0x3c,0x18,0x18,0x00,0x00,
    0x00,0x18,0x00,0x66,0x18,0x46,0x3b,0x00,
    0x0e,0x70,0x00,0x00,0x18,0x00,0x18,0x40,
    0x3c,0x7e,0x7e,0x3c,0x0c,0x3c,0x3c,0x30,
    0x3c,0x38,0x18,0x18,0x06,0x00,0x60,0x18,
    0x3e,0x66,0x7c,0x3c,0x78,0x7e,0x60,0x3e,
    0x66,0x7e,0x3c,0x66,0x7e,0x63,0x66,0x3c,
    0x60,0x36,0x66,0x3c,0x18,0x7e,0x18,0x63,
    0x66,0x18,0x7e,0x1e,0x06,0x78,0x00,0xff,
    0x18,0x3e,0x7c,0x3c,0x3e,0x3c,0x18,0x06,
    0x66,0x3c,0x06,0x66,0x3c,0x63,0x66,0x3c,
    0x60,0x06,0x60,0x7c,0x0e,0x3e,0x18,0x36,
    0x66,0x0c,0x7e,0x3c,0x18,0x06,0x08,0x10,
    0xff,0xe7,0xfc,0xff,0xe7,0xe7,0x1f,0xf8,
    0x80,0xf0,0x01,0xff,0xff,0xff,0x00,0x0f,
    0xe3,0xe7,0xff,0xe7,0xc3,0x00,0x3f,0xe7,
    0xff,0x0f,0xff,0xe1,0xe7,0xe7,0xff,0xff,
    0xff,0xe7,0xff,0x99,0xe7,0xb9,0xc4,0xff,
    0xf1,0x8f,0xff,0xff,0xe7,0xff,0xe7,0xbf,
    0xc3,0x81,0x81,0xc3,0xf3,0xc3,0xc3,0xcf,
    0xc3,0xc7,0xe7,0xe7,0xf9,0xff,0x9f,0xe7,
    0xc1,0x99,0x83,0xc3,0x87,0x81,0x9f,0xc1,
    0x99,0x81,0xc3,0x99,0x81,0x9c,0x99,0xc3,
    0x9f,0xc9,0x99,0xc3,0xe7,0x81,0xe7,0x9c,
    0x99,0xe7,0x81,0xe1,0xf9,0x87,0xff,0x00,
    0xe7,0xc1,0x83,0xc3,0xc1,0xc3,0xe7,0xf9,
    0x99,0xc3,0xf9,0x99,0xc3,0x9c,0x99,0xc3,
    0x9f,0xf9,0x9f,0x83,0xf1,0xc1,0xe7,0xc9,
    0x99,0xf3,0x81,0xc3,0xe7,0xf9,0xf7,0xef,

    0x00,0x18,0x03,0x00,0x18,0x18,0xc0,0x03,
    0xff,0x0f,0xff,0x00,0x00,0x00,0xff,0xf0,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7c,
    0x00,0x00,0x3c,0x00,0x00,0x00,0x00,0x00,
    0x60,0x06,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x78,0x00,0x00,0x18,0x00,0x00,0x00,
    0xff,0xe7,0xfc,0xff,0xe7,0xe7,0x3f,0xfc,
    0x00,0xf0,0x00,0xff,0xff,0xff,0x00,0x0f,
    0xff,0xe7,0xff,0xe7,0xff,0x00,0x3f,0xe7,
    0xff,0x0f,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xcf,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xcf,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x83,
    0xff,0xff,0xc3,0xff,0xff,0xff,0xff,0xff,
    0x9f,0xf9,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0x87,0xff,0xff,0xe7,0xff,0xff,0xff,
};

const char* _menu[] = {
    "Files",
    "Info",
    "Help",
    NULL
};

class Overlay {
public:
    uint8_t* _buf;
    uint32_t _blit[16];

    uint8_t** _lines;
    int _width;
    int _height;
    int _hilite;
    int _flavor;
    int OVERLAY_WIDTH;
    int OVERLAY_HEIGHT;

    Overlay() : _hilite(0),_buf(0)
    {
    }

    void set_colors(uint8_t fg, uint8_t bg)
    {
        for (int i = 0; i < 16; i++) {
            uint32_t m = 0;
            for (int j = 0; j < 4; j++) {
                m <<= 8;
                if ((1 << j) & i)
                    m |= fg;
                else
                    m |= bg;
            }
            _blit[i] = m;
        }
    }

    void init(uint8_t** lines, int width, int height, int flavor)
    {
        _lines = lines;
        _width = width;
        _height = height;
        _flavor = flavor;
        switch (_flavor) {
            case EMU_NES:
                OVERLAY_WIDTH = 28;
                OVERLAY_HEIGHT = 20;
                set_colors(0x39,0x09);  // nes
                break;
            case EMU_ATARI:
                OVERLAY_WIDTH = 34;
                OVERLAY_HEIGHT = 22;
                set_colors(0xCE,0xC2);  // atari
                break;
            case EMU_SMS:
                OVERLAY_WIDTH = 28;
                OVERLAY_HEIGHT = 20;
                set_colors(7<<2,1<<3);  // sms - 332 rgb
                break;
        }

        _buf = new uint8_t[OVERLAY_WIDTH*OVERLAY_HEIGHT];
        memset(_buf,0,OVERLAY_WIDTH*OVERLAY_HEIGHT);
    }

    void set_hilite(int c)
    {
        _hilite = c;
    }

    void draw_char(int c, int x, int y)
    {
        x *= 8;
        y *= 8;
        const uint8_t* src = _font + c;
        for (int line = 0; line < 8; line++) {
            uint32_t* dst = (uint32_t*)(x + _lines[y++]);
            uint8_t b = src[0];
            dst[0] = _blit[b >> 4]; // long words
            dst[1] = _blit[b & 0xF];
            src += 256;
        }
    }

    // draw a note at bottom of screen
    void draw_msg(const string& msg)
    {
        int n = (int)msg.size();
        for (int i = 0; i < n; i++) {
            int x = i - n/2 + _width/16;
            if (x >= _width)
                break;
            if (x >= 0)
                draw_char(msg[i],x,_height/8-2);
        }
    }

    void erase_msg()
    {
        for (int i = _height-16; i < _height-8; i++)
        {
            uint32_t* d = (uint32_t*)_lines[i];
            int n = _width/4;
            while (n--)
                *d++ = 0;
        }
    }


    // 32 bit aligned access
    inline uint8_t getp(const uint8_t* src)
    {
        size_t s = (size_t)src;
        return *((uint32_t*)(s & ~3)) >> ((s & 3) << 3);
    }

    inline void setp(uint8_t* dst, uint8_t p)
    {
        size_t s = (size_t)dst;
        size_t shift = (s & 3) << 3;
        uint32_t* d = (uint32_t*)(s & ~3);
        d[0] = (d[0] & ~(0xFF << shift)) | (p << shift);
    }

    inline void luma(uint8_t* dst, int c)
    {
        uint8_t p = getp(dst);
        uint8_t r,g,b;
        switch (_flavor) {
            case EMU_NES:
                b = (((p >> 4) & 3) * c) >> 7;
                if (b > 3) b = 3;
                p = (b << 4) | (p & 0xCF);  // nes luma adjust
                break;
            case EMU_ATARI:
                p = (((p & 0xF) * c) >> 7) | (p & 0xF0);  // atari luma adjust
                break;
            case EMU_SMS:
                r = ((p & 0xE0) * c >> 7) & 0xE0;  // sms
                g = ((p & 0x1C) * c >> 7) & 0x1C;
                b = ((p & 0x03) * c >> 7) & 0x03;
                p = r | g | b;
                break;
        }
        setp(dst,p);
    }

    void hline(int c, int x0, int x1, int y)
    {
        uint8_t* dst = _lines[y] + x0;
        if (c < 0) {
            for (int x = x0; x < x1; x++)
                 luma(dst++,-c);
        } else {
            for (int x = x0; x < x1; x++)
                 setp(dst++,c);
        }
    }

    void vline(int c, int x, int y0, int y1)
    {
        if (c < 0) {
            for (int y = y0; y < y1; y++)
                 luma(_lines[y] + x,-c);
        } else {
            for (int y = y0; y < y1; y++)
                 setp(_lines[y] + x,c);
        }
    }

    void frame()
    {
        int left = (_width-OVERLAY_WIDTH*8)/2;    // in pixels
        int top = (_height-OVERLAY_HEIGHT*8)/2;
        int bottom = top + OVERLAY_HEIGHT*8;
        int right = left + OVERLAY_WIDTH*8;

        for (int i = 0; i < 4; i++) {
            int c = -(i*32 + 16);
            hline(c,left+i-1,right+i+1,bottom+i);
            vline(c,right+i,top+i,bottom+i);
        }
        hline(-32,left-1,right+1,top-1);
        vline(-32,left-1,top-1,bottom+1);
    }

    void update()
    {
        uint8_t* s = _buf;
        int xx = (_width-OVERLAY_WIDTH*8) >> 4;
        int yy = (_height-OVERLAY_HEIGHT*8) >> 4;
        for (int y = 0; y < OVERLAY_HEIGHT; y++) {
            for (int x = 0; x < OVERLAY_WIDTH; x++) {
                draw_char(*s++,x+xx,y+yy);
            }
        }
    }

    void plot_char(int c, int x, int y)
    {
        if (x < 0 || x >= OVERLAY_WIDTH)   //
            return;
        if (y < 0 || y >= OVERLAY_HEIGHT)
            return;
        _buf[x + y*OVERLAY_WIDTH] = c + (_hilite << 7);
    }

    void plot_str(const char* t, int x, int y)
    {
        int len = (int)strlen(t);
        for (int i = 0; i < len; i++)
            plot_char((uint8_t)*t++,x++,y);
    }

    void fill(int y, int x0, int x1, int c)
    {
        for (int x = x0; x < x1; x++)
            plot_char(c,x,y);
    }
};

string get_ext(const string& s);

class GUI {
public:

    string _path;
    vector<string> _files;
    vector<string> _info;
    int _disks[2];
    int _tab_hilited[3];
    int _tab_scroll[3];
    int _active;
    int _hilited;
    int _scroll;
    int _tab;
    int _visible;
    bool _dirty;
    int _click;
    Emu* _emu;
    Overlay* _overlay;

    string _msg;
    uint32_t _msg_ticks;

    GUI() : _active(0),_hilited(0),_tab(0),_visible(0),_dirty(true),_click(0),_emu(0)
    {
        _disks[0] = _disks[1] = -1;
        _tab_hilited[0] = _tab_hilited[1] = _tab_hilited[2] = 0;
        _tab_scroll[0] = _tab_scroll[1] = _tab_scroll[2] = 0;
    }

    void msg(const char* m)
    {
        _msg = m;
        _msg_ticks = 120;
    }

    int want(const char* ext)
    {
        auto exts = _emu->_ext;
        for (int i = 0; exts[i]; i++)
            if (strcmp(ext,exts[i]) == 0)
                return i;
        return -1;
    }

    // map to sort
    void read_directory(const char* name)
    {
        _path = name;
        _files.clear();
        map<string,int> files;  // sort by name
        DIR dir;
        FRESULT res = f_opendir(&dir, name);
        if (res != FR_OK)
            return;             // no folder yet
        FILINFO fileInfo;
        while(f_readdir(&dir, &fileInfo) == FR_OK && fileInfo.fname[0] != '\0') {
            if (fileInfo.fattrib & AM_DIR) {
                // directory
            } else {
                string ext = get_ext(fileInfo.fname);
                int e = want(ext.c_str());
                if (e != -1)
                    files[fileInfo.fname] = e;
            }
        }
        for (auto& p : files)
            _files.push_back(p.first);
        f_closedir(&dir);
    }

    void draw_menu(int x, const char* name, bool selected)
    {
        _overlay->set_hilite(selected);
        _overlay->plot_str(" ",x++,0);
        _overlay->plot_str(name,x,0);
        x += strlen(name);
        _overlay->plot_str(" ",x++,0);
        _overlay->set_hilite(0);
    }

    void move_tab(int dir)
    {
        dir += _tab;
        if (dir < 0 || dir > 2)
            return;
        _tab_scroll[_tab] = _scroll;
        _tab_hilited[_tab] = _hilited;
        _tab = dir;
        _scroll = _tab_scroll[_tab] ;
        _hilited = _tab_hilited[_tab];
        _dirty = true;
    }

    void move_v(int dir)
    {
        int c = count();
        _hilited += dir;
        if (_tab != 0) {
            int h = _overlay->OVERLAY_HEIGHT;
            _hilited = dir == -1 ? _scroll-1 : _scroll+(h-2);   //
        }
        _hilited = max(0,min(_hilited,c-1));
    }
    
    int count()
    {
        switch (_tab) {
            case 0: return (int)_files.size();
            case 1: return (int)_info.size();
            case 2: {
                const char** s = _emu->_help;
                int i = 0;
                while (s[i++])
                    ;
                return i;
            }
        }
        return 0;
    }

    void eject_disk(int findex)
    {
        for (int i = 0; i < 2; i++) {
            if (_disks[i] == findex) {
                set_pref(disk_name(i),"");
                _disks[i] = -1;
            }
        }
    }

    void insert(const string& path, int flags)
    {
        set_pref("recent",path);
        _emu->insert(_path + "/" + path,flags);
    }

    void insert_disk(int dindex, int findex, int reboot = 0)
    {
        eject_disk(findex);
        const string& file = _files[findex];
        _disks[dindex] = findex;
        set_pref(disk_name(dindex),file);
        if (dindex == 0)
            set_pref("recent",file);
        _emu->insert(_path + "/" + file,reboot,dindex);
    }

    void enter(int mods)
    {
        if (_tab != 0)
            return;

        int flags = 1;
        if (mods & 2)
            flags |= 2; // shift key
        if (is_disk(_hilited))
            insert_disk(0,_hilited,flags);
        else {
            insert(_files[_hilited],flags);
            if (_disks[0] != -1)
                insert_disk(0,_disks[0]);   // reinsert disk 1 after restart
        }
        if (_disks[1] != -1)
            insert_disk(1,_disks[1]);       // reinsert disk 2 after restart
        _visible = false;
    }

    bool is_disk(int i)
    {
        string ext = get_ext(_files[i]);
        return (ext == "atr" || ext == "atx");
    }

    string disk_name(int index)
    {
        return string("disk") + (char)('0' + index);
    }

    int find_file(const string& file)
    {
        for (auto i = 0; i < _files.size(); i++) {
            if (_files[i] == file)
                return i;
        }
        return -1;
    }

    int find_disk(int index)
    {
        string n = get_pref(disk_name(index));
        int i = find_file(n);
        if (i >= 0)
            insert_disk(index,i);
        return i;
    }

    void disk_key(int dindex)
    {
        if (_emu->flavor != EMU_ATARI)
            return;
        if (dindex == 9 && is_disk(_hilited)) { // '0' key
            eject_disk(_hilited);
            return;
        }
        if (!is_disk(_hilited)) //|| (_disks[dindex] == _hilited))
            return;
        insert_disk(dindex,_hilited);
    }

    // raw keycode
    bool key(int keycode, int pressed, int mods)
    {
        printf("key:%02X %02X %02X\n",keycode,pressed,mods);
        if (pressed && _visible)
            _click = 1;

        if (pressed && keycode == 58) { // F1 - GUI key
            _visible = !_visible;       // toggle GUi
            if (_visible)
                _overlay->frame();      // draw the frame when it first appears
            _click = 1;
            return true;
        }
        if (!_visible)
            return false;

        if (pressed) {
            switch (keycode) {
                case 30:        // 1 key
                case 31:        // 2 key
                case 39:        // 0 key
                    disk_key(keycode-30);
                    break;
                case 40:
                    enter(mods);    // return
                    break;
                case 82:    // up
                    move_v(-1);
                    break;
                case 81:    // down
                    move_v(1);
                    break;
                case 80:    // left
                    move_tab(-1);
                    break;
                case 79:    // right
                    move_tab(1);
                    break;
                case 224:   // left control key
                    break;  // FIRE
            }
        }
        return true;
    }

    void menu()
    {
        int h = 0;
        for (int i = 0; _menu[i]; i++) {
            const char* item = _menu[i];
            draw_menu(h,item,i == _tab);
            h += 1 + strlen(item) + 1;
        }
        _overlay->fill(1,0,_overlay->OVERLAY_WIDTH,'_');
    }

    int item_y(int i)
    {
        int y = 2+i;
        //if (_tab == 0)
            y -= _scroll;
        if (y <= 1 || y >= _overlay->OVERLAY_HEIGHT)
            return -1;
        return y;
    }

    void draw_item(int i, const char* str, bool selected)
    {
        int y = item_y(i);
        if (y < 0)
            return;
        _overlay->set_hilite(selected);
        _overlay->fill(y,0,_overlay->OVERLAY_WIDTH-1,' ');
        _overlay->plot_str(str,1,y);
        _overlay->set_hilite(0);
    }

    int range(int i)
    {
        i *= _overlay->OVERLAY_HEIGHT-4;
        return (i + count()/2)/count();
    }

    void scrollbar()
    {
        int h = _overlay->OVERLAY_HEIGHT;
        int w = _overlay->OVERLAY_WIDTH;

        _overlay->plot_str("\x1C",w-1,2);
        _overlay->plot_str("\x1D",w-1,h-1);
        _scroll = min(_hilited,_scroll);
        _scroll = max(_hilited-(h-3),_scroll);

        for (int i = 0; i < (h-4); i++)
            _overlay->plot_str(" ",w-1,i+3);
        int b = h-4;
        int top = 0;
        if (count() > (h-2)) {
            b = max(1,range(h-2));
            top = range(_scroll);
        }
        for (int i = 0; i < b; i++)
            _overlay->plot_str("|",w-1,top+i+3);
    }

    void draw_icon(int index, int icon)
    {
        int y = item_y(index);
        if (y < 0)
            return;
        _overlay->plot_char(icon+128,0,y);
    }

    void draw_disk(int index)
    {
        for (int i = 0; i < 2; i++) {
            if (index == _disks[i])
                draw_icon(index,'1' + i);
        }
    }

    void draw_files()
    {
        int i;
        for (i = 0; i < (int)_files.size(); i++) {
            string c = _files[i];
            int w = _overlay->OVERLAY_WIDTH-2;
            if (c.length() > w)
                c.resize(w);
            draw_item(i,c.c_str(),i == _hilited);
            draw_disk(i);
        }
        clear(i);
    }

    void draw_help()
    {
        const char** s = _emu->_help;
        if (!s)
            return;
        int i;
        for (i = 0; s[i]; i++)
            draw_item(i,s[i],false);
        clear(i);
    }

    void clear(int i)
    {
        while (i < _overlay->OVERLAY_HEIGHT - 2)
            draw_item(i++," ",false);
    }

    void draw_info()
    {
        if (_dirty) {
            _dirty = false;
            _info.clear();
            int index = _tab_hilited[0];
            _emu->info(_path + "/" + _files[index],_info);
        }
        int i;
        for (i = 0; i < (int)_info.size(); i++)
            draw_item(i,_info[i].c_str(),false);
        clear(i);
    }

    string get_pref(const string& key)
    {
        char buf[256] = {0};
        sys_get_pref((_path[1]+key).c_str(),buf,sizeof(buf)-1); // keys are limited to 15 bytes. great
        return buf;
    }

    void set_pref(const string& key, const string& value)
    {
        sys_set_pref((_path[1]+key).c_str(),value.c_str());     // first char discrminates current emus at least
    }

    void insert_default(const char* path)
    {
        read_directory(path);
        if (_files.empty()) {
            _emu->make_default_media(_path);
            read_directory(path);
        }

        int recent = find_file(get_pref("recent"));

        for (int i = 0; i < 2; i++)
            _disks[i] = find_disk(i);

        // just insert the first one
        if (_files.empty()) {
            _visible = true;
        } else {
            _hilited = recent == -1 ? 0 : recent;
            enter(0);
        }
    }

    void update_video()
    {
        if (_visible) {
            menu();
            scrollbar();
            switch (_tab) {
                case 0: draw_files(); break;
                case 1: draw_info(); break;
                case 2: draw_help(); break;
            }
            _overlay->update();
        } else {
            _emu->update();
        }

        // message goes over both
        if (_msg.size()) {
            if (--_msg_ticks == 0) {
                _overlay->erase_msg();
                _msg.clear();
            } else
                _overlay->draw_msg(_msg);
        }
    }

    // soft click wave soundy thing
    const uint16_t _wav[16] =  {
        0x0000,0x187D,0x2D41,0x3B20,0x3FFF,0x3B20,0x2D41,0x187D,
        0x0000,0xE783,0xD2BF,0xC4E0,0xC001,0xC4E0,0xD2BF,0xE783
    };

    void update_audio()
    {
        int16_t abuffer[313*2];
        int format = _emu->audio_format >> 8;
        int sample_count = _emu->frame_sample_count();
        if (_visible) {
            format = 1;
            if (_click) {
                _click = 0;
                for (int i = 0; i < sample_count; i++)
                    abuffer[i] = _wav[i&0xF];  // just a signed sine click
            } else
              memset(abuffer,0,sizeof(abuffer));
        } else {
            sample_count = _emu->audio_buffer(abuffer,sizeof(abuffer));
        }
        audio_write_16(abuffer,sample_count,format);
    }
};

Overlay _overlay;
GUI _gui;
void gui_start(Emu* emu, const char* path)
{
    _gui._emu = emu;
    _gui._overlay = &_overlay;
    _gui.insert_default(path);
    _overlay.init(emu->video_buffer(),emu->width,emu->height,emu->flavor);
}

void gui_update()
{
    _gui.update_audio();
    _gui.update_video();

    uint8_t buf[64];
    int n = hid_get(buf,sizeof(buf));    // called from emulation loop
    if (n > 0)
        gui_hid(buf,n);
    
    n = get_hid_ir(buf);
    if (n > 0)
        gui_hid(buf,n);
}

void gui_key(int keycode, int pressed, int mods)
{
    if (!_gui.key(keycode,pressed,mods))
        _gui._emu->key(keycode,pressed,mods);
}

//==================================================================
//==================================================================
// hid keyboard events
// 01 mods XX k k k k k k
// 03 XXXXXXXXX // weird apple/android bitmask mode?

// http://www.usb.org/developers/devclass_docs/Hut1_12v2.pdf
// Same as SDL scancodes

static int _last_key = 0;
static void keyboard(const uint8_t* d, int len)
{
    int mods = d[1];          // can we use hid mods instead of SDL? TODO
    int key_code = d[3];      // only care about first key
    if (key_code != _last_key) {
        if (key_code) {
            if (_last_key)
                gui_key(_last_key,0,mods);
            gui_key(key_code,1,mods);
            _last_key = key_code;
        } else {
            gui_key(_last_key,0,mods);
            _last_key = 0;
        }
    }
}

// Handle WII/IR controllers for operating GUI
static int _last_pad = 0;
static void pad_key(int mask, int state, int key)
{
    if ((_last_pad & mask) == (state & mask))
        return;
    gui_key(key,state & mask,0);
}

static void wii()
{
    int pad = wii_states[0].common();
    pad_key(wii_right,pad,82);  // up
    pad_key(wii_left,pad,81);   // down
    pad_key(wii_down,pad,79);   // right
    pad_key(wii_up,pad,80);     // left
    pad_key(wii_home,pad,58);   // home/gui
    pad_key(wii_a | wii_one | wii_two,pad,40); // enter (A)
    _last_pad = pad;
}

static void ir(const uint8_t* j, int len)
{
    int pad = j[0] + (j[1] << 8);
    pad_key(GENERIC_UP,pad,82);  // up
    pad_key(GENERIC_DOWN,pad,81);   // down
    pad_key(GENERIC_RIGHT,pad,79);   // right
    pad_key(GENERIC_LEFT,pad,80);     // left
    pad_key(GENERIC_OTHER,pad,58);   // home/gui (NES controller)
    pad_key(GENERIC_RESET | GENERIC_FIRE_Z,pad,58);   // home/gui
    pad_key(GENERIC_FIRE | GENERIC_FIRE_C | GENERIC_FIRE_B | GENERIC_FIRE_A,pad,40); // enter (A)
    _last_pad = pad;
}

void gui_hid(const uint8_t* hid, int len)  // Parse HID event
{
    if (hid[0] != 0xA1)
        return;
    /*
    for (int i = 0; i < len; i++)
        printf("%02X",hid[i]);
    printf("\n");
    */
    switch (hid[1]) {
        case 0x01: keyboard(hid+1,len-1);   break;   // parse keyboard and maintain 1 key state
        case 0x32: wii();                   break;   // parse wii stuff: generic?
        case 0x42: ir(hid+2,len);           break;   // ir joy
    }
    _gui._emu->hid(hid+1,len-1);    // send raw events
}

void gui_msg(const char* msg)         // temporarily display a msg
{
    _gui.msg(msg);
}

void sys_msg(const char* msg)          // temporarily display a msg
{
    gui_msg(msg);
}
