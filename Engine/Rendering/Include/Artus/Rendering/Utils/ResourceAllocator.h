//
// Created by fightinghawks18 on 2/13/2026.
//


#ifndef ARTUS_RENDERING_RESOURCE_ALLOCATOR_H
#define ARTUS_RENDERING_RESOURCE_ALLOCATOR_H

#include <cstdint>
#include <vector>

namespace Artus::Rendering {
    constexpr uint32_t INVALID_HANDLE = UINT32_MAX;

    template <typename T>
    struct Handle {
        uint32_t id, generation;
        bool valid;
    };

    template <typename T>
    class ResourceAllocator {
    public:
        explicit ResourceAllocator(uint32_t reserveCount = 5);
        ~ResourceAllocator();

        template <typename... Args>
        Handle<T> Allocate(Args&&... constructorArgs) {
            uint32_t id = INVALID_HANDLE;
            if (mFreeSlotHead != INVALID_HANDLE) {
                id = mFreeSlotHead;
                mFreeSlotHead = mSlots[id].nextFreeSlot;
            } else {
                id = mSlots.size();
                mSlots.push_back({});
            }

            mSlots[id].resource = new T(std::forward<Args>(constructorArgs)...);
            mSlots[id].allocated = true;
            ++mSlots[id].generation;

            return Handle<T>{ id, mSlots[id].generation, true };
        }

        T* Get(Handle<T>& handle) {
            if (!IsHandleValid(handle))
                return nullptr;
            return mSlots[handle.id].resource;
        }

        void Free(Handle<T>& handle) {
            if (!IsHandleValid(handle))
                return;
            delete mSlots[handle.id].resource;
            mSlots[handle.id].resource = nullptr;
            mSlots[handle.id].allocated = false;
            mSlots[handle.id].nextFreeSlot = mFreeSlotHead;
            mFreeSlotHead = handle.id;
        }
    private:
        struct Slot {
            union {
                T* resource;
                uint32_t nextFreeSlot;
            };
            uint32_t id;
            uint32_t generation;
            bool allocated;
        };

        std::vector<Slot> mSlots;
        uint32_t mFreeSlotHead = INVALID_HANDLE;

        static void InvalidateHandle(Handle<T>& handle) {
            handle.generation = INVALID_HANDLE;
            handle.id = INVALID_HANDLE;
            handle.valid = false;
        }

        bool IsHandleValid(Handle<T>& handle) {
            if (handle.id >= mSlots.size()) {
                InvalidateHandle(handle);
                return false;
            }

            if (!mSlots[handle.id].allocated) {
                InvalidateHandle(handle);
                return false;
            }

            if (handle.generation != mSlots[handle.id].generation) {
                InvalidateHandle(handle);
                return false;
            }

            return true;
        }
    };
}

#endif // ARTUS_RENDERING_RESOURCE_ALLOCATOR_H