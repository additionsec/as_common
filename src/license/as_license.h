#ifndef _AS_LICENSE_H_
#define _AS_LICENSE_H_

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

typedef struct __attribute__((__packed__)) {
	uint8_t product;
	uint8_t flags;
	uint16_t platforms;
	uint32_t expire;
} ASL_License_Header_t;

typedef struct __attribute__((__packed__)) {
	ASL_License_Header_t header;
	uint32_t wildcard_id;
	uint8_t len;
	uint8_t nom[];
} ASL_License_App_t;

#define ASL_PRODUCT_ASMA	1
#define ASL_PRODUCT_ASGP	3
#define ASL_PRODUCT_ASGPL	4

#define ASL_FLAGS_FREEMIUM	0x80

#endif
