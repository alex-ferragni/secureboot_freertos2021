# Testing

Ideally, we want to fully test the bootloader with the board locked, to ensure that it works in a real situation. The problem is that when the board is locked, we cannot influence it properly from the outside (e.g., Flash is completely inaccessible form the outside, debugging features might be disabled, etc.). It becomes difficult to flash images, check variable states with a debugger, execute arbitrary test functions etc. Including any way to do this in a production build would break the threat model by adding critical vulnerabilities anyway. We can therefore not fully test on the device a production version of the bootloader.

A possible solution for this is to control the board directly from within, by _asking_ the bootloader to do it. Therefore, we defined a test variable `ENABLE_TEST` that enable such behaviour directly from the inside of the bootloader only when it is defined. In this case, the bootloader will use UART communication to take orders; it will halt upon boot and wait for instructions such that it is possible to influence its execution from the outside, observe the resulting prints and decide whether a test is passed or failed, and repeat for the following tests by giving subsequent orders. We allow a minimal degree of debugging by including such a test menu in multiple places in the bootloader, each one with a specific identifier indicating which part of the code is currently being executed and each one waiting for orders from UART.

Unfortunately, too many of the commands that we need should not be part of the bootloader to begin with. For this reason, we decide to pollute the bootloader as little as possible during tests and delegate such commands to a test helper software. Since the tests will likely modify the content of both primary, secondary and scratch area, we store the test helper in the remaining area; the user area. Therefore, if the tests are enabled, the bootloader includes a command to jump directly to the test image. This test image will contain many commands to download and program images in primary or secondary slot, erase some flash area, or trigger specific tests.

To automate the test process, a python test script (`Bootloader/scripts/test_main.py`) is used to communicate with the device. It will connect to the UART canal and automatically launch every test. For every test, it will setup the state of the device (by erasing primary and secondary slots, or uploading images via UART as required by each test), execute the series of commands that define the test and print results. 

However, one should note that since tests are done by sending inputs to the bootloader, they will immediately fail if the bootloader crashes and the test script will most likely hang. A hang might also indicate a synchornization issue with the test script, if a test is defined incorrectly (e.g., each party waits for the other). The tests are thus here to provide more thourough tests, _once basic functionalities of the bootloader have been manually tested_ by validating that it can verify basic images, apply simple updates and download an image in secondary slot.

As mentioned earlier, this test framework will by definition break the threat model, so we cannot fully test on the board a definitive version of the bootloader. It is however relatively close, as changes to the bootloader are kept minimal and most of the changes are delegated to another software.

## Launching tests

To launch the tests, it is first necessary to install them. For that purpose, the bootloader's makefile defines an `install_tests` target (executed by launching `make install_tests`)which will compile the bootloader with the test flag and install it, compile the test helper and install it in user flash and compile some dummy valid and invalid images. 

At this point, the board will wait for instructions, so you should launch the test script. Make sure to free the UART canal if you are using it since the script will need to connect to the board. To execute it, simply launch `python3 scripts/test_main.py`. The script will thus wait for an input from the board to begin and you can simply start the tests by resetting the board, which will initiate the process.

Note that the utility BIOS is included in the bootloader during the tests by default in case you need to monitor the state of the device after a failure. It is especially important since the device will go through states in which no valid image exist in both primary and secondary slots, and it may be necessary to unlock the device directly from the bootloader, to avoid bricking it during tests.

## UART test interface

To communicate with the device in a satisfying way, the communication protocol (device-side) should happen in multiple programs. Indeed, the bootloader should communicate with the script, but so should the test helper image. Therefore, both will share the correspondng code.

To syncronize the test script and the device, the device will use multiple menus, each waiting for an input before proceeding and offering potentially different options, depending on its location. To clearly indicate that a test menu was reached (and to distinguish from usual logs, which might still be activated), a specific sequence of two control characters `$>` is sent, followed by a character which uniquely identifies the location of the menu. For example, the test menu in the test image will print `$>t` while the test menu when the bootloader refuses an image will print `$>x`.

The test script will then listen to UART, wait for a `$>` sequence and read the next char to identify the current location of the processor.

## Test utility commands

The test helper image posesses a certain number of commands:

* `c`: Corrupt the primary slot (on STM32H753, will cause a double ECC error, meaning an subsequent read will trigger a hardfault if the driver do not hande it).
* `d`: Corrupt the secondary slot (on STM32H753, will cause a double ECC error, meaning an subsequent read will trigger a hardfault if the driver do not hande it).
* `e`: Corrupt the scratch area (on STM32H753, will cause a double ECC error, meaning an subsequent read will trigger a hardfault if the driver do not hande it).
* `h`: Trigger a hardfault (good to know that the hardfault handler menu executes as expected); executes the failure menu afterwards if no hardfault was generated.
* `k`: Try to read the bootloader's secret key; executes the failure menu if anything other that all 0s is read, otherwise executes the success menu.
* `p`: Install an image in primary slot; downloads it from UART (uses the [image download protocol](utility_bios.md/#image-download-upload))
* `s`: Install an image in secondary slot; downloads it from UART (uses the [image download protocol](utility_bios.md/#image-download-upload))
* `u`: Mark the image in secondary slot for update
* `w`: Try to write to the bootloader area. To not corrupt anything, it will try to write same content as it previously read. Either the write succeeds, the content was not modified and the failure menu executes, or an error occurs (fault, or the flash returns an error).

Then, all other test menus have different commands, which are described case-by-case below. They have in common at least a couple of basic commands which allow the test script to start a new test at any point: 

* `r`, Reset the device
* `t`, Jump to the test helper image.

## Test menu identifiers

The complete list of different test menus is the following:

* `$>i`: Board initialization, called inside the bootloader right after driver initialization. Special commands: `c` (exit the menu, let MCUboot execute) and `b` (lauch bios, for manual use only, test script will not use it).
* `$>v`: Right after MCUboot, when the primary image was considered as valid.
* `$>x`: Right after MCUboot, when mcuboot refuses to boot from primary image (so both primary and secondary images are invalid).
* `$>m`: From within mcuboot, during an update. Shows up twice during an update, at 1/3 and 2/3 progress. Special command: `c` (exit the menu, let MCUboot continue the update).
* `$>h`: From test helper image's harfault handler. Good to recover from an expected hardfault (caused by a test command) without manual intervention.
* `$>f`: Generic test menu to indicate a failure.
* `$>s`: Generic test menu to indicate a success.
* `$>t`: Main test helper menu, executed at the start of the test helper image. Offers many commands necessary for some tests, described above.

## Running tests

The test script will use those commands to execute tests which are defined in the [Bootloader/scripts/test\_cases.py](Bootloader/scripts/test_cases.py) file. It will read tests defined in this file and execute them one by one, resetting the board between each test. Additionally, it will prepare the state of the device by uploading images in primary and secondary slot (as requested by the test) or erasing those slots.

To execute the tests, you first need to install the bootloader, modified to include a test menu. For this purpose, run `make install_tests`. The board will now wait for instructions. Then, run the test script: `python3 Bootloader/scripts/test_main.py`. The script will wait for any test menu identifier. Finally, reset the board to initiate the communication between both sides.

## Defining tests

The tests, defined in [Bootloader/scripts/test\_cases.py](Bootloader/scripts/test_cases.py), are composed of a few parameters, as follows:

* `test_name`, a string describing the name of the test.
* `primary_name`, the full name of the image that should be loaded in the primary slot, `None` if the slot should be empty.
* `secondary_name`, the full name of the image that should be loaded in the secondary slot, `None` if the slot should be empty.
* `modify_images`, a booloean indicating whether the test might corrupt the content of any image (and should thus be re-loaded for next test).
* `expected_sequence`, the sequence of commands and exected test menu identifiers that the device should go through, as defined below.

To encode a list of commands and the expected behavior (i.e., `expected_sequence`) that identify a test, we proceed as follows. First, one should note that an execution of a test will always be an alternance between two steps: the bootloader waits for an input at some test menu, and it receives a command and executes it untill the next menu. For this reason, we define a single action as a pair {issued command, identifier of the next menu reached}. Therefore, `expected_sequence` is represented by a list of actions, each action being a pair of characters, representing a command issued and a test menu identifier that should be received afterwards.

The test script will then follow this sequence, issuing commands and comparing the received test menu identifier against the expected one, abording and considering the test as failed at the first mismatch.

### Example 1

For example, consider a test that checks that hardfaults are thrown as expected.

`primary_name` and `secondary_name` are set to `None`, as we don't need any image for this test. `modify_images` is set to `False` since we won't modify them.

Let us now consider the command sequence. We expect all tests to start in the init menu. We should first jump to the test menu (`t`) and expect to reach the test menu (`$>t`), so the first command is `('tt')`. Then, we issue a hardfault command (`h`), and expect to land in the hardfault menu (`$>h`), so the second command is `('hh')`. So the final test sequence is `[('tt'),('hh')]`.

### Example 2

Another example is to test that a valid image can be installed from the secondary slot. We want to start with no valid image in the primary slot, so `primary_name` is `None` and `secondary_image` is set to a valid image (say, the full path to `hello_world_enc.bin`). `modify_images` is set to `True` as we will modify images.

Now, for the command sequence. We need to mark the image for update first, so the first step is to jump to the test image and issue a 'mark image' command: `[('tt'),('ut')]`. Next, we need to apply the update. We need to reset the board (`r`), and we will reach the init menu (`$>i`). Next, we can let MCUboot execute (`c`), and we should reach MCUboot first update menu (`$>m`). Let it continue (`c`) and we should reach the scond one (`$>m`). finally, let it continue (`c`), it should complete the update and verify the image, we expect the bootloader to consider the image as valid (`$>v`). So the final sequence will be: `[('tt'),('ut'),('ri'),('cm'),('cm'),('cv')]`.