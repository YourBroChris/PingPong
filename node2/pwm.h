

typedef enum {
    DIS = 0,
    EN = 1
} prt_cmd;
void disable_write_protection(prt_cmd cmd);
void init_pwm();
int change_pwm(uint8_t joystickpos);