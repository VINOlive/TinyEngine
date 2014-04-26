TinyEngine
==========

学校的《Windows游戏编程》课程，Win32项目，利用Windows SDK构建一个微型游戏引擎！<br/>
注：代码仅供学习，版权归我的老师：[万立中](http://www.wanlizhong.com)所有，因为老师已经就内容出书：[《游戏程序设计教程》](http://product.dangdang.com/23413918.html?_ddclickunion=400-kw-%CD%BC%CA%E9-%CA%E9%C3%FB%B4%CA_%BD%CC%B2%C4_|ad_type=0|sys_id=1)。

T_Engine - 所有引擎类的基类<br/>
主要做游戏主窗口的事件处理，所有的引擎必须继承该父类，并实现其中的纯虚函数。该类拥有一个本类实例指针的成员变量。

T_Config - 引擎类的配置文件<br/>
只有一个头文件，包含引擎类自定义的类型和一些常量。

T_Display - 显示相关的分辨率设置类<br/>
该类主要做游戏全屏和退出全屏的分辨率设置和恢复。

GameTest - 游戏测试类<br/>
该类继承自T_Engine类，并实现其纯虚函数。

by：[JayFang](http://fangjie.sinaapp.com)
