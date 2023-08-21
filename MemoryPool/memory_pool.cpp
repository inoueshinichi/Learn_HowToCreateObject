/**
 * @file memory_pool.cpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief メモリプール
 * @version 0.1
 * @date 2023-08-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <Memory/memory.hpp>
#include <PoolAllocator/pool_allocator.hpp>

#include <cstdio>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <map>

static constexpr int ROUND_SMALL = 512;       // 512B
static constexpr int ROUND_LARGE = 128 << 10; // 128KB
static constexpr int SMALL_ALLOC = 1 << 20;   // 1MB

using Key = int;
using Value = std::tuple<size_t, Memory *>;

using CacheMap = std::map<Key, Value>;



int main(int, char**)
{
    try
    {
        // Mmeoryクラスのテスト
        {
            std::cout << "Hello Memory" << std::endl;

            // 新規でメモリを確保
            {
                std::cout << "新規でメモリを確保" << std::endl;
                int mem1_bytes = ROUND_SMALL + 128; // 640B
                Memory *mem1 = new Memory(mem1_bytes);
                mem1->alloc();
                delete mem1;
            }

            // 確保済メモリを分割してメモリ確保-1
            {
                std::cout << "確保済メモリを分割してメモリ確保-1" << std::endl;
                int second_start = 0;

                int mem1_bytes = ROUND_SMALL + 128; // 640B
                Memory *mem1 = new Memory(mem1_bytes);
                mem1->alloc();
                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock");

                std::cout << "dividing... 256 from mem1" << std::endl;
                second_start = ROUND_SMALL / 2; // 256B
                Memory *mem2 = mem1->divide(second_start);
                mem1->lock(); // mem1はロック
                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock");
                std::printf("mem2 bytes: %d (%s)\n", (int)mem2->bytes(), mem2->locked() ? "lock" : "unlock");

                std::cout << "try_merge... " << std::endl;
                mem1->try_merge(mem2);
                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock");
                std::printf("mem2 bytes: %d (%s)\n", (int)mem2->bytes(), mem2->locked() ? "lock" : "unlock");

                delete mem2;
                delete mem1;
            }

            // 確保済メモリを分割してメモリ確保-2
            {
                std::cout << "確保済メモリを分割してメモリ確保-2" << std::endl;

                int second_start = 0;

                int mem1_bytes = ROUND_SMALL + 128; // 640B
                Memory *mem1 = new Memory(mem1_bytes);
                mem1->alloc();
                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock");

                std::cout << "dividing... 256 from mem1" << std::endl;
                second_start = ROUND_SMALL / 2; // 256B
                Memory *mem2 = mem1->divide(second_start);
                mem1->lock(); // mem1はロック
                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock"); // 256B lock
                std::printf("mem2 bytes: %d (%s)\n", (int)mem2->bytes(), mem2->locked() ? "lock" : "unlock"); // 384B unlock

                std::cout << "dividing... 128 from mem2" << std::endl;
                second_start = 128; // 128B
                Memory *mem3 = mem2->divide(second_start);
                mem2->lock(); // mem2はロック
                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock"); // 256B lock
                std::printf("mem2 bytes: %d (%s)\n", (int)mem2->bytes(), mem2->locked() ? "lock" : "unlock"); // 128B lock
                std::printf("mem3 bytes: %d (%s)\n", (int)mem3->bytes(), mem3->locked() ? "lock" : "unlock"); // 256B unlock

                std::cout << "dividing... 128 from mem3" << std::endl;
                second_start = 128; // 128B
                Memory *mem4 = mem3->divide(second_start);
                mem3->lock(); // mem3はロック
                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock"); // 256B lock
                std::printf("mem2 bytes: %d (%s)\n", (int)mem2->bytes(), mem2->locked() ? "lock" : "unlock"); // 128B lock
                std::printf("mem3 bytes: %d (%s)\n", (int)mem3->bytes(), mem3->locked() ? "lock" : "unlock"); // 128B lock
                std::printf("mem4 bytes: %d (%s)\n", (int)mem4->bytes(), mem4->locked() ? "lock" : "unlock"); // 128B unlock

                /* release mem2 */
                mem2->release();
#if (1)
                // 合理的なマージ
                std::cout << "mem1->try_merge(mem2)..." << std::endl;
                mem1->try_merge(mem2);
#else
                // 非合理なマージ -> PoolAllocatorを使用してマージする際は, 実装されない
                std::cout << "mem3->try_merge(mem2)..." << std::endl;
                mem3->try_merge(mem2);
#endif
                std::printf("*** mem2 bytes: %d (%s)\n", (int)mem2->bytes(), mem2->locked() ? "lock" : "unlock"); // 128B lock
                delete mem2;

                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock"); // 384B lock
                std::printf("mem3 bytes: %d (%s)\n", (int)mem3->bytes(), mem3->locked() ? "lock" : "unlock"); // 128B lock
                std::printf("mem4 bytes: %d (%s)\n", (int)mem3->bytes(), mem4->locked() ? "lock" : "unlock"); // 128B unlock

                /* release mem4 */
                mem4->release();
                std::cout << "mem3->try_merge(mem4)..."  << std::endl;
                mem3->try_merge(mem4);
                delete mem4;

                /* release mem3 */
                mem3->release();
                std::cout << "mem1->try_merge(mem3)..." << std::endl;
                mem1->try_merge(mem3);
                delete mem3;

                /* release mem1 */
                mem1->release();
                delete mem1;
            }
            
        }

        // PoolAlocatorクラスのテスト
        {

        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}