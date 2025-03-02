/*
■これは何か?
共立エレショップの7セグ液晶ディスプレイ
https://eleshop.jp/shop/g/gG5G411/?srsltid=AfmBOorQeM3JZSzYW6oN9abr3Djqhq95ISfsXJx6QbF-3_koH-WWYWV-
用のライブラリです。

■使い方
サンプルを見ていただければすぐわかると思います。
タイマーを使うと機種依存が面倒ですし、それほどシビアなものでもないので
タイマーは使っておりません。
サンプルのようにloop内でledPolling()を呼び出してください。
クラス化してもいいんですけど、二つ以上同時に使いたい人はご自分でどうぞ、、

■ちょっと説明
initLCDの引数 coms は液晶の(左から)1,2,3ピンに接続しているGPIO
segsは4..15ピンに接続しているGPIOです。

■物理的な接続
　液晶同梱の説明書にあるとおり
COMすなわち液晶の1,2,3ピン:
 3.7V-22kΩ・22kΩ-GND という分圧セットを3セット作り、その中点を各ピンに接続します。
 (1.8V程度に吊った状態にする)
4..15はそのままマイコンのGPIOに接続してかまいません。

動作確認はSTM32G431+STM32duinoで行いました。

在庫がなくなろうとしているのになぜいまごろ？
ながらく放置していたのを最近引っ張り出して使おうとしたので、、

■多少の解説
ソースを読む上で液晶同梱の取説にあるセグメントマップにおける
COM1..3 は本ソースでは __coms[0..2] に相当します。
同様に
SEG1..12は __segs[0..11] です。
セグメントを表すa..g は以下のレイアウトです。
  a
f	  b			B00000110 ならbcが表示セグメント他は消灯セグメント　なので 1を表示します。 
  g				
e	  c
  d
  
*/

#ifndef ld_bu9332_h
#define ld_bu9332_h

byte digs[4];//各セグメントの表示バッファプログラムから直接書き込んでもよい

uint32_t __coms[3];
uint32_t __segs[12];
byte number[] = {B00111111,
                  B00000110,
                  B01011011,
                  B01001111,
                  B01100110,
                  B01101101,
                  B01111101,
                  B00000111,
                  B01111111,
                  B01100111,
                  B01110111,
                  B01111100,
                  B01011000,
                  B01011110,
                  B01111001,
                  B01110001
                };


void initLCD(uint32_t *COMs, uint32_t *SEGs) {
  for ( int i = 0; i < 3; i++) {
    __coms[i] = COMs[i];
    pinMode(__coms[i], INPUT);
  }
  for ( int i = 0; i < 12; i++) {
    __segs[i] = SEGs[i];
    pinMode(__segs[i], OUTPUT);
    digitalWrite(__segs[i], HIGH);
  }
}

//COMとsegが同位相なら点灯、逆位相なら消灯
//途中でCOMをHZにし、segを逆にしCOMを反転させる(ことで液晶の寿命を延ばす)
//4桁の通常の7セグ配列をこの液晶配列に変換する8bit⇒12bit
uint16_t segArr[3];//下12bitのみ使う
void convDigs() {
  segArr[0] = 0;
  segArr[1] = 0;
  segArr[2] = 0;
  for ( int i = 0; i < 4; i++) {
    byte curr = digs[i];
    //各ビットをチェック
    for ( int b = 0; b < 8; b++) {
      if ((curr & (1 << b)) > 0) {
        switch (b) {
          case 0: segArr[0] = segArr[0] | (1 << (1 + 3 * i));    break; //a
          case 1: segArr[0] = segArr[0] | (1 << (2 + 3 * i));     break; //b
          case 2: segArr[1] = segArr[1] | (1 << (2 + 3 * i));     break; //c
          case 3: segArr[2] = segArr[2] | (1 << (1 + 3 * i));     break; //d
          case 4: segArr[1] = segArr[1] | (1 << (3 * i));     break; //e
          case 5: segArr[0] = segArr[0] | (1 << (3 * i));     break; //f
          case 6: segArr[1] = segArr[1] | (1 << (1 + 3 * i));     break; //g
          case 7://他の記号独自に割り付ける
            if (i == 0) {
              segArr[2] = segArr[2] | (1 << 9); //再生
            }
            if (i == 1) {
              segArr[2] = segArr[2] | 1; //電池 の両端
            }
            if (i == 2) {
              segArr[2] = segArr[2] | (1 << 2); //電池の真ん中
            }
            if (i == 3) {
              segArr[2] = segArr[2] | (1 << 5); //コロン
            }
            break;
        }
      }
    }
  }
}


	/*
	0:全消し
	1:中央部だけ
	2:両端だけ
	3:全部表示
	*/
void battery( byte state )
{
	switch ( state )
	{
	case 0:
		digs[1] =  digs[1] & B01111111;
		digs[2] =  digs[2] & B01111111;
		break;
	case 1:
		digs[2] =  digs[2] | B10000000;
		break;
	case 2:
		digs[1] =  digs[1] | B10000000;
		break;
	}
	convDigs();
}

//再生アイコン表示/非表示
void playMark( byte state )
{
	switch ( state )
	{
	case 0:
		digs[0] =  digs[0] & B01111111;
		break;
	case 1:
		digs[0] =  digs[0] | B10000000;
		break;
	}
	convDigs();
}

//コロン表示/非表示
void colon( byte state )
{
	switch ( state )
	{
	case 0:
		digs[3] =  digs[3] & B01111111;
		break;
	case 1:
		digs[3] =  digs[3] | B10000000;
		break;
	}
	convDigs();
}

//ビットが立っているsegをHLにする
void setSeg(byte HL, byte comIdx) {
  if (HL) {//HL==HIGHなので、bitの立ちと同じにする
    for ( int i = 0; i < 12; i++) {
      digitalWrite(__segs[i], ((segArr[comIdx] & ((uint16_t)(1 << i))) > 0));
    }
  } else {//HL==LOWなので、bitの立ちと逆にする
    for ( int i = 0; i < 12; i++) {
      digitalWrite(__segs[i], !((segArr[comIdx] & ((uint16_t)(1 << i))) > 0));
    }
  }
}

//4桁の表示データを配列で直接アサインする
void setSegments( byte * digits )
{
	for ( int i = 0; i < 4; i++ )
	{
		digs[i] = digits[i];
	}
	convDigs();
}

//タイマー使わずにLEDのスキャンを行う
#define SCAN_INTERVAL	5
void ledPolling() {
  static uint32_t last = 0;
  static byte currCOM = 0;
  static bool upSide = true;
  uint32_t curr = millis();
  if ((curr - last) > SCAN_INTERVAL) {
    last = curr;
    if (upSide) {
      pinMode(__coms[currCOM], INPUT);
      setSeg(1, currCOM); //segArr[currCOM]のbitが立っているものを0=LOWにする
      pinMode(__coms[currCOM], OUTPUT);
      digitalWrite(__coms[currCOM], LOW);
      upSide = false;
    }
    else {
      pinMode(__coms[currCOM], INPUT);
      setSeg(0, currCOM); //segArr[currCOM]のbitが立っているものを0=LOWにする
      pinMode(__coms[currCOM], OUTPUT);
      digitalWrite(__coms[currCOM], HIGH);
      upSide = true;
      currCOM++;
      if (currCOM == 3) {
        currCOM = 0;
      }
    }
  }
}

//10進で数字を表示　上限9999
void showNumber(uint16_t value) {
  value = value % 10000; //9999がmaxなので、、
  digs[0] = number[value / 1000];
  value = value % 1000;
  digs[1] = number[value / 100];
  value = value % 100;
  digs[2] = number[value / 10];
  value = value % 10;
  digs[3] = number[value];
  convDigs();
}

void clear()
{
	for ( int i = 0; i < 4; i++ )
	{
		digs[i] = 0;
	}
}
#endif