#include "unity_fixture.h"
#include "fixed_ringbuffer.h"

TEST_GROUP(fixed_ringbuffer);

#define BLOCK_SIZE  (16)
#define BLOCK_COUNT (4)
#define POOL_SIZE   ((BLOCK_SIZE) * (BLOCK_COUNT))
static handle_t s_handle = NULL;
static uint8_t s_pool[POOL_SIZE];
static size_t s_blocksize = BLOCK_SIZE;
static size_t s_blockcount = BLOCK_COUNT;

TEST_SETUP(fixed_ringbuffer)
{
}

TEST_TEAR_DOWN(fixed_ringbuffer)
{
	fixedringbuf_destroy(s_handle);
}

TEST(fixed_ringbuffer, create)
{
	s_handle = fixedringbuf_create(s_pool, s_blocksize, s_blockcount);
	TEST_ASSERT( s_handle );
}

TEST(fixed_ringbuffer, destroy)
{
	s_handle = fixedringbuf_create(s_pool, s_blocksize, s_blockcount);
	TEST_ASSERT_NOT_NULL( s_handle );
	TEST_ASSERT_UNLESS( fixedringbuf_destroy(s_handle) );
	TEST_ASSERT_NULL( fixedringbuf_get(s_handle) );
}

TEST(fixed_ringbuffer, write_read)
{
	uint8_t txblk[2][BLOCK_SIZE] = {
		{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F },
		{ 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
		  0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F }
	};
	uint8_t rxblk[BLOCK_SIZE] = { 0 };

	s_handle = fixedringbuf_create(s_pool, s_blocksize, s_blockcount);
	TEST_ASSERT_NOT_NULL( s_handle );

	TEST_ASSERT_UNLESS( fixedringbuf_write(s_handle, txblk[0]) );
	TEST_ASSERT_UNLESS( fixedringbuf_write(s_handle, txblk[1]) );

	TEST_ASSERT_UNLESS( fixedringbuf_read(s_handle, rxblk) );
	TEST_ASSERT_EQUAL_UINT8_ARRAY( txblk[0], rxblk, BLOCK_SIZE );

	TEST_ASSERT_UNLESS( fixedringbuf_read(s_handle, rxblk) );
	TEST_ASSERT_EQUAL_UINT8_ARRAY( txblk[1], rxblk, BLOCK_SIZE );
}

TEST(fixed_ringbuffer, get)
{
	uint8_t txblk[2][BLOCK_SIZE] = {
		{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F },
		{ 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
		  0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F }
	};
	uint8_t *rxp;

	s_handle = fixedringbuf_create(s_pool, s_blocksize, s_blockcount);
	TEST_ASSERT_NOT_NULL( s_handle );

	TEST_ASSERT_UNLESS( fixedringbuf_write(s_handle, txblk[0]) );
	TEST_ASSERT_UNLESS( fixedringbuf_write(s_handle, txblk[1]) );

	TEST_ASSERT_NOT_NULL( (rxp = fixedringbuf_get(s_handle)) );
	TEST_ASSERT_EQUAL_UINT8_ARRAY( txblk[0], rxp, BLOCK_SIZE );

	TEST_ASSERT_NOT_NULL( (rxp = fixedringbuf_get(s_handle)) );
	TEST_ASSERT_EQUAL_UINT8_ARRAY( txblk[1], rxp, BLOCK_SIZE );
}

TEST(fixed_ringbuffer, size)
{
	uint8_t txblk[BLOCK_SIZE] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
	};
	uint8_t *rxp;
	uint32_t using = 0;

	s_handle = fixedringbuf_create(s_pool, s_blocksize, s_blockcount);
	TEST_ASSERT_NOT_NULL( s_handle );

	TEST_ASSERT_EQUAL_UINT32( s_blockcount, fixedringbuf_capacity(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( s_blockcount, fixedringbuf_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, fixedringbuf_used(s_handle) );

	fixedringbuf_write(s_handle, txblk); using++;
	fixedringbuf_write(s_handle, txblk); using++;
	fixedringbuf_write(s_handle, txblk); using++;

	TEST_ASSERT_EQUAL_UINT32( s_blockcount, fixedringbuf_capacity(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( s_blockcount - using, fixedringbuf_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( using, fixedringbuf_used(s_handle) );

	fixedringbuf_write(s_handle, txblk); using++;

	TEST_ASSERT_EQUAL_UINT32( s_blockcount, fixedringbuf_capacity(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( s_blockcount - using, fixedringbuf_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( using, fixedringbuf_used(s_handle) );

	rxp = fixedringbuf_get(s_handle); using--;
	rxp = fixedringbuf_get(s_handle); using--;
	rxp = fixedringbuf_get(s_handle); using--;

	TEST_ASSERT_EQUAL_UINT32( s_blockcount, fixedringbuf_capacity(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( s_blockcount - using, fixedringbuf_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( using, fixedringbuf_used(s_handle) );
}

TEST(fixed_ringbuffer, purge)
{
	uint8_t txblk[BLOCK_SIZE] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
	};
	uint32_t using = 0;

	s_handle = fixedringbuf_create(s_pool, s_blocksize, s_blockcount);
	TEST_ASSERT_NOT_NULL( s_handle );

	TEST_ASSERT_EQUAL_UINT32( s_blockcount, fixedringbuf_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, fixedringbuf_used(s_handle) );

	fixedringbuf_write(s_handle, txblk); using++;
	fixedringbuf_write(s_handle, txblk); using++;
	fixedringbuf_write(s_handle, txblk); using++;

	TEST_ASSERT_EQUAL_UINT32( s_blockcount - using, fixedringbuf_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( using, fixedringbuf_used(s_handle) );

	TEST_ASSERT_UNLESS( fixedringbuf_purge(s_handle) );

	TEST_ASSERT_EQUAL_UINT32( s_blockcount, fixedringbuf_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( 0, fixedringbuf_used(s_handle) );
}

TEST(fixed_ringbuffer, write_overflow)
{
	uint8_t txblk[5][BLOCK_SIZE] = {
		{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F },
		{ 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
		  0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F },
		{ 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
		  0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F },
		{ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
		  0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F },
		{ 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
		  0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F }
	};
	uint8_t *rxp;

	s_handle = fixedringbuf_create(s_pool, s_blocksize, s_blockcount);
	TEST_ASSERT_NOT_NULL( s_handle );

	TEST_ASSERT_UNLESS( fixedringbuf_write(s_handle, txblk[0]) );
	TEST_ASSERT_UNLESS( fixedringbuf_write(s_handle, txblk[1]) );
	TEST_ASSERT_UNLESS( fixedringbuf_write(s_handle, txblk[2]) );
	TEST_ASSERT_UNLESS( fixedringbuf_write(s_handle, txblk[3]) );

	TEST_ASSERT_EQUAL_UINT32( 0, fixedringbuf_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( s_blockcount, fixedringbuf_used(s_handle) );

	TEST_ASSERT_UNLESS( fixedringbuf_write(s_handle, txblk[4]) );
	TEST_ASSERT_EQUAL_UINT32( 0, fixedringbuf_available(s_handle) );
	TEST_ASSERT_EQUAL_UINT32( s_blockcount, fixedringbuf_used(s_handle) );

	TEST_ASSERT_NOT_NULL( (rxp = fixedringbuf_get(s_handle)) );
	TEST_ASSERT_EQUAL_UINT8_ARRAY( txblk[1], rxp, BLOCK_SIZE );

	TEST_ASSERT_NOT_NULL( (rxp = fixedringbuf_get(s_handle)) );
	TEST_ASSERT_EQUAL_UINT8_ARRAY( txblk[2], rxp, BLOCK_SIZE );

	TEST_ASSERT_NOT_NULL( (rxp = fixedringbuf_get(s_handle)) );
	TEST_ASSERT_EQUAL_UINT8_ARRAY( txblk[3], rxp, BLOCK_SIZE );

	TEST_ASSERT_NOT_NULL( (rxp = fixedringbuf_get(s_handle)) );
	TEST_ASSERT_EQUAL_UINT8_ARRAY( txblk[4], rxp, BLOCK_SIZE );

	TEST_ASSERT_NULL( (rxp = fixedringbuf_get(s_handle)) );
}

TEST_GROUP_RUNNER(fixed_ringbuffer)
{
	/* normal tests */
    RUN_TEST_CASE(fixed_ringbuffer, create);
    RUN_TEST_CASE(fixed_ringbuffer, destroy);
	RUN_TEST_CASE(fixed_ringbuffer, write_read);
	RUN_TEST_CASE(fixed_ringbuffer, get);
	RUN_TEST_CASE(fixed_ringbuffer, size);
	RUN_TEST_CASE(fixed_ringbuffer, purge);

	/* abnormal tests */
	RUN_TEST_CASE(fixed_ringbuffer, write_overflow);
}

