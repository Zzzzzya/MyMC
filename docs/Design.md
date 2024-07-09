# 初步设计
1. 采用cpp和OpenGL来构建。
2. 渲染方面，采用前向渲染。光源只有太阳一个。Shader采用Phong氏光照，部分金属可以加Pbr。
3. 目前想到需要拿出实现的模块：
   - 日志系统。建议采用spdlog，自己封装日志类。
   - 物理系统。AABB的构建和碰撞检测。
   - 纹理素材。各方块的纹理素材搜寻。
   - 界面系统，QT or EasyX？
4. 初步架构设计
   - 场景类 Scene\
     管理所有物体 光源 人物
   - 玩家类 Player\
     拥有自己的摄像机
   - 物体类 Mesh
   - 光照类 Light
   - Shader类 Shader
   - 纹理类 Texture
   - AABB

## 一些说明
1. res/ 下为model和music文件夹。model存放模型和纹理，按照模型的形状划分。