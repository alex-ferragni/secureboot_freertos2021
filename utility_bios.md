# Utility BIOS

The utility BIOS comes as a handy debug tool that can be used to interract with in the bootloader via UART, even when debugging features are not available (e.g., when the board protections are enabled). It can also be a practical tool for demonstration purposes.

This tool can be included in the bootloader. In particular, it is sufficient to add `ADDITIONAL_CFLAGS += -DENABLE_BIOS` in your `bootloader_config.mk` to fully include the bios, which will run only if `$(PATH_PORT)/Includes/drivers/gpio.h:GET_USER_INPUT_STATE() != 0` when the device restarts.

It should however be noted that the utility BIOS introduce vulnerabilities in the design. It is possible to entirely bypass the secure boot process, so it should only be used during development and should ___never___ appear in a deployment version of the bootloader.

## Including the BIOS in an image

The utility BIOS can also be used in images. This time, it is completely safe to include it in all images, even in production version, as it does not break the threat model (it would still execute in an image, only if the bootloader validated it).

To include the BIOS in an image, you first need to add `ADDITIONAL_CFLAGS += -DENABLE_BIOS` to the image makefile, like you would do for the bootloader. However, the files required by the BIOS are not included by default in the image makefile, so you will also need to add some files to this makefile. Namely, the list of required files is the following (to be added to `SRC_FILES`):

* `$(PATH_MCUBOOT)/boot/bootutil/src/bootutil_public.`
* `Bootloader/Common/src/bios.c`
* `Bootloader/Common/src/image_util/image_util.c`
* `Bootloader/Common/src/image_util/image_download.c`
* `Bootloader/Common/src/image_util/image_upload.c`
* `$(PATH_PORT)/src/bios_port/bios.c`
* `$(PATH_PORT)/src/drivers/security.c`

Additionally, you may need to add all necessary include paths to `INCLUDE_PATHS`, if they are not yet included:

* `$(PATH_MCUBOOT)/boot/bootutil/include`
* `Bootloader/Common/Includes`
* `$(PATH_PORT)/Includes`

Finally, you can call `bios_start()`in your image wherever you want the utility BIOS to start.

## Features

The utility BIOS communicates via UART; it prints information and waits for inputs. It expects single-character inputs, without terminating characters. The characters used to execute commands are displayed by the BIOS itself. Here is the complete list of possible actions that can be done by the utility BIOS:

* Print the start of the header of the primary (or secondary) slot and the start of the content of the primary (or secondary) slot (i.e., starting from the interrupt vector table). This can be useful to monitor that an image is installed or not, encrypted or not and to check its header information.
* Upload an encrypted image via UART to the device and store it in the secondary slot. See [Image download / upload protocol](#image-download-upload).
* Download the complete content of the primary (or secondary) slot. See [Image download / upload protocol](#image-download-upload). Can be useful for debugging when peeking into the content of an image is not enough.
* Mark the image in secondary slot (if any) for update so that the bootloader can apply it.
* Force boot from image in primary slot. This is useful especially when located in the bootloader, in case MCUboot fails consistently, in which case we may still want to execute the image for debugging purposes.
* Restart the board
* Close the BIOS and continue the execution of the program
* Read the content of the bootloader's secreat area (where the private key is embedded). This can be useful to monitor the currently installed key (when executed from the bootloader) or to validate that an image cannot access it (when executed from an image).
* A port-specific print, meant to monitor the status of board protections. On STM32H753, display the current state of the secure mode, secure area and readout protection.
* An entire port-specific menu, meant to configure the board protections. On STM32H753, can set the secure mode, set the secure area, enable RDP1 or disable the board security (while wiping the flash memory).

## Image download / upload

### Principle

It may be desirable to download images from the board, or to upload images to the board, for examples for debugging purposes, or to allow an image to update itself by downloading an uptade. For this reason, two scripts have been included, to communicate with the board and upload or download images.

Since the device will have to read or write flash memory, it may be a problem if the communication is faster that the flash; data may be lost during the process. This is a serious issue since files corrupting the integrity of an image will automatically cause it to malfunction. For this purpose, a upload and downlaod protocol has been used to send or receive images via UART.

To cope with the fact that communication should not be faster that flash memory, we will exploit the fact that the device should have a UART buffer. Using it. Indeed, we can safely transfer images if the communication wait for flash instructions to complete.

Let us take the example of uploading an image to the device. Here, we simply require that the image sends a RDY signal (in our case, a `:` will do fine) between each flash word, so that the sender of the image waits for a confirmation before sending the next flash word. This way, both sides are synchronized even when using a minimal blocking UART driver and when flash operations are slow.

Similarly, while downlaoding an image from the board, the device will wait for an RDY signal before sending the next flash word, so that the recipient has enough time to store data. 

protocol to upload to board: image waits for image size (4 bytes) (little-endian), script sends the size, then for the entire size of the image, image waits for 32 bytes of the image, script sends 32 bytes, until done

### Usage

The protocol to download and upload images is implemented in the test helper image and automatically synchronizes with the test script. The utility BIOS also has this feature. To download an image from the device using the BIOS, you should first issue a download command in the BIOS. the BIOS will then wait for the download script. You should then run `python3 Bootloader/scripts/image_download.py [output binary]`, which will fully download the image.

Similarly, to upload an image to the device, you should first issue the corresponding command in the BIOS (e.g., upload an image to the secondary slot). Then, run `python3 Bootloader/scripts/image_upload.py [input image binary]`. Note that if the image is sent to the primary slot, it should be signed (i.e., `$(BUILD_DIR)/$(TARGET).bin`), whereas if the image is sent to the secondary slot, it should also be encrypted (i.e., `$(BUILD_DIR)/$(TARGET)_enc.bin`).
