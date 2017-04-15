# AABB

## Symbol，Sprite和Actor各自的AABB

Symbol的AABB是它所有子节点一起算出的最大的包围盒，比如Complex的所有Action的，Anim的所有帧。在渲染和查询时做减枝，排除掉范围之外的。多算的部分会放到子节点来处理。

Sprite的AABB有旋转信息，有OBB的结构。

Actor的AABB是这个节点实例化出来的实际大小，比如Text节点就会计算它的str排版后的大小。返回给脚本层用的就是这个AABB。

## AABB的update

采用一些策略来减少update的频率。

Symbol的AABB存的是最大的包围盒，只有Symbol的子节点有变化时才改变。

脚本层的一些操作会对AABB有改变，比如Complex切换action或者平移子节点等，这种信息只存在对应的Actor中。

动画更新时不更新的update。

## 用于查询的减枝



## 用于渲染的减枝