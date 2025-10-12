typedef struct {
    unsigned short id;
    unsigned char length;
    char data[8];
} can_message


typedef enum {
    RESET = 0b11000000,
    READ = 0b00000011,
    WRITE = 0b00000010
} can_instruction