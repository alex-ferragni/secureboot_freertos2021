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

#ifdef ENABLE_BIOS

#include "bios_port/bios.h"

// Declaration of a submenu of the utility BIOS, specific to your device
uint32_t bios_system(){
	// Control the current submenu with done (0 = re-prompt, 1 = go back one level)
	// Control the parent menu with result (0 = continue with parent, 1 = completely exit bios)
	uint32_t result = BIOS_STAY_IN_SUBMENU;
	WHILE_NOT_DONE(response, done){
		DECLARE_COMMAND_INPUT('a', "Do nothing");
		DECLARE_COMMAND_INPUT('b', "Do something");
		DECLARE_COMMAND_INPUT('c', "Do neither");
		
		ASK_FOR_RESPONSE(response);
		switch(response){
			DECLARE_CASE('a'):
				// Exit current sub-menu, signal to parent to completely close the bios
				done = BIOS_EXIT_SUBMENU
				result = BIOS_EXIT_SUBMENU;
				break;

			DECLARE_CASE('b'):
				// Exit current sub-menu, keep the parent menu open
				done = BIOS_EXIT_SUBMENU;
				break;

			DECLARE_CASE('c'):
				// Stay in this sub-menu; re-prompt actions
				break;

			default:
				break;
		}
	}
	return result;
}

#endif /* ENABLE_BIOS */