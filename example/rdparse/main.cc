#include <interbuf/io_service.h>
#include <peff/base/deallocable.h>
#include <peff/advutils/unique_ptr.h>

int main() {
	{
		peff::UniquePtr<interbuf::IOService, peff::DeallocableDeleter<interbuf::IOService>> ioService;

		interbuf::IOServiceCreationParams params(peff::getDefaultAlloc(), peff::getDefaultAlloc());

		interbuf::ExceptionPointer e = interbuf::createDefaultIOService(ioService.getRef(), params);

		if (e) {
			std::terminate();
		}
	}
}
