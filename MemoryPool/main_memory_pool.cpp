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

void show_info_caching_memory(PoolAllocator &pool)
{
    std::printf("********** Caching memory info **********\n");

    auto cache_counts = pool.get_used_memory_counts(); // [small], [large]
    std::printf("[Caching counts] Small: [%d], Large: [%d]\n", cache_counts.at(0), cache_counts.at(1));
}

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
                mem1->lock();                                                                                 // mem1はロック
                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock"); // 256B lock
                std::printf("mem2 bytes: %d (%s)\n", (int)mem2->bytes(), mem2->locked() ? "lock" : "unlock"); // 384B unlock

                // | Mem1 |
                // | Mem2 |

                std::cout << "dividing... 128 from mem2" << std::endl;
                second_start = 128; // 128B
                Memory *mem3 = mem2->divide(second_start);
                mem2->lock();                                                                                 // mem2はロック
                std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock"); // 256B lock
                std::printf("mem2 bytes: %d (%s)\n", (int)mem2->bytes(), mem2->locked() ? "lock" : "unlock"); // 128B lock
                std::printf("mem3 bytes: %d (%s)\n", (int)mem3->bytes(), mem3->locked() ? "lock" : "unlock"); // 256B unlock

                // | Mem1 |
                // | Mem2 |
                // | Mem3 |

                std::cout << "dividing... 128 from mem3" << std::endl;
                second_start = 128; // 128B
                Memory *mem4 = mem3->divide(second_start);
                mem3->lock();                                                                                 // mem3はロック
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
                // delete mem1; // 先頭chunkなので, デリートOK
            }
        }

        // PoolAlocatorクラスのテスト
        {
            std::cout << "======= PoolAllocatorの動作確認 =======" << std::endl;

            auto pool = PoolAllocator();
            int second_start = 0;
            int mem1_bytes = ROUND_SMALL + 128; // 640B (1024B - 640B = 384B)

            // | Mem1 |
            // | rest |
            auto *mem1 = pool.alloc(mem1_bytes);
            mem1->set_required_bytes(mem1_bytes);

            std::printf("mem1 bytes: %d (%s)\n", (int)mem1->bytes(), mem1->locked() ? "lock" : "unlock");
            // std::printf("mem1'next bytes: %d (%s)\n", (int)mem1->next()->bytes(), mem1->next()->locked() ? "lock" : "unlock");
            // show_info_caching_memory(pool);

            pool.show_status_caching();

            // | Mem1 |
            // | Mem2 |
            // | rest |
            int mem2_bytes = 200; // 200B (512B - 200B = 184B)
            auto *mem2 = pool.alloc(mem2_bytes);
            mem2->set_required_bytes(mem2_bytes);

            pool.show_status_caching();

            // | Mem1 |
            // | Mem2 |
            // | Mem3 |
            // | rest |
            int mem3_bytes = 680; // 680B
            auto *mem3 = pool.alloc(mem3_bytes);
            mem3->set_required_bytes(mem3_bytes);

            pool.show_status_caching();

            mem2->lock();

            // | Mem1 |
            // | Mem2 |
            // | Mem3 |
            // | Mem4 |
            // | rest |
            int mem4_bytes = 1 << 19; // 512KB
            auto *mem4 = pool.alloc(mem4_bytes);
            mem4->set_required_bytes(mem4_bytes);

            pool.show_status_caching();

            // | Mem1 |
            // | Mem2 |
            // | Mem3 |
            // | Mem4 |
            // | Mem5 |
            // | rest |
            int mem5_bytes = 1 << 18; // 256KB
            auto *mem5 = pool.alloc(mem5_bytes);
            mem5->set_required_bytes(mem5_bytes);

            pool.show_status_caching();

            int large1_bytes = (1 << 20) + 1000; // 1MB + 1000B
            auto *large1_mem = pool.alloc(large1_bytes);
            large1_mem->set_required_bytes(large1_bytes);

            pool.show_status_caching();

            int mem6_bytes = 100000;
            auto *mem6 = pool.alloc(mem6_bytes);
            mem6->set_required_bytes(mem6_bytes);

            pool.show_status_caching();

            int mem7_bytes = 100000;
            auto *mem7 = pool.alloc(mem7_bytes);
            mem7->set_required_bytes(mem7_bytes);

            pool.show_status_caching();

            // ここポイント. small_cache or large_cache どっちからメモリを借用するのか... -> _SMALL_ALLOCより小さい限りsmall_cache
            int memA_bytes = 100000;
            auto *memA = pool.alloc(memA_bytes);
            memA->set_required_bytes(memA_bytes);

            pool.show_status_caching();

            int large2_bytes = (1 << 21); // 2MB
            auto *large2_mem = pool.alloc(large2_bytes);
            large2_mem->set_required_bytes(large2_bytes);

            pool.show_status_caching();

            // lock
            {
                std::printf("===== locking =====\n");
                // lock Mem5
                mem5->lock();
                // lock MemA
                memA->lock();
                // lock large1_mem
                large1_mem->lock();

                pool.show_status_caching();
            }

            // info
            {
                int index = 1;
                std::printf("########## Small ############\n");
                for (auto &head : pool.get_small_cache_chain())
                {
                    index = 1;
                    Memory *mem = head;
                    while (mem->next())
                    {
                        std::printf("[mem%d] prev: %p, this: %p(%lu), next: %p\n",
                                    index++, mem1->prev(), mem1, mem1->bytes(), mem1->next());
                        mem = mem->next();
                    }

                    std::printf("[mem%d] prev: %p, this: %p(%lu), next: %p\n",
                                mem ? -1 : 1, mem1->prev(), mem1, mem1->bytes(), mem1->next());
                }

                std::printf("########## Large ############\n");
                for (auto &head : pool.get_large_cache_chain())
                {
                    index = 1;
                    Memory *mem = head;
                    while (mem->next())
                    {
                        std::printf("[mem%d] prev: %p, this: %p(%lu), next: %p\n",
                                    index++, mem1->prev(), mem1, mem1->bytes(), mem1->next());
                        mem = mem->next();
                    }

                    std::printf("[mem%d] prev: %p, this: %p(%lu), next: %p\n",
                                mem ? -1 : 1, mem1->prev(), mem1, mem1->bytes(), mem1->next());
                }
            }

            // release & free
            {
                std::printf("===== release & free =====\n");
                // free Mem3
                pool.free(mem3);
                // free large2_mem
                // pool.free(large2_mem);

                pool.show_status_caching();
            }

            // info
            {
                int index = 1;
                std::printf("########## Small ############\n");
                for (auto &head : pool.get_small_cache_chain())
                {
                    index = 1;
                    Memory *mem = head;
                    while (mem->next())
                    {
                        std::printf("[mem%d] prev: %p, this: %p(%lu), next: %p\n",
                                    index++, mem->prev(), mem, mem->bytes(), mem->next());
                        mem = mem->next();
                    }

                    std::printf("[mem%d] prev: %p, this: %p(%lu), next: %p\n",
                                mem ? -1 : 1, mem->prev(), mem, mem->bytes(), mem->next());
                }

                std::printf("########## Large ############\n");
                for (auto &head : pool.get_large_cache_chain())
                {
                    index = 1;
                    Memory *mem = head;
                    while (mem->next())
                    {
                        std::printf("[mem%d] prev: %p, this: %p(%lu), next: %p\n",
                                    index++, mem->prev(), mem, mem->bytes(), mem->next());
                        mem = mem->next();
                    }

                    std::printf("[mem%d] prev: %p, this: %p(%lu), next: %p\n",
                                mem ? -1 : 1, mem->prev(), mem, mem->bytes(), mem->next());
                }
            }

            // // try freeing mem1 (but failed)
            // {
            //     std::printf("===== freeing Mem1 (but failed) =====\n");
            //     // free Mem1
            //     pool.free(mem1);

            //     pool.show_status_caching();
            // }

            // // all freeing mem-list
            // {
            //     std::printf("===== all freeing mem-list =====\n");
            //     for (auto& head : pool.get_small_cache_chain())
            //     {
            //         Memory *mem = head;
            //         mem->release();
            //         while (mem->next())
            //         {
            //             mem->release();
            //             mem = mem->next();
            //         }
            //         mem->release();
            //     }
            //     for (auto& head : pool.get_large_cache_chain())
            //     {
            //         Memory *mem = head;
            //         mem->release();
            //         while (mem->next())
            //         {
            //             mem->release();
            //             mem = mem->next();
            //         }
            //         mem->release();
            //     }

            //     pool.show_status_caching();

            //     pool.free(memA);

            //     pool.show_status_caching();

            //     pool.free(mem2); // mem2 -> mem1

            //     pool.show_status_caching();

            //     std::printf("mem1 ptr: %p\n", mem1->pointer());
            //     std::printf("mem2 ptr: %p\n", mem2->pointer());
            //     std::printf("mem1'next ptr: %p\n", mem1->next());
            //     std::printf("mem2'next ptr: %p\n", mem2->next());
            //     std::printf("mem1'prev ptr: %p\n", mem1->prev());
            //     std::printf("mem2'prev ptr: %p\n", mem2->prev());

            //     pool.free(mem2); // head mem

            //     pool.show_status_caching();
            // }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}