#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/vga.h>

void kernel_main(void) {
  char* logo = "   ,--,,\n"
               " _\"  ; ;          ___    ___\n"
               "/ .. _&           \\   \\  \\___\n"
               "\\___/  )  i n k g  \\___\\   ___\\\n"
               "      /\n"
               "\\____/\n";

  terminal_initialize();
  terminal_setcolor(vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_DARK_GREY));
  printf(logo);
  terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_DARK_GREY));
  printf("\nHello from the kernel\n");
}
