/* write commands */
enum {UARTFS_RESET = 0, UARTFS_SELECT, UARTFS_OPEN, UARTFS_CLOSE, UARTFS_READ,
      UARTFS_WRITE, UARTFS_SEEK, UARTFS_TELL};

/* read commands */
enum {UARTFS_EOF = 0, UARTFS_ESCAPE};

/* seek commands */
enum {UARTFS_SEEK_SET = 1, UARTFS_SEEK_CUR, UARTFS_SEEK_END};

union offset {
   int32_t off;
   char data[4];
};
