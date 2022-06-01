#define PASSWORD "2020"
#define KEY_ENTER 0xB0
#define KEY_PRT_SC 0xCE

#include <Arduboy2.h>
#include "Keyboard.h"
#include "Mouse.h"
#include <avr/pgmspace.h>

// Make an instance of arduboy used for many functions
Arduboy2 arduboy;

byte menu_value = 0;     //主菜单索引
byte btn_pressed = 0;
//String pwd_input = "0000";
uint8_t hot_keys[5] = {0, '0', '0', '0', '0'}; //快捷键缓存，第一位记录按键个数;后四位同时用于存储密码输入
byte pos = 0;            //当前位置,0=未登录，1=主菜单，2=功能项
byte action_value = 0;//通过B键切换

//清空快捷键缓存，也是重置密码输入
void clean_hotkey() {
  hot_keys[0] = 0;
  hot_keys[1] = hot_keys[2] = hot_keys[3] = hot_keys[4] = '0';
}

// This function runs once in your game.
// use it for anything that needs to be set only once in your game.
void setup() {
  // initiate arduboy instance
  arduboy.begin();

  // here we set the framerate to 30, we do not need to run at default 60 and
  // it saves us battery life.
  arduboy.setFrameRate(30);

  // initialize control over the keyboard:
  // Keyboard.begin();
}

// our main game loop, this runs once every cycle/frame.
// this is where our game logic goes.
void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;
  if (pos == 0) {
    login_screen();
  } else if (pos == 1) {
    main_screen();
  } else {
    do_func();
  }
  hold_screen_by_btn();
}

//执行功能
void do_func() {
  switch (menu_value) {
    case 0:
      func_keyboard();
      break;
    case 1:
      func_mouse();
      break;
    default:
      break;
  }
}

//显示操作模式
void ms_action_title() {
  //String action_title[] = {"Click left", "Click right","Hold left", "Hold left","Exit"};
  if (action_value == 0) arduboy.print("Left click");
  if (action_value == 1) arduboy.print("Right click");
  if (action_value == 2) arduboy.print("Middle click");
  if (action_value == 3) arduboy.print("Wheel up");
  if (action_value == 4) arduboy.print("Wheel down");
  if (action_value == 5) arduboy.print("Left down");
  if (action_value == 6) arduboy.print("Left up");
  if (action_value == 7) arduboy.print("Right down");
  if (action_value == 8) arduboy.print("Right up");
  if (action_value == 9) arduboy.print("Middle down");
  if (action_value == 10) arduboy.print("Middle up");
  if (action_value == 11) arduboy.print("Exit");
}

//移动鼠标指针
void ms_move_cur(int x, int y, int wheel) {
  while (arduboy.buttonsState() != 0) {
    Mouse.move(x, y, wheel);
    delay(50);
    if (x < 0) x--;
    if (x > 0) x++;
    if (y < 0) y--;
    if (y > 0) y++;
    if (wheel < 0) y--;
    if (wheel > 0) y++;
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
  if (arduboy.pressed(LEFT_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    ms_move_cur(-1, 0, 0);
  }
  if (arduboy.pressed(RIGHT_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    ms_move_cur(1, 0, 0);
  }
  if (arduboy.pressed(UP_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    ms_move_cur(0, -1, 0);
  }
  if (arduboy.pressed(DOWN_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    ms_move_cur(0, 1, 0);
  }
  if (arduboy.pressed(A_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    switch (action_value) {
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
        pos = 1;
        action_value = 0;
        Mouse.release(MOUSE_LEFT);
        Mouse.release(MOUSE_RIGHT);
        Mouse.release(MOUSE_MIDDLE);
        Mouse.end();
        break;
      default:
        break;
    }
  }
  if (arduboy.pressed(B_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    if (action_value == 11) {
      action_value = 0;
    } else {
      action_value++;
    }
  }

  arduboy.clear();

  //显示功能模式
  arduboy.setCursor(0, 0);
  arduboy.print("Mouse:");
  ms_action_title();
  arduboy.print("\n");

  arduboy.display();
}

/** keboard 功能 **/
//根据子菜单返回键值范围最小值
uint8_t kb_get_key_min(byte n) {
  if (n == 0) return uint8_t('0');
  if (n == 1) return uint8_t('a');
  if (n == 2) return uint8_t('A');
  if (n == 3) return uint8_t(' ');
  if (n == 4) return uint8_t(':');
  if (n == 5) return uint8_t('[');
  if (n == 6) return uint8_t('{');
  if (n == 7) return KEY_F1;
  if (n == 8) return KEY_LEFT_CTRL;
  if (n == 9) return KEY_ENTER;
  if (n == 10) return KEY_RIGHT_ARROW;
  if (n == 11) return KEY_INSERT;
}

//根据子菜单返回键值范围最大值
uint8_t kb_get_key_max(byte n) {
  if (n == 0) return uint8_t('9');
  if (n == 1) return uint8_t('z');
  if (n == 2) return uint8_t('Z');
  if (n == 3) return uint8_t('/');
  if (n == 4) return uint8_t('@');
  if (n == 5) return uint8_t('`');
  if (n == 6) return uint8_t('~');
  if (n == 7) return KEY_PRT_SC;
  if (n == 8) return KEY_RIGHT_GUI;
  if (n == 9) return KEY_TAB;
  if (n == 10) return KEY_UP_ARROW;
  if (n == 11) return KEY_PAGE_DOWN;
}


//根据子菜单值和键索引值，返回键名
String kb_key_name(uint8_t indx, byte kb_menu_value) {
  if (kb_menu_value == 7) {
    if (indx == 12) {
      return "PRT_SC";
    } else {
      return String("F") + (indx + 1);
    }
  }
  if (kb_menu_value == 8) { // 0x80~0x87
    if (indx == 0) return "L_CTRL";
    if (indx == 1) return "L_SHIFT";
    if (indx == 2) return "L_ALT";
    if (indx == 3) return "L_GUI";
    if (indx == 4) return "R_CTRL";
    if (indx == 5) return "R_SHIFT";
    if (indx == 6) return "R_ALT";
    if (indx == 7) return "R_GUI";
  }
  if (kb_menu_value == 9) { // 0xB0~0xB3
    if (indx == 0) return "ENTER";
    if (indx == 1) return "ESC";
    if (indx == 2) return "BACKSPACE";
    if (indx == 3) return "TAB";
  }
  if (kb_menu_value == 10) { // 0xD7~0xDA
    if (indx == 0) return "RIGHT";
    if (indx == 1) return "LEFT";
    if (indx == 2) return "DOWN";
    if (indx == 3) return "UP";
  }
  if (kb_menu_value == 11) { // 0xD1~0xD6
    if (indx == 0) return "INSERT";
    if (indx == 1) return "HOME";
    if (indx == 2) return "PG_UP";
    if (indx == 3) return "DELETE";
    if (indx == 4) return "END";
    if (indx == 5) return "PG_DN";
  }
}

//显示操作模式
void kb_action_title(byte action_value) {
  //String action_title[] = {"Click", "Hold","Cache", "Send cache","Hold cache", "Clean cache", "Hotkey","Send hotkey","Clean hotkey","Exit"};
  if (action_value == 0) arduboy.print("Click");
  if (action_value == 1) arduboy.print("Hold");
  if (action_value == 2) arduboy.print("Cache");
  if (action_value == 3) arduboy.print("Send cache");
  if (action_value == 4) arduboy.print("Clean cache");
  if (action_value == 5) arduboy.print("Hotkey");
  if (action_value == 6) arduboy.print("Send hotkey");
  if (action_value == 7) arduboy.print("Clean hotkey");
  if (action_value == 8) arduboy.print("Exit");
}

//显示键盘功能菜单标题
void kb_menu_title(byte kb_menu_value) {
  //String kb_menu_title[] = {"0~9", "a~z", "A~Z", " ~/", ":~@", "[~`", "{~~", "F1~PRT_SC", "CTRL~ALT", "ENTER~TAB", "RIGHT~UP", "INSERT~PG_DOWN"};
  if (kb_menu_value == 0) arduboy.print("0~9");
  if (kb_menu_value == 1) arduboy.print("a~z");
  if (kb_menu_value == 2) arduboy.print("A~Z");

  if (kb_menu_value == 7) arduboy.print("F1~PRT_SC");
  if (kb_menu_value == 8) arduboy.print("CTRL~ALT");
  if (kb_menu_value == 9) arduboy.print("ENTER~TAB");
  if (kb_menu_value == 10) arduboy.print("RIGHT~UP");
  if (kb_menu_value == 11) arduboy.print("INSERT~PG_DN");
}



//键盘功能
void func_keyboard() {
  //static byte action_value = 0;//通过B键切换
  static byte kb_menu_value = 0;//通过上下键切换
  static uint8_t key_value = '0';//键值
  static String key_cache = "";//字符缓存
  //static uint8_t hot_keys[5] = {0, 0, 0, 0, 0}; //快捷键缓存，第一位记录按键个数
  static String hot_key_name[4] = {"", "", "", ""}; //快捷键缓存名称
  char tmp = ' ';

  //左按键,切换键值
  if (arduboy.pressed(LEFT_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    if (key_value == kb_get_key_min(kb_menu_value)) {
      key_value = kb_get_key_max(kb_menu_value);
    } else {
      key_value--;
    }
  }
  //右按键,切换键值
  if (arduboy.pressed(RIGHT_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    if (key_value == kb_get_key_max(kb_menu_value)) {
      key_value = kb_get_key_min(kb_menu_value);
    }
    else {
      key_value++;
    }
  }

  //上键,切换菜单值
  if (arduboy.pressed(UP_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    if (kb_menu_value == 0) {
      kb_menu_value = 11;
    } else {
      kb_menu_value--;
    }
    key_value = kb_get_key_min(kb_menu_value);
  }

  //下键,切换菜单值
  if (arduboy.pressed(DOWN_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    if (kb_menu_value == 11) {
      kb_menu_value = 0;
    } else {
      kb_menu_value++;
    }
    key_value = kb_get_key_min(kb_menu_value);
  }

  // A键
  if (arduboy.pressed(A_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    switch (action_value) {
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
        if (kb_menu_value < 7 && key_cache.length() < 21) {
          key_cache += char(key_value);
        }
        break;
      case 3: // send cache
        if (kb_menu_value < 7) {
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
          if (kb_menu_value < 7) {
            hot_key_name[hot_keys[0] - 1] = char(key_value);
          } else {
            hot_key_name[hot_keys[0] - 1] = kb_key_name(key_value - kb_get_key_min(kb_menu_value), kb_menu_value);
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
        hot_key_name[0] = hot_key_name[1] = hot_key_name[2] = hot_key_name[3] = "";
        break;
      case 8: // Exit
        pos = 1;
        action_value = 0;
        kb_menu_value = 0;
        key_value = '0';
        key_cache = "";
        //hot_keys[0] = 0;
        //hot_key_name[0] = hot_key_name[1] = hot_key_name[2] = hot_key_name[3] = "";
        //kb_clean_hotkey(hot_keys, hot_key_name);
        clean_hotkey();
        hot_key_name[0] = hot_key_name[1] = hot_key_name[2] = hot_key_name[3] = "";
        Keyboard.releaseAll();
        Keyboard.end();
        break;
      default:
        break;
    }
  }

  // B键
  if (arduboy.pressed(B_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    if (action_value == 8) {
      action_value = 0;
    } else {
      action_value++;
    }
  }

  arduboy.clear();

  //显示功能模式
  arduboy.setCursor(0, 0);
  arduboy.print("Keyboard:");
  kb_action_title(action_value);
  arduboy.print("\n");
  //显示标题
  arduboy.print(kb_menu_value + 1);
  arduboy.print(":");
  if (kb_menu_value < 3 || kb_menu_value > 6) {
    kb_menu_title(kb_menu_value);
  } else {
    for (tmp = kb_get_key_min(kb_menu_value); tmp <= kb_get_key_max(kb_menu_value); tmp++) {
      arduboy.print(tmp);
    }
  }
  arduboy.print("\n");
  //显示键名
  arduboy.print("Key:");
  if (kb_menu_value < 7) {
    arduboy.print(char(key_value));
  } else {
    arduboy.print(kb_key_name(key_value - kb_get_key_min(kb_menu_value), kb_menu_value));
    //kb_spkey_name(sp_key - kb_get_spkey_min(kb_menu_value),kb_menu_value);
  }
  arduboy.print("_\n");
  //显示字符缓存区
  arduboy.print("Cache:\n");
  arduboy.print(key_cache);
  arduboy.print("\n");
  //显示热键缓存区
  arduboy.print("Hotkey cache:\n");
  if (hot_keys[0] > 0) {
    for (tmp = 1; tmp <= hot_keys[0]; tmp++) {
      arduboy.print(hot_key_name[tmp - 1]);
      arduboy.print('+');
    }
  }


  // then we finaly we tell the arduboy to display what we just wrote to the display.
  arduboy.display();
}
/** keboard 功能 end **/


/** 登录 **/
//登录界面
void login_screen() {
  //左键，第一位
  if (arduboy.pressed(LEFT_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    pwd_numup(1);
  }
  //上键，第二位
  if (arduboy.pressed(UP_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    pwd_numup(2);
  }
  //右键，第三位
  if (arduboy.pressed(RIGHT_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    pwd_numup(3);
  }
  //下键，第四位
  if (arduboy.pressed(DOWN_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    pwd_numup(4);
  }
  // A键，确认
  if (arduboy.pressed(A_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    //if (pwd_input == PASSWORD) {
    if ((String(char(hot_keys[1])) + char(hot_keys[2]) + char(hot_keys[3]) +  char(hot_keys[4])) == PASSWORD) {
      pos = 1;
    }
  }
  // B键，重置
  if (arduboy.pressed(B_BUTTON) && btn_pressed == 0)
  {
    btn_pressed = 1;
    //pwd_input = "0000";
    clean_hotkey();
  }

  // we clear our screen to black
  arduboy.clear();

  arduboy.setCursor(0, 0);
  arduboy.print("Password:");
  //arduboy.print(pwd_input);
  arduboy.print((String(char(hot_keys[1])) + char(hot_keys[2]) + char(hot_keys[3]) +  char(hot_keys[4])));

  // then we finaly we tell the arduboy to display what we just wrote to the display.
  arduboy.display();
}

//密码数字递增
void pwd_numup(byte n) {
  if (hot_keys[n] == '9') {
    hot_keys[n] = '0';
  } else {
    hot_keys[n]++;
  }
}

//暂停，直到按键抬起
void hold_screen_by_btn() {
  //如果按键没抬起就暂停程序
  while (arduboy.buttonsState() != 0)
  {
  }
  btn_pressed = 0;
}
/** 登录 end **/

/** 主屏 **/
void main_menu_title() {
  //String menu_title[] = {"Keyboard", "Password", "Hotkey", "None"};
  if (menu_value == 0) arduboy.print("Keyboard");
  if (menu_value == 1) arduboy.print("Mouse");
  if (menu_value == 2) arduboy.print("Password");
  if (menu_value == 3) arduboy.print("Hotkey");
  if (menu_value == 4) arduboy.print("Text");
}
//主屏
void main_screen() {
  //左按钮
  if (arduboy.pressed(LEFT_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
  }
  //右按钮
  if (arduboy.pressed(RIGHT_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
  }

  //上键，变更menu_value值
  if (arduboy.pressed(UP_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    if (menu_value == 0) {
      menu_value = 4;
    } else {
      menu_value--;
    }
  }

  //下键，变更menu_value值
  if (arduboy.pressed(DOWN_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    if (menu_value == 4) {
      menu_value = 0;
    } else {
      menu_value++;
    }
  }
  //A键
  if (arduboy.pressed(A_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    pos = 2;
    action_value = 0;
    if (menu_value == 0) {
      Keyboard.begin();
    }
    if (menu_value == 1) {
      Mouse.begin();
    }
  }

  if (arduboy.pressed(B_BUTTON) && btn_pressed == 0) {
    btn_pressed = 1;
    pos = 0;
    //pwd_input = "0000";
    clean_hotkey();//清密码输入
  }

  // we clear our screen to black
  arduboy.clear();
  arduboy.setCursor(0, 0);
  //显示菜单名
  arduboy.print("Menu:\n");
  arduboy.print(menu_value + 1);
  arduboy.print('.');
  main_menu_title();
  //显示功能解释
  arduboy.print("\n");
  if (menu_value == 0) arduboy.print("As a keyboard");
  if (menu_value == 1) arduboy.print("As a mouse");
  if (menu_value == 2) arduboy.print("Nothing");
  if (menu_value == 3) arduboy.print("Nothing");
  if (menu_value == 4) arduboy.print("Nothing");
  //显示操作说明
  arduboy.print("\n\nUp & Dn:Change menu;\nA:OK;\nB:Exit");

  // then we finaly we tell the arduboy to display what we just wrote to the display.
  arduboy.display();
}
/** 主屏 end **/
