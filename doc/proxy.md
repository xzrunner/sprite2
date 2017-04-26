# Proxy

## 功能

透明的一层，所有对这个节点的操作会转给它的所有子节点。

## 使用

Sprite的FetchChild(name)接口可能会创建出来Proxy节点，如果Sprite有多个子节点的名字是name的话。

这样方便批量操作多个节点，把它们设置相同名字就可以了，否则只能用Complex再包一层。

## 注意

有一些操作是没有意义的，比如：
s2_spr_get_pos()
s2_spr_get_angle()
s2_spr_get_scale()
