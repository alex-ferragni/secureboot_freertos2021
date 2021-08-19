# Porting the bootloader

## Platform prerequisites

To port the system to another device, you should first make sure that the new device is compatible with this design. To be compatible, it should fullfill the following requirements:

* Flash memory is required. All flash sectors should have the same size (the bootloader does not currently support different sector sizes, but that could work if it is modified). It should be possible to erase an entire flash sector, then write any of its words a single time each.
* It needs hardware mechanisms to make the bootloader area immutable, as well as having a subset of the bootloader unreadable once the bootloader is done executing.
* It also needs a hardware mechanism that can prevent any debugging feature, or any way to inspect internal memory or influence the execution of the bootloader from outside of the device.
* It needs some RAM (at the very least 16KB).
* It needs to have access to UART (this is not strictly necessary, since the bootloader should not use UART in a production version anyway, but it is very useful. You could also "simulate" UART by implementing another driver under the UART interface, if that works).
* Bonus: any simple way to interract with the board by manually giving a booloean input, for example with a button or a switch. This can help to only run the BIOS when necessary, instead of running it every time during development and having to exit the BIOS at every reboot.

## Port folder

Now, you need to implement the core of the port, which is your port folder. All functions, variables or macros used anywhere in the bootloader that may depend on the hardware are centralized in a single folder, which is the port folder. This means that there is a certain number of prototypes that are used in the project that you should implement.

For that purpose, a [template](Bootloader/Common/template/port_template) port is provided. This template contains the protoype of every single function, constant or macro that is required by the bootloader. Therefore, to port the bootloader, you should copy this folder somewhere else and implement everything that it contains. Note that you are not restricted to the definitions and files already present, this is a minimum set of what you need. 

A description of the expected behavior of prototypes in provided directly in the template port, in the form of comments. The port folder should thus be used as a porting reference guide.

If you decide to add other source files, they should be added in the `bootloader_config.mk`, `test_config.mk` files and in the image makefiles, as with all other port-specific values.

Note that it also contains a `stlink.cfg` file that should configure OpenOCD for your platform. It should be such that it is possible to successfully program the device by typing `openocd -f stlink.cfg -c "program [...]"`, which is exactly what the makefiles will do.

After this, it will directly be usable as any other template folder.

## Fork of MCUboot

Additionally, it is possible that MCUboot itself is not fully compatible with your device (as it was during the time of writing this project; it does not support STM32 flash). In this case, you will have to fork this MCUboot repository (the `mcuboot` submodule), and apply your changes. Note that changes were made in the `mcuboot` submodule which are custom-tailored to the bootloader, and inversely. This means that you should not fork the original MCUboot for that purpose, it will not contain the changes done in this submodule.

Once you forked it, you can simply indicate its location by setting the `PATH_MCUBOOT` variable in the `project_config.mk` file.