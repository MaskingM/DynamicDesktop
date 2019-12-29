# DynamicDesktop
动态桌面

**采用技术：**  
本应用基于 win32api + opengl + opencv，通过 opencv 解码图片或视频提取 RGB 数据，然后通过 opengl 进行硬件渲染。解码高清视频和迅雷看看一致。通过 win32api 将 RGB 数据专场 GDI，并渲染到 桌面图标之下
