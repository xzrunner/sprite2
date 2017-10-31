# Reference

## Symbol和Sprite

Symbol实例化出来Sprite。
Sprite会持有Symbol的一个引用，Symbol没有记录它的Sprite，如果有需要可以在外部实现，比如编辑器里改变Symbol后需要更新它所有Sprite的AABB这种操作。

Symbol会持有它子节点Sprite的引用，比如ComplexSymbol保存children Sprite的引用。这里子节点没有指向父节点的引用，放到Actor里来实现。

## Sprite和Actor

Sprite实例化出来Actor。

Sprite中记录所有Actor的引用，查找当前对应的Actor。Actor弱引用指向Sprite，为了消除环状引用。

## Actor

用弱引用指向父节点Actor，允许父节点被释放掉不影响子节点。相关操作需要检查父节点是否有效，比如计算世界坐标。

## ActorProxy

用来记录逻辑层的引用，处理GC。与逻辑层的接口只有Actor，ActorProxy这里持有了Actor和它的Sprite的引用，因为Actor到Sprite是弱引用的，不能只通过Actor来清掉所有资源。GC以这里的Sprite为入口，清掉Actor和Symbol的引用。
