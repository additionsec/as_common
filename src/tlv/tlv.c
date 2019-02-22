// Copyright 2019 J Forristal LLC
// Copyright 2016 Addition Security Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include PLATFORM_H

#include "as_tlv.h"

uint8_t ASTLV_Parse( uint8_t *data, uint16_t data_len, ASTLV_CALLBACK(callback), void *state )
{
	// Sanity check
	if( data_len < 2 || callback == NULL || data == NULL ) return 1;

	while( data_len >= 2 ){
		uint8_t tag = *data++;
		uint16_t len = *data++;
		data_len -= 2;
		if( len & 0x80 ){
			if( data_len == 0 ) return 2; // Overflow
			len = ((len & 0x7f)<<8)|(*data++);
			data_len--;
		}
		if( len > data_len ) return 3; // Overflow
		callback( tag, len, data, state );
		data_len -= len;
		data += len;
	}

	if( data_len > 0 ) return 4; // Trailing bytes
	return 0;
}
