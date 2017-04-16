# AABB

## Symbol，Sprite和Actor各自的AABB

Symbol的AABB是它所有子节点一起算出的最大的包围盒，比如Complex的所有Action的，Anim的所有帧。在渲染和查询时做减枝，排除掉范围之外的。多算的部分会放到子节点来处理。

Sprite的AABB默认有旋转信息，是个OBB，受Sprite SRT操作的影响。主要用于帮助创建Symbol的AABB。

Actor的AABB是这个节点实例化出来的实际大小，比如Text节点就会计算它的str排版后的大小。返回给脚本层用的就是这个AABB。代表当前Actor的Symbol的实际AABB，也就是不包括Sprite和Actor的SRT变换。也用在Symbol的GetBounding()接口中，如果参数中存在Actor并且它的AABB有效，就直接用这个值代表当前Symbol的值来返回，这样能使结果更准确。
```
sm::rect Symbol::GetBounding(const Sprite* spr, const Actor* actor, bool cache) const
{
	if (cache && actor) {
		const sm::rect& rect = actor->GetAABB().GetRect();
		if (rect.IsValid()) {
			return rect;
		}
	}
	return GetBoundingImpl(spr, actor, cache);
}
```

获取一个节点的AABB，是用它的Local Matrix乘以Symbol的这个接口返回的size。这里应该改成用Sprite的接口，如果Actor不存在，能节省一次矩阵变换[TODO]。

## AABB的update

采用一些策略来减少update的频率。

高频更新的事件，不更新AABB。比如粒子、动画等节点每帧的AABB都可能会变化，Symbol只存所有状态的最大值。这里会有浪费，但会保证正确，用于粗略的剔除。

不是高频的事件会更新相应的AABB。比如SRT操作、Complex切换action、Textbox改变text、Anchor改变挂点等。编辑器中的这些操作会改变Sprite的AABB，进而影响到他所在的Symbol，通过Sprite-Symbol这个顺序一直往上传。游戏运行时的这些操作，会改变Actor的AABB，通过它的parent传到上级Actor，如有需要会一直传下去。

## 用于查询的减枝

有层级结构的节点会用它的AABB来粗略的剔除，这里指用Complex和Anim的Symbol的AABB来计算，没相交的会被减枝掉，相交的话就用子节点递归来计算。

没层级结构的节点会直接用它的AABB来判断。

## 用于渲染的减枝

节点的AABB与显示范围的AABB不相交的话，这个节点会被剔除。