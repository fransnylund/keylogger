#include <stdio.h>
#include <X11/Xlib.h>
#include <string.h>

int main(void) {

	char new_keys[32] = {0};
	char old_keys[32] = {0};

	int byte;
	int bit;
	int keyCode;
	int keySym;
	char *key;

	int bitCheck;

	// Open display. Default name is :0
	Display *display = XOpenDisplay(":0");

	if (display == NULL) {
		fprintf(stderr, "ERROR: Could not open display\n");
		return 1;
	}

	while (1) {
		
		// Get the current state of the keyboard(which keys are pressed and not pressed) and store it as a bit vector in new_keys
		XQueryKeymap(display, new_keys);

		// Check if the new_keys array is NOT identical to the old_keys array
		if (memcmp(new_keys, old_keys, 32) != NULL) {
			
			for (byte = 0; byte < 32; byte++) {
				
				// Start is "00000001"
				bitCheck = 1;

				for (bit = 0; bit < 8; bit++) {

					// If the individual bit is "on" in the new_keys and "off" in the old_keys, then we know it has been pressed
					if ( (new_keys[byte] & bitCheck) && !(old_keys[byte] & bitCheck) ) {
						printf("Pressed\n");
					}
					bitCheck = bitCheck << 1;
				}
			}
		}
		memcpy(old_keys, new_keys, 32);


	}

	XCloseDisplay(display);
}
