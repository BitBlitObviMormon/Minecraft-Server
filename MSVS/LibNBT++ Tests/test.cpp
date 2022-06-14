/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#define _CXXTEST_HAVE_EH
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ParenPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ParenPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ParenPrinter >( tmp, argc, argv );
    return status;
}
bool suite_endian_str_test_init = false;
#include "..\..\lib\nbtplusplus\test\endian_str_test.h"

static endian_str_test suite_endian_str_test;

static CxxTest::List Tests_endian_str_test = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_endian_str_test( "../../lib/nbtplusplus/test/endian_str_test.h", 28, "endian_str_test", suite_endian_str_test, Tests_endian_str_test );

static class TestDescription_suite_endian_str_test_test_uint : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_endian_str_test_test_uint() : CxxTest::RealTestDescription( Tests_endian_str_test, suiteDescription_endian_str_test, 31, "test_uint" ) {}
 void runTest() { suite_endian_str_test.test_uint(); }
} testDescription_suite_endian_str_test_test_uint;

static class TestDescription_suite_endian_str_test_test_sint : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_endian_str_test_test_sint() : CxxTest::RealTestDescription( Tests_endian_str_test, suiteDescription_endian_str_test, 84, "test_sint" ) {}
 void runTest() { suite_endian_str_test.test_sint(); }
} testDescription_suite_endian_str_test_test_sint;

static class TestDescription_suite_endian_str_test_test_float : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_endian_str_test_test_float() : CxxTest::RealTestDescription( Tests_endian_str_test, suiteDescription_endian_str_test, 137, "test_float" ) {}
 void runTest() { suite_endian_str_test.test_float(); }
} testDescription_suite_endian_str_test_test_float;

#include "..\..\lib\nbtplusplus\test\nbttest.h"

static nbttest suite_nbttest;

static CxxTest::List Tests_nbttest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_nbttest( "../../lib/nbtplusplus/test/nbttest.h", 29, "nbttest", suite_nbttest, Tests_nbttest );

static class TestDescription_suite_nbttest_test_tag : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_nbttest_test_tag() : CxxTest::RealTestDescription( Tests_nbttest, suiteDescription_nbttest, 32, "test_tag" ) {}
 void runTest() { suite_nbttest.test_tag(); }
} testDescription_suite_nbttest_test_tag;

static class TestDescription_suite_nbttest_test_get_type : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_nbttest_test_get_type() : CxxTest::RealTestDescription( Tests_nbttest, suiteDescription_nbttest, 68, "test_get_type" ) {}
 void runTest() { suite_nbttest.test_get_type(); }
} testDescription_suite_nbttest_test_get_type;

static class TestDescription_suite_nbttest_test_tag_primitive : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_nbttest_test_tag_primitive() : CxxTest::RealTestDescription( Tests_nbttest, suiteDescription_nbttest, 84, "test_tag_primitive" ) {}
 void runTest() { suite_nbttest.test_tag_primitive(); }
} testDescription_suite_nbttest_test_tag_primitive;

static class TestDescription_suite_nbttest_test_tag_string : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_nbttest_test_tag_string() : CxxTest::RealTestDescription( Tests_nbttest, suiteDescription_nbttest, 113, "test_tag_string" ) {}
 void runTest() { suite_nbttest.test_tag_string(); }
} testDescription_suite_nbttest_test_tag_string;

static class TestDescription_suite_nbttest_test_tag_compound : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_nbttest_test_tag_compound() : CxxTest::RealTestDescription( Tests_nbttest, suiteDescription_nbttest, 133, "test_tag_compound" ) {}
 void runTest() { suite_nbttest.test_tag_compound(); }
} testDescription_suite_nbttest_test_tag_compound;

static class TestDescription_suite_nbttest_test_value : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_nbttest_test_value() : CxxTest::RealTestDescription( Tests_nbttest, suiteDescription_nbttest, 231, "test_value" ) {}
 void runTest() { suite_nbttest.test_value(); }
} testDescription_suite_nbttest_test_value;

static class TestDescription_suite_nbttest_test_tag_list : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_nbttest_test_tag_list() : CxxTest::RealTestDescription( Tests_nbttest, suiteDescription_nbttest, 306, "test_tag_list" ) {}
 void runTest() { suite_nbttest.test_tag_list(); }
} testDescription_suite_nbttest_test_tag_list;

static class TestDescription_suite_nbttest_test_tag_byte_array : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_nbttest_test_tag_byte_array() : CxxTest::RealTestDescription( Tests_nbttest, suiteDescription_nbttest, 376, "test_tag_byte_array" ) {}
 void runTest() { suite_nbttest.test_tag_byte_array(); }
} testDescription_suite_nbttest_test_tag_byte_array;

static class TestDescription_suite_nbttest_test_tag_int_array : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_nbttest_test_tag_int_array() : CxxTest::RealTestDescription( Tests_nbttest, suiteDescription_nbttest, 407, "test_tag_int_array" ) {}
 void runTest() { suite_nbttest.test_tag_int_array(); }
} testDescription_suite_nbttest_test_tag_int_array;

static class TestDescription_suite_nbttest_test_tag_long_array : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_nbttest_test_tag_long_array() : CxxTest::RealTestDescription( Tests_nbttest, suiteDescription_nbttest, 436, "test_tag_long_array" ) {}
 void runTest() { suite_nbttest.test_tag_long_array(); }
} testDescription_suite_nbttest_test_tag_long_array;

static class TestDescription_suite_nbttest_test_visitor : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_nbttest_test_visitor() : CxxTest::RealTestDescription( Tests_nbttest, suiteDescription_nbttest, 465, "test_visitor" ) {}
 void runTest() { suite_nbttest.test_visitor(); }
} testDescription_suite_nbttest_test_visitor;

#include "..\..\lib\nbtplusplus\test\read_test.h"

static read_test suite_read_test;

static CxxTest::List Tests_read_test = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_read_test( "../../lib/nbtplusplus/test/read_test.h", 32, "read_test", suite_read_test, Tests_read_test );

static class TestDescription_suite_read_test_test_stream_reader_big : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_read_test_test_stream_reader_big() : CxxTest::RealTestDescription( Tests_read_test, suiteDescription_read_test, 35, "test_stream_reader_big" ) {}
 void runTest() { suite_read_test.test_stream_reader_big(); }
} testDescription_suite_read_test_test_stream_reader_big;

static class TestDescription_suite_read_test_test_stream_reader_little : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_read_test_test_stream_reader_little() : CxxTest::RealTestDescription( Tests_read_test, suiteDescription_read_test, 81, "test_stream_reader_little" ) {}
 void runTest() { suite_read_test.test_stream_reader_little(); }
} testDescription_suite_read_test_test_stream_reader_little;

static class TestDescription_suite_read_test_test_read_bigtest : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_read_test_test_read_bigtest() : CxxTest::RealTestDescription( Tests_read_test, suiteDescription_read_test, 146, "test_read_bigtest" ) {}
 void runTest() { suite_read_test.test_read_bigtest(); }
} testDescription_suite_read_test_test_read_bigtest;

static class TestDescription_suite_read_test_test_read_littletest : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_read_test_test_read_littletest() : CxxTest::RealTestDescription( Tests_read_test, suiteDescription_read_test, 157, "test_read_littletest" ) {}
 void runTest() { suite_read_test.test_read_littletest(); }
} testDescription_suite_read_test_test_read_littletest;

static class TestDescription_suite_read_test_test_read_errors : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_read_test_test_read_errors() : CxxTest::RealTestDescription( Tests_read_test, suiteDescription_read_test, 169, "test_read_errors" ) {}
 void runTest() { suite_read_test.test_read_errors(); }
} testDescription_suite_read_test_test_read_errors;

static class TestDescription_suite_read_test_test_read_misc : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_read_test_test_read_misc() : CxxTest::RealTestDescription( Tests_read_test, suiteDescription_read_test, 202, "test_read_misc" ) {}
 void runTest() { suite_read_test.test_read_misc(); }
} testDescription_suite_read_test_test_read_misc;

static class TestDescription_suite_read_test_test_read_gzip : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_read_test_test_read_gzip() : CxxTest::RealTestDescription( Tests_read_test, suiteDescription_read_test, 223, "test_read_gzip" ) {}
 void runTest() { suite_read_test.test_read_gzip(); }
} testDescription_suite_read_test_test_read_gzip;

#include "..\..\lib\nbtplusplus\test\write_test.h"

static write_test suite_write_test;

static CxxTest::List Tests_write_test = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_write_test( "../../lib/nbtplusplus/test/write_test.h", 34, "write_test", suite_write_test, Tests_write_test );

static class TestDescription_suite_write_test_test_stream_writer_big : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_write_test_test_stream_writer_big() : CxxTest::RealTestDescription( Tests_write_test, suiteDescription_write_test, 37, "test_stream_writer_big" ) {}
 void runTest() { suite_write_test.test_stream_writer_big(); }
} testDescription_suite_write_test_test_stream_writer_big;

static class TestDescription_suite_write_test_test_stream_writer_little : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_write_test_test_stream_writer_little() : CxxTest::RealTestDescription( Tests_write_test, suiteDescription_write_test, 71, "test_stream_writer_little" ) {}
 void runTest() { suite_write_test.test_stream_writer_little(); }
} testDescription_suite_write_test_test_stream_writer_little;

static class TestDescription_suite_write_test_test_write_payload_big : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_write_test_test_write_payload_big() : CxxTest::RealTestDescription( Tests_write_test, suiteDescription_write_test, 95, "test_write_payload_big" ) {}
 void runTest() { suite_write_test.test_write_payload_big(); }
} testDescription_suite_write_test_test_write_payload_big;

static class TestDescription_suite_write_test_test_write_bigtest : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_write_test_test_write_bigtest() : CxxTest::RealTestDescription( Tests_write_test, suiteDescription_write_test, 230, "test_write_bigtest" ) {}
 void runTest() { suite_write_test.test_write_bigtest(); }
} testDescription_suite_write_test_test_write_bigtest;

#include "..\..\lib\nbtplusplus\test\zlibstream_test.h"

static zlibstream_test suite_zlibstream_test;

static CxxTest::List Tests_zlibstream_test = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_zlibstream_test( "../../lib/nbtplusplus/test/zlibstream_test.h", 28, "zlibstream_test", suite_zlibstream_test, Tests_zlibstream_test );

static class TestDescription_suite_zlibstream_test_test_inflate_gzip : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_zlibstream_test_test_inflate_gzip() : CxxTest::RealTestDescription( Tests_zlibstream_test, suiteDescription_zlibstream_test, 44, "test_inflate_gzip" ) {}
 void runTest() { suite_zlibstream_test.test_inflate_gzip(); }
} testDescription_suite_zlibstream_test_test_inflate_gzip;

static class TestDescription_suite_zlibstream_test_test_inflate_zlib : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_zlibstream_test_test_inflate_zlib() : CxxTest::RealTestDescription( Tests_zlibstream_test, suiteDescription_zlibstream_test, 97, "test_inflate_zlib" ) {}
 void runTest() { suite_zlibstream_test.test_inflate_zlib(); }
} testDescription_suite_zlibstream_test_test_inflate_zlib;

static class TestDescription_suite_zlibstream_test_test_inflate_corrupt : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_zlibstream_test_test_inflate_corrupt() : CxxTest::RealTestDescription( Tests_zlibstream_test, suiteDescription_zlibstream_test, 116, "test_inflate_corrupt" ) {}
 void runTest() { suite_zlibstream_test.test_inflate_corrupt(); }
} testDescription_suite_zlibstream_test_test_inflate_corrupt;

static class TestDescription_suite_zlibstream_test_test_inflate_trailing : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_zlibstream_test_test_inflate_trailing() : CxxTest::RealTestDescription( Tests_zlibstream_test, suiteDescription_zlibstream_test, 144, "test_inflate_trailing" ) {}
 void runTest() { suite_zlibstream_test.test_inflate_trailing(); }
} testDescription_suite_zlibstream_test_test_inflate_trailing;

static class TestDescription_suite_zlibstream_test_test_inflate_open : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_zlibstream_test_test_inflate_open() : CxxTest::RealTestDescription( Tests_zlibstream_test, suiteDescription_zlibstream_test, 166, "test_inflate_open" ) {}
 void runTest() { suite_zlibstream_test.test_inflate_open(); }
} testDescription_suite_zlibstream_test_test_inflate_open;

static class TestDescription_suite_zlibstream_test_test_deflate_zlib : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_zlibstream_test_test_deflate_zlib() : CxxTest::RealTestDescription( Tests_zlibstream_test, suiteDescription_zlibstream_test, 219, "test_deflate_zlib" ) {}
 void runTest() { suite_zlibstream_test.test_deflate_zlib(); }
} testDescription_suite_zlibstream_test_test_deflate_zlib;

static class TestDescription_suite_zlibstream_test_test_deflate_gzip : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_zlibstream_test_test_deflate_gzip() : CxxTest::RealTestDescription( Tests_zlibstream_test, suiteDescription_zlibstream_test, 290, "test_deflate_gzip" ) {}
 void runTest() { suite_zlibstream_test.test_deflate_gzip(); }
} testDescription_suite_zlibstream_test_test_deflate_gzip;

static class TestDescription_suite_zlibstream_test_test_deflate_open : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_zlibstream_test_test_deflate_open() : CxxTest::RealTestDescription( Tests_zlibstream_test, suiteDescription_zlibstream_test, 311, "test_deflate_open" ) {}
 void runTest() { suite_zlibstream_test.test_deflate_open(); }
} testDescription_suite_zlibstream_test_test_deflate_open;

static class TestDescription_suite_zlibstream_test_test_deflate_closed : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_zlibstream_test_test_deflate_closed() : CxxTest::RealTestDescription( Tests_zlibstream_test, suiteDescription_zlibstream_test, 349, "test_deflate_closed" ) {}
 void runTest() { suite_zlibstream_test.test_deflate_closed(); }
} testDescription_suite_zlibstream_test_test_deflate_closed;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
