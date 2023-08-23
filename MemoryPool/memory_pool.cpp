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

int ROUND_SMALL = 512;

int main(int, char **)
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

                // | Mem1 |
        
                std::cout << "dividing... 256 from mem1" << std::endl;
                second_start = ROUND_SMALL / 2; // 256B
                Memory *mem2 = mem1->divide(second_start);
                mem1->lock(); // mem1はロック
                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock");
                std::printf("mem2 bytes: %d (%s)\n", (int)mem2->bytes(), mem2->locked() ? "lock" : "unlock");

                // | Mem1 |
                // | Mem2 |

                // 解放するMemoryの両隣とマージできるかチェックする
                std::cout << "mem2->try_merge(next)... " << std::endl;
                Memory *next = mem2->next();
                mem2->try_merge(next);
                std::cout << "mem2->try_merge(prev)..." << std::endl;
                Memory *prev = mem2->prev();
                mem2->try_merge(prev); 
                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock");
                std::printf("mem2 bytes: %d (%s)\n", (int)mem2->bytes(), mem2->locked() ? "lock" : "unlock");
                mem2->release();
                // delete mem2; // mem1 == mem2->prev() が有効なので, デリートしてはいけない. つまり, 貸出を終えてキャッシュに戻す.

                std::cout << "mem1->try_merge(next)..." << std::endl;
                next = mem1->next();
                mem1->try_merge(next);
                std::cout << "mem1->try_merge(prev)..." << std::endl;
                prev = mem1->prev();
                mem1->try_merge(prev);
                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock");
                mem1->release();
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

                // | Mem1 |

                std::cout << "dividing... 256 from mem1" << std::endl;
                second_start = ROUND_SMALL / 2; // 256B
                Memory *mem2 = mem1->divide(second_start);
                mem1->lock(); // mem1はロック
                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock"); // 256B lock
                std::printf("mem2 bytes: %d (%s)\n", (int)mem2->bytes(), mem2->locked() ? "lock" : "unlock"); // 384B unlock

                // | Mem1 |
                // | Mem2 |

                std::cout << "dividing... 128 from mem2" << std::endl;
                second_start = 128; // 128B
                Memory *mem3 = mem2->divide(second_start);
                mem2->lock(); // mem2はロック
                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock"); // 256B lock
                std::printf("mem2 bytes: %d (%s)\n", (int)mem2->bytes(), mem2->locked() ? "lock" : "unlock"); // 128B lock
                std::printf("mem3 bytes: %d (%s)\n", (int)mem3->bytes(), mem3->locked() ? "lock" : "unlock"); // 256B unlock

                // | Mem1 |
                // | Mem2 |
                // | Mem3 |

                std::cout << "dividing... 128 from mem3" << std::endl;
                second_start = 128; // 128B
                Memory *mem4 = mem3->divide(second_start);
                mem3->lock(); // mem3はロック
                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock"); // 256B lock
                std::printf("mem2 bytes: %d (%s)\n", (int)mem2->bytes(), mem2->locked() ? "lock" : "unlock"); // 128B lock
                std::printf("mem3 bytes: %d (%s)\n", (int)mem3->bytes(), mem3->locked() ? "lock" : "unlock"); // 128B lock
                std::printf("mem4 bytes: %d (%s)\n", (int)mem4->bytes(), mem4->locked() ? "lock" : "unlock"); // 128B unlock

                // | Mem1 |
                // | Mem2 |
                // | Mem3 |
                // | Mem4 |

                /* release mem2 */

                // 解放するMemoryのprev側のMmeoryとマージを実行
                std::cout << "mem2->try_merge(prev)..." << std::endl;
                Memory *prev = mem2->prev();
                mem2->try_merge(prev);

                // 解放するMemoryのnext側のMemoryとマージを実行
                std::cout << "mem2->try_merge(next)..." << std::endl;
                Memory *next = mem2->next();
                mem2->try_merge(next);
                std::printf("*** mem2 bytes: %d (%s)\n", (int)mem2->bytes(), mem2->locked() ? "lock" : "unlock"); // 128B lock

                mem2->release();
                // delete mem2; // デリート禁止. キャッシュに戻す.

                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock"); // 384B lock
                std::printf("mem3 bytes: %d (%s)\n", (int)mem3->bytes(), mem3->locked() ? "lock" : "unlock"); // 128B lock
                std::printf("mem4 bytes: %d (%s)\n", (int)mem3->bytes(), mem4->locked() ? "lock" : "unlock"); // 128B unlock

                // | Mem1 |
                // | Mem3 |
                // | Mem4 |

                /* release mem4 */
                next = mem4->next();
                mem4->try_merge(next);
                prev = mem4->prev();
                mem4->try_merge(prev);
                mem4->release();
                // delete mem4; // デリート禁止. キャッシュに戻す.

                /* release mem3 */
                next = mem3->next();
                mem3->try_merge(next);
                prev = mem3->prev();
                mem3->try_merge(prev);
                mem3->release();
                // delete mem3; // デリート禁止. キャッシュに戻す.

                /* release mem1 */
                next = mem1->next();
                mem1->try_merge(next);
                prev = mem1->prev();
                mem1->try_merge(prev);
                mem1->release();
                delete mem1; // 先頭chunkなので, デリートOK
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