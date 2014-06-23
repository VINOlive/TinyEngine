TinyEngine
==========

学校的《Windows游戏编程》课程，Win32项目，利用Windows SDK构建一个微型游戏引擎！<br/>
注：代码仅供学习，版权归我的老师：[万立中](http://www.wanlizhong.com)所有，因为老师已经就内容出书：[《游戏程序设计教程》](http://product.dangdang.com/23413918.html?_ddclickunion=400-kw-%CD%BC%CA%E9-%CA%E9%C3%FB%B4%CA_%BD%CC%B2%C4_|ad_type=0|sys_id=1)。

一、TinyEngine引擎主要类解释
------------------------

![TinyEngine](http://fangjie.sinaapp.com/upload/images/TinyEngine.jpg "TinyEngine")  

### T_Engine - 所有引擎类的基类
主要做游戏主窗口的事件处理，所有的引擎必须继承该父类，并实现其中的纯虚函数。该类拥有一个本类实例指针的成员变量。

### T_Config - 引擎类的配置文件
只有一个头文件，包含引擎类自定义的类型和一些常量。

### T_Display - 显示相关的分辨率设置类
该类主要做游戏全屏和退出全屏的分辨率设置和恢复。

### T_Menu - 游戏开始界面菜单选项操作类
该类主要封装了游戏开始界面的菜单操作。

### T_Graph - 窗口绘制操作类
该类主要做游戏窗口的图像绘制操作。

### T_Layer - 游戏图层类（抽象类，是T_Sprite 的基类）
该类主要是T_Sprite 和T_Map类的基类，是一个抽象类，有个纯虚函数函数Draw()，对不同的子类有不同的实现。

### T_Sprite - 游戏精灵类
该类是主要做游戏精灵（玩家角色+NPC角色）基本操作的类，包括位置、方向更新，和图像绘制。

### T_AI - AI类
该类处理游戏角色的智能行为，躲避、碰撞检测等。

### T_Scene-游戏场景类
该类负责管理T_Sprite和T_Map类

### T_Audio-游戏音频类
该类主要操作游戏的音频播放、暂停、继续，用到了DirectX库


二、使用该TinyEngine开发的飞机大战
------------------------------
飞机大战是仿微信飞机大战开发的游戏。利用VS2008开发，Win32项目。可以直接打开FlyGame.exe文件试玩。


by：[JayFang](http://fangjie.sinaapp.com)
