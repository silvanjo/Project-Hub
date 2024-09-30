#include <iostream>
#include <vector>
#include <new>    // For placement new
#include <cstddef> // For std::size_t

// MemoryPool class definition
class MemoryPool {
private:
    std::size_t blockSize;                 // Size of each block
    std::size_t poolSize;                  // Total number of blocks in the pool
    std::vector<char> memory;              // Continuous block of memory
    std::vector<void*> freeBlocks;         // Pointers to free blocks

public:
    // Constructor: initializes the memory pool
    MemoryPool(std::size_t blockSize, std::size_t poolSize)
        : blockSize(blockSize), poolSize(poolSize), memory(blockSize * poolSize)
    {
        // Populate the freeBlocks vector with pointers to each block
        for (std::size_t i = 0; i < poolSize; ++i) {
            freeBlocks.push_back(&memory[i * blockSize]);
        }
    }

    // Allocate a block of memory from the pool
    void* allocate() {
        if (freeBlocks.empty()) {
            std::cerr << "Memory Pool exhausted!" << std::endl;
            return nullptr;
        }
        void* ptr = freeBlocks.back();
        freeBlocks.pop_back();
        return ptr;
    }

    // Deallocate a block of memory and return it to the pool
    void deallocate(void* ptr) {
        freeBlocks.push_back(ptr);
    }
};

// Example object to be managed by the memory pool
struct MyObject {
    int data;
    // Constructor
    MyObject(int value) : data(value) {
        std::cout << "MyObject constructed with data = " << data << std::endl;
    }
    // Destructor
    ~MyObject() {
        std::cout << "MyObject with data = " << data << " destroyed" << std::endl;
    }
};

int main() {
    const std::size_t poolSize = 5; // Number of objects the pool can hold
    MemoryPool pool(sizeof(MyObject), poolSize);

    std::vector<MyObject*> objects;

    // Allocate objects from the memory pool
    for (int i = 0; i < 5; ++i) {
        void* mem = pool.allocate();
        if (mem) {
            // Use placement new to construct the object in the allocated memory
            MyObject* obj = new (mem) MyObject(i);
            objects.push_back(obj);
        }
    }

    // Attempt to allocate beyond the pool size to demonstrate exhaustion
    void* extraMem = pool.allocate();
    if (!extraMem) {
        std::cout << "No more memory available in the pool." << std::endl;
    }

    // Use the objects
    std::cout << "\nUsing the objects:\n";
    for (MyObject* obj : objects) {
        std::cout << "Object data: " << obj->data << std::endl;
    }

    // Deallocate objects and return memory to the pool
    std::cout << "\nDeallocating objects:\n";
    for (MyObject* obj : objects) {
        obj->~MyObject();       // Explicitly call the destructor
        pool.deallocate(obj);   // Return the memory to the pool
    }

    // Now we can allocate again after deallocation
    std::cout << "\nAllocating after deallocation:\n";
    for (int i = 5; i < 7; ++i) {
        void* mem = pool.allocate();
        if (mem) {
            MyObject* obj = new (mem) MyObject(i);
            // Do something with the new object
            obj->~MyObject();       // Clean up
            pool.deallocate(obj);   // Return the memory to the pool
        }
    }

    return 0;
}

