# Nodes

## anchor

挂点，需要在游戏运行时用mount()来绑定一个node，在编辑器中这个节点只是展示效果，打包时不会输出。

## image

图片，纹理资源。

## scale9

九宫格，保持4角不变，中间可以自由拉伸。

## icon

纹理动画

## texture

填充了颜色或纹理的一组多边形。

## textbox

文本框。

## complex

组合node，有个裁剪框。

## anim

动画

## anim2

另一种动画，见rigging

## particle3d

3维空间中的粒子。

## particle2d

2维平面上的粒子。

## shape

几何图形。

## mesh

顶点动画

## mask

遮罩，有base和mask两层，用mask层的alpha乘以base层的颜色。

## trail

移动路径上的拖尾效果。

## proxy

透明的一层，所有对这个节点的操作会转给它的所有子节点。
