#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include "as_cti.h"

int main(void){

	ASCTI_Config_t config;
	bzero( &config, sizeof(config) );

	memcpy( config.org, "OOOO", 4 );
	memcpy( config.system, "SSSS", 4 );
	memcpy( config.user, "UUUU", 4 );
	config.app = "com.myapp";
	config.system_type = 2;
	config.time_base = (uint32_t)time(0);

	ASCTI_Item_t item;
	bzero( &item, sizeof(item) );

	item.test = 2;
	item.subtest = 4;

	item.data1 = "DDDD";
	item.data1_len = 4;
	item.data1_type = 0;

	item.data2 = "FFFF";
	item.data2_len = 4;
	item.data2_type = 1;

	item.data3_type = 2;
	item.data3 = 42;

	item.time_delta = 6;

	uint8_t buff[128];
	uint32_t out_len = 0;

	if( ASCTI_Encode_Item( &config, &item, buff, sizeof(buff), &out_len ) != ASCTI_ENCODE_SUCCESS )
		return 1;
	printf("Encoded item to %d bytes\n", out_len);

	int fd = open("/tmp/test_o.pb.bin", O_RDWR|O_CREAT|O_TRUNC, 0644);
	if( fd == -1 ) return 3;
	if( write(fd, buff, out_len) != out_len ) return 4;
	close(fd);

	ASCTI_ItemVec_t items;
	items.data = buff;
	items.data_len = out_len;

	uint8_t buff2[2048];

	if( ASCTI_Encode_Report( &config, &items, 1, buff2, sizeof(buff2), &out_len ) != ASCTI_ENCODE_SUCCESS )
		return 2;
	printf("Encoded report to %d bytes\n", out_len);

	fd = open("/tmp/test_r.pb.bin", O_RDWR|O_CREAT|O_TRUNC, 0644);
	if( fd == -1 ) return 3;
	if( write(fd, buff2, out_len) != out_len ) return 4;
	close(fd);

	printf("Success wrote /tmp/test.pb.bin\n");

	return 0;
}
