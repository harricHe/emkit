#include "unity_fixture.h"
#include "ringbuffer.h"
#include <string.h>

TEST_GROUP(ringbuffer);

#define POOL_SIZE (64)
static handle_t s_handle = NULL;
static uint8_t s_pool[POOL_SIZE];
static size_t s_poolsize = POOL_SIZE;

TEST_SETUP(ringbuffer)
{
}

TEST_TEAR_DOWN(ringbuffer)
{
	ringbuf_destroy(s_handle);
}

TEST(ringbuffer, create)
{
	s_handle = ringbuf_create(s_pool, s_poolsize);
	TEST_ASSERT( s_handle );
}

TEST(ringbuffer, destroy)
{
	s_handle = ringbuf_create(s_pool, s_poolsize);
	TEST_ASSERT_NOT_NULL( s_handle );
	TEST_ASSERT_UNLESS( ringbuf_destroy(s_handle) );
	TEST_ASSERT( ringbuf_purge(s_handle) );
}

TEST(ringbuffer, write_read)
{
	uint8_t txdata[POOL_SIZE];
	uint8_t rxdata[POOL_SIZE];
	uint8_t *tp = txdata;
	uint8_t *rp = rxdata;
	int32_t i;

	s_handle = ringbuf_create(s_pool, s_poolsize);
	TEST_ASSERT_NOT_NULL( s_handle );

	// prepare data
	for (i=0; i<POOL_SIZE; i++) {
		txdata[i] = i;
	}

	TEST_ASSERT_EQUAL_UINT32( 10, ringbuf_write(s_handle, tp, 10) ); tp += 10;
	TEST_ASSERT_EQUAL_UINT32( 20, ringbuf_write(s_handle, tp, 20) ); tp += 20;

	TEST_ASSERT_EQUAL_UINT32( 20, ringbuf_read(s_handle, rp, 20) ); rp += 20;

	TEST_ASSERT_EQUAL_UINT32( 15, ringbuf_write(s_handle, tp, 15) ); tp += 15;
	TEST_ASSERT_EQUAL_UINT32( 15, ringbuf_write(s_handle, tp, 15) ); tp += 15;
	/* Tx total 60Byte */

	TEST_ASSERT_EQUAL_UINT32( 10, ringbuf_read(s_handle, rp, 10) ); rp += 10;
	TEST_ASSERT_EQUAL_UINT32( 20, ringbuf_read(s_handle, rp, 20) ); rp += 20;
	TEST_ASSERT_EQUAL_UINT32( 10, ringbuf_read(s_handle, rp, 10) ); rp += 10;
	/* Rx total 60Byte */

	TEST_ASSERT_EQUAL_UINT8_ARRAY( txdata, rxdata, 60 ); 
}

TEST(ringbuffer, read_to)
{
	const char *txmsg = "Hello world!\nGoodbye thank you!";
	const char *expect = "Hello world!\n";
	char rxmsg[POOL_SIZE] = { 0 };

	s_handle = ringbuf_create(s_pool, s_poolsize);
	TEST_ASSERT_NOT_NULL( s_handle );

	TEST_ASSERT_EQUAL_UINT32( strlen(txmsg), ringbuf_write(s_handle, txmsg, strlen(txmsg)) );
	TEST_ASSERT_EQUAL_UINT32( strlen(expect), ringbuf_read_to(s_handle, rxmsg, POOL_SIZE, '\n') );

	TEST_ASSERT_EQUAL_STRING( expect, rxmsg );
}

TEST(ringbuffer, size)
{
	uint8_t txdata[POOL_SIZE];

	s_handle = ringbuf_create(s_pool, s_poolsize);
	TEST_ASSERT_NOT_NULL( s_handle );

	TEST_ASSERT_EQUAL_UINT32( s_poolsize, ringbuf_capacity(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( s_poolsize, ringbuf_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, ringbuf_used(s_handle) );

	TEST_ASSERT_EQUAL_UINT32( 30, ringbuf_write(s_handle, txdata, 30) );
	TEST_ASSERT_EQUAL_UINT32( s_poolsize - 30, ringbuf_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 30, ringbuf_used(s_handle) );
}

TEST(ringbuffer, purge)
{
	uint8_t txdata[POOL_SIZE];

	s_handle = ringbuf_create(s_pool, s_poolsize);
	TEST_ASSERT_NOT_NULL( s_handle );

	TEST_ASSERT_EQUAL_UINT32( s_poolsize, ringbuf_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, ringbuf_used(s_handle) );

	TEST_ASSERT_EQUAL_UINT32( 30, ringbuf_write(s_handle, txdata, 30) );
	TEST_ASSERT_EQUAL_UINT32( s_poolsize - 30, ringbuf_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 30, ringbuf_used(s_handle) );

	TEST_ASSERT_UNLESS( ringbuf_purge(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( s_poolsize, ringbuf_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, ringbuf_used(s_handle) );
}


TEST(ringbuffer, read_underflow)
{
	uint8_t txdata[POOL_SIZE];
	uint8_t rxdata[POOL_SIZE];
	uint8_t *rp;
	int32_t i;

	s_handle = ringbuf_create(s_pool, s_poolsize);
	TEST_ASSERT_NOT_NULL( s_handle );

	// prepare data
	for (i=0; i<POOL_SIZE; i++) {
		txdata[i] = i;
	}

	TEST_ASSERT_EQUAL_UINT32( 30, ringbuf_write(s_handle, txdata, 30) );
	TEST_ASSERT_EQUAL_UINT32( 20, ringbuf_read(s_handle, rxdata, 20) );
	TEST_ASSERT_EQUAL_UINT8_ARRAY( txdata, rxdata, 20 ); 

	/* read underflow */
	TEST_ASSERT_EQUAL_UINT32( 10, ringbuf_read(s_handle, rxdata, 30) );
	TEST_ASSERT_EQUAL_UINT8_ARRAY( &txdata[20], rxdata, 10 ); 

	/* read overrun */
	memset(rxdata, 0, POOL_SIZE);
	TEST_ASSERT_EQUAL_UINT32( 50, ringbuf_write(s_handle, txdata, 50) );
	TEST_ASSERT_EQUAL_UINT32( 50, ringbuf_read(s_handle, rxdata, 60) );
	TEST_ASSERT_EQUAL_UINT8_ARRAY( txdata, rxdata, 50 ); 
	rp = &rxdata[50];
	for (i=0; i<POOL_SIZE - 50; i++) {
		TEST_ASSERT_EQUAL_UINT8( 0, rp[i] ); 
	}
}


TEST(ringbuffer, read_to_not_found)
{
	const char *txmsg = "Hello world!\nGoodbye thank you!";
	char rxmsg[POOL_SIZE] = { 0 };

	s_handle = ringbuf_create(s_pool, s_poolsize);
	TEST_ASSERT_NOT_NULL( s_handle );

	TEST_ASSERT_EQUAL_UINT32( strlen(txmsg), ringbuf_write(s_handle, txmsg, strlen(txmsg)) );
	/* search non-existent token */
	TEST_ASSERT_EQUAL_UINT32( strlen(txmsg), ringbuf_read_to(s_handle, rxmsg, POOL_SIZE, '\r') );

	TEST_ASSERT_EQUAL_STRING( txmsg, rxmsg );
}


TEST_GROUP_RUNNER(ringbuffer)
{
	/* normal tests */
	RUN_TEST_CASE(ringbuffer, create);
	RUN_TEST_CASE(ringbuffer, destroy);
	RUN_TEST_CASE(ringbuffer, write_read);
	RUN_TEST_CASE(ringbuffer, read_to);
	RUN_TEST_CASE(ringbuffer, size);
	RUN_TEST_CASE(ringbuffer, purge);

	/* abnormal tests */
	RUN_TEST_CASE(ringbuffer, read_underflow);
	RUN_TEST_CASE(ringbuffer, read_to_not_found);
}

