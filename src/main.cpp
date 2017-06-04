#include "server/server.h"
#include "data/datatypes.h"
#include <iostream>
using namespace std;

/* Main entry point for the server */
int main()
{
	Server* server = new Server();

	// Test the VarInts
	VarInt a1(0);
	VarInt a2(1);
	VarInt a3(2);
	VarInt a4(127);
	VarInt a5(128);
	VarInt a6(255);
	VarInt a7(0x7FFFFFFF);
	VarInt a8(0xFFFFFFFF);
	VarInt a9(0x80000000);
	cout << (Int)a1.getSize() << ", " << a1.toInt() << endl;
	cout << (Int)a2.getSize() << ", " << a2.toInt() << endl;
	cout << (Int)a3.getSize() << ", " << a3.toInt() << endl;
	cout << (Int)a4.getSize() << ", " << a4.toInt() << endl;
	cout << (Int)a5.getSize() << ", " << a5.toInt() << endl;
	cout << (Int)a6.getSize() << ", " << a6.toInt() << endl;
	cout << (Int)a7.getSize() << ", " << a7.toInt() << endl;
	cout << (Int)a8.getSize() << ", " << a8.toInt() << endl;
	cout << (Int)a9.getSize() << ", " << a9.toInt() << endl;
	cout << (Int)(a1 = a2).getSize() << ", " << (a1 = a2).toInt() << endl;

	// Test the VarLongs
	VarLong b1((Long)0);
	VarLong b2(1);
	VarLong b3(2);
	VarLong b4(127);
	VarLong b5(128);
	VarLong b6(255);
	VarLong b7(0x7FFFFFFF);
	VarLong b8(0x7FFFFFFFFFFFFFFF);
	VarLong b9(0xFFFFFFFFFFFFFFFF);
	VarLong ba(0xFFFFFFFF80000000);
	VarLong bb(0x8000000000000000);
	cout << (Long)b1.getSize() << ", " << b1.toLong() << endl;
	cout << (Long)b2.getSize() << ", " << b2.toLong() << endl;
	cout << (Long)b3.getSize() << ", " << b3.toLong() << endl;
	cout << (Long)b4.getSize() << ", " << b4.toLong() << endl;
	cout << (Long)b5.getSize() << ", " << b5.toLong() << endl;
	cout << (Long)b6.getSize() << ", " << b6.toLong() << endl;
	cout << (Long)b7.getSize() << ", " << b7.toLong() << endl;
	cout << (Long)b8.getSize() << ", " << b8.toLong() << endl;
	cout << (Long)b9.getSize() << ", " << b9.toLong() << endl;
	cout << (Long)ba.getSize() << ", " << ba.toLong() << endl;
	cout << (Long)bb.getSize() << ", " << bb.toLong() << "\n\n";

	server->start();

	return 0;
}