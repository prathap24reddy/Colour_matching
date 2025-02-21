#include <cstdlib>
#include <ctime>

// Define colour constants
#define BLUE    0x187F
#define RED     0xb800
#define YELLOW  0xf600
#define GREEN   0x07F0
#define BLACK   0x0000
#define TEST1   0x07fa
#define WHITE   0xFFFF
#define CYAN    0x07FF
#define MAGENTA 0xF81F

// Memory mapped I/O for keys
#define KEY_BASE 0xFF200050
volatile int * KEYS_ptr = (int *) KEY_BASE;
int keys;

// Global game variables
// Note: The array size is 30x30 but only part of it is used in the game.
int arr[30][30];
int delay = 6000;
int ct = 0;
int Score = 0;

// Delay function
void do_nothing(int delayVal) {
    while(delayVal--) {
        // empty loop to create delay
    }
}

// Write a pixel to the VGA display
void write_pixel(int x, int y, short colour) {
    volatile short *vga_addr = (volatile short *)(0x08000000 + (y << 10) + (x << 1));
    *vga_addr = colour;
}

// Write a character to the character buffer
void write_char(int x, int y, char c) {
    vvolatile char *character_buffer = (char *)(0x09000000 + (y << 7) + x);
    *character_buffer = c;
}

// Clear the pixel display (non‐character part)
void clear_screen_notchar() {
    for (int x = 0; x < 320; x++) {
        for (int y = 0; y < 240; y++) {
            write_pixel(x, y, BLACK);
        }
    }
}

// Clear the character screen
void clear_screen_char() {
    for (int x = 0; x < 80; x++) {
        for (int y = 0; y < 60; y++) {
            write_char(x, y, 0);
        }
    }
}

// Draw a brick (rectangle) centered at (cx, cy)
void drawbrick(int cx, int cy, short color) {
    int startX = cx - 8;
    int startY = cy - 5;
    for (int i = startX; i < startX + 16; i++) {
        for (int j = startY; j < startY + 10; j++) {
            write_pixel(i, j, color);
        }
    }
}

// Offsets for checking neighboring blocks
int dx[] = { -1, 1, 0, 0 };
int dy[] = { 0, 0, 1, -1 };

// Check if the position (x, y) is within range and matches value z in arr.
int tell(int x, int y, int z) {
    if (x < 0 || x >= 20 || y < 0 || y >= 20)
        return 0;
    if (z != arr[x][y])
        return 0;
    return 1;
}

// Recursive function to “pop” matching adjacent bricks.
void popup(int c, int k, int p) {
    for (int i = 0; i < 4; i++) {
        if (tell(c + dx[i], k + dy[i], p)) {
            drawbrick((c + dx[i]) * 16 + 8, (k + dy[i]) * 10 + 5, BLACK);
            delay = 100000;
            do_nothing(delay);
            arr[c + dx[i]][k + dy[i]] = 0;
            ct++;
            popup(c + dx[i], k + dy[i], p);
        }
    }
}

// Main game function
int play_game() {
    clear_screen_char();
    ct = 0;
    Score = 0;
    int p, ok = 1;
    
    // Initialize the blocks (brick grid)
    for (int i = 8; i < 320; i += 16) {
        for (int j = 5; j < 160; j += 10) {
            p = rand() % 5;
            short color;
            switch(p) {
                case 0: color = RED; break;
                case 1: color = BLUE; break;
                case 2: color = YELLOW; break;
                case 3: color = GREEN; break;
                default: color = CYAN; break;
            }
            drawbrick(i, j, color);
            arr[(i - 8) / 16][(j - 5) / 10] = p + 1;
        }
    }
    
    // Set initial slider brick color
    short colour1;
    p = rand() % 5;
    switch(p) {
        case 0: colour1 = RED; break;
        case 1: colour1 = BLUE; break;
        case 2: colour1 = YELLOW; break;
        case 3: colour1 = GREEN; break;
        default: colour1 = CYAN; break;
    }
    
    int c = 152;
    drawbrick(c, 235, colour1);
    
    while (true) {
        // Check a condition on a row (for example, if a brick reaches a limit)
        for (int i = 0; i < 20; i++) {
            if (arr[i][20] > 0) {
                ok = 1;
                break;
            }
        }
        
        keys = *(KEYS_ptr + 3);
        // Move slider left
        if (keys & 0x1) {
            *(KEYS_ptr + 3) = keys;
            drawbrick(c, 235, BLACK);
            c -= 16;
            c = (c + 320) % 320;
            drawbrick(c, 235, colour1);
            delay = 1000000;
            do_nothing(delay);
        }
        // Move slider right
        if (keys & 0x2) {
            *(KEYS_ptr + 3) = keys;
            drawbrick(c, 235, BLACK);
            c = c + 16;
            c = (c + 320) % 320;
            drawbrick(c, 235, colour1);
            delay = 1000000;
            do_nothing(delay);
        }
        // Fire the brick upward
        if (keys & 0x8) {
            *(KEYS_ptr + 3) = keys;
            ct = 0;
            int l = (c - 8) / 16;
            int k = 235;
            while ((k - 15) > 0 && arr[l][(k - 15) / 10] == 0) {
                drawbrick(c, k, BLACK);
                k -= 10;
                delay = 1000000;
                do_nothing(delay);
                drawbrick(c, k, colour1);
            }
            popup((c - 8) / 16, (k - 5) / 10, p + 1);
            if (ct) {
                drawbrick(c, k, BLACK);
            }
            int y = 0;
            // Note: corrected inner loop condition here (using j instead of i)
            for (int i = 0; i < 20; i++) {
                for (int j = 0; j < 20; j++) {
                    if (arr[i][j] > 0)
                        y = 1;
                }
            }
            if (y == 0) {
                ok = 0;
                break;
            }
            delay = 1000000;
            do_nothing(delay);
            Score += ct;
            
            // Set a new slider brick color and reset slider position
            p = rand() % 5;
            switch(p) {
                case 0: colour1 = RED; break;
                case 1: colour1 = BLUE; break;
                case 2: colour1 = YELLOW; break;
                case 3: colour1 = GREEN; break;
                default: colour1 = CYAN; break;
            }
            c = 152;
        }
    }
    return ok;
}

// Placeholder for the trial game
void trail_game() {
    // Implementation of trial game (if desired)
}

// Display the main page and wait for key selection
void main_page() {
    // Initialize the array of blocks to 0
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            arr[i][j] = 0;
        }
    }
    
    while (true) {
        clear_screen_notchar();
        const char* hw1 = "Welcome to the Block Shooter";
        const char* hw2 = "Press key 0 for trial game";
        const char* hw3 = "Press key 1 for main game";
        int x = 16;
        for (int i = 0; hw1[i] != '\0'; i++, x++) {
            write_char(x, 30, hw1[i]);
        }
        x = 30;
        for (int i = 0; hw2[i] != '\0'; i++, x++) {
            write_char(x, 30, hw2[i]);
        }
        x = 32;
        for (int i = 0; hw3[i] != '\0'; i++, x++) {
            write_char(x, 30, hw3[i]);
        }
        keys = *KEYS_ptr;
        if (keys & 0x1) {
            *KEYS_ptr = keys;
            trail_game();
            delay = 10000000;
            do_nothing(delay);
            break;
        }
        if (keys & 0x2) {
            *KEYS_ptr = keys;
            play_game();
            delay = 10000000;
            do_nothing(delay);
            break;
        }
    }
}

// Final page displayed when the game ends
void fin_page(int ok) {
    clear_screen_notchar();
    if (ok) {
        const char* hw = "You lost the game";
        int x = 30;
        for (int i = 0; hw[i] != '\0'; i++, x++) {
            write_char(x, 30, hw[i]);
        }
    } else {
        const char* hw = "You won the game";
        int x = 30;
        for (int i = 0; hw[i] != '\0'; i++, x++) {
            write_char(x, 30, hw[i]);
        }
    }
    const char* hw = "You will be taken back to the main page";
    int x = 27;
    for (int i = 0; hw[i] != '\0'; i++, x++) {
        write_char(x, 38, hw[i]);
    }
    main_page();
}

// Main entry point
int main() {
    
    clear_screen_notchar();
    main_page();
    int ok = play_game();
    // ok -> 1 indicates loss, 0 indicates win
    fin_page(ok);
    
    return 0;
}
