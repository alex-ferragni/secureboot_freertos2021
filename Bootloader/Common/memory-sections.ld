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

/* MCUBOOT Config */

INCLUDE port.ld
INCLUDE sections_config.ld

_lnImgContent = _lnImg - _lnImgHeader;

_enRAM = _stRAM + _lnRAM;

_stImgHeader = _stFlash + _lnBootloader;
_stImgContent = _stImgHeader + _lnImgHeader;

_stImg2Header = _stImgContent + _lnImgContent;
_stImg2Content = _stImg2Header + _lnImgHeader;

_lnUsrFlash = _lnFlash - 2*_lnImg - _lnBootloader - _lnScratch;

_stUsrFlash = _stImg2Content + _lnImgContent;
_stScratch = _stUsrFlash + _lnUsrFlash;

_lnBootloaderCode = _lnBootloader - _lnBootloaderStorage;
_stBootloaderCode = _stFlash;
_stBootloaderStorage = _stBootloaderCode + _lnBootloaderCode;

ASSERT(_lnBootloader % _lnFlashSector == 0, "_lnBootloader must be a multiple of the flash sector size");
ASSERT(_lnImg % _lnFlashSector == 0, "_lnImage must be a multiple of the flash sector size");
ASSERT(_lnScratch % _lnFlashSector == 0, "_lnScratch must be a multiple of the flash sector size");
ASSERT(_lnUsrFlash % _lnFlashSector == 0, "_lnUsrFlash must be a multiple of the flash sector size");
ASSERT(_lnFlash == 2*_lnImg + _lnBootloader + _lnScratch + _lnUsrFlash, "The size of the Bootloader + two images + scratch + user area does not correspond to the total flash size");

ASSERT(_stBootloaderCode < _stBootloaderStorage, "_stBootloaderCode should come before _stBootloaderStorage (the bootloader should start with code!");
ASSERT(_stBootloaderCode % _lnFlashSector == 0, "_stBootloaderCode must be aligned on a flash sector");
ASSERT(_stImgHeader % _lnFlashSector == 0, "_stImgHeader must be aligned on a flash sector");
ASSERT(_stImg2Header % _lnFlashSector == 0, "_stImg2Header must be aligned on a flash sector");
ASSERT(_stUsrFlash % _lnFlashSector == 0, "_stUsrFlash must be aligned on a flash sector");
ASSERT(_stScratch % _lnFlashSector == 0, "_stScratch must be aligned on a flash sector");

ASSERT(_stImgContent % _minVTORAlign == 0, "_stImgContent must be aligned with _minVTORAlign (vtor requirement)");
ASSERT(_stImg2Content % _minVTORAlign == 0, "_stImg2Content must be aligned with _minVTORAlign (vtor requirement)");
ASSERT(_stUsrFlash % _minVTORAlign == 0, "_stUsrFlash must be aligned with _minVTORAlign (vtor requirement, only for tests)");

ASSERT(_lnBootloaderStorage < _lnBootloader, "_lnBootloaderStorage must be smaller than _lnBootloader");

ASSERT(_lnImgHeader < _lnImg, "_lnImgHeader must be smaller than _lnImg");

MEMORY
{
	BOOTLOADER_CODE (rx) : ORIGIN = _stBootloaderCode, LENGTH = _lnBootloaderCode
	BOOTLOADER_STORAGE (x) : ORIGIN = _stBootloaderStorage, LENGTH = _lnBootloaderStorage
	IMG_HDR (rw) : ORIGIN = _stImgHeader, LENGTH = _lnImgHeader
	IMG (rxw) : ORIGIN = _stImgContent, LENGTH = _lnImgContent
	IMG2_HDR (rw) : ORIGIN = _stImg2Header, LENGTH = _lnImgHeader
	IMG2 (rxw) : ORIGIN = _stImg2Content, LENGTH = _lnImgContent
	FLASH (rw) : ORIGIN = _stUsrFlash, LENGTH = _lnUsrFlash
	SCRATCH (rw):	ORIGIN = _stScratch, LENGTH = _lnScratch
	RAM (rwx) :  ORIGIN = _stRAM, LENGTH = _lnRAM
}