#include <stdint.h>
#define pwm_MAX 2756
#define pwm_MIN 1181

#define WP_SW_MASK (PWM_WPSR_WPSWS0 | PWM_WPSR_WPSWS1 | PWM_WPSR_WPSWS2 | \
                    PWM_WPSR_WPSWS3 | PWM_WPSR_WPSWS4 | PWM_WPSR_WPSWS5)
#define WP_HW_MASK (PWM_WPSR_WPHWS0 | PWM_WPSR_WPHWS1 | PWM_WPSR_WPHWS2 | \
                    PWM_WPSR_WPHWS3 | PWM_WPSR_WPHWS4 | PWM_WPSR_WPHWS5)

typedef enum {
    DIS = 0,
    EN = 1
} prt_cmd;
void disable_write_protection(prt_cmd cmd);
void init_pwm();
void change_pwm(uint8_t rawjoystickpos);