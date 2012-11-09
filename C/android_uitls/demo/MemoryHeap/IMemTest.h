#ifndef T_MEMTEST_H
#define T_MEMTEST_H

#include <utils/RefBase.h>
#include <utils/Errors.h>

#include <binder/IInterface.h>

namespace android {

struct priv_info {
	int a;
	char c;
};

class IMemTest : public IInterface
{
public:
	DECLARE_META_INTERFACE(MemTest);

	virtual sp<IMemoryHeap> getMem() const = 0;
};

class BnMemTest : public BnInterface<IMemTest>
{
public:
	enum {
		GET_MEM = IBinder::FIRST_CALL_TRANSACTION
	};

	virtual status_t onTransact(uint32_t code, const Parcel& data,
							Parcel* reply, uint32_t flags = 0);

};

}; //namespace android

#endif //T_MEMTEST_H
