#ifdef KEYBOARD_GAMEBOY
#define key_up 0xFE
#define key_down 0xFD
#define key_left 0xFB
#define key_right 0xF7
#define key_lp 0xBF
#define key_navigation 0x7F
#define key_wp 0xEF
#define key_wplist 0xDF
// not really existing
#define key_setting 0x5C
#define key_info 0x0x3D
#define key_m_reset 0x38
#endif

#ifdef KEYBOARD_CUSTOM
#define key_up 0x4D
#define key_down 0x35
#define key_left 0x37
#define key_right 0x39
#define key_lp 0x2F
#define key_navigation 0x2B
#define key_wp 0x2E
#define key_wplist 0x30
#define key_setting 0x5C
#define key_info 0x3D
#define key_m_reset 0x38
#endif




void get_data() {

  while (digitalRead(KEYBOARD_INT) == LOW) {
    Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);  // request 1 byte from keyboard
    
    while (Wire.available()) {
      //  Serial.print((char)key_val);
      uint8_t key_val = Wire.read();  // receive a byte as character
      Serial.print((char)key_val);
      //Serial.print(key_val);
      if (key_val != 0) {
        switch (key_val) {
          case key_up:
            button_up();
            break;
          case key_down:
            button_down();
            break;
          case key_left:
            button_left();
            break;
          case key_right:
            button_right();
            break;
          case key_lp:
            button_select();
            break;
          case key_navigation:
            button_navigation();
            break;
          case key_wp:
            button_a();
            break;
          case key_wplist:
            button_wplist();
            break;
          case key_setting:
            button_setting();
            break;
          case key_info:
            button_info();
            break;
          case key_m_reset:
            button_m_reset();
            break;
          default:
            break;
        }
      }
    }
  }
}
