/*
■これは何か?
共立エレショップの7セグ液晶ディスプレイ
https://eleshop.jp/shop/g/gG5G411/?srsltid=AfmBOorQeM3JZSzYW6oN9abr3Djqhq95ISfsXJx6QbF-3_koH-WWYWV-
用のライブラリです。

■使い方
サンプルを見ていただければすぐわかると思います。
タイマーを使うと機種依存が面倒ですし、それほどシビアなものでもないので
タイマーは使っておりません。
サンプルのようにloop内でpolling()を呼び出してください。

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

extern const uint8_t lcd_number[];
class ld_bu9332
{
public:
	ld_bu9332();
	byte digs[4];//各セグメントの表示バッファ
	void initLCD( uint32_t *COMs, uint32_t *SEGs );
	void battery( byte state );
	//再生アイコン表示/非表示
	void playMark( byte state );
	//コロン表示/非表示
	void colon( byte state );
	//4桁の表示データを配列で直接アサインする
	void setSegments( byte * digits );
	//タイマー使わずにLEDのスキャンを行う
	void polling();
	//10進で数字を表示　上限9999
	void showNumber( uint16_t value );
	void clear();
private:
	uint32_t __coms[3];
	uint32_t __segs[12];

	void convDigs();
	void setSeg( byte HL, byte comIdx );
};
#endif