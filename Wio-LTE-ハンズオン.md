[LTEモデム搭載プロトタイプ向けデバイス 「Wio LTE」](https://soracom.jp/products/wio_lte/) を用いたハンズオンテキストです

## 手順
* [開発環境の準備](#開発環境の準備)
    * [Wio LTE ハードウェア解説](#wio-lte-ハードウェア解説)
* [ハンズオン](#ハンズオン)
    * [ステップ 1: LED点灯](#作業ステップ-1-led点灯)
    * [ステップ 2: デバイス稼働時間を SORACOM Harvest で可視化](#作業ステップ-2-デバイス稼働時間を-soracom-harvest-で可視化)
    * [ステップ 3: 温湿度センサーのデータを SORACOM Harvest で可視化](#作業ステップ-3-温湿度センサーのデータを-soracom-harvest-で可視化)
    * [ステップ 4: 温湿度センサーのデータを SORACOM Funnel 利用して SAP IoT Services へ転送](#ステップ-4-温湿度センサーのデータを-soracom-funnel-利用して-sap-iot-services-へ転送)
    * [ステップ 5: SORACOM Beam と SAP IoT Services を用いた MQTT Pub/Sub 双方向通信](#ステップ-5-soracom-beam-sap-iot-services-用-mqtt-pubsub-双方向通信)
    * [おわりに](#おわりに)
* [Appendix(付録)](#appendix付録)
    * [ハンズオンで使用した環境の構築方法](#使用環境構築方法)

## 準備するもの

* パソコン / 1台
    * ※タブレット等の下記に当てはまらないPCでの受講は自己責任としております
    * Wi-Fi 接続可能
    * USB Type-A ポートが最低1つ以上あり、利用可能なこと
        * 電力供給が1A以上であること (USB 3.0対応していれば概ね安心です)
        * USB Type-C のみの機種につきましては、Type-A への変換アダプタのご用意をお願いいたします
    * OS: macOS(10.11 El Capitan 以上) もしくは Windows(8.1 以上)
    * ブラウザ: Google Chrome
    * ※ソフトウェアをインストールするため、PCに対する管理者権限を持っている事 と ブラウザでのアクセス制限(HTTPプロキシ等)がかかっていない事
* 有効なSORACOMアカウント / 1つ
    * 持っていない場合: 有効なクレジットカード(1枚) と この場で確認可能なメールアドレス(1つ) を利用し [SORACOM アカウントの作成](https://dev.soracom.io/jp/start/console/#account) の手順に沿って作成します
* SORACOMアカウントに登録済みの SORACOM Air SIM (nanoサイズ) / 1つ
    * 未登録の場合: [Air SIM の登録](https://dev.soracom.io/jp/start/console/#registsim) の手順に沿って登録します
* Wio LTE (本体、 アンテナ2本、 電源兼シリアルコンソール用microUSBケーブル) / 1式
* Grove デジタル温湿度センサー (DHT11) / 1つ
* Grove コネクタケーブル / 1つ

## 受講における注意点    

* 本ハンズオンで利用するサービスには有料のものを含んでおります
* ハンズオンに伴い発生した費用（ハンズオン中、ハンズオン後問わず）は原則として受講者にご負担いただいております。係る費用について確認・ご理解いただいたうえでの受講をお願いしております
    *  [SORACOM Air 料金](https://soracom.jp/services/air/cellular/price/)、[SORACOM Harvest 料金](https://soracom.jp/services/harvest/price/)、[SORACOM Beam 料金](https://soracom.jp/services/beam/price/)、[SORACOM Funnel 料金](https://soracom.jp/services/funnel/price/)
* 本ドキュメントで発生した不具合等につきましては弊社は一切責任を負いません

<a name="prep"></a>
# 開発環境の準備

Wio LTE を使うためには、開発環境の準備を行います  
OS 毎に準備がありますので、下記を参照の上開発環境を準備してください

* [Windows 編](https://github.com/shakkaponk/IoTworkshop/blob/master/Wio-LTE-%E9%96%8B%E7%99%BA%E7%92%B0%E5%A2%83%E6%A7%8B%E7%AF%89---Windows-%E7%B7%A8.md)
* [macOS 編](https://github.com/shakkaponk/IoTworkshop/blob/master/Wio-LTE-%E9%96%8B%E7%99%BA%E7%92%B0%E5%A2%83%E6%A7%8B%E7%AF%89-macOS-%E7%B7%A8.md)

<a name="wiolte-hardware"></a>
## Wio LTE ハードウェア解説

Wio LTE は Seeed Studio が開発・販売しているマイコンモジュールです  
Grove コネクタと STM32F4 マイコン、LTE Cat.1 モジュールが搭載されており、 Arduino IDE で素早くプロトタイピングができます

![Wio LTE](https://soracom.jp/img/products_wio_lte.png)

### USB 電源・アンテナ・ Grove コネクタピンの番号

![Wio LTE 本体](https://docs.google.com/drawings/d/e/2PACX-1vSgTN46GBaOzqnnmg6tzc7S6UOvzefQhewbGrrRYCrn4RKz1_MMO77zPoCFisCRjztfmHYuac9VIQmN/pub?w=639&h=299)

### Grove IoT スターターキット for SORACOM

「Grove IoT スターターキット for SORACOM」は、 Wio LTE 本体に加え、７つのセンサーと SORACOM Air SIM (日本向け) と SORACOM クーポンが一つにパッケージされたキットです

本ハンズオンでは **デジタル温湿度センサー** を利用します

![Grove IoT スターターキット for SORACOM](https://docs.google.com/drawings/d/e/2PACX-1vT7i6svO5xJj56o0x1aUaW3qhkb4aCWynxSzbwVG3boCqe-TQooCC-qAm0Nfa3QBIb9YIum1G7Zkw-Z/pub?w=640&h=347)

## Wio LTE の電源ON/OFFの方法

Wio LTE には電源スイッチがありませんので、下記作業で ON / OFF してください

### 電源 ON

microUSB ケーブルを Wio LTE の microUSB ポートに接続すると自動的にONになります

![Wio LTE と PC を microUSBケーブルで取り付けたところ](https://docs.google.com/drawings/d/e/2PACX-1vSgLUCOrN928URIfbcNC0VR4xwSBOCYm8ngs0d2edkWyu4ZC7VNoXjALvKOXv121zk3RZB2vF9J40fB/pub?w=640&h=480)

### 電源 OFF

microUSB ケーブルを抜きます。いきなり抜いて OK です

※シャットダウン処理は存在しません
<a name="wiolte-modes"></a>
## Wio LTE の "通常モード" と "DFUモード"

Wio LTE は２つのモードを持っています
**この操作は Wio LTE の開発で何度も行うことになりますので、必ず覚えてください**

* 書き込まれたプログラムを実行する「通常モード」
* プログラムを書き込むことができる「DFUモード」

![フロー](https://docs.google.com/drawings/d/e/2PACX-1vQAcnymqWTTneRwnc9EFz21YvrmfCsIuV33yfqf1ODC_LKQR-6762CJDMclRIWC8BfUeDDLpC6KKs-2/pub?w=581&h=253)

これらのモードの切り替えは Wio LTE 上の **RSTボタン** と **BOOTボタン** の組み合わせで行います  
各ボタンの位置は下記のとおりです（ Wio LTE の表裏にボタンがあるため、横からみた図で確認ください）

![Wio LTE を横からみた図](https://docs.google.com/drawings/d/e/2PACX-1vRnhRiZC7-jRCqLaxJO6E7Bmq0_8BxornXgP1y6UHdYXhr6iBm_RNoV148oSzJKeHBYXRjYai9msQoz/pub?w=480&h=249)

### Wio LTE の動作モードを変更する・確認する

Wio LTE の動作モードの確認の仕方は、環境構築の準備でご確認ください

* [Windows で Wio LTE の動作モードを変更する・確認する](https://github.com/soracom/handson/wiki/Wio-LTE-%E9%96%8B%E7%99%BA%E7%92%B0%E5%A2%83%E6%A7%8B%E7%AF%89---Windows-%E7%B7%A8#wio-lte-%E3%81%AE-%E9%80%9A%E5%B8%B8%E3%83%A2%E3%83%BC%E3%83%89-%E3%81%A8-dfu%E3%83%A2%E3%83%BC%E3%83%89)
* [macOS で Wio LTE の動作モードを変更する・確認する](https://github.com/soracom/handson/wiki/Wio-LTE-%E9%96%8B%E7%99%BA%E7%92%B0%E5%A2%83%E6%A7%8B%E7%AF%89-macOS-%E7%B7%A8#wio-lte-%E3%81%AE-%E9%80%9A%E5%B8%B8%E3%83%A2%E3%83%BC%E3%83%89-%E3%81%A8-dfu%E3%83%A2%E3%83%BC%E3%83%89)

<a name="handson"></a>
# ハンズオン
<a name="step1"></a>
# 【作業】ステップ 1: LED点灯

Wio LTE ライブラリに付属しているスケッチ例から、内蔵のLEDを点灯してみましょう

※Wio LTE 開発ツールの使い方を学びます

![ステップ1 overview](https://docs.google.com/drawings/d/e/2PACX-1vQuxiAIWQlqEF0aIgIZtYGemOl7BFPnqKTU3WxKbyk3F8PG0zkhYBWUrVUpJyq8LDqzQwAiBMFUxPKD/pub?w=371&h=247)

## 最初に Wio LTE の電源を OFF にする

Wio LTE の microUSB ケーブルを抜き、電源を OFF にしてください

※いきなり抜いてOKです。また、すでに OFF になっている場合は次に進んでください

## 1. スケッチを作成する

### 1-1. Arduino IDE を起動する

* Windowsの場合: スタートメニュー等から **Arduino** を起動します
* macOSの場合: アプリケーションフォルダ等の中から **Arduino** を起動します

### 1-2. メニューの [ツール] > [ボード: "Arduino/Genuino UNO] で表示される一覧から **Wio Tracker LTE** を選択

![](https://docs.google.com/drawings/d/e/2PACX-1vQKCIKzOA6NSb0-3kNvL5i9lpZSNAS5OXklLbFITCP2vHvEjM2gL3qKdo8WzYZjifjajFe3YovtiUEI/pub?w=333&h=507)

### 1-3. メニューの [ファイル] > [スケッチ例] > [Wio LTE for Arduino] > [basic] > [LedSetRGB]

### 1-4. Wio LTE を PC を接続して DFUモード にする

### 1-5. 新しく開いたウィンドウの ![マイコンボードに書き込むアイコン](https://docs.google.com/drawings/d/e/2PACX-1vQiO83cFcX3LCXeioiTiaao57T4SGiIV6XZzcBP6poTwssCxmo7hLpoMh5qG3btyqgzs8Q-lAoE6Q0f/pub?w=100&h=100)(マイコンボードに書き込む) をクリック

### 1-6. Arduino IDE のウィンドウ下部に、下記のように表示されたら書き込み完了です  

```
DFU end
can't detach

   もしくは

Resetting USB to switch back to runtime mode
DFU end
```

### 1-7. Wio LTE を 通常モードにする (RSTボタンを押せば通常モードになります)

## 2. 確認

下図のように Wio LTE 内蔵のLEDがカラフルに光ります  

![Wio LTE / LedSetRGB](https://dev.soracom.io/img/gs_wio-lte/wio-lte_led.gif)

ここまでの手順の動画です (画面は Windows ですが、 macOS でも同様の手順です)

[![Wio LTE / LedSetRGB](http://img.youtube.com/vi/g9LiH_g-TuE/0.jpg)](http://www.youtube.com/watch?v=g9LiH_g-TuE)

## 3. やってみよう (LEDの変更周期)

スケッチ※を編集して LED の色が変わる周期を変えてみましょう  
※Arduino IDE では、プログラムのことを「スケッチ」と呼びます

スケッチ3行目の `INTERVAL` が周期を指定しています。ミリ秒(ms)単位になっており、 50 から変更してみましょう

変更前

```
#define INTERVAL  (50)
```

変更後

```
#define INTERVAL  (10)
```

スケッチを変更したら、先ほどの 1-4 ～ 1-7 の手順を行って、LEDの発行色の周期が変わっていることを確認してみてください

## 4. Wio LTE の動作を止める

止める方法は Wio LTE の電源を OFF (= microUSBケーブルを抜く) してください

## うまく動かなかったら（トラブルシュート）

**[ファイル] > [スケッチ例] の中に "Wio LTE for Arduino" が表示されない**

* 原因: "ボード" が "Wio Tracker LTE" になっていません
* 対策: メニューの [ツール] > [ボード: XXXX] から **Wio Tracker LTE** を選択してください

**「マイコンボードに書き込む」を実行した結果、ウィンドウに下記のように表示された**

```
exit status 1
ボードArduino/Genuino Unoに対するコンパイル時にエラーが発生しました。
```

* 原因: "ボード" が "Wio Tracker LTE" になっていません
* 対策: メニューの [ツール] > [ボード: XXXX] から **Wio Tracker LTE** を選択してください

**「マイコンボードに書き込む」を実行した結果、ウィンドウに下記のように表示された**

```
No DFU capable USB device available
DFU end
```

* 原因: Wio LTE が「通常モード」で書き込もうとした
* 対策: Wio LTE を「DFU モード」にしてから、再度「マイコンボードに書き込む」を実行してください

**「マイコンボードに書き込む」を実行した結果、ウィンドウに下記のように表示された**

```
java.io.IOException: jssc.SerialPortException: ....
    ... 色々表示されて ...
    ... 4 more もしくは ... 6 more
```

* 原因: おもに Windows でシリアルモニタ―を表示した後に発生します。シリアルポートの解放に失敗しています
* 対策: Arduino IDE 終了＆再度立ち上げてください。また Arduino IDE のシリアルモニターは使わずに TeraTerm を使うことで回避しやすくなります

**「マイコンボードに書き込む」を実行した結果、ウィンドウに下記のように表示された**

```
dyld: Library not loaded: /opt/local/lib/libusb-1.0.0.dylib
  Referenced from: /Users/user1/Library/Arduino15/packages/Seeeduino/tools/stm32_dfu_upload_tool/1.0.0/macosx/dfu-util/dfu-util
  Reason: image not found
/Users/user1/Library/Arduino15/packages/Seeeduino/tools/stm32_dfu_upload_tool/1.0.0/macosx/dfu_upload: line 5: 15851 Abort trap: 6           $(dirname $0)/dfu-util/dfu-util -d $2 -a $1 -D $3 -s $4 -R
DFU end
```

* 原因: macOS で libusb がインストールされていない
* 対策: [Wio LTE 開発環境の準備 / libusb のインストール](https://github.com/soracom/handson/wiki/Wio-LTE-%E9%96%8B%E7%99%BA%E7%92%B0%E5%A2%83%E6%A7%8B%E7%AF%89-macOS-%E7%B7%A8#libusb)を参照して libusb をインストールしてから、再度「マイコンボードに書き込む」を実行してください
<a name="step2"></a>
# 【作業】ステップ 2: デバイス稼働時間を SORACOM Harvest で可視化

Wio LTE の稼働時間を SORACOM Harvest で可視化してみましょう

※Wio LTE 単体の構成でモバイル通信の活用を学びます

![ステップ2 ovewview](https://docs.google.com/drawings/d/e/2PACX-1vQYbHwh3ec7Bfl7o8Hj3a6FvmnHJWfbeahOi0wsUn_BzhhC_-mX-sHmEV85-vFEdd5m6HQRAvh_lyrW/pub?w=371&h=247)

## 最初に. Wio LTE の電源を OFF にする

Wio LTE の microUSB ケーブルを抜き、電源を OFF にしてください

※いきなり抜いてOKです。また、すでに OFF になっている場合は次に進んでください

## 1. Wio LTE にアンテナを取り付ける

**※Wio LTE から microUSB ケーブルを抜いた状態にしてください**

Wio LTE に 添付されているアンテナ2本を取り付けます (**※アンテナは必ず2本取り付けてください**)

![Wio LTE にアンテナを取り付けたところ](https://docs.google.com/drawings/d/e/2PACX-1vQeeuUwMU4t2mjArmyPGsciGmXCYxl8gXPt_gTFiCh-iMOnv-c0s22otGPmvG7-PsMDX34Pe0d3afqC/pub?w=640&h=480)

## 2. Wio LTE に SIM を取り付ける

Wio LTE に SIM (nanoサイズ) を取り付けます

<img src="https://docs.google.com/drawings/d/e/2PACX-1vQ_jUPQyYsHnk0v5yFZpdbJ6n7l2U6Pf1SexxpQ1DyvrxYVbiDTmfT2GjyUmZpR7Dc2dtZneXpkFqZz/pub?w=550&amp;h=255">

### SIM の取り出し

SIM の取り出し方も一緒に紹介いたします

<img src="https://docs.google.com/drawings/d/e/2PACX-1vSHV26TeW7Z3rEbr-OOVQ-6GyI8GijLNChn5ayEspNVPZqmbZTj3pkDdBCZhLEetrMqLdDuXm-GpQk9/pub?w=683&amp;h=322">

<img src="https://docs.google.com/drawings/d/e/2PACX-1vRn4PgJFdW-IV-Fr4JTq7dj6wz4KBSyG3pM6W4Wxdbtt-FunzEy4aSK1_QpssqfoATEWfP_9IJL8vtV/pub?w=508&amp;h=370">

**抜く際フックを引っ張りすぎると金具が取れてしまうため、図示されている程度まで引っ張り出したらSIMを取り出し、金具を元に戻してください**

## 3. SORACOM Harvest の設定をする

**※ SORACOM のアカウントをお持ちでない方:** [SORACOM アカウントの作成](https://dev.soracom.io/jp/start/console/#account) の手順に沿って作成してください   
（有効なクレジットカード(1枚) と この場で確認可能なメールアドレス(1つ)が必要です）

**※ SORACOM アカウントを持っているが SIM を登録していない場合:** [Air SIM の登録](https://dev.soracom.io/jp/start/console/#registsim) の手順に沿って登録してください

### 3-1. [SORACOM Webコンソール](https://console.soracom.io/) で 左上[Menu] > [SIM グループ]

![soracom-menu](https://docs.google.com/drawings/d/e/2PACX-1vRhgmsjqpncv2HQ0jAZwiYf0knTfvmCMl6x_flrdeGQV4N60trp8M981gCAfitVSmXU4tqAYm6MmyRb/pub?w=331&h=410)
![soracom-menu-sim-group](https://docs.google.com/drawings/d/e/2PACX-1vTqI-f2K8n-TuUvVEGPnmDcFxG2f87so3Qfe5K11sn0pXG8Q4v2lJX0UT9tjlH7sDQRb1FC7aFfckjb/pub?w=353&h=290)

### 3-2. [追加] で、SIMグループを作成します (グループ名 `harvest` もしくは任意でかまいません)

![soracom-menu-sim-group-create](https://docs.google.com/drawings/d/e/2PACX-1vQ-wJ7Ixk-BQDtxXweBkhl-deBJzh3behOo_rQNNxm3gO73sKHEV_RvqO7cWrSKJT0AZltPaF_K0qPf/pub?w=381&h=315)

![soracom-menu-sim-group-create-dialog](https://docs.google.com/drawings/d/e/2PACX-1vRjDUj0AzCWEBNyy9GTqWf6jPANTk4WIEZcarMaYd9GhbM-_2AhBru9WglGRplqo0jUroC9rIq82G8h/pub?w=631&h=306)

### 3-3. 先ほど作成した SIMグループ をクリックし、 SORACOM Harvest の設定を ON にして保存をクリック

![soracom-menu-sim-group-list](https://docs.google.com/drawings/d/e/2PACX-1vTpWazZ3_xwnViyK1XJXVo3Aa8BeqhLsdeE4v1SHsUNUhKQw-mS15ZovR4kEzNfhJZw2PYdGEcLB9Fr/pub?w=316&h=334)

![soracom-harvest-on](https://docs.google.com/drawings/d/e/2PACX-1vRjSkL7huwCXXSSknkDnuVbPqYSo9a-rJ0PInLFa-mmgBx1fhhHdVu339RbtSuAuhY2bcFlyMxsGWs1/pub?w=504&h=685)

### 3-4. [SORACOM Webコンソール](https://console.soracom.io/) で 左上[Menu] > [SIM 管理]

![soracom-menu](https://docs.google.com/drawings/d/e/2PACX-1vRhgmsjqpncv2HQ0jAZwiYf0knTfvmCMl6x_flrdeGQV4N60trp8M981gCAfitVSmXU4tqAYm6MmyRb/pub?w=331&h=410)
![soracom-sim-mgr](https://docs.google.com/drawings/d/e/2PACX-1vTUi6LN6Hsctv4KdaZj8uOUFg_ZyROx73f1TzFq41KIlRzjUmE_bc2NR5UnS8cn15TD_S2s8FA-DHzA/pub?w=353&h=290)

### 3-5. Wio LTE に取り付けている SIM を選択 > [操作] > [所属グループ変更]

![soracom-select-sim](https://docs.google.com/drawings/d/e/2PACX-1vQpULGXvkk5htY266aDd2iWJueVphdm8DFRVy_BF5JnWnZfBBLF19U42ni5lU6VxN5ucmwqKHx4ACjg/pub?w=526&h=489)

### 3-6. 先ほど作成した SIMグループ に所属させる

![sim-group-select](https://docs.google.com/drawings/d/e/2PACX-1vR1DJQnKw0NVvv83qxiTiDkh0AYfF6u8g3En7EDQtt2M2OjCRzl_tmlB-02cyiLBHLwWHjpOshFKTAA/pub?w=643&h=334)

以上で SORACOM Web コンソール上での作業は終了です

下記のように Wio LTE に取り付けている SIM の "グループ" が、先ほど作った SIM グループ名になっていれば成功です

![sim-list](https://docs.google.com/drawings/d/e/2PACX-1vSjr7j-ld8piy6POBYX1r8Ib2nW1DLjwanI1bqDXS0VsWh6SFK8RXvfDop5X0hzg2Auq2aSvdH8eDPm/pub?w=520&h=464)

## 4. スケッチを作成する

### 4-1. Arduino IDE を起動する

### 4-2. メニューの [ツール] で [ボード: "Wio Tracker LTE"] と表示されていることを確認する

なっていなければ一覧から "Wio Tracker LTE" を選んでください

### 4-3. Arduino IDE の [ファイル] > [スケッチ例] > [Wio LTE for Arduino] > [soracom] > [soracom-harvest]

新しいウィンドウが開きます

### 4-4. Wio LTE と PC を接続して DFUモード にする

### 4-5. 新しく開いたウィンドウの ![マイコンボードに書き込むアイコン](https://docs.google.com/drawings/d/e/2PACX-1vQiO83cFcX3LCXeioiTiaao57T4SGiIV6XZzcBP6poTwssCxmo7hLpoMh5qG3btyqgzs8Q-lAoE6Q0f/pub?w=100&h=100)(マイコンボードに書き込む) をクリック

### 4-6. 書き込みが完了したら、Wio LTE を 通常モードにする (RSTボタンを押せば通常モードになります)

通常モードで起動次第 SORACOM Harvest へデータを送信し始めます (電源投入から送信開始までは20~25秒程度かかります)

## 5. 確認

### 5-1. [SORACOM Webコンソール](https://console.soracom.io/) で 左上[Menu] > [SIM 管理]

![soracom-menu](https://docs.google.com/drawings/d/e/2PACX-1vRhgmsjqpncv2HQ0jAZwiYf0knTfvmCMl6x_flrdeGQV4N60trp8M981gCAfitVSmXU4tqAYm6MmyRb/pub?w=331&h=410)
![soracom-sim-mgr](https://docs.google.com/drawings/d/e/2PACX-1vTUi6LN6Hsctv4KdaZj8uOUFg_ZyROx73f1TzFq41KIlRzjUmE_bc2NR5UnS8cn15TD_S2s8FA-DHzA/pub?w=353&h=290)

### 5-2. Wio LTE に取り付けている SIM を SORACOM Webコンソール上で選択し [操作] > [データを確認]

![harvest-view](https://docs.google.com/drawings/d/e/2PACX-1vRGN09AF9n0GafAg8Ut9s8QYAmEd4h5Oj4fTYUQjqKEFXCj_aIRjyS3u5zpim0eqtnnh-csIl6sAHaU/pub?w=526&h=489)

下図のように SORACOM Harvest 上で稼働時間が表示されるようになります  
※自動更新を ON にすると、画面の再描画が自動的に行われます  
※グラフ種類を折れ線グラフや棒グラフに変更することができます

![SORACOM Harvest で表示している様子](https://dev.soracom.io/img/gs_wio-lte/soracom-harvest-rendering.png)

## 6. やってみよう(送信間隔の変更)

スケッチを編集して稼働時間の送信間隔を変えてみましょう

スケッチ4行目の `INTERVAL` が周期を指定しています。ミリ秒(ms)単位になっており、 60000 (= 60秒) から変更し、マイコンボードに書き込んでみましょう

```
#define INTERVAL  (60000)
```

↓

```
#define INTERVAL  (5000)
```

## 7. Wio LTE の動作を止める

止める方法は Wio LTE の電源を OFF (= microUSBケーブルを抜く) してください

## うまく動かなかったら（トラブルシュート）

データが表示されない場合は、下記を確認してください

* Wio LTE へアンテナが正しく取り付けられているか？
* SIM が正しく挿入されているか？
* SIM が SORACOM に登録されているか？
* SORACOM Harvest が ON なグループに SIM が所属しているか？

以上を確認しても、まだ尚データ送信されない場合は RST ボタンを押して Wio LTE の再起動を行ってみてください

また、Arduino IDE のシリアルモニターで接続やデータ送信状況が確認できます

※注意: Windows の Arduino IDE のシリアルモニタは、 Wio LTE 開発においては動作が不安定になることがあるため、Windows の方は TeraTerm を使用してください
<a name="step3"></a>
# 【作業】ステップ 3: 温湿度センサーのデータを SORACOM Harvest で可視化

デジタル温湿度センサーのデータを SORACOM Harvest で可視化してみましょう

※センサーを組み合わせた開発を学びます

![ステップ3 ovewview](https://docs.google.com/drawings/d/e/2PACX-1vSWZW16P0NALgTq2O1w1MuAhlK_NagFV-HwU8NNa4Sui1mxDXUdK6Y4TSRfzrAgqDhd5IXPWTvgZjpG/pub?w=615&h=247)

## 最初に. Wio LTE の電源を OFF にする

Wio LTE の microUSB ケーブルを抜き、電源を OFF にしてください

※いきなり抜いてOKです。また、すでに OFF になっている場合は次に進んでください

## 1. Wio LTE にデジタル温湿度センサーを取り付ける

**※Wio LTE から microUSB ケーブルを抜いた状態にしてください**

Grove デジタル温湿度センサーを Wio LTE に取り付けます

Wio LTE の **D38** に取り付けてください

![Wio LTE にデジタル温湿度センサーを取り付けたところ](https://docs.google.com/drawings/d/e/2PACX-1vTZiJ7ep0q7IR2KoNJPnvHcTA_JMGEWTn5qwrlp8AC2YLAtETPRAGXss98LMKm061OlM76hDasUyIoS/pub?w=640&h=480)

## 2. スケッチを作成する

### 2-1. Arduino IDE を起動する

### 2-2. メニューの [ツール] で [ボード: "Wio Tracker LTE"] と表示されていることを確認する

なっていなければ一覧から "Wio Tracker LTE" を選んでください

### 2-3. Arduino IDE の [ファイル] > [スケッチ例] > [Wio LTE for Arduino] > [soracom] > [soracom-harvest]

※上記の通り新規に始めても、ステップ2の続きから始めても構いません

### 2-4. スケッチの8行目の行頭 `//` を取り除きます (アンコメントする)

変更前

```
// #define SENSOR_PIN    (WIOLTE_D38)
```

変更後

```
#define SENSOR_PIN    (WIOLTE_D38)
```

### 2-5. Wio LTE と PC を接続して DFUモード にする

### 2-6. 新しく開いたウィンドウの ![マイコンボードに書き込むアイコン](https://docs.google.com/drawings/d/e/2PACX-1vQiO83cFcX3LCXeioiTiaao57T4SGiIV6XZzcBP6poTwssCxmo7hLpoMh5qG3btyqgzs8Q-lAoE6Q0f/pub?w=100&h=100)(マイコンボードに書き込む) をクリック

### 2-7. 書き込みが完了したら、Wio LTE を 通常モードにする (RSTボタンを押せば通常モードになります)

通常モードで起動次第 SORACOM Harvest へデータを送信し始めます (電源投入から送信開始までは20~25秒程度かかります)

※標準では送信間隔が60秒です。早めたい場合は ステップ2 のやってみようを参考に `INTERVAL` を `5000` などに変えてください

## 3. 確認

SORACOM Harvest 上で温度(temp)・湿度(humi)が表示されるようになります  
※SORACOM Harvest の操作方法は ステップ2 で確認してください

![harvest-all-plot](https://docs.google.com/drawings/d/e/2PACX-1vSwebGsd_kOHhagej9sCP5WEVVYZt45KKKa_vgd343pLYyIMj95sFvdMtDtDSe3eixfDjJBizt3wlS5/pub?w=674&h=333)

## 4. Wio LTE の動作を止める

止める方法は Wio LTE の電源を OFF (= microUSBケーブルを抜く) してください

## うまく動かなかったら（トラブルシュート）

**Wio LTE を通常モードで動かして1分経ってもデータが表示されない**

* 原因: 電波状況などによりセルラー通信に失敗している
* 対策: RST ボタンを押して Wio LTE を再度起動しなおしてください

**SORACOM Harvest でデータ表示がされない**

* 原因: ステップ2 で行った稼働時間のデータが原因で、データのY軸が大きすぎて表示されない場合があります
* 対策: SORACOM Harvest 上で下記操作を行ってください
    - 凡例をクリックすることで当該のデータを非表示できる機能
    - 他の形式で表示する機能
    - 最大値/最小値の設定ができる機能

![harvest-exclude-plot](https://docs.google.com/drawings/d/e/2PACX-1vRa8wgI9GtmrCNPLiKhwF6o-tkCTg4QBC3xUBY_nxK3urV4B0r0b5yEsSLWjIy88tJJKAwCQYJVcvGm/pub?w=634&h=300)

![harvest-alt-plot](https://docs.google.com/drawings/d/e/2PACX-1vR_nFPgaTosGb5Ywy0KsNp6d7yP1MHkMcM6uUqT8fuw4WMdsSTn3fct1izl6MjEmeWLDq6yi3_5lnwW/pub?w=731&h=306)
<a name="step4"></a>
# 【作業】ステップ 4: 温湿度センサーのデータを SORACOM Funnel 利用して SAP IoT Services へ転送

本格的なデータ収集 IoT システムの構築として、クラウド側のデータ処理に SAP IoT Services を活用する構成を学びます

※本ハンズオンでは SAP IoT Services までとしていますが、下図は実際のシステム構成例として[IoT向けアーキテクチャパターン](https://www.slideshare.net/AmazonWebServicesJapan/aws-black-belt-online-seminar-2017-iot/34)を盛り込んでいます
# <<<<<<<<<<<要編集>>>>>>>>>>>
![ステップ4 overview](https://docs.google.com/drawings/d/e/2PACX-1vRkHbjrlzpsr2HX34VyZsQIdlx5m7_BjbA-k3tiHmhkLh9OqbzXvuVLiOY3M6ZPwezpwKcMyU8cxnUH/pub?w=997&h=275)

## 最初に. Wio LTE の電源を OFF にする

Wio LTE の microUSB ケーブルを抜き、電源を OFF にしてください

※いきなり抜いてOKです。また、すでに OFF になっている場合は次に進んでください

## 1. SORACOM へ認証情報を保管する

### 1-1. [SORACOM Webコンソール](https://console.soracom.io/) で 左上[Menu] > [セキュリティ]

![soracom-menu](https://docs.google.com/drawings/d/e/2PACX-1vRhgmsjqpncv2HQ0jAZwiYf0knTfvmCMl6x_flrdeGQV4N60trp8M981gCAfitVSmXU4tqAYm6MmyRb/pub?w=331&h=410)
![soracom-menu-security](https://docs.google.com/drawings/d/e/2PACX-1vRjYW4eP-Cv1GTPmgGD01OEGiszvYmy1gmbsAQx50O6knq0UEPBozSQ8W3ezngFS6Z7B-8ArZkIXSWW/pub?w=344&h=334)

### 1-2. [認証情報ストア] > [認証情報を登録] で 認証情報入力画面を開きます

![soracom-cred-store](https://docs.google.com/drawings/d/e/2PACX-1vRoPNOwuGigBjl6MNfqQpXZL1MFmgPeUuwFLNF3TTRDZMQJf1G-HdwukH5otE7tdHy0YM9MWXmLac6z/pub?w=624&h=309)

### 1-3. 認証情報を登録する画面では下記の通り入力し、保存してください

* 認証情報ID: `awsiot-dataaccess`
* 概要: `AWS IoT DataAccess`
* 種別: *AWS 認証情報* (これを選択すると、下記IDを入力するテキストボックスが増えます)
* AWS Access Key ID: **ハンズオン運営から入手 (aws-accesskey-id)**
* AWS Secret Access Key ID: **ハンズオン運営から入手 (aws-secret-accesskey-id)**

![soracom-cred-save](https://docs.google.com/drawings/d/e/2PACX-1vR6S9T-9TZbcdB8XwcUrBE7MQCyIsT-zOA2LQGspbv5r72CxT_qc1pyIdgmIXqVU79qOEKfOzuA8vXZ/pub?w=644&h=642)

## 2. SORACOM Funnel 設定

### 2-1. [SORACOM Webコンソール](https://console.soracom.io/) で 左上[Menu] > [SIM グループ]

[追加] で、SIMグループを作成します (グループ名 `funnel` もしくは任意でかまいません)

### 2-2. 先ほど作成した SIMグループ をクリックし、 SORACOM Funnel の設定を開きます

### 2-3. SORACOM Funnel の設定では、下記の通り入力し、保存してください

* 転送先サービス: *AWS IoT*
* 転送先URL: **ハンズオン運営から入手 (endpoint-url)**
* 認証情報: *awsiot-dataaccess (AWS IoT DataAccess)*
* 送信データ形式: *JSON*

![soracom-funnel](https://docs.google.com/drawings/d/e/2PACX-1vQ1u87_1m7Mlk-t9G33ho7s8f_4-F8pIGjksjJxBFRhgYYVwJiBWAOVRr0_XWv5ehjU_4hqDvH7kXI_/pub?w=926&h=435)

### 2-4. 左上[Menu] > [SIM 管理]

* Wio LTE に取り付けている SIM を選択 > [操作] > [所属グループ変更]
* 先ほど作成した SIMグループ に所属させる

## 3. SAP IoT Servicesを設定する

SAP Cloud Platform IoT services Cockpitにアクセスする
```
https://<HOST_NAME>/iot/cockpit/
```
今回使うインスタンスがこちらです：https://coil.eu10.cp.iot.sap/iot/cockpit/

講師に伝えられたログイン・パスワードでCockpitにアクセスしてください
![awsiot-monitor](https://docs.google.com/drawings/d/e/2PACX-1vRJyT93F0VK2jYSPQ4izxXEjiJhWQHKZJJr90U3VeX5_WCLfdmVftP5v_rrmYIX5ZXWnD9dLwsYzWdY/pub?w=928&h=367)


### 3-1. Capabilityを作る
Device ManagementカテゴリーのCapabilitiesを押下する    
 **+ (Create a capability)** でCapabilityを作成する

**Name:** climate   
**Alternate ID:** (任意)

Propertiesセクションでプロパティを二つ追加する。Add a propertyボタンを押す

**Name:** temperature  
**Data Type:** float

**Name:** humidity  
**Data Type:** float    

Unit of Measureは入れなくてもいいです。

Createを押して、capabilityが正しく生成されたメッセージが表示される。

### 3-2. SensorTypeを作る
Device ManagementカテゴリーのSensor Typesを押下する    
 **+ (Create a sensor type)** でSensor typeを作成する

**Name:** climate_sensor_type   
**Alternate ID:** (任意)

CapabilitiesセクションでAdd a capabilityボタンを押す

**Name:** ドロップダウンで先ほど作ったCapabilityを選択する  
**Type:** measure

Createを押して、sensor typeが正しく生成されたメッセージが表示される。

### 3-3. MQTT Deviceを作る
Device ManagementカテゴリーのDevicesを押下する    
 **+ (Create a device)** でdeviceを作成する

**Name:** climate_device   
**Gateway:** IoT Gateway MQTT   
**Alternate ID:** (任意)

Createを押して、deviceが正しく生成されたメッセージが表示される。


### 3-4. Sensorを作る
Device ManagementカテゴリーのDevicesを押下する   
先ほど作ったdeviceをクリックする    
 **+ (Create a sensor)** でsensorを作成する

**Name:** climate_sensor1   
**Sensor Type:** ドロップダウンで先ほど作ったSensor Typeを選択する   
**Alternate ID:** (任意)

Createを押して、sensorが正しく生成されたメッセージが表示される。


### 3-5.Paho を使う場合は Device Certificateを生成する
続きはこちら：
Send data with MQTT
https://help.sap.com/viewer/e765b2a5b99540ce84da397c20cc1993/Cloud/en-US



## 4. スケッチを作成する

### 4-1. Arduino IDE を起動する

### 4-2. メニューの [ツール] で [ボード: "Wio Tracker LTE"] と表示されていることを確認する

なっていなければ一覧から "Wio Tracker LTE" を選んでください

### 4-3. mqtt-client.ino を Arduino IDEで立ち上げる


### 4-4. スケッチを編集して、先ほど設定したendpointと合わせる

変更前

```
// 定数
#define capabilityAlternateId  "9ae29168b5ae2233"
#define sensorAlternateId      "2a58771a9b9efe26"

#define APN               "soracom.io"
#define USERNAME          "sora"
#define PASSWORD          "sora"

#define MQTT_SERVER_HOST  "beam.soracom.io"
#define MQTT_SERVER_PORT  (1883)

#define ID                "ab69d565671f1846"
#define OUT_TOPIC         "measures/ab69d565671f1846"
#define IN_TOPIC          "commands/ab69d565671f1846"
```

変更後

```
// 定数
#define capabilityAlternateId  "定義したId"
#define sensorAlternateId      "定義したId"

#define APN               "soracom.io"
#define USERNAME          "sora"
#define PASSWORD          "sora"

#define MQTT_SERVER_HOST  "beam.soracom.io"
#define MQTT_SERVER_PORT  (1883)

#define ID                "定義したdevice AtlernateId"
#define OUT_TOPIC         "measures/定義したdevice AtlernateId"
#define IN_TOPIC          "commands/定義したdevice AtlernateId"
```

### 4-5. Wio LTE と PC を接続して DFUモード にする

### 4-6. 新しく開いたウィンドウの ![マイコンボードに書き込むアイコン](https://docs.google.com/drawings/d/e/2PACX-1vQiO83cFcX3LCXeioiTiaao57T4SGiIV6XZzcBP6poTwssCxmo7hLpoMh5qG3btyqgzs8Q-lAoE6Q0f/pub?w=100&h=100)(マイコンボードに書き込む) をクリック

### 4-7. 書き込みが完了したら、Wio LTE を 通常モードにする (RSTボタンを押せば通常モードになります)

通常モードで起動次第 SORACOM Funnel へデータを送信し始めます (電源投入から送信開始までは15~20秒程度かかります)

※標準では送信間隔が60秒です。早めたい場合は ステップ2 のやってみようを参考に `INTERVAL` を `5000` などに変えてください

## 5. デバイスからデータが正しく届いているか確認

デバイスからデータが正しく送られているのか、SAP Cloud Platform IoT services cockpitから確認する

## 6. Wio LTE の動作を止める

止める方法は Wio LTE の電源を OFF (= microUSBケーブルを抜く) してください

## うまく動かなかったら（トラブルシュート）

利用可能なツールは「シリアルモニター」「SORACOM Webコンソール上のSIMのログ」です  

**(シリアルモニターを見ると) 1回しかデータを送っていない**

* 原因: SORACOM Funnel へのホスト名やポート番号が違う
    * 対策: スケッチ69行目の SORACOM Funnel へのホスト名やポート番号を確認し、再度マイコンへ書き込んでください

**データは送られているがクラウド上で確認できない**

* 原因: SORACOM Funnel が ON なグループに SIM が所属していない
    * 対策: SIM を SIM グループへ所属させるようにしてください
<a name="step5"></a>
# 【作業】ステップ 5: SORACOM Beam と SAP IoT Services を用いた MQTT Pub/Sub 双方向通信

本格的な IoT システムの構築として、データ収集だけでなく、クラウド側からのデータをデバイス側で受信の仕組みを学びます

## 最初に. Wio LTE の電源を OFF にする

Wio LTE の microUSB ケーブルを抜き、電源を OFF にしてください

※いきなり抜いてOKです。また、すでに OFF になっている場合は次に進んでください

## 1. SORACOM Beam 設定

### 1-1. [SORACOM Webコンソール](https://console.soracom.io/) で 左上[Menu] > [SIM グループ]

[追加] で、SIMグループを作成します (グループ名 `beam` もしくは任意でかまいません)

### 1-2. 先ほど作成した SIMグループ をクリックし、 SORACOM Beam の設定を開きます

### 1-3. SORACOM Beam の「＋」をクリックし、一覧の中から **MQTT エントリポイント** をクリック

![beam-menu](https://docs.google.com/drawings/d/e/2PACX-1vTomsQdSLg-QaoYG591c1-NJYobwgXI6YqppP8h16NnxBLi8sd-QXFJvrrKrKCz6JW4d0oVp-UXCIdT/pub?w=435&h=407)

### 3-4. SORACOM Beam の設定では、下記の通り入力し、保存してください

* 設定名: `to-amazon-mq`
* 種別: *Other MQTT Broker*
* プロトコル: *MQTTS*
* ホスト名: **ハンズオン運営から入手 (amazonmq-host)**
* ポート番号: **ハンズオン運営から入手 (amazonmq-port)**
* ユーザ名: **ハンズオン運営から入手 (amazonmq-username)**
* パスワード: **ハンズオン運営から入手 (amazonmq-password)**
* IMSI 付与: *ON*

![beam-setting](https://docs.google.com/drawings/d/e/2PACX-1vRSDWtOWelYGw9paqusSx3AEfC4nQPGzJ_xjZD8pb7bT3e1aaWtDYNt5iDi8rRGzVmxfq8FjjH5QXnu/pub?w=436&h=908)   

### 3-5. 左上[Menu] > [SIM 管理]

* Wio LTE に取り付けている SIM を選択 > [操作] > [所属グループ変更]
* 先ほど作成した SIMグループ に所属させる

## 4. PubSubClient ライブラリのインストール

MQTT 通信を行うにあたり PubSubClient というライブラリを利用します

### 4-1. Arduino IDE を起動する

### 4-2. Arduino IDE の [スケッチ] > [ライブラリをインクルード] > [ライブラリを管理…]

一覧の中から **PubSubClient** を選択しインストールをクリックします  

※ `mqtt pubsub` で検索すると見つけやすいです

## 5. スケッチを作成する

### 5-1. Arduino IDE を起動する

### 5-2. メニューの [ツール] で [ボード: "Wio Tracker LTE"] と表示されていることを確認する

なっていなければ一覧から "Wio Tracker LTE" を選んでください

### 5-3. Arduino IDE の [ファイル] > [スケッチ例] > [Wio LTE for Arduino] > [mqtt] > [mqtt-client]

### 5-4. スケッチを下記の通りに変更します (変更例は後述)

* 10行目: `hostname` を `beam.soracom.io` に変更
* 13行目: `WioLTE` を **ハンズオン運営から入手した値 (mqtt-iid)** に変更
* 14行目: `outTopic` を **ハンズオン運営から入手した値 (mqtt-pub-topic)** に変更
* 15行目: `inTopic` を **ハンズオン運営から入手した値 (mqtt-sub-topic)** に変更
* 17行目: `60000` を `5000` に変更

**スケッチの変更例**

変更前 (10行目 ～ 17行目までを一括で表示)

```
#define MQTT_SERVER_HOST  "hostname"
#define MQTT_SERVER_PORT  (1883)

#define ID                "WioLTE"
#define OUT_TOPIC         "outTopic"
#define IN_TOPIC          "inTopic"

#define INTERVAL          (60000)
```

変更後 ( `ID` や `OUT_TOPIC` や `IN_TOPIC` の値はハンズオン運営から入手してください)

```
#define MQTT_SERVER_HOST  "beam.soracom.io"
#define MQTT_SERVER_PORT  (1883)

#define ID                "h1203-mqtt-01"
#define OUT_TOPIC         "h1203/04/cloud"
#define IN_TOPIC          "h1203/04/device"

#define INTERVAL          (5000)
```

### 5-5. Wio LTE を PC を接続して DFUモード にする

### 5-6. 新しく開いたウィンドウの ![マイコンボードに書き込むアイコン](https://docs.google.com/drawings/d/e/2PACX-1vQiO83cFcX3LCXeioiTiaao57T4SGiIV6XZzcBP6poTwssCxmo7hLpoMh5qG3btyqgzs8Q-lAoE6Q0f/pub?w=100&h=100)(マイコンボードに書き込む) をクリック

### 5-7. 書き込みが完了したら、Wio LTE を 通常モードにする (RSTボタンを押せば通常モードになります)

通常モードで起動次第 SORACOM Beam へデータを送信し始めます (電源投入から送信開始までは15~20秒程度かかります)

## 6. 確認

### 6-1. [MQTT over WebSocket monitor](https://s3-ap-northeast-1.amazonaws.com/ma2shita/mqtt-monitor/wss.html) を新しいタブ等で開きます

monitor では、下記の値を入力して [CONNECT] をクリックしてください

* WebSocket server address: **ハンズオン運営から入手した値 (ws-address)**
* ClientId for MQTT: _デフォルトから変更する必要はありません_

![wss-setting](https://docs.google.com/drawings/d/e/2PACX-1vSFxfEQ_5O7kShpoynfj8MTBTNrqoWVGpBRCVJpLDPTrGkmTeR3KPR0EAp-TZumZ6pSrsBrkoZi5FhC/pub?w=659&h=433)

### 6-2. Subscribe と Publish の設定

#### Subscribe (Wio LTE → クラウド)

* monitor の **SUBSCRIBE** タブをクリック
* monitor の **Add New Topic Subscription** に **ハンズオン運営から入手した値 (ws-sub-topic)** を入力  
例: `bh0701/01/#`
* monitor の [ADD SUBSCRIBE] をクリック

**Monitoring** に Wio LTE からのデータを受信していることが確認できます

![mqtt-sub](https://docs.google.com/drawings/d/e/2PACX-1vQsc6capNMYLbbUduifhMyZiyT2GTm1NE79BHqBqSIh3gYN7w38r0Hb2Wdym_LkDTF6Nnn-d5IwshUm/pub?w=891&h=714)

#### Publish (クラウド → Wio LTE)

* monitor の **PUBLISH** タブをクリック
* monitor の **Topic** に **ハンズオン運営から入手した値 (ws-pub-topic)** を入力  
例: `h1203/04/device`
* **Content** に任意の文字を入力する
* [Publish] をクリックすると Wio LTE 側のシリアルモニターに Content で入力した文字が受信できていることが確認できます (同時に monitor の Subscribe の方にも表示されます)

![mqtt-pub](https://docs.google.com/drawings/d/e/2PACX-1vT2KoOzkGfflDShd412jVb1fm7qPUDg6u9prKrx4p636rE3behb8geswVNQweenKaXxfX1nNSAyU2kU/pub?w=891&h=714)

## やってみよう

23 行目の `void callback()` の内容を「 `#xxxxxx` というデータを受信したら LED の発光色として使う」としてみましょう

変更前;

```c
void callback(char* topic, byte* payload, unsigned int length) {
  SerialUSB.print("Subscribe:");
  for (int i = 0; i < length; i++) SerialUSB.print((char)payload[i]);
  SerialUSB.println("");
}
```

変更後;

```c
void callback(char* topic, byte* payload, unsigned int length) {
  char c[length + 1];
  for (int i = 0; i < length; i++) c[i] =  (char) payload[i];
  c[length] = 0;
  SerialUSB.print("Subscribe:");
  SerialUSB.println(c);
  char *pos = strchr(c, '#');
  if (pos != NULL) {
    unsigned long r = 0;
    unsigned long g = 0;
    unsigned long b = 0;
    char rx[3] = {pos[1], pos[2], 0};
    r = strtoul(rx, NULL, 16);
    char gx[3] = {pos[3], pos[4], 0};
    g = strtoul(gx, NULL, 16);
    char bx[3] = {pos[5], pos[6], 0};
    b = strtoul(bx, NULL, 16);
    Wio.LedSetRGB(r, g, b);
  }
}
```

試験用カラーコード

* 赤 : `#300000`
* 緑 : `#002000`
* 青 : `#000018`
* SORACOM カラー : `#001410`
* オレンジ : `#481400`
* 消灯 : `#000000`

その他の色も試してみてください

## 4. Wio LTE の動作を止める

止める方法は Wio LTE の電源を OFF (= microUSBケーブルを抜く) してください

## うまく動かなかったら（トラブルシュート）

**「マイコンボードに書き込む」を実行した結果、ウィンドウに下記のように表示された**

```
 #include <PubSubClient.h>  // https://github.com/knolleary/pubsubclient
                                                                        ^
compilation terminated.

exit status 1
ボードWio Tracker LTEに対するコンパイル時にエラーが発生しました。
```

* 原因: *PubSubClient* ライブラリが不足しています
* 対策: ステップ5 の本文を読み *PubSubClient* をインストールした後、再度書き込みを行ってください

**(シリアルモニタ―で) `### Connecting to "soracom.io".` で止まってしまう**

* 原因: 電波状況などによりモバイル通信に失敗している
* 対策: RST ボタンを押して Wio LTE を再度起動しなおしてください

**最初の数回だけ Websockets Client 側で受信ができて、その後の受信が確認できない / Websockets Client 側からの Publish が Wio LTE に届かない**

* 対策: 下記手順を実施してみてください
    1. (Wio LTE の電源が入ったままで) SORACOM Webコンソール上で、当該の SIM のセッションを切断
    2. Wio LTE を再起動 (RST ボタンなど)
<a name="end"></a>
# おわりに

おつかれさまでした  
Wio LTE ハンズオンは以上で終了です。最後に注意点の確認をお願いいたします

## 《知識》今後の費用について

SORACOM Air SIM は**実際の通信の有無に限らず**日当たりの基本料金が発生いたします    
詳細は [ご利用料金 - 日本向け Air SIM](https://soracom.jp/services/air/price/) をご覧ください

### SORACOM Harvest

本ハンズオンで使用した SORACOM Harvest は、**実際の通信の有無に限らず** SORACOM Harvest が利用可能状態の SIM の枚数に応じて日当たりのオプション料金が発生します  
詳細は [SORACOM Harvest のご利用料金](https://soracom.jp/services/harvest/price/) をご覧ください

### SORACOM Beam および SORACOM Funnel

本ハンズオンで使用した SORACOM Beam および SORACOM Funnel は、発生したリクエストに応じた課金がされるサービスです  
詳細は [SORACOM Beam のご利用料金](https://soracom.jp/services/beam/price/) ならびに [SORACOM Funnel のご利用料金](https://soracom.jp/services/funnel/price/) をご覧ください

### クーポンの登録方法

[クーポンコードの使い方](https://soracom.zendesk.com/hc/ja/articles/218121348-%E3%82%AF%E3%83%BC%E3%83%9D%E3%83%B3%E3%82%B3%E3%83%BC%E3%83%89%E3%81%AE%E4%BD%BF%E3%81%84%E6%96%B9%E3%82%92%E6%95%99%E3%81%88%E3%81%A6%E3%81%8F%E3%81%A0%E3%81%95%E3%81%84)をご覧ください

※その他[クーポンに関する注意事項](https://soracom.zendesk.com/hc/ja/articles/115002423011-%E3%82%AF%E3%83%BC%E3%83%9D%E3%83%B3%E3%82%B3%E3%83%BC%E3%83%89%E3%82%92%E9%81%A9%E7%94%A8%E3%81%97%E3%81%A6%E3%81%84%E3%81%A6%E3%82%82%E6%96%99%E9%87%91%E3%81%8C%E8%AB%8B%E6%B1%82%E3%81%99%E3%82%8B%E3%81%AE%E3%81%AF%E3%81%AA%E3%81%9C%E3%81%A7%E3%81%99%E3%81%8B-)もご参照ください

### 長期利用割引

Air SIM のご契約期間をお約束いただくことで、基本料金が割引になる仕組みをご検討ください

* [長期利用割引 - 日本向け Air SIM](https://soracom.jp/services/air/cellular/discount_price/)
* [長期利用割引に関する手続きやFAQ](https://blog.soracom.jp/blog/2017/05/23/long-term-discount/)

## 【作業】Wio LTE を片付ける

### Wio LTE の初期化

Wio LTE を初期化しておくことで、次回起動時に不用意な通信などを防ぐことができます

作業としては「空っぽのスケッチ」を書き込むことで、初期化できます

1. Arduino IDE を起動する
2. メニューの [ツール] で [ボード: "Wio Tracker LTE"] と表示されていることを確認する
    * なっていなければ一覧から "Wio Tracker LTE" を選んでください
3. Arduino IDE の [ファイル] > [新規ファイル]
4. Wio LTE を PC を接続して DFUモード にする
5. 新しく開いたウィンドウの ![マイコンボードに書き込むアイコン](https://docs.google.com/drawings/d/e/2PACX-1vQiO83cFcX3LCXeioiTiaao57T4SGiIV6XZzcBP6poTwssCxmo7hLpoMh5qG3btyqgzs8Q-lAoE6Q0f/pub?w=100&h=100)(マイコンボードに書き込む) をクリック
6. 書き込みが完了したら、Wio LTE を 通常モードにする (RSTボタンを押せば通常モードになります)

### SIM の取り出し

<img src="https://docs.google.com/drawings/d/e/2PACX-1vSHV26TeW7Z3rEbr-OOVQ-6GyI8GijLNChn5ayEspNVPZqmbZTj3pkDdBCZhLEetrMqLdDuXm-GpQk9/pub?w=683&amp;h=322">

<img src="https://docs.google.com/drawings/d/e/2PACX-1vRn4PgJFdW-IV-Fr4JTq7dj6wz4KBSyG3pM6W4Wxdbtt-FunzEy4aSK1_QpssqfoATEWfP_9IJL8vtV/pub?w=508&amp;h=370">

**抜く際フックを引っ張りすぎると金具が取れてしまうため、図示されている程度まで引っ張り出したらSIMを取り出し、金具を元に戻してください**

### 箱へ戻す

手渡された時同様に箱へ格納してください

* アンテナは取り外して Wio LTE 本体が格納されていた白いトレーのミゾに入れてください
* microUSB ケーブルは白いトレーを取り外した中に入れてください

<a name="appendix"></a>
# Appendix(付録):
##ハンズオンで使用した環境の構築方法
