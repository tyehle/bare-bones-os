.section .init
  /* apparently gcc will copy the contents of crtend.o's .init section here */
  popl %ebp
  ret

.section .fini
  /* again gcc puts the .fini section from crtend.o here */
  popl %ebp
  ret
