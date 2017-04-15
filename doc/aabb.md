# AABB

## Symbol，Sprite和Actor各自的AABB

Symbol的AABB是它所有子节点一起算出的最大的包围盒，比如Complex的所有Action的，Anim的所有帧。在渲染和查询时做减枝，排除掉范围之外的。多算的部分会放到子节点来处理。

Sprite的AABB有旋转信息，有OBB的结构。

Actor的AABB是这个节点实例化出来的实际大小，比如Text节点就会计算它的str排版后的大小。

## AABB的update



## 用于查询的减枝



## 用于渲染的减枝