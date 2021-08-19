/*
 *    Copyright 2021 CSEM SA
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
*/

#ifndef __DRIVERS_UTIL_H__
#define __DRIVERS_UTIL_H__

// Instruction Synchronization Barrier; flushes the pipeline in the processor,
// so that all instructions following the ISB are fetched from cache or memory,
// after the instruction has been completed.
__attribute__((always_inline)) static inline void ISB(void)
{
	__asm volatile ("isb 0xF":::"memory");
}

// Data Synchronization Barrier; acts as a special kind of Data Memory Barrier.
// It completes when all explicit memory accesses before this instruction completes.
__attribute__((always_inline)) static inline void DSB(void)
{
	__asm volatile ("dsb 0xF":::"memory");
}

#endif /* __DRIVERS_UTIL_H__ */