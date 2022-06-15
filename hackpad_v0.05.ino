#define PASSWORD "2020"
#define KEY_ENTER 0xB0
#define KEY_PRT_SC 0xCE

#include <Arduboy2.h>
#include "Keyboard.h"
#include "Mouse.h"
#include <avr/pgmspace.h>

// Make an instance of arduboy used for many functions
Arduboy2 arduboy;

byte menu_value = 0;     //主菜单索引，前两位用于标记页面位置，0=登录页面，1=主菜单页面，2=功能页面，后6位用于记录菜单值
//byte btn_pressed = 0;
uint8_t hot_keys[5] = {0, '0', '0', '0', '0'}; //快捷键缓存，第一位记录按键个数;后四位在登录页面用于存储密码输入
byte action_value = 0;//第一位标记按钮是否已经按下，后7位用于记录功能项的动作模式，通过B键切换
byte fn_menu_value = 0;//子菜单值，通过上下键切换
uint8_t key_value = '0';//keboard模式下的键值，password模式下'0'=用户名'1'=密码

//键盘功能动作模式标题
const char kb_act_title_0[] PROGMEM = "Click";
const char kb_act_title_1[] PROGMEM = "Hold";
const char kb_act_title_2[] PROGMEM = "Cache";
const char kb_act_title_3[] PROGMEM = "Send cache";
const char kb_act_title_4[] PROGMEM = "Clean cache";
const char kb_act_title_5[] PROGMEM = "Hotkey";
const char kb_act_title_6[] PROGMEM = "Send hotkey";
const char kb_act_title_7[] PROGMEM = "Clean hotkey";
const char kb_act_title_8[] PROGMEM = "Exit";
//鼠标功能动作模式标题
const char ms_act_title_0[] PROGMEM = "Left click";
const char ms_act_title_1[] PROGMEM = "Right click";
const char ms_act_title_2[] PROGMEM = "Middle click";
const char ms_act_title_3[] PROGMEM = "Wheel up";
const char ms_act_title_4[] PROGMEM = "Wheel down";
const char ms_act_title_5[] PROGMEM = "Left down";
const char ms_act_title_6[] PROGMEM = "Left up";
const char ms_act_title_7[] PROGMEM = "Right down";
const char ms_act_title_8[] PROGMEM = "Right up";
const char ms_act_title_9[] PROGMEM = "Middle down";
const char ms_act_title_10[] PROGMEM = "Middle up";
const char ms_act_title_11[] PROGMEM = "Exit";
//Password功能动作模式标题
const char pwd_act_title_0[] PROGMEM = "Send";
const char pwd_act_title_1[] PROGMEM = "Show";
const char pwd_act_title_2[] PROGMEM = "Enter";
const char pwd_act_title_3[] PROGMEM = "TAB";
const char pwd_act_title_4[] PROGMEM = "Backspace";
const char pwd_act_title_5[] PROGMEM = "Delete";
const char pwd_act_title_6[] PROGMEM = "Caps Lock";
const char pwd_act_title_7[] PROGMEM = "Up";
const char pwd_act_title_8[] PROGMEM = "Down";
const char pwd_act_title_9[] PROGMEM = "Left";
const char pwd_act_title_10[] PROGMEM = "Right";
const char pwd_act_title_11[] PROGMEM = "Exit";

//键盘功能菜单标题
const char kb_menu_title_0[] PROGMEM = "0~9";
const char kb_menu_title_1[] PROGMEM = "a~z";
const char kb_menu_title_2[] PROGMEM = "A~Z";

const char kb_menu_title_7[] PROGMEM = "F1~PRT_SC";
const char kb_menu_title_8[] PROGMEM = "CTRL~ALT";
const char kb_menu_title_9[] PROGMEM = "ENTER~TAB";
const char kb_menu_title_10[] PROGMEM = "RIGHT~UP";
const char kb_menu_title_11[] PROGMEM = "INSERT~PG_DN";
//Password功能菜单标题
const char pwd_menu_title_0[] PROGMEM = "Password 01";
const char pwd_menu_title_1[] PROGMEM = "Password 02";
const char pwd_menu_title_2[] PROGMEM = "Password 03";

//按键名称
const char kn_L_CTRL[] PROGMEM = "L_CTRL";
const char kn_L_SHIFT[] PROGMEM = "L_SHIFT";
const char kn_L_ALT[] PROGMEM = "L_ALT";
const char kn_L_GUI[] PROGMEM = "L_GUI";
const char kn_R_CTRL[] PROGMEM = "R_CTRL";
const char kn_R_SHIFT[] PROGMEM = "R_SHIFT";
const char kn_R_ALT[] PROGMEM = "R_ALT";
const char kn_R_GUI[] PROGMEM = "R_GUI";

const char kn_ENTER[] PROGMEM = "ENTER";
const char kn_ESC[] PROGMEM = "ESC";
const char kn_BACKSPACE[] PROGMEM = "BACKSPACE";
const char kn_TAB[] PROGMEM = "TAB";

const char kn_RIGHT[] PROGMEM = "RIGHT";
const char kn_LEFT[] PROGMEM = "LEFT";
const char kn_DOWN[] PROGMEM = "DOWN";
const char kn_UP[] PROGMEM = "UP";

const char kn_INSERT[] PROGMEM =  "INSERT";
const char kn_HOME[] PROGMEM =  "HOME";
const char kn_PG_UP[] PROGMEM =  "PG_UP";
const char kn_DELETE[] PROGMEM =  "DELETE";
const char kn_END[] PROGMEM =  "END";
const char kn_PG_DN[] PROGMEM =  "PG_DN";

//功能名称
const char fn_name_0[] PROGMEM = "Keyboard";
const char fn_name_1[] PROGMEM = "Mouse";
const char fn_name_2[] PROGMEM = "Password";
const char fn_name_3[] PROGMEM = "Hotkey";
const char fn_name_4[] PROGMEM = "Script";
const char fn_name_5[] PROGMEM = "Text";

//用户名和密码
const char pwd_uname_0[] PROGMEM = "uname1";
const char pwd_uname_1[] PROGMEM = "uname2";
const char pwd_uname_2[] PROGMEM = "uname3";
const char pwd_pwd_0[] PROGMEM = "password1";
const char pwd_pwd_1[] PROGMEM = "password2";
const char pwd_pwd_2[] PROGMEM = "password3";



//返回菜单值
byte get_menu_value() {
  byte tmp = menu_value << 2;
  return tmp >> 2;
}
//返回pos值
byte get_pos() {
  return menu_value >> 6;
}
//设置pos值
void set_pos(byte n) {
  menu_value = menu_value << 2;//清空前两位
  menu_value = menu_value >> 2;
  n = n << 6;
  menu_value = menu_value | n;
}
//获取按键执行值
byte get_btn_pressed() {
  return action_value >> 7;
}
//设置按键执行值,1=有按键已按过
void set_btn_pressed(byte n) {
  action_value = action_value << 1;//清空第一位
  action_value = action_value >> 1;
  n = n << 7;
  action_value = action_value | n;
}
//获取动作模式值
byte get_action_value() {
  byte tmp = action_value << 1;
  return tmp >> 1;
}
//action_value 归零
void init_action_value() {
  action_value = action_value >> 7;//归零
  action_value = action_value << 7;
}

//暂停，直到按键抬起
void hold_screen_by_btn() {
  //如果按键没抬起就暂停程序
  while (arduboy.buttonsState() != 0) {
  }
  set_btn_pressed(0);
}

//清空快捷键缓存，也是重置密码输入
void clean_hotkey() {
  hot_keys[0] = 0;
  hot_keys[1] = hot_keys[2] = hot_keys[3] = hot_keys[4] = '0';
}

//从flash读取字符串
String read_flash_str(const char *fstr) {
  char tmp;
  int len;
  int i;
  String rstr = "";
  len = strlen_P(fstr);
  for (i = 0; i < len; i++) {
    tmp =  pgm_read_byte_near(fstr + i);
    rstr = rstr + tmp;
  }
  return rstr;
}

//显示功能名称
void show_func_name() {
  if (get_pos() == 0) { //密码页
    arduboy.print(read_flash_str(fn_name_2));
  }
  if (get_pos() == 1) {//主菜单页
    arduboy.print(get_menu_value() + 1);
    arduboy.print('.');
  }
  if (get_pos() != 0) { //主菜单页和功能页
    if (get_menu_value() == 0) arduboy.print(read_flash_str(fn_name_0));
    if (get_menu_value() == 1) arduboy.print(read_flash_str(fn_name_1));
    if (get_menu_value() == 2) arduboy.print(read_flash_str(fn_name_2));
    if (get_menu_value() == 3) arduboy.print(read_flash_str(fn_name_3));
    if (get_menu_value() == 4) arduboy.print(read_flash_str(fn_name_4));
    if (get_menu_value() == 5) arduboy.print(read_flash_str(fn_name_5));
  }

  if (get_pos() != 1) {//密码页和功能页
    arduboy.print(':');
  }
}

//显示功能菜单标题
void show_menu_title() {
  //String kb_menu_title[] = {"0~9", "a~z", "A~Z", " ~/", ":~@", "[~`", "{~~", "F1~PRT_SC", "CTRL~ALT", "ENTER~TAB", "RIGHT~UP", "INSERT~PG_DOWN"};
  if (get_menu_value() == 0) { //键盘
    if (fn_menu_value == 0) arduboy.print(read_flash_str(kb_menu_title_0));
    if (fn_menu_value == 1) arduboy.print(read_flash_str(kb_menu_title_1));
    if (fn_menu_value == 2) arduboy.print(read_flash_str(kb_menu_title_2));

    if (fn_menu_value == 7) arduboy.print(read_flash_str(kb_menu_title_7));
    if (fn_menu_value == 8) arduboy.print(read_flash_str(kb_menu_title_8));
    if (fn_menu_value == 9) arduboy.print(read_flash_str(kb_menu_title_9));
    if (fn_menu_value == 10) arduboy.print(read_flash_str(kb_menu_title_10));
    if (fn_menu_value == 11) arduboy.print(read_flash_str(kb_menu_title_11));
  } else if (get_menu_value() == 2) { //password
    if (fn_menu_value == 0) arduboy.print(read_flash_str(pwd_menu_title_0));
    if (fn_menu_value == 1) arduboy.print(read_flash_str(pwd_menu_title_1));
    if (fn_menu_value == 2) arduboy.print(read_flash_str(pwd_menu_title_2));
  }
}

//显示操作模式
void show_action_title() {
  //String action_title[] = {"Click", "Hold","Cache", "Send cache","Hold cache", "Clean cache", "Hotkey","Send hotkey","Clean hotkey","Exit"};
  if (get_menu_value() == 0) { //键盘
    if (get_action_value() == 0) arduboy.print(read_flash_str(kb_act_title_0));
    if (get_action_value() == 1) arduboy.print(read_flash_str(kb_act_title_1));
    if (get_action_value() == 2) arduboy.print(read_flash_str(kb_act_title_2));
    if (get_action_value() == 3) arduboy.print(read_flash_str(kb_act_title_3));
    if (get_action_value() == 4) arduboy.print(read_flash_str(kb_act_title_4));
    if (get_action_value() == 5) arduboy.print(read_flash_str(kb_act_title_5));
    if (get_action_value() == 6) arduboy.print(read_flash_str(kb_act_title_6));
    if (get_action_value() == 7) arduboy.print(read_flash_str(kb_act_title_7));
    if (get_action_value() == 8) arduboy.print(read_flash_str(kb_act_title_8));
  } else if (get_menu_value() == 1) { //鼠标
    if (get_action_value() == 0) arduboy.print(read_flash_str(ms_act_title_0));
    if (get_action_value() == 1) arduboy.print(read_flash_str(ms_act_title_1));
    if (get_action_value() == 2) arduboy.print(read_flash_str(ms_act_title_2));
    if (get_action_value() == 3) arduboy.print(read_flash_str(ms_act_title_3));
    if (get_action_value() == 4) arduboy.print(read_flash_str(ms_act_title_4));
    if (get_action_value() == 5) arduboy.print(read_flash_str(ms_act_title_5));
    if (get_action_value() == 6) arduboy.print(read_flash_str(ms_act_title_6));
    if (get_action_value() == 7) arduboy.print(read_flash_str(ms_act_title_7));
    if (get_action_value() == 8) arduboy.print(read_flash_str(ms_act_title_8));
    if (get_action_value() == 9) arduboy.print(read_flash_str(ms_act_title_9));
    if (get_action_value() == 10) arduboy.print(read_flash_str(ms_act_title_10));
    if (get_action_value() == 11) arduboy.print(read_flash_str(ms_act_title_11));
  } else if (get_menu_value() == 2) { //password
    if (get_action_value() == 0) arduboy.print(read_flash_str(pwd_act_title_0));
    if (get_action_value() == 1) arduboy.print(read_flash_str(pwd_act_title_1));
    if (get_action_value() == 2) arduboy.print(read_flash_str(pwd_act_title_2));
    if (get_action_value() == 3) arduboy.print(read_flash_str(pwd_act_title_3));
    if (get_action_value() == 4) arduboy.print(read_flash_str(pwd_act_title_4));
    if (get_action_value() == 5) arduboy.print(read_flash_str(pwd_act_title_5));
    if (get_action_value() == 6) arduboy.print(read_flash_str(pwd_act_title_6));
    if (get_action_value() == 7) arduboy.print(read_flash_str(pwd_act_title_7));
    if (get_action_value() == 8) arduboy.print(read_flash_str(pwd_act_title_8));
    if (get_action_value() == 9) arduboy.print(read_flash_str(pwd_act_title_9));
    if (get_action_value() == 10) arduboy.print(read_flash_str(pwd_act_title_10));
    if (get_action_value() == 11) arduboy.print(read_flash_str(pwd_act_title_11));
  }
}

/** 运行开始 **/
// This function runs once in your game.
// use it for anything that needs to be set only once in your game.
void setup() {
  // initiate arduboy instance
  arduboy.begin();

  // here we set the framerate to 30, we do not need to run at default 60 and
  // it saves us battery life.
  arduboy.setFrameRate(30);

}

// our main game loop, this runs once every cycle/frame.
// this is where our game logic goes.
void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;
  if (get_pos() == 0) {
    login_screen();
  } else if (get_pos() == 1) {
    main_screen();
  } else {
    do_func();
  }
  hold_screen_by_btn();
}

//执行功能
void do_func() {
  switch (get_menu_value()) {
    case 0:
      func_keyboard();
      break;
    case 1:
      func_mouse();
      break;
    case 2:
      func_password();
      break;
    default:
      break;
  }
}
/** 运行开始 end **/

/** Password 功能 **/
//获取用户名
String pwd_get_uname() {
  if (fn_menu_value == 0) return read_flash_str(pwd_uname_0);
  if (fn_menu_value == 1) return read_flash_str(pwd_uname_1);
  if (fn_menu_value == 2) return read_flash_str(pwd_uname_2);
}
//获取密码
String pwd_get_password() {
  if (fn_menu_value == 0) return read_flash_str(pwd_pwd_0);
  if (fn_menu_value == 1) return read_flash_str(pwd_pwd_1);
  if (fn_menu_value == 2) return read_flash_str(pwd_pwd_2);
}
//切换用户名和密码
void pwd_flip_key_value() {
  if (key_value == '0') {
    key_value = '1';
  } else {
    key_value = '0';
  }
}
//发送按键
void pwd_press_key(uint8_t key_value) {
  Keyboard.begin();
  Keyboard.press(key_value);
  delay(100);
  Keyboard.releaseAll();
  Keyboard.end();
}
//发送字符串
void pwd_key_send(String str) {
  Keyboard.begin();
  Keyboard.print(str);
  delay(100);
  Keyboard.end();
}

//密码本功能
void func_password() {
  //左键
  if (arduboy.pressed(LEFT_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    pwd_flip_key_value();
  }
  //右键
  if (arduboy.pressed(RIGHT_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    pwd_flip_key_value();
  }
  //切换功能菜单，值减少
  if (arduboy.pressed(UP_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    if (fn_menu_value == 0) {
      fn_menu_value = 2;
    } else {
      fn_menu_value--;
    }
  }
  //切换功能菜单，值增加
  if (arduboy.pressed(DOWN_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    if (fn_menu_value == 2) {
      fn_menu_value = 0;
    } else {
      fn_menu_value++;
    }
  }

  //A键，执行动作
  if (arduboy.pressed(A_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    switch (get_action_value()) {
      case 0://Send
        if (key_value == '0') { //发送用户名
          pwd_key_send(pwd_get_uname());
        } else { //发送密码
          pwd_key_send(pwd_get_password());
        }
        break;
      case 1://Show
        break;
      case 2://Enter
        pwd_press_key(KEY_ENTER);
        break;
      case 3://TAB
        pwd_press_key(KEY_TAB);
        break;
      case 4://Backspace
        pwd_press_key(KEY_ESC);
        break;
      case 5://Delete
        pwd_press_key(KEY_DELETE);
        break;
      case 6://Caps Lock
        pwd_press_key(KEY_CAPS_LOCK);
        break;
      case 7://Up
        pwd_press_key(KEY_UP_ARROW);
        break;
      case 8://Down
        pwd_press_key(KEY_DOWN_ARROW);
        break;
      case 9://Left
        pwd_press_key(KEY_LEFT_ARROW);
        break;
      case 10://Right
        pwd_press_key(KEY_RIGHT_ARROW);
        break;
      case 11://exit
        set_pos(1);//退回主菜单页面
        init_action_value();//归0
        fn_menu_value = 0;//功能项菜单归0
        key_value = '0';
        break;
      default:
        break;
    }


  }
  //B键，切换动作模式
  if (arduboy.pressed(B_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    if (get_action_value() == 11) {
      init_action_value();//归0
    } else {
      action_value++;
    }
  }

  arduboy.clear();

  //显示功能模式
  arduboy.setCursor(0, 0);
  show_func_name();
  show_action_title();
  arduboy.print("\n");
  //显示标题
  show_menu_title();
  arduboy.print("\n");
  //显示输入的是用户名还是密码
  arduboy.print("Input:");
  if (key_value == '0') {
    arduboy.print("User name");
  } else {
    arduboy.print("Password");
  }
  //显示明文
  arduboy.print("\nShow:");
  if (get_action_value() == 1 && arduboy.pressed(A_BUTTON)) {
    if (key_value == '0') {
      arduboy.print(pwd_get_uname());
    } else {
      arduboy.print(pwd_get_password());
    }
  }

  arduboy.display();
}
/** Password 功能 end **/

/** 模拟鼠标 **/
//移动鼠标指针
void ms_move_cur(int x, int y, int wheel) {
  while (arduboy.buttonsState() != 0) {
    Mouse.move(x, y, wheel);
    delay(50);
    if (x < 0 && x > -32768) x--;
    if (x > 0 && x < 32767) x++;
    if (y < 0 && y > -32768) y--;
    if (y > 0 && y < 32767) y++;
    if (wheel < 0 && wheel > -32768) y--;
    if (wheel > 0 && wheel < 32767) y++;
  }
}

//鼠标按键点击
void ms_click(uint8_t btn) {
  Mouse.press(btn);
  delay(100);
  Mouse.release(btn);
}

//鼠标功能
void func_mouse() {
  //static byte action_value = 0;//通过B键切换
  if (arduboy.pressed(LEFT_BUTTON) && get_btn_pressed() == 0) {
    //btn_pressed = 1;
    set_btn_pressed(1);
    ms_move_cur(-1, 0, 0);
  }
  if (arduboy.pressed(RIGHT_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    ms_move_cur(1, 0, 0);
  }
  if (arduboy.pressed(UP_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    ms_move_cur(0, -1, 0);
  }
  if (arduboy.pressed(DOWN_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    ms_move_cur(0, 1, 0);
  }
  if (arduboy.pressed(A_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    switch (get_action_value()) {
      case 0://left click
        ms_click(MOUSE_LEFT);
        break;
      case 1://right click
        ms_click(MOUSE_RIGHT);
        break;
      case 2://middle click
        ms_click(MOUSE_MIDDLE);
        break;
      case 3://wheel up
        ms_move_cur(0, 0, 1);
        break;
      case 4://wheel down
        ms_move_cur(0, 0, -1);
        break;
      case 5://left down
        Mouse.press(MOUSE_LEFT);
        break;
      case 6://left up
        Mouse.release(MOUSE_LEFT);
        break;
      case 7://right down
        Mouse.press(MOUSE_RIGHT);
        break;
      case 8://right up
        Mouse.release(MOUSE_RIGHT);
        break;
      case 9://middle down
        Mouse.press(MOUSE_MIDDLE);
        break;
      case 10://middle up
        Mouse.release(MOUSE_MIDDLE);
        break;
      case 11://exit
        set_pos(1);
        init_action_value();//归0
        Mouse.release(MOUSE_LEFT);
        Mouse.release(MOUSE_RIGHT);
        Mouse.release(MOUSE_MIDDLE);
        Mouse.end();
        break;
      default:
        break;
    }
  }
  if (arduboy.pressed(B_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    if (get_action_value() == 11) {
      init_action_value();//归0
    } else {
      action_value++;
    }
  }

  arduboy.clear();

  //显示功能模式
  arduboy.setCursor(0, 0);
  show_func_name();
  show_action_title();
  arduboy.print("\n");

  arduboy.display();
}
/** 模拟鼠标 end**/

/** keboard 功能 **/
//根据子菜单返回键值范围最小值
uint8_t kb_get_key_min() {
  if (fn_menu_value == 0) return uint8_t('0');
  if (fn_menu_value == 1) return uint8_t('a');
  if (fn_menu_value == 2) return uint8_t('A');
  if (fn_menu_value == 3) return uint8_t(' ');
  if (fn_menu_value == 4) return uint8_t(':');
  if (fn_menu_value == 5) return uint8_t('[');
  if (fn_menu_value == 6) return uint8_t('{');
  if (fn_menu_value == 7) return KEY_F1;
  if (fn_menu_value == 8) return KEY_LEFT_CTRL;
  if (fn_menu_value == 9) return KEY_ENTER;
  if (fn_menu_value == 10) return KEY_RIGHT_ARROW;
  if (fn_menu_value == 11) return KEY_INSERT;
}

//根据子菜单返回键值范围最大值
uint8_t kb_get_key_max() {
  if (fn_menu_value == 0) return uint8_t('9');
  if (fn_menu_value == 1) return uint8_t('z');
  if (fn_menu_value == 2) return uint8_t('Z');
  if (fn_menu_value == 3) return uint8_t('/');
  if (fn_menu_value == 4) return uint8_t('@');
  if (fn_menu_value == 5) return uint8_t('`');
  if (fn_menu_value == 6) return uint8_t('~');
  if (fn_menu_value == 7) return KEY_PRT_SC;
  if (fn_menu_value == 8) return KEY_RIGHT_GUI;
  if (fn_menu_value == 9) return KEY_TAB;
  if (fn_menu_value == 10) return KEY_UP_ARROW;
  if (fn_menu_value == 11) return KEY_PAGE_DOWN;
}

//根据子菜单值和键索引值，返回键名
String kb_key_name(uint8_t indx) {
  if (fn_menu_value == 7) {
    if (indx == 12) {
      return "PRT_SC";
    } else {
      return String("F") + (indx + 1);
    }
  }
  if (fn_menu_value == 8) { // 0x80~0x87
    if (indx == 0) return read_flash_str(kn_L_CTRL);
    if (indx == 1) return read_flash_str(kn_L_SHIFT);
    if (indx == 2) return read_flash_str(kn_L_ALT);
    if (indx == 3) return read_flash_str(kn_L_GUI);
    if (indx == 4) return read_flash_str(kn_R_CTRL);
    if (indx == 5) return read_flash_str(kn_R_SHIFT);
    if (indx == 6) return read_flash_str(kn_R_ALT);
    if (indx == 7) return read_flash_str(kn_R_GUI);
  }
  if (fn_menu_value == 9) { // 0xB0~0xB3
    if (indx == 0) return read_flash_str(kn_ENTER);
    if (indx == 1) return read_flash_str(kn_ESC);
    if (indx == 2) return read_flash_str(kn_BACKSPACE);
    if (indx == 3) return read_flash_str(kn_TAB);
  }
  if (fn_menu_value == 10) { // 0xD7~0xDA
    if (indx == 0) return read_flash_str(kn_RIGHT);
    if (indx == 1) return read_flash_str(kn_LEFT);
    if (indx == 2) return read_flash_str(kn_DOWN);
    if (indx == 3) return read_flash_str(kn_UP);
  }
  if (fn_menu_value == 11) { // 0xD1~0xD6
    if (indx == 0) return read_flash_str(kn_INSERT);
    if (indx == 1) return read_flash_str(kn_HOME);
    if (indx == 2) return read_flash_str(kn_PG_UP);
    if (indx == 3) return read_flash_str(kn_DELETE);
    if (indx == 4) return read_flash_str(kn_END);
    if (indx == 5) return read_flash_str(kn_PG_DN);
  }
}

//键盘功能
void func_keyboard() {
  //static uint8_t key_value = '0';//键值
  static String key_cache = "";//字符缓存
  static String hot_key_name = "";
  char tmp = ' ';

  //左按键,切换键值
  if (arduboy.pressed(LEFT_BUTTON) && get_btn_pressed() == 0) {
    //btn_pressed = 1;
    set_btn_pressed(1);
    if (key_value == kb_get_key_min()) {
      key_value = kb_get_key_max();
    } else {
      key_value--;
    }
  }
  //右按键,切换键值
  if (arduboy.pressed(RIGHT_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    if (key_value == kb_get_key_max()) {
      key_value = kb_get_key_min();
    }
    else {
      key_value++;
    }
  }

  //上键,切换菜单值
  if (arduboy.pressed(UP_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    if (fn_menu_value == 0) {
      fn_menu_value = 11;
    } else {
      fn_menu_value--;
    }
    key_value = kb_get_key_min();
  }

  //下键,切换菜单值
  if (arduboy.pressed(DOWN_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    if (fn_menu_value == 11) {
      fn_menu_value = 0;
    } else {
      fn_menu_value++;
    }
    key_value = kb_get_key_min();
  }

  // A键
  if (arduboy.pressed(A_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    switch (get_action_value()) {
      case 0: // click
        Keyboard.press(key_value);
        delay(100);
        Keyboard.releaseAll();
        break;
      case 1: //hold
        Keyboard.press(key_value);
        while (arduboy.buttonsState() != 0) {
        }
        Keyboard.releaseAll();
        break;
      case 2: // cache
        if (fn_menu_value < 7 && key_cache.length() < 21) {
          key_cache += char(key_value);
        }
        break;
      case 3: // send cache
        if (fn_menu_value < 7) {
          Keyboard.print(key_cache);
          //key_cache = "";
        }
        break;
      case 4: // clean cache
        key_cache = "";
        break;
      case 5: // hotkey
        if (hot_keys[0] < 4) {
          hot_keys[0]++;
          hot_keys[hot_keys[0]] = key_value;
          if (fn_menu_value < 7) {
            hot_key_name = hot_key_name + "+" + char(key_value);
          } else {
            hot_key_name = hot_key_name + "+" + kb_key_name(key_value - kb_get_key_min());
          }
        }
        break;
      case 6: // send hotkey
        if (hot_keys[0] > 0) {
          for (tmp = 1; tmp <= hot_keys[0]; tmp++) {
            Keyboard.press(hot_keys[tmp]);
          }
          delay(100);
          Keyboard.releaseAll();
        }
        break;
      case 7: // clean hotkey
        clean_hotkey();
        hot_key_name = "";
        break;
      case 8: // Exit
        set_pos(1);//退回主菜单页面
        init_action_value();//归0
        fn_menu_value = 0;//功能项菜单归0
        key_value = '0';//键值归‘0’
        key_cache = "";//清空按键缓存
        clean_hotkey();//情况快捷键缓存
        hot_key_name = "";//情况快捷键名称缓存
        Keyboard.releaseAll();
        Keyboard.end();
        break;
      default:
        break;
    }
  }

  // B键
  if (arduboy.pressed(B_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    if (get_action_value() == 8) {
      init_action_value();//归0
    } else {
      action_value++;
    }
  }

  arduboy.clear();

  //显示功能模式
  arduboy.setCursor(0, 0);
  show_func_name();
  show_action_title();
  arduboy.print("\n");
  //显示标题
  arduboy.print(fn_menu_value + 1);
  arduboy.print(":");
  if (fn_menu_value < 3 || fn_menu_value > 6) {
    show_menu_title();
  } else {
    for (tmp = kb_get_key_min(); tmp <= kb_get_key_max(); tmp++) {
      arduboy.print(tmp);
    }
  }
  arduboy.print("\n");
  //显示键名
  arduboy.print("Key:");
  if (fn_menu_value < 7) {
    arduboy.print(char(key_value));
  } else {
    arduboy.print(kb_key_name(key_value - kb_get_key_min()));
  }
  arduboy.print("_\n");
  //显示字符缓存区
  arduboy.print("Cache:\n");
  arduboy.print(key_cache);
  arduboy.print("\n");
  //显示热键缓存区
  arduboy.print("Hotkey cache:\n");
  if (hot_keys[0] > 0) {
    arduboy.print(hot_key_name);
  }


  // then we finaly we tell the arduboy to display what we just wrote to the display.
  arduboy.display();
}
/** keboard 功能 end **/


/** 登录 **/

//密码数字递增
void pwd_numup(byte n) {
  if (hot_keys[n] == '9') {
    hot_keys[n] = '0';
  } else {
    hot_keys[n]++;
  }
}
//登录界面
void login_screen() {
  //左键，第一位
  if (arduboy.pressed(LEFT_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    pwd_numup(1);
  }
  //上键，第二位
  if (arduboy.pressed(UP_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    pwd_numup(2);
  }
  //右键，第三位
  if (arduboy.pressed(RIGHT_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    pwd_numup(3);
  }
  //下键，第四位
  if (arduboy.pressed(DOWN_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    pwd_numup(4);
  }
  // A键，确认
  if (arduboy.pressed(A_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    //if (pwd_input == PASSWORD) {
    if ((String(char(hot_keys[1])) + char(hot_keys[2]) + char(hot_keys[3]) +  char(hot_keys[4])) == PASSWORD) {
      //pos = 1;
      set_pos(1);
    }
  }
  // B键，重置
  if (arduboy.pressed(B_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    clean_hotkey();//清空密码输入
  }

  // we clear our screen to black
  arduboy.clear();

  arduboy.setCursor(0, 0);
  show_func_name();
  arduboy.print((String(char(hot_keys[1])) + char(hot_keys[2]) + char(hot_keys[3]) +  char(hot_keys[4])));

  // then we finaly we tell the arduboy to display what we just wrote to the display.
  arduboy.display();
}
/** 登录 end **/

/** 主屏 **/
//主屏
void main_screen() {
  //左按钮
  if (arduboy.pressed(LEFT_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
  }
  //右按钮
  if (arduboy.pressed(RIGHT_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
  }

  //上键，变更menu_value值
  if (arduboy.pressed(UP_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    if (get_menu_value() == 0) {
      menu_value += 4;
    } else {
      menu_value--;
    }
  }

  //下键，变更menu_value值
  if (arduboy.pressed(DOWN_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    if (get_menu_value() == 5) {
      menu_value = menu_value >> 6;
      menu_value = menu_value << 6;
    } else {
      menu_value++;
    }
  }
  //A键
  if (arduboy.pressed(A_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    set_pos(2);//进入功能项
    init_action_value();//acttion_value 归零
    if (get_menu_value() == 0) {
      Keyboard.begin();
    }
    if (get_menu_value() == 1) {
      Mouse.begin();
    }
  }

  if (arduboy.pressed(B_BUTTON) && get_btn_pressed() == 0) {
    set_btn_pressed(1);
    set_pos(0);
    clean_hotkey();//清密码输入
  }

  // we clear our screen to black
  arduboy.clear();
  arduboy.setCursor(0, 0);
  //显示菜单名
  show_func_name();
  //显示功能解释
  arduboy.print("\n");
  if (get_menu_value() == 0) arduboy.print("As a keyboard");
  if (get_menu_value() == 1) arduboy.print("As a mouse");
  if (get_menu_value() == 2) arduboy.print("Password book");
  if (get_menu_value() == 3) arduboy.print("Some hotkeys");
  if (get_menu_value() == 4) arduboy.print("Some text");
  //显示操作说明
  arduboy.print("\n\nUp & Dn:Change menu;\nA:OK;\nB:Exit");

  // then we finaly we tell the arduboy to display what we just wrote to the display.
  arduboy.display();
}
/** 主屏 end **/
