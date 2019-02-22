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

#include <string.h>
#include <time.h>

#include PLATFORM_H

#include "as_cti.h"
#include "addsec_cti.pb.h"

#define WT_VARINT	0
#define WT_LD		2

static int _encode_varint( uint32_t val, uint8_t *buff, uint32_t *i, uint32_t imax )
{
	while( val >= 0x80 ) {
		if( (*i)+1 >= imax ) return -1;
		buff[ *i ] = (val & 0x7f) | 0x80;
		val >>= 7;
		*i = (*i)+1;
	}
	if( (*i)+1 >= imax ) return -1;
	buff[*i] = val;
	*i = (*i)+1;
	return 0;
}

#define _encode_tag(t,wt,b,i,im) _encode_varint( (t<<3)|wt, b, i, im )

static inline int _encode_buffer( uint8_t *val, uint32_t val_len, uint8_t *buff, uint32_t *i, 
	uint32_t imax )
{
	if( _encode_varint( val_len, buff, i, imax ) != 0 ) return -1;
	if( (*i + val_len) >= imax ) return -1;
	TFMEMCPY( &buff[ *i ], val, val_len );
	*i = (*i) + val_len;
	return 0;
}

#ifndef CONSTRAINED_DEVICE
#ifndef ASCTI_NO_OTP
static inline int _encode_buffer_otp( ASCTI_Config_t *config, uint8_t *val, uint32_t val_len, 
	uint8_t *buff, uint32_t *i, uint32_t imax )
{
	if( _encode_varint( val_len, buff, i, imax ) != 0 ) return -1;
	if( (*i + val_len) >= imax ) return -1;

	int j;
	buff = &buff[ *i ];
	for( j=0; j<val_len; j++ ){
		*buff = val[j] ^ config->otp[j % 32];
		buff++;
	}
	//MEMCPY( &buff[ *i ], val, val_len );
	*i = (*i) + val_len;
	return 0;
}
#endif
#endif


static int _encode_tag_varint( int tag, uint32_t val, uint8_t *buff, uint32_t *i, uint32_t imax )
{
	if( _encode_tag(tag, WT_VARINT, buff, i, imax) != 0 ) return -1;
	return _encode_varint(val, buff, i, imax);
}

static int _encode_tag_buffer( int tag, uint8_t *val, uint32_t val_len, 
	uint8_t *buff, uint32_t *i, uint32_t imax )
{
	if( _encode_tag(tag, WT_LD, buff, i, imax) != 0 ) return -1;
	return _encode_buffer(val, val_len, buff, i, imax);
}

#ifndef CONSTRAINED_DEVICE
#ifndef ASCTI_NO_OTP
static int _encode_tag_buffer_otp( ASCTI_Config_t *config, int tag, uint8_t *val, uint32_t val_len, 
	uint8_t *buff, uint32_t *i, uint32_t imax )
{
	if( _encode_tag(tag, WT_LD, buff, i, imax) != 0 ) return -1;
	return _encode_buffer_otp(config, val, val_len, buff, i, imax);
}
#endif

#ifndef ASCTI_NO_OTP
void ASCTI_Item_OTP( ASCTI_Config_t *config, uint8_t *buffer, uint32_t len )
{
	int i;
	for( i=0; i<len; i++ ){
		buffer[i] ^= config->otp[ i % 32 ];
	}
}
#endif
#endif

static void _varint_size( uint32_t val, uint32_t *i )
{
	while( val >= 0x80 ) {
		val >>= 7;
		*i = (*i)+1;
	}
	*i = (*i)+1;
}
#define _tag_size(t,wt,i) _varint_size( (t<<3)|wt, i )
#define _tag_varint_size(t,v,i) _tag_size(t,WT_VARINT,i);_varint_size(v,i);
#define _tag_buffer_size(t,vl,i) _tag_size(t,WT_LD,i);_varint_size(vl,i);*i=(*i)+vl;

int ASCTI_Encode_Item_Size( ASCTI_Item_t *item )
{
	uint32_t iv = 0;
	uint32_t *i = &iv;

	if( item->test != 0 ){ _tag_varint_size( Sighting_testId_tag, item->test, i ); }
	if( item->subtest != 0 ){_tag_varint_size( Sighting_testSubId_tag, item->subtest, i ); }

	if( item->time_delta > 0 ){
		_tag_varint_size( Sighting_timeDelta_tag, item->time_delta, i );
	} else {
		_tag_varint_size( Sighting_timestamp_tag, (uint32_t)TIME(0), i );
	}

#ifndef CONSTRAINED_DEVICE
	if( item->type != 0 ){ _tag_varint_size( Sighting_sightingType_tag, item->type, i ); }
	if( item->confidence != 0 ){ _tag_varint_size( Sighting_confidence_tag, item->confidence, i ); }
#endif

	uint32_t tv = 0;
	uint32_t *t = &tv;

	// ObservationData datas
	uint8_t j;
	for( j=0; j<3; j++ ){
		tv = 0;
		uint16_t dtype=0;
		uint16_t dlen=0;

		uint32_t size = 2; // data type encoding (tag and varint)

		switch(j){
			case 0:
				if( item->data1 == NULL ) continue;
				dtype = item->data1_type;
				dlen = item->data1_len;
				break;
			case 1:
				if( item->data2 == NULL ) continue;
				dtype = item->data2_type;
				dlen = item->data2_len;
				break;
			case 2:
				if( item->data3 == 0 ) continue;
				dtype = item->data3_type;
				break;
		}

		tv = 0;
		if( j == 2 ){
			_varint_size( item->data3, t );
		} else {
			size += dlen;
			_varint_size( dlen, t );
		}
		size += tv + 1; // +1 is for tag byte

		// now figure out the varint size of the size (heh)
		//tv = 0;
		//_varint_size( size, t );

		tv = 0;
		_tag_size( Sighting_datas_tag, WT_LD, i );
		_varint_size( size, i );
		_tag_varint_size( ObservationData_dataType_tag, dtype, i );

		if( j == 2 ){
			_tag_varint_size( ObservationData_num_tag, item->data3, i );
		}
		else{
			_tag_buffer_size( ObservationData_data_tag, dlen, i );
		}

		// extra +1 is for Observation datas tag
		//iv += 1 + tv + size;
	}

	return (int)(iv & 0xffff);
}

int ASCTI_Encode_Item( ASCTI_Config_t *config, ASCTI_Item_t *item,
        uint8_t *out, uint32_t out_max, uint32_t *i )
{
	*i = 0;

	if( item->test != 0 ){
		if( _encode_tag_varint( Sighting_testId_tag, item->test, 
			out, i, out_max ) != 0 ) goto err;
	}

	if( item->subtest != 0 ){
		if( _encode_tag_varint( Sighting_testSubId_tag, item->subtest, 
			out, i, out_max ) != 0 ) goto err;
	}

	if( item->time_delta > 0 ){
		if( _encode_tag_varint( Sighting_timeDelta_tag, item->time_delta,
			out, i, out_max ) != 0 ) goto err;
	} else {
		if( _encode_tag_varint( Sighting_timestamp_tag, (uint32_t)TIME(0),
			out, i, out_max ) != 0 ) goto err;
	}

	if( item->type != 0 ){
		if( _encode_tag_varint( Sighting_sightingType_tag, item->type, 
			out, i, out_max ) != 0 ) goto err;
	}

#ifndef CONSTRAINED_DEVICE
	if( item->confidence != 0 ){
		if( _encode_tag_varint( Sighting_confidence_tag, item->confidence, 
			out, i, out_max ) != 0 ) goto err;
	}
#endif

	uint8_t temp[16];
	uint32_t j, t = 0;

	// ObservationData datas

	for( j=0; j<3; j++ ){
		uint16_t dtype=0, dlen=0;
		void *data = NULL;

		switch(j){
			case 0:
				dtype = item->data1_type;
				dlen = item->data1_len;
				data = item->data1;
				break;
			case 1:
				dtype = item->data2_type;
				dlen = item->data2_len;
				data = item->data2;
				break;
			case 2:
				dtype = item->data3_type;
				// NOTE: we just use the non-NULL as a signal that we
				// have a value to encode; we don't actually use
				// the value of data here
				if( item->data3 > 0 ) data = &item->data3;
				break;
		}

		if( data != NULL ){
			uint32_t size = 2;
			//if( _encode_varint( item->data1_type, temp, &t, 16 ) != 0 ) goto err;
			// we assume t=1 since types are an enum of under 128 values
			//size += 1 + 1; // +1 is for tag byte

			t = 0;
			if( j == 2 ){
				// figure out the size of the varint data
				if( _encode_varint( item->data3, temp, &t, 16 ) != 0 ) goto err;
			} else {
				size += dlen; // add in the length of the data
				// figure out the size of the varint representing the length
				if( _encode_varint( dlen, temp, &t, 16 ) != 0 ) goto err;
			}
			size += t + 1; // +1 is for tag byte

			// now figure out the varint size of the size (heh)
			t = 0;
			if( _encode_varint( size, temp, &t, 16 ) != 0 ) goto err;

			// extra +1 is for Observation datas tag
			if( ((*i) + size + 1 + t) >= out_max ) goto err;

			// Observation.datas submessage
			if( _encode_tag(Sighting_datas_tag, WT_LD, 
				out, i, out_max) != 0 ) goto err;
			if( _encode_varint(size, out, i, out_max) != 0 ) goto err;

			if( _encode_tag_varint( ObservationData_dataType_tag, dtype, 
				out, i, out_max ) != 0 ) goto err;
	
			if( j == 2 ){
				if( _encode_tag_varint( ObservationData_num_tag, item->data3,
					out, i, out_max ) != 0 ) goto err;
			} else {	
				if( _encode_tag_buffer( ObservationData_data_tag, data, dlen, 
					out, i, out_max ) != 0 ) goto err;
			}
		}
	}

	return ASCTI_ENCODE_SUCCESS;
err:
	return ASCTI_ENCODE_ERR_INTERNAL;

}

int ASCTI_Encode_Report_Header( ASCTI_Config_t *config, uint8_t *out, uint32_t out_max, uint32_t *i)
{
	*i = 0;

	if( _encode_tag_buffer(Report_version_tag, (uint8_t*)&config->version, sizeof(uint32_t),
		out, i, out_max) != 0 ) goto err;

	if( config->has_org ){
		if( _encode_tag_buffer(Report_organizationId_tag, config->org, 
			sizeof(config->org), out, i, out_max) != 0 ) goto err;
	}

	if( config->has_system != 0 ){
		if( _encode_tag_buffer(Report_systemId_tag, config->system, 
			sizeof(config->system), out, i, out_max) != 0 ) goto err;
	}

	// NOTE: the value ASCTI_SYSTEM_TYPE is provided by build system define:
	if( _encode_tag_varint(Report_systemType_tag, ASCTI_SYSTEM_TYPE,
		out, i, out_max) != 0 ) goto err;

	if( config->app != NULL ){
		if( _encode_tag_buffer(Report_applicationId_tag, (uint8_t*)config->app, 
			STRLEN(config->app), out, i, out_max) != 0 ) goto err;
	}

#ifndef CONSTRAINED_DEVICE
	if( config->has_user ){
		if( _encode_tag_buffer(Report_userId_tag, config->user, 
			sizeof(config->user), out, i, out_max) != 0 ) goto err;
	}
	if( config->user2 != NULL ){
		if( _encode_tag_buffer(Report_userIdSecondary_tag, (uint8_t*)config->user2, 
			STRLEN(config->user2), out, i, out_max) != 0 ) goto err;
	}

	if( config->time_base != 0 ){
		if( _encode_tag_varint(Report_timeBase_tag, config->time_base,
			out, i, out_max) != 0 ) goto err;
	}
#endif

	return ASCTI_ENCODE_SUCCESS;
err:
	return ASCTI_ENCODE_ERR_INTERNAL;
}

int ASCTI_Encode_Item_Header( uint16_t len, uint8_t *buff, uint32_t imax, uint32_t *i )
{
	// An Item header is basically the Report.Sightings tag followed by
	// the varint of the size of the item (encoded buffer). It will typically
	// be 2-3 bytes, depending upon the item size.
	if( _encode_tag(Report_sightings_tag, WT_LD, buff, i, imax) != 0 ) return -1;
	return _encode_varint( len, buff, i, imax );
}

int ASCTI_Encode_Report_Ex( ASCTI_Config_t *config, ASCTI_ItemVec_t *items, uint8_t item_count,
        uint8_t *out, uint32_t out_max, uint32_t *i, int do_otp_decode )
{
	*i = 0;

	if( ASCTI_Encode_Report_Header( config, out, out_max, i ) != ASCTI_ENCODE_SUCCESS )
		goto err;

	int j;
	for( j=0; j<item_count; j++){
		if( items[j].data == NULL || items[j].data_len == 0 ) continue;
#ifndef CONSTRAINED_DEVICE
#ifndef ASCTI_NO_OTP
		if( do_otp_decode ){
			if( _encode_tag_buffer_otp(config, Report_sightings_tag, items[j].data, 
				items[j].data_len, out, i, out_max) != 0 ) goto err;
		} else {
#endif
#endif
			if( _encode_tag_buffer(Report_sightings_tag, items[j].data, 
				items[j].data_len, out, i, out_max) != 0 ) goto err;
#ifndef CONSTRAINED_DEVICE
#ifndef ASCTI_NO_OTP
		}
#endif
#endif
	}
	
	return ASCTI_ENCODE_SUCCESS;
err:
	return ASCTI_ENCODE_ERR_INTERNAL;
}
