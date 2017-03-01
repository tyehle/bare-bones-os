#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/vga.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void) {
  terminal_row = 0;
  terminal_color = 0;
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_DARK_GREY);
  terminal_buffer = VGA_MEMORY;
  for(size_t y = 0; y < VGA_HEIGHT; y++) {
    for(size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y*VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
}

void terminal_setcolor(uint8_t color) {
  terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y*VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}

void scroll(void) {
  for(size_t y = 1; y < VGA_HEIGHT; y++) {
    // copy this row into the one above it
    for(size_t x = 0; x < VGA_WIDTH; x++) {
      size_t index = y*VGA_WIDTH + x;
      terminal_buffer[index - VGA_WIDTH] = terminal_buffer[index];
    }
  }

  // clear the last row
  for(size_t x = 0; x < VGA_WIDTH; x++) {
    size_t index = VGA_WIDTH*(VGA_HEIGHT-1) + x;
    terminal_buffer[index] = vga_entry(' ', terminal_color);
  }
}

void terminal_putchar(char c) {
  if(c == '\n') {
    // TODO: handle '\\r'?
    // write spaces so the colors are correct
    for(size_t x = terminal_column; x < VGA_WIDTH; x++) {
      terminal_putentryat(' ', terminal_color, x, terminal_row);
    }
    terminal_row += 1;
    terminal_column = 0;
  } else {
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    terminal_column += 1;
  }

  if(terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    terminal_row += 1;
  }

  if(terminal_row == VGA_HEIGHT) {
    scroll();
    terminal_row -= 1;
  }
}

void terminal_write(const char* data, size_t size) {
  for(size_t i = 0; i < size; i++) {
    terminal_putchar(data[i]);
  }
}

void terminal_writestring(const char* data) {
  terminal_write(data, strlen(data));
}
