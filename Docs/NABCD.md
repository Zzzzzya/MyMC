# NABCD模型分析
## N(Need)
使用C++和OpenGL，从零模仿MineCraft创建一个自己的体素游戏。\
- 提供更好的基础渲染效果
- 提供更为灵活和方便的方块创建交互模式
- 与Microsoft启动器解耦，实现跨平台
- 针对低端机型对于高性能渲染效果和运算效果的优化

## A(Approach)
1. 提供更好的基础渲染效果
    - 水面渲染方面
      - 使用屏幕空间光线追踪(SSRR)制作屏幕空间反射效果
    - 阴影方面
      - 使用屏幕空间环境光遮蔽(SSAO)制作AO
      - 使用ShadowMap结合PCF和PCSS算法生成软阴影
      - 使用CSM生成级联阴影，加强近处阴影分辨率
    - 真实感方面
      - 使用PBR构建部分材质，使用IBL来计算环境光照
      - 增加雾效
2. 提供更为灵活和方便的方块创建交互模式
   - 创新性提供类似方块工厂模式，玩家可以自由创建一个大物体并保存为一个物体，直接放置（如自己搭建房屋后存为一个模块，之后可以直接在指定位置放置一个房屋）。
   - 提供飞行，走路，奔跑，摄像机等多种模式
3. 与Microsoft启动器解耦，实现跨平台
   - 使用OpenGL
4. 针对低端机型对于高性能渲染效果和运算效果的优化
    - DrawCall动态合批
    - 实例化使用
    - 多Chunk实时卸载加载更新
    - 多种可调节参数的剔除效果。包括视锥剔除，遮挡剔除，距离剔除

## B(Benefit)
 - 用户无需自己寻找复杂的材质包，无需懂得shader编写就可以看到更好的画面
 - 用户可以在不同的机型上畅玩
 - 用户可以更方便的搭建大型地形
 - 低端机型用户可以更好地体验游戏

## C(Competitors)
 - 微软的MC😤😤😤 缺点是我们不是微软的
 - MC未制作较好的水面效果
 - MC没有大型物体的保存和重复构建功能
 - 能够更好的聆听周围玩家的诉求

## D(Delivery)
- 使用Hexo搭建博客介绍、发布、收集反馈。