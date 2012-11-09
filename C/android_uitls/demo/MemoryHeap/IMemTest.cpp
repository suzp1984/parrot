#include "IMemTest.h"

namespace android {

class BpMemTest : public BpInterface<IMemTest>
{
public:
	BpMemTest(const sp<IBinder>& impl)
		: BpInterface<IMemTest>(impl)
	{
	}

	virtual sp<IMemoryHeap> getMem() const 
	{
		Parcel data, reply;
		data.writeInterfaceToken(IMemTest::getInterfaceDescriptor());
		remote()->transact(BnMemTest::GET_MEM, data, &reply);
		return interface_cast<IMemTest>(reply.readStrongBinder());
	}
};

IMPLEMENT_META_INTERFACE(MemTest, "android.test.IMemTest");

status_t BnMemTest::onTransact(
		uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
	switch(code) {
		case GET_MEM: {
			CHECK_INTERFACE(IMemTest, data, reply);
			sp<IBinder> b = getMem()->asBinder();
			reply->writeStrongBinder(b);
		} break;
		default:
			return BBinder::onTransact(code, data, reply, flags);
	}

	return NO_ERROR;
}

}; //namespace andorid
