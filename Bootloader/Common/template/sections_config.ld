/*
 *    Code Released under MIT License
 *
 *    STM32 port
 *    Copyright (c) 2021 CSEM SA
 *
 *    Interrupt fork
 *    Copyright (c) 2019 Memfault, Inc.
 * 
 *    Original Jekyll Template
 *    Copyright (c) 2015 Jacopo Rabolini
 * 
 *    Permission is hereby granted, free of charge, to any person obtaining a 
 *    copy of this software and associated documentation files (the "Software"), 
 *    to deal in the Software without restriction, including without limitation 
 *    the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 *    and/or sell copies of the Software, and to permit persons to whom the 
 *    Software is furnished to do so, subject to the following conditions:
 * 
 *    The above copyright notice and this permission notice shall be included 
 *    in all copies or substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 *    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 *    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 *    THE SOFTWARE.
 */

INCLUDE port.ld

_lnBootloader = 0x20000;

/* must be a multiple of 256 and at least 512 */
_lnBootloaderStorage = 512;

/* header + payload */
_lnImg = 0x60000;
/* header only */
_lnImgHeader = 0x200;

_lnScratch = 0x40000;

ASSERT(_lnBootloader % 256 == 0 && _lnBootloader >= 512, "_lnBootloader must be a multiple of 256 >= 512");

/* This simple version only sets secure area in first bank, if the bootloader does not fit inside the first bank,
   you have to extend the port to also set part of secure area in second bank (if that is feasible) first */
ASSERT(_lnBootloader <= _lnFlash / 2, "Bootloader does not fit in the first bank, the secure area cannot be set!")