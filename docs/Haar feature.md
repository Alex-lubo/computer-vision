# Haar特征

在opencv2.4.11版本中，提供了14中Haar特征：
+ BASIC
  + x2
  + y2
  + x3
  + y3
  + x2_y2
+ CORE
  + x4
  + y4
  + point
+ ALL(Titled)
  + titled_x2
  + titled_y2
  + titled_x3
  + titled_y3
  + titled_x4
  + titled_y4
  + titled_point(未使用)

在宽度为W，高度为H的检测窗口中， 可以获得的矩形特征的数量为：
<a href="https://www.codecogs.com/eqnedit.php?latex=X(W&plus;1-w\frac{X&plus;1}{2})Y(H&plus;1-h\frac{Y&plus;1}{2})" target="_blank"><img src="https://latex.codecogs.com/gif.latex?X(W&plus;1-w\frac{X&plus;1}{2})Y(H&plus;1-h\frac{Y&plus;1}{2})" title="X(W+1-w\frac{X+1}{2})Y(H+1-h\frac{Y+1}{2})" /></a>

其中w，h为Haar算子的宽度和高度。

Haar特征值的计算：

<a href="https://www.codecogs.com/eqnedit.php?latex=featureV(x)=weight_{all}\times&space;\sum_{pixel&space;\in&space;all}Pixel&space;&plus;&space;weight_{black}\times&space;\sum_{pixel&space;\in&space;black}Pixel" target="_blank"><img src="https://latex.codecogs.com/gif.latex?featureV(x)=weight_{all}\times&space;\sum_{pixel&space;\in&space;all}Pixel&space;&plus;&space;weight_{black}\times&space;\sum_{pixel&space;\in&space;black}Pixel" title="featureV(x)=weight_{all}\times \sum_{pixel \in all}Pixel + weight_{black}\times \sum_{pixel \in black}Pixel" /></a>

使用积分图（integral graph）可以加速Haar的计算。（空间换时间）


adaboost级联分类器
 简单来说就是通过弱分类器的组合成强分类器来对目标特征进行分类。