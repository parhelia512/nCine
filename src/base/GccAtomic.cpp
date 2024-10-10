#include <ncine/common_macros.h>
#include <nctl/Atomic.h>

namespace nctl {

///////////////////////////////////////////////////////////
// AtomicFence
///////////////////////////////////////////////////////////

void Atomic::threadFence(MemoryModel memModel)
{
	switch (memModel)
	{
		case MemoryModel::RELAXED:
			__atomic_thread_fence(__ATOMIC_RELAXED);
			break;
		case MemoryModel::ACQUIRE:
			__atomic_thread_fence(__ATOMIC_ACQUIRE);
			break;
		case MemoryModel::RELEASE:
			FATAL_MSG("Incompatible memory model");
			break;
		case MemoryModel::SEQ_CST:
		default:
			__atomic_thread_fence(__ATOMIC_SEQ_CST);
			break;
	}
}

void Atomic::signalFence(MemoryModel memModel)
{
	switch (memModel)
	{
		case MemoryModel::RELAXED:
			__atomic_signal_fence(__ATOMIC_RELAXED);
			break;
		case MemoryModel::ACQUIRE:
			__atomic_signal_fence(__ATOMIC_ACQUIRE);
			break;
		case MemoryModel::RELEASE:
			FATAL_MSG("Incompatible memory model");
			break;
		case MemoryModel::SEQ_CST:
		default:
			__atomic_signal_fence(__ATOMIC_SEQ_CST);
			break;
	}
}

/*
bool Atomic::alwaysLockFree(unsigned long int size, void *ptr)
{
	return __atomic_always_lock_free(size, ptr);
}

bool Atomic::isLockFree(unsigned long int size, const void *ptr)
{
	return __atomic_is_lock_free(size, ptr);
}
*/

///////////////////////////////////////////////////////////
// Atomic32
///////////////////////////////////////////////////////////

int32_t Atomic32::load(MemoryModel memModel) const
{
	switch (memModel)
	{
		case MemoryModel::RELAXED:
			__atomic_load_n(&value_, __ATOMIC_RELAXED);
			return value_;
		case MemoryModel::ACQUIRE:
			__atomic_load_n(&value_, __ATOMIC_ACQUIRE);
			return value_;
		case MemoryModel::RELEASE:
			FATAL_MSG("Incompatible memory model");
			return 0;
		case MemoryModel::SEQ_CST:
		default:
			__atomic_load_n(&value_, __ATOMIC_SEQ_CST);
			return value_;
	}
}

void Atomic32::store(int32_t value, MemoryModel memModel)
{
	switch (memModel)
	{
		case MemoryModel::RELAXED:
			__atomic_store_n(&value_, value, __ATOMIC_RELAXED);
			break;
		case MemoryModel::ACQUIRE:
			FATAL_MSG("Incompatible memory model");
			break;
		case MemoryModel::RELEASE:
			__atomic_store_n(&value_, value, __ATOMIC_RELEASE);
			break;
		case MemoryModel::SEQ_CST:
		default:
			__atomic_store_n(&value_, value, __ATOMIC_SEQ_CST);
			break;
	}
}

int32_t Atomic32::fetchAdd(int32_t value, MemoryModel memModel)
{
	switch (memModel)
	{
		case MemoryModel::RELAXED:
			return __atomic_fetch_add(&value_, value, __ATOMIC_RELAXED);
		case MemoryModel::ACQUIRE:
			return __atomic_fetch_add(&value_, value, __ATOMIC_ACQUIRE);
		case MemoryModel::RELEASE:
			return __atomic_fetch_add(&value_, value, __ATOMIC_RELEASE);
		case MemoryModel::SEQ_CST:
		default:
			return __atomic_fetch_add(&value_, value, __ATOMIC_SEQ_CST);
	}
}

int32_t Atomic32::fetchSub(int32_t value, MemoryModel memModel)
{
	switch (memModel)
	{
		case MemoryModel::RELAXED:
			return __atomic_fetch_sub(&value_, value, __ATOMIC_RELAXED);
		case MemoryModel::ACQUIRE:
			return __atomic_fetch_sub(&value_, value, __ATOMIC_ACQUIRE);
		case MemoryModel::RELEASE:
			return __atomic_fetch_sub(&value_, value, __ATOMIC_RELEASE);
		case MemoryModel::SEQ_CST:
		default:
			return __atomic_fetch_sub(&value_, value, __ATOMIC_SEQ_CST);
	}
}

bool Atomic32::cmpExchange(int32_t newValue, int32_t cmpValue, MemoryModel memModel)
{
	switch (memModel)
	{
		case MemoryModel::RELAXED:
			return __atomic_compare_exchange_n(&value_, &cmpValue, newValue, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
		case MemoryModel::ACQUIRE:
			return __atomic_compare_exchange_n(&value_, &cmpValue, newValue, false, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED);
		case MemoryModel::RELEASE:
			return __atomic_compare_exchange_n(&value_, &cmpValue, newValue, false, __ATOMIC_RELEASE, __ATOMIC_RELAXED);
		case MemoryModel::SEQ_CST:
		default:
			return __atomic_compare_exchange_n(&value_, &cmpValue, newValue, false, __ATOMIC_SEQ_CST, __ATOMIC_RELAXED);
	}
}

///////////////////////////////////////////////////////////
// Atomic64
///////////////////////////////////////////////////////////

int64_t Atomic64::load(MemoryModel memModel) const
{
	switch (memModel)
	{
		case MemoryModel::RELAXED:
			__atomic_load_n(&value_, __ATOMIC_RELAXED);
			return value_;
		case MemoryModel::ACQUIRE:
			__atomic_load_n(&value_, __ATOMIC_ACQUIRE);
			return value_;
		case MemoryModel::RELEASE:
			FATAL_MSG("Incompatible memory model");
			return 0;
		case MemoryModel::SEQ_CST:
		default:
			__atomic_load_n(&value_, __ATOMIC_SEQ_CST);
			return value_;
	}
}

void Atomic64::store(int64_t value, MemoryModel memModel)
{
	switch (memModel)
	{
		case MemoryModel::RELAXED:
			__atomic_store_n(&value_, value, __ATOMIC_RELAXED);
			break;
		case MemoryModel::ACQUIRE: FATAL_MSG("Incompatible memory model");
		case MemoryModel::RELEASE:
			__atomic_store_n(&value_, value, __ATOMIC_RELEASE);
			break;
		case MemoryModel::SEQ_CST:
		default:
			__atomic_store_n(&value_, value, __ATOMIC_SEQ_CST);
			break;
	}
}

int64_t Atomic64::fetchAdd(int64_t value, MemoryModel memModel)
{
	switch (memModel)
	{
		case MemoryModel::RELAXED:
			return __atomic_fetch_add(&value_, value, __ATOMIC_RELAXED);
		case MemoryModel::ACQUIRE:
			return __atomic_fetch_add(&value_, value, __ATOMIC_ACQUIRE);
		case MemoryModel::RELEASE:
			return __atomic_fetch_add(&value_, value, __ATOMIC_RELEASE);
		case MemoryModel::SEQ_CST:
		default:
			return __atomic_fetch_add(&value_, value, __ATOMIC_SEQ_CST);
	}
}

int64_t Atomic64::fetchSub(int64_t value, MemoryModel memModel)
{
	switch (memModel)
	{
		case MemoryModel::RELAXED:
			return __atomic_fetch_sub(&value_, value, __ATOMIC_RELAXED);
		case MemoryModel::ACQUIRE:
			return __atomic_fetch_sub(&value_, value, __ATOMIC_ACQUIRE);
		case MemoryModel::RELEASE:
			return __atomic_fetch_sub(&value_, value, __ATOMIC_RELEASE);
		case MemoryModel::SEQ_CST:
		default:
			return __atomic_fetch_sub(&value_, value, __ATOMIC_SEQ_CST);
	}
}

bool Atomic64::cmpExchange(int64_t newValue, int64_t cmpValue, MemoryModel memModel)
{
	switch (memModel)
	{
		case MemoryModel::RELAXED:
			return __atomic_compare_exchange_n(&value_, &cmpValue, newValue, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
		case MemoryModel::ACQUIRE:
			return __atomic_compare_exchange_n(&value_, &cmpValue, newValue, false, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED);
		case MemoryModel::RELEASE:
			return __atomic_compare_exchange_n(&value_, &cmpValue, newValue, false, __ATOMIC_RELEASE, __ATOMIC_RELAXED);
		case MemoryModel::SEQ_CST:
		default:
			return __atomic_compare_exchange_n(&value_, &cmpValue, newValue, false, __ATOMIC_SEQ_CST, __ATOMIC_RELAXED);
	}
}

}
