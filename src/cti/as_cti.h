#ifndef _AS_CTI_H_
#define _AS_CTI_H_

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

#include <stdint.h>

typedef struct {

#ifndef CONSTRAINED_DEVICE
	uint8_t org[32];	// SHA1 or SHA256
	uint8_t system[32];
#else
	uint8_t org[20];	// SHA1
	uint8_t system[16];
#endif

	char *app;

	uint8_t has_org : 1;
	uint8_t has_system : 1;
	uint8_t has_user : 1;

#ifndef CONSTRAINED_DEVICE
	uint8_t user[32];
	uint8_t otp[32];
	uint32_t time_base;
	char *user2;
#endif

	uint32_t version;

} ASCTI_Config_t;

typedef struct {
	uint32_t test;
	uint32_t subtest;
	uint32_t time_delta;
	uint16_t type;
	uint16_t data1_type;
	uint16_t data2_type;
	uint16_t data3_type;
	uint16_t data1_len;
	uint16_t data2_len;
	void *data1;
	void *data2;
	uint32_t data3;

	uint8_t flag_no_send : 1;

#ifndef CONSTRAINED_DEVICE
	//uint16_t impact;
	uint16_t confidence;
#endif
} ASCTI_Item_t;

typedef struct {
	uint8_t *data;
	uint32_t data_len;
	uint32_t flags;
} ASCTI_ItemVec_t;

int ASCTI_Encode_Item( ASCTI_Config_t *config, ASCTI_Item_t *item,
        uint8_t *output, uint32_t output_max, uint32_t *output_len );
int ASCTI_Encode_Item_Size( ASCTI_Item_t *item );

void ASCTI_Item_OTP( ASCTI_Config_t *config, uint8_t *buffer, uint32_t len );

#define ASCTI_Encode_Report(...) ASCTI_Encode_Report_Ex(__VA_ARGS__,0)

int ASCTI_Encode_Report_Ex( ASCTI_Config_t *config, ASCTI_ItemVec_t *items, uint8_t item_count,
        uint8_t *output, uint32_t output_max, uint32_t *output_len, int do_otp_decode );

int ASCTI_Encode_Item_Header( uint16_t item_len, uint8_t *buff, uint32_t imax, uint32_t *i );
int ASCTI_Encode_Report_Header( ASCTI_Config_t *config, uint8_t *out, uint32_t out_max, uint32_t *i);

#define ASCTI_ENCODE_SUCCESS		0
#define ASCTI_ENCODE_ERR_INTERNAL	-1

#define ASCTI_OBT_UNKNOWN		0
#define ASCTI_OBT_INFO			1
#define ASCTI_OBT_SYSTEM		2
#define ASCTI_OBT_APP			3
#define ASCTI_OBT_MALWARE		4
#define ASCTI_OBT_NETWORK		5
#define ASCTI_OBT_USER			6
#define ASCTI_OBT_COMP			7
#define ASCTI_OBT_CUST			8

#define ASCTI_CONFIDENCE_UNKNOWN	0
#define ASCTI_CONFIDENCE_LOW		1
#define ASCTI_CONFIDENCE_MEDIUM		2
#define ASCTI_CONFIDENCE_HIGH		3

#if 0
#define ASCTI_IMPACT_UNKNOWN		0
#define ASCTI_IMPACT_NONE		1
#define ASCTI_IMPACT_MINOR		2
#define ASCTI_IMPACT_MODERATE		3
#define ASCTI_IMPACT_MAJOR		4
#endif

#define ASCTI_DT_UNKNOWN		0
#define ASCTI_DT_HASHMD5		1
#define ASCTI_DT_HASHSHA1		2
#define ASCTI_DT_HASHSHA256		3
//#define ASCTI_DT_HASHAS1		4
//#define ASCTI_DT_HASHAS2		5
#define ASCTI_DT_CVE			6
#define ASCTI_DT_VERSTR			7
#define ASCTI_DT_MODEL			8
#define ASCTI_DT_ASLIBVER		9
#define ASCTI_DT_FILE			10
#define ASCTI_DT_X509			11
#define ASCTI_DT_X509S			12
#define ASCTI_DT_X509I			13
#define ASCTI_DT_USERNAME		14
#define ASCTI_DT_PROCESS		15
#define ASCTI_DT_COMMAND		16
#define ASCTI_DT_APPLICATION		17
#define ASCTI_DT_STRING			18
#define ASCTI_DT_NUMBER			19
#define ASCTI_DT_IPV4			20
//#define ASCTI_DT_IPV6			21
#define ASCTI_DT_PORT			22
#define ASCTI_DT_HOSTNAME		23
#define ASCTI_DT_MAC			24
#define ASCTI_DT_ASCONFTS		25
#define ASCTI_DT_ASDEFVER		26
#define ASCTI_DT_HPKP			27
#define ASCTI_DT_VRID			28
#define ASCTI_DT_ENV			29
#define ASCTI_DT_SYMBOL			30
#define ASCTI_DT_PROPERTYNAME		31
#define ASCTI_DT_LIBRARY		32
#define ASCTI_DT_SSID			33
#define ASCTI_DT_BSSID			34
#define ASCTI_DT_SYSID			35
#define ASCTI_DT_NATIVEPTR		36
#define ASCTI_DT_NATIVEINT		37
#define ASCTI_DT_RAWBINARY		38

#endif
