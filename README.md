# ガヴリールドロップアウトアドバンス(ガヴドロアドバンス)

[ガヴリールドロップアウト](http://gabdro.com/)の同人ゲームです。  
なんとゲームボーイアドバンス(以下GBA)で動きます。大学の実験で作りました。  

ちなみにタイトルは575です。

## 概要

プレイヤー（ガヴリール）を操作して、敵（ゼルエル）の攻撃を避けながらなるべく多くのお金(コイン)を集めるゲームです。  

ガヴリールはゼルエルに特訓の名目でしばかれています。これもまた愛の形です。

## あそびかた

gabriel.gbaをゲームボーイアドバンス用エミュレータで起動してください。  

ブラウザ上で動作するGBAエミュレータ  

- [GBA.js](http://endrift.github.io/gbajs/)
- [IodineGBA](https://taisel.github.io/IodineGBA/) (要GBA用BIOSファイル)  

などをを使えばブラウザでプレイできるっぽいです。（試したけどエミュレータ側に若干OAMのバグあり？） 

または、gabriel.gbaをブートケーブルなどを用いて実機本体にMulti-bootで転送してください。  

実機上での動作に加え、[mGBA 0.52](https://mgba.io/)上で動作を確認しています。  

## コンパイル

以下の環境  

- macOS Sierra 10.12.6(16G29)  
- gcc version 6.3.0 (devkitARM release 46)  
- GNU objcopy (GNU Binutils) 2.27  
- GNU Make 3.81  

でmakeすれば動くと思います。MacならdevkitProをインストールするだけでコンパイルできるはず。

    > make clean
    rm -f build/*
    rm -f gabriel.gba
    > make
    arm-none-eabi-gcc -W -Wall -O3 -mcpu=arm7tdmi -mtune=arm7tdmi -ffast-math -I include -o build/bg.o -c source/bg.c
    arm-none-eabi-gcc -W -Wall -O3 -mcpu=arm7tdmi -mtune=arm7tdmi -ffast-math -I include -o build/chr.o -c source/chr.c
    arm-none-eabi-gcc -W -Wall -O3 -mcpu=arm7tdmi -mtune=arm7tdmi -ffast-math -I include -o build/enemy.o -c source/enemy.c
    arm-none-eabi-gcc -W -Wall -O3 -mcpu=arm7tdmi -mtune=arm7tdmi -ffast-math -I include -o build/game.o -c source/game.c
    arm-none-eabi-gcc -W -Wall -O3 -mcpu=arm7tdmi -mtune=arm7tdmi -ffast-math -I include -o build/item.o -c source/item.c
    arm-none-eabi-gcc -W -Wall -O3 -mcpu=arm7tdmi -mtune=arm7tdmi -ffast-math -I include -o build/main.o -c source/main.c
    arm-none-eabi-gcc -W -Wall -O3 -mcpu=arm7tdmi -mtune=arm7tdmi -ffast-math -I include -o build/player.o -c source/player.c
    arm-none-eabi-gcc -W -Wall -O3 -mcpu=arm7tdmi -mtune=arm7tdmi -ffast-math -I include -o build/screen.o -c source/screen.c
    arm-none-eabi-gcc -W -Wall -O3 -mcpu=arm7tdmi -mtune=arm7tdmi -ffast-math -I include -o build/sprite.o -c source/sprite.c
    arm-none-eabi-gcc -specs=gba_mb.specs -o build/gabriel.elf build/bg.o build/chr.o build/enemy.o build/game.o build/item.o build/main.o build/player.o build/screen.o build/sprite.o
    arm-none-eabi-objcopy -O binary build/gabriel.elf gabriel.gba

## ライセンス (ソースコード部分のみ)

    Copyright (c) 2017 Hayato Kohara

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## 説明・操作方法

以下の4つの画面から構成されています。  

### TITLE画面

起動した直後はTITLE画面とハイスコアが表示されます。  
TITLE画面でスタートボタンを押すと、READY画面に遷移します。  

### READY画面

いきなりゲームが開始するのは酷だと思うのでプレイヤーと敵の位置を確認して準備をします。  
数秒でGAME画面に遷移します。  

### GAME画面

十字キーの左右でプレイヤーの移動・Aボタンで浮遊します。  
プレイヤーはコインに触れると1pt獲得します。累計ptは右上に表示されます。  

画面は自動でスクロールするためゲームの初級者の人でも片手で簡単に遊べます。  
上級者の人は左右に動いてたくさんコインを集めてみてください！  

敵の攻撃に触れると残機が1つ減り、しばらく点滅して無敵となります。残機は左上に表示されます。  
残機が0になるとゲームオーバーとなり、OVER画面に遷移します。  

スコアが一定数(200,500,1000)に達すると敵の動きや弾の速度が上昇し、避けるのが難しくなります。  
特に1000pt以上になるとかなりの難易度になります。  

また、スコアが500増えるごとにアイテム「ラッパ」が下から出現します。うまく回収しましょう。  
ラッパはBボタンを押すとで使うことができ、画面を振動させ表示されているすべてのコインを回収できます。  

マップは一定の長さを超えるとループします。  

### OVER画面

ゲームオーバー時の自分のスコアと今までのハイスコアが表示されます。  

ハイスコアを更新していた場合は自分のスコアの横に王冠マーク、ハイスコアの横に"NEW"が表示されます。  

OVER画面でスタートボタンを押すと、TITLE画面に遷移します。  

## 使用しているライブラリや素材とか

特にないです。  
画像に関してはすべて自作です。多分そういう系の問題はないはず…。  

プログラムに関して、強いていえば [西田 亙](http://www.skyfree.org/jpn/) 氏のGBA用各種定義ファイル(gba.h)を参考にしました。

## おわりに

大学の指導教員の先生・TAの方々、[金沢大学](http://jaco.ec.t.kanazawa-u.ac.jp/edu/GBA/index.html) 様、[GBA develop Wiki](http://akkera102.sakura.ne.jp/gbadev/) 様、各開発ツールの作者様 などGBA開発において多くの知見をいただきました。  
この場を借りて感謝させていただきます。:)  

原作者のうかみ先生、ガヴドロファンの方々、レトロゲーマーの方々に捧げます。  
2017年8月1日 yaplus