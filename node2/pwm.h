#define pwm_MAX 2756
#define pwm_MIN 1181

typedef enum {
    DIS = 0,
    EN = 1
} prt_cmd;
void disable_write_protection(prt_cmd cmd);
void init_pwm();
void change_pwm(uint8_t joystickpos);